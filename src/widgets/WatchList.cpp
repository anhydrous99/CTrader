//
// Created by Armando Herrera on 3/4/21.
//

#include "WatchList.h"

#include <iomanip>
#include <utility>
#include "imgui.h"

using namespace std::chrono;

WatchList::WatchList(libCTrader::Websock *websock, const std::vector<libCTrader::Product> &av_products, const std::vector<int> &active) {
    websock_ptr = websock;
    available_products = av_products;

    for (int i : active)
        selected_products.emplace_back(available_products[i], high_resolution_clock::now(), websock->get_ticker(available_products[i].id));
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
        for (auto &p : selected_products) {
            if (duration_cast<seconds>(t - p.timePoint).count() > 2) {
                p.ticker = websock_ptr->get_ticker(p.product.id);
                p.timePoint = t;
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", p.product.display_name.c_str());
            ImGui::TableSetColumnIndex(1);
            if (!p.ticker.price.empty())
                ImGui::Text("  %.2f", std::stof(p.ticker.price));
            ImGui::TableSetColumnIndex(2);
            if(!p.ticker.price.empty() && !p.ticker.open_24h.empty()) {
                float price = std::stof(p.ticker.price);
                float open_24h = std::stof(p.ticker.open_24h);
                float change = 100.f * (price - open_24h) / open_24h;
                ImGui::TextColored((change > 0) ? Green : Red, "  %.2f%%", change);
            }
            ImGui::TableSetColumnIndex(3);
            if (!p.ticker.volume_24h.empty()) {
                float vol = std::stof(p.ticker.volume_24h);
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
    auto itr = std::find_if(
            selected_products.begin(), selected_products.end(),
            [&](const ProductDetails& a) {
                return a.product == p;
            });
    if (itr == selected_products.end()) {
        websock_ptr->add_channel_product_pair("ticker", p, 0);
        selected_products.emplace_back(p, high_resolution_clock::now(), websock_ptr->get_ticker(p.id));
    }
}

void WatchList::remove_product(const libCTrader::Product &p) {
    websock_ptr->remove_channel_product_pair("ticker", p, 0);
    auto itr = std::find_if(
            selected_products.begin(), selected_products.end(),
            [&](const ProductDetails& a) {
        return a.product == p;
    });
    if (itr != selected_products.end())
        selected_products.erase(itr);
}

WatchList::ProductDetails::ProductDetails(libCTrader::Product p, const high_resolution_clock::time_point &t,
                                          libCTrader::WSTicker tick) : product(std::move(p)), timePoint(t), ticker(std::move(tick)) {}
