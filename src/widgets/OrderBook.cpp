//
// Created by Armando Herrera on 3/7/21.
//

#include "OrderBook.h"
#include "imgui.h"

OrderBook::OrderBook(libCTrader::Websock *websock, libCTrader::Product product) : websock(websock), current_product(std::move(product)) {
    websock->on_lvl2_snapshot([&](const libCTrader::LVL2Snapshot &snapshot) {
        {
            std::unique_lock lock(bids_mutex);
            bids.clear();
            for (const auto& snap : snapshot.bids) {
                std::shared_lock product_lock(product_mutex);
                bids[libCTrader::Decimal(snap.first, current_product.quote_increment)] = snap.second;
            }
        }
        {
            std::unique_lock lock(asks_mutex);
            asks.clear();
            for (const auto& snap : snapshot.asks) {
                std::shared_lock product_lock(product_mutex);
                asks[libCTrader::Decimal(snap.first, current_product.quote_increment)] = snap.second;
            }
        }
    });

    websock->on_lvl2_update([&](const libCTrader::LVL2Update &update) {
        for (const auto& change : update.changes) {

            std::shared_lock product_lock(product_mutex);
            libCTrader::Decimal price(std::get<1>(change), current_product.quote_increment);
            product_lock.unlock();

            const std::string& size = std::get<2>(change);
            if (std::get<0>(change) == "buy") {
                if (size == "0") {
                    std::unique_lock lock(bids_mutex);
                    bids.erase(price);
                } else {
                    std::unique_lock lock(bids_mutex);
                    bids[price] = size;
                }
            } else {
                if (size == "0") {
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

std::map<float, float> OrderBook::get_best_bids(int n, int grouping) {
    std::shared_lock lock(bids_mutex);
    auto begin = bids.begin();
    auto end = bids.end();
    std::map<float, float> ret;
    for (int i = 0; i < n; i++) {
        if (begin == end)
            break;
        float price = begin->first.get_flt();
        float size = std::stof(begin->second);
        for (int j = 1; j < grouping; j++) {
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

std::map<float, float> OrderBook::get_best_asks(int n, int grouping) {
    std::shared_lock lock(asks_mutex);
    auto begin = asks.rbegin();
    auto end = asks.rend();
    std::map<float, float> ret;
    for (int i = 0; i < n; i++) {
        if (begin == end)
            break;
        float price = begin->first.get_flt();
        float size = std::stof(begin->second);
        for (int j = 1; j < grouping; j++) {
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

bool OrderBook::display_orderbook_window() {
    bool close = false;
    if (ImGui::Button("Close"))
        close = true;
    return close;
}
