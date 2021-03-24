//
// Created by Armando Herrera on 3/4/21.
//

#include "WatchList.h"

#include <iomanip>
#include <utility>
#include "imgui.h"

using namespace std::chrono;

WatchList::WatchList(libCTrader::Websock *websock, std::vector<libCTrader::Product> available_products,
                     std::vector<libCTrader::Product> sel_products) : available_products(std::move(available_products)),
                     selected_products(std::move(sel_products)) {
    websock_ptr = websock;
    for (const auto& selected : selected_products) {
        latest_tickers[selected.id] = libCTrader::WSTicker();
    }
    websock->on_new_ticker([&](const libCTrader::WSTicker& tick) {
        std::shared_lock selected_product_lock(selected_products_mut);
        auto itr = std::find_if(selected_products.begin(), selected_products.end(), [&](const libCTrader::Product& p) {
            return tick.product_id == p.id;
        });
        if (itr != selected_products.end()) {
            std::unique_lock latest_tickers_lock(latest_tickers_mut);
            latest_tickers[tick.product_id] = tick;
        }
    });
    websock_ptr->add_channel("ticker", selected_products, 0);
}

bool WatchList::display_watch_list_window() {
    static std::string current_product;
    const static ImVec4 Red(1.0f, 0.0f, 0.0f, 1.0f);
    const static ImVec4 Green{0.0f, 1.0f, 0.0f, 1.0f};
    bool close = false;

    ImGui::Begin("WatchList", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::BeginTable("List", 4)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Product");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("| Price");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("| Change (24hr)");
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("| Vol. (24hr)");
        auto t = high_resolution_clock::now();
        std::shared_lock selected_products_lock(selected_products_mut);
        for (auto &p : selected_products) {
            std::shared_lock latest_tickers_lock(latest_tickers_mut);
            auto tick = latest_tickers[p.id];
            latest_tickers_lock.unlock();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", p.display_name.c_str());
            ImGui::TableSetColumnIndex(1);
            if (!tick.price.empty())
                ImGui::Text("  %.2f", std::stof(tick.price));
            ImGui::TableSetColumnIndex(2);
            if (!tick.price.empty() && !tick.open_24h.empty()) {
                float price = std::stof(tick.price);
                float open_24h = std::stof(tick.open_24h);
                float change = 100.f * (price - open_24h) / open_24h;
                ImGui::TextColored((change > 0) ? Green : Red, "  %.2f%%", change);
            }
            ImGui::TableSetColumnIndex(3);
            if (!tick.volume_24h.empty()) {
                float vol = std::stof(tick.volume_24h);
                ImGui::Text("  %s", aggregator(vol).c_str());
            }
        }
        ImGui::EndTable();
    }

    if (ImGui::BeginCombo("", current_product.c_str(), ImGuiComboFlags_NoArrowButton)) {
        for (const auto& available_product : available_products) {
            bool is_selected = (current_product == available_product.display_name);
            if (ImGui::Selectable(available_product.display_name.c_str(), is_selected))
                current_product = available_product.display_name;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Add")) {
        auto itr = std::find_if(available_products.begin(), available_products.end(),
            [&](const libCTrader::Product& p) {
                return current_product == p.display_name;
            });
        add_product(*itr);
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove")) {
        auto itr = std::find_if(available_products.begin(), available_products.end(),
            [&](const libCTrader::Product& p) {
                return current_product == p.display_name;
            });
        remove_product(*itr);
    }
    ImGui::SameLine();
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}

std::string WatchList::aggregator(float num) {
    float values[] = {1.f, 1.e3, 1.e6, 1.e9, 1.e12, 1.e15, 1.e18};
    std::string sym[] = {"", "k", "M", "B"};
    int i = 3;
    for (; i > 0; i--) {
        if(num >= values[i])
            break;
    }
    std::stringstream stream;
    stream << std::setprecision(4) << num / values[i] << sym[i];
    return stream.str();
}

void WatchList::add_product(const libCTrader::Product &p) {
    std::unique_lock selected_products_lock(selected_products_mut);
    auto itr = std::find(selected_products.begin(), selected_products.end(), p);
    if (itr == selected_products.end()) {
        websock_ptr->add_channel_product_pair("ticker", p, 0);
        std::unique_lock latest_ticker_lock(latest_tickers_mut);
        latest_tickers[p.id] = libCTrader::WSTicker();
        latest_ticker_lock.unlock();
        selected_products.push_back(p);
    }
}

void WatchList::remove_product(const libCTrader::Product &p) {
    std::unique_lock selected_products_lock(selected_products_mut);
    auto itr = std::find(selected_products.begin(), selected_products.end(), p);
    if (itr != selected_products.end()) {
        websock_ptr->remove_channel_product_pair("ticker", p, 0);
        selected_products.erase(itr);
        latest_tickers.erase(p.id);
        std::unique_lock latest_ticker_lock(latest_tickers_mut);
    }
}
