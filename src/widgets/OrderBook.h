//
// Created by Armando Herrera on 3/7/21.
//

#ifndef CTRADER_ORDERBOOK_H
#define CTRADER_ORDERBOOK_H

#include <libCTrader/Websock.h>

class OrderBook {
    libCTrader::Websock *websock;
    std::map<std::string, std::string> bids;
    std::map<std::string, std::string> asks;
    std::shared_mutex bids_mutex;
    std::shared_mutex asks_mutex;

public:
    explicit OrderBook(libCTrader::Websock *websock);
};


#endif //CTRADER_ORDERBOOK_H
