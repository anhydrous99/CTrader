//
// Created by Armando Herrera on 3/5/21.
//

#include "TradeHistory.h"
#include "imgui.h"

TradeHistory::TradeHistory(libCTrader::Websock* sock, const libCTrader::Product &product) : current_product(product),
    trade_history(192), websock(sock) {
    websock->add_channel_product_pair("ticker", product, 1);
    current_connection = websock->on_new_ticker([&](const libCTrader::WSTicker& ticker) {
        std::unique_lock trade_history_lock(trade_history_mutex);
        trade_history.push(ticker);
    }, current_product);
}

void TradeHistory::change_product(const libCTrader::Product &new_product) {
    current_connection.disconnect();
    trade_history.clear();
    {
        std::shared_lock lock(current_product_mutex);
        websock->remove_channel_product_pair("ticker", current_product, 1);
    }
    websock->add_channel_product_pair("ticker", new_product, 1);
    {
        std::unique_lock lock(current_product_mutex);
        current_product = new_product;
    }
    current_connection = websock->on_new_ticker([&](const libCTrader::WSTicker& ticker) {
        std::unique_lock trade_history_lock(trade_history_mutex);
        trade_history.push(ticker);
    }, current_product);
}

void TradeHistory::display_trade_history_window() {
    const static ImVec4 Red(1.0f, 0.0f, 0.0f, 1.0f);
    const static ImVec4 Green{0.0f, 1.0f, 0.0f, 1.0f};

    ImGui::SetNextWindowPos(ImVec2(1065.f,19.f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(215.f,701.f), ImGuiCond_FirstUseEver);
    ImGui::Begin("Trade History", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if (ImGui::BeginTable("Trade History", 2)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Price");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("| Trade Size");
        std::shared_lock trade_history_lock(trade_history_mutex);
        for (int64_t i = trade_history.size() - 1; i >= 0; i--) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            float price = std::stof(trade_history[i].price);
            ImGui::TextColored((trade_history[i].side == "buy") ? Green : Red, "%.2f", price);
            ImGui::TableSetColumnIndex(1);
            float size = std::stof(trade_history[i].last_size);
            ImGui::Text("  %.8f", size);
        }
        trade_history_lock.unlock();
        ImGui::EndTable();
    }
    ImGui::End();
}
