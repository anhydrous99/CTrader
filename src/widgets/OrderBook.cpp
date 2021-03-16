//
// Created by Armando Herrera on 3/7/21.
//

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>
#include "OrderBook.h"
#include "implot.h"
#include "imgui.h"

using namespace std::chrono;

const static ImVec4 Red(1.0f, 0.0f, 0.0f, 1.0f);
const static ImVec4 Green{0.0f, 1.0f, 0.0f, 1.0f};

OrderBook::OrderBook(libCTrader::Websock *websock, libCTrader::Product  product) : websock(websock), current_product(std::move(product)) {
    websock->on_lvl2_snapshot([&](const libCTrader::LVL2Snapshot &snapshot) {
        {
            std::unique_lock lock(bids_mutex);
            bids.clear();
            for (const auto& snap : snapshot.bids) {
                std::shared_lock product_lock(product_mutex);
                bids[snap.first] = snap.second;
            }
        }
        {
            std::unique_lock lock(asks_mutex);
            asks.clear();
            for (const auto& snap : snapshot.asks) {
                std::shared_lock product_lock(product_mutex);
                asks[snap.first] = snap.second;
            }
        }
    });

    websock->on_lvl2_update([&](const libCTrader::LVL2Update &update) {
        for (const auto& change : update.changes) {

            std::shared_lock product_lock(product_mutex);
            std::string price = std::get<1>(change);
            product_lock.unlock();

            const std::string& size = std::get<2>(change);
            if (std::get<0>(change) == "buy") {
                if (std::stoi(size) == 0) {
                    std::unique_lock lock(bids_mutex);
                    bids.erase(price);
                } else {
                    std::unique_lock lock(bids_mutex);
                    bids[price] = size;
                }
            } else {
                if (std::stoi(size) == 0) {
                    std::unique_lock lock(asks_mutex);
                    asks.erase(price);
                } else {
                    std::unique_lock lock(asks_mutex);
                    asks[price] = size;
                }
            }
        }
    });
}

void OrderBook::change_product(const libCTrader::Product &product) {
    websock->remove_channel_product_pair("level2", current_product, 2);
    std::unique_lock product_lock(product_mutex), bids_lock(bids_mutex), asks_lock(asks_mutex);
    current_product = product;
    bids.clear();
    asks.clear();
    websock->add_channel_product_pair("level2", product, 2);
}

std::map<float, float> OrderBook::get_best_bids(int n, int grping) {
    std::shared_lock lock(bids_mutex);
    auto begin = bids.rbegin();
    auto end = bids.rend();
    std::map<float, float> ret;
    for (int i = 0; i < n; i++) {
        if (begin == end)
            break;
        float price = std::stof(begin->first);
        float size = std::stof(begin->second);
        for (int j = 1; j < grping; j++) {
            if (begin == end)
                break;
            size += std::stof(begin->second);
            begin++;
        }
        ret[price] = size;
        begin++;
    }
    return ret;
}

std::map<float, float> OrderBook::get_best_asks(int n, int grping) {
    std::shared_lock lock(asks_mutex);
    auto begin = asks.begin();
    auto end = asks.end();
    std::map<float, float> ret;
    for (int i = 0; i < n; i++) {
        if (begin == end)
            break;
        float price = std::stof(begin->first);
        float size = std::stof(begin->second);
        for (int j = 1; j < grping; j++) {
            if (begin == end)
                break;
            size += std::stof(begin->second);
            begin++;
        }
        ret[price] = size;
        begin++;
    }
    return ret;
}

std::map<double, double> OrderBook::get_best_asks_hist(double stop) {
    std::shared_lock lock(asks_mutex);
    std::map<double, double> ret;
    double last_size = 0;
    for (const auto& ask : asks) {
        if (std::stod(ask.first) > stop)
            break;
        double new_value = std::stod(ask.second) + last_size;
        ret[std::stod(ask.first)] = new_value;
        last_size = new_value;
    }
    return ret;
}

std::map<double, double> OrderBook::get_best_bids_hist(double stop) {
    std::shared_lock lock(bids_mutex);
    std::map<double, double> ret;
    double last_size = 0;
    for (auto itr = bids.rbegin(); itr != bids.rend(); itr++) {
        if (std::stod(itr->first) < stop)
            break;
        double new_value = std::stod(itr->second) + last_size;
        ret[std::stod(itr->first)] = new_value;
        last_size = new_value;
    }
    return ret;
}

double OrderBook::mid_market_price() {
    double best_bid = 0.f;
    double best_ask = 0.f;
    bids_mutex.lock_shared();
    asks_mutex.lock_shared();
    if (!bids.empty() && !asks.empty()) {
        best_bid = std::stod(bids.rbegin()->first);
        best_ask = std::stod(asks.begin()->first);
    }
    bids_mutex.unlock_shared();
    asks_mutex.unlock_shared();
    return (best_bid + best_ask) / 2;
}


bool OrderBook::display_order_book_window() {
    bool close = false;

    if (duration_cast<milliseconds>(high_resolution_clock::now() - last_book_t).count() > 500 || book_first) {
        displayed_bids = get_best_bids(book_count, book_grouping);
        displayed_asks = get_best_asks(book_count, book_grouping);
        last_book_t = high_resolution_clock::now();
        book_first = false;
    }

    ImGui::Begin("Order Book", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::BeginTable("Trade History", 2)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Price");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("| Market Size");
        for (auto itr = displayed_asks.rbegin(); itr != displayed_asks.rend(); itr++) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextColored(Green, "%f", itr->first);
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(Green, "  %f", itr->second);
        }
        ImGui::EndTable();
    }
    if (ImGui::BeginTable("", 2)) {
        for (auto itr = displayed_bids.rbegin(); itr != displayed_bids.rend(); itr++) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextColored(Red, "%f", itr->first);
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(Red, "  %f", itr->second);
        }
        ImGui::EndTable();
    }

    if (ImGui::ArrowButton("##left1", ImGuiDir_Left)) { book_grouping--; }
    ImGui::SameLine(0.0f);
    if (ImGui::ArrowButton("##right1", ImGuiDir_Right)) { book_grouping++; }
    ImGui::SameLine();
    ImGui::Text("Grouping size: %i", book_grouping);

    if (ImGui::ArrowButton("##left2", ImGuiDir_Left)) { book_count = std::max(1, book_count - 1); }
    ImGui::SameLine(0.0f);
    if (ImGui::ArrowButton("##right2", ImGuiDir_Right)) { book_count++; }
    ImGui::SameLine();
    ImGui::Text("Count per side: %i", book_count);

    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}

void OrderBook::display_order_histogram_window() {
    static std::vector<double> x_bids, x_asks, y_bids, y_asks;
    static double xmin, xmax, ymax, mid_mark;
    const static double ymin = 0;
    static int asks_count, bids_count;
    if (duration_cast<milliseconds>(high_resolution_clock::now() - last_hist_t).count() > 800 || hist_first) {
        x_bids.clear();
        x_asks.clear();
        y_bids.clear();
        y_asks.clear();
        mid_mark = mid_market_price();
        auto displayed_hist_bids = get_best_bids_hist(mid_mark - mid_mark / 60);
        auto displayed_hist_asks = get_best_asks_hist(mid_mark + mid_mark / 60);

        bids_count = displayed_hist_bids.size();
        asks_count = displayed_hist_asks.size();
        x_bids.reserve(bids_count);
        y_bids.reserve(bids_count);
        x_asks.reserve(asks_count);
        y_asks.reserve(asks_count);

        auto dhb_itr = displayed_hist_bids.rbegin();
        auto dha_itr = displayed_hist_asks.begin();

        for (int i = 0; i < displayed_hist_bids.size() && i < 500; i++) {
            x_bids.push_back(dhb_itr->first);
            y_bids.push_back(dhb_itr->second);
            dhb_itr++;
        }
        for (int i = 0; i < displayed_hist_asks.size() && i < 500; i++) {
            x_asks.push_back(dha_itr->first);
            y_asks.push_back(dha_itr->second);
            dha_itr++;
        }
        xmin = mid_mark - mid_mark / 60;
        xmax = mid_mark + mid_mark / 60;
        if (!displayed_asks.empty() && !displayed_hist_bids.empty()) {
            double b_max = std::max_element(displayed_hist_bids.begin(), displayed_hist_bids.end(),
                                            [](const auto &a, const auto &b) {
                                                return a.second < b.second;
                                            })->second;
            double a_max = std::max_element(displayed_hist_asks.begin(), displayed_hist_asks.end(),
                                            [](const auto &a, const auto &b) {
                                                return a.second < b.second;
                                            })->second;
            ymax = static_cast<int>(std::max(b_max, a_max));
        } else {
            ymax = 1;
        }
        last_hist_t = high_resolution_clock::now();
        hist_first = false;
    }

    std::stringstream price_ss;
    price_ss << "Price ";
    price_ss << mid_mark;

    ImPlot::CreateContext();
    ImGui::Begin("Order Histogram");
    ImPlot::SetNextPlotLimits(xmin, xmax, ymin, ymax);
    if (ImPlot::BeginPlot("Order Histogram", price_ss.str().c_str(), "Size")) {
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
        if (asks_count != 0) {
            ImPlot::SetNextFillStyle(ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
            ImPlot::PlotShaded("Asks", x_asks.data(), y_asks.data(), asks_count, static_cast<double>(-INFINITY));
        }
        if (bids_count != 0) {
            ImPlot::SetNextFillStyle(ImVec4(0.0f, 1.0f, 0.0f, 0.5f));
            ImPlot::PlotShaded("Bids", x_bids.data(), y_bids.data(), bids_count, static_cast<double>(-INFINITY));
        }
        ImPlot::EndPlot();
    }
    ImPlot::DestroyContext();

    ImGui::End();
}

int OrderBook::get_bids_size() {
    std::shared_lock lock(bids_mutex);
    return bids.size();
}

int OrderBook::get_asks_size() {
    std::shared_lock lock(asks_mutex);
    return asks.size();
}
