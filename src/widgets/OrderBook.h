//
// Created by Armando Herrera on 3/7/21.
//

#ifndef CTRADER_ORDERBOOK_H
#define CTRADER_ORDERBOOK_H

#include <libCTrader/Websock.h>
#include <libCTrader/Decimal.h>
#include <chrono>

class OrderBook {
    libCTrader::Websock *websock;
    libCTrader::Product current_product;
    std::map<libCTrader::Decimal, std::string> bids;
    std::map<libCTrader::Decimal, std::string> asks;
    std::shared_mutex bids_mutex;
    std::shared_mutex asks_mutex;
    std::shared_mutex product_mutex;
    std::chrono::high_resolution_clock::time_point last_t = std::chrono::high_resolution_clock::now();

    std::map<float, float> displayed_bids;
    std::map<float, float> displayed_asks;
    int grouping = 1;
    int count = 10;

public:
    explicit OrderBook(libCTrader::Websock *websock, libCTrader::Product product);
    void change_product(const libCTrader::Product &product);

    std::map<float, float> get_best_bids(int n, int grouping);
    std::map<float, float> get_best_asks(int n, int grouping);

    bool display_orderbook_window();
};


#endif //CTRADER_ORDERBOOK_H
