//
// Created by Armando Herrera on 3/7/21.
//

#ifndef CTRADER_ORDERBOOK_H
#define CTRADER_ORDERBOOK_H

#include <libCTrader/Websock.h>
#include <libCTrader/Decimal.h>

class OrderBook {
    libCTrader::Websock *websock;
    libCTrader::Product current_product;
    std::map<libCTrader::Decimal, std::string> bids;
    std::map<libCTrader::Decimal, std::string> asks;
    std::shared_mutex bids_mutex;
    std::shared_mutex asks_mutex;
    std::shared_mutex product_mutex;

public:
    explicit OrderBook(libCTrader::Websock *websock, libCTrader::Product product);
    void change_product(const libCTrader::Product &product);
};


#endif //CTRADER_ORDERBOOK_H
