//
// Created by Armando Herrera on 3/7/21.
//

#include "OrderBook.h"

OrderBook::OrderBook(libCTrader::Websock *websock) : websock(websock) {
    websock->on_lvl2_snapshot([&](const libCTrader::LVL2Snapshot &snapshot) {
        {
            std::unique_lock lock(bids_mutex);
            bids.clear();
            bids = snapshot.bids;
        }
        {
            std::unique_lock lock(asks_mutex);
            asks.clear();
            asks = snapshot.asks;
        }
    });

    websock->on_lvl2_update([&](const libCTrader::LVL2Update &update) {
        for (const auto& change : update.changes) {
            const std::string& size = std::get<2>(change);
            if (std::get<0>(change) == "buy") {
                if (size == "0") {
                    std::unique_lock lock(bids_mutex);
                    bids.erase(std::get<1>(change));
                } else {
                    std::unique_lock lock(bids_mutex);
                    bids[std::get<1>(change)] = size;
                }
            } else {
                if (size == "0") {
                    std::unique_lock lock(asks_mutex);
                    asks.erase(std::get<1>(change));
                } else {
                    std::unique_lock lock(asks_mutex);
                    asks[std::get<1>(change)] = size;
                }
            }
        }
    });
}
