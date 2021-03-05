//
// Created by Armando Herrera on 3/5/21.
//

#ifndef CTRADER_TRADEHISTORY_H
#define CTRADER_TRADEHISTORY_H

#include <libCTrader/Websock.h>
#include "../MaxQueue.h"

class TradeHistory {
    libCTrader::Websock* websock;
    libCTrader::Product current_product;
    MaxQueue<libCTrader::WSTicker> trade_history;

public:
    explicit TradeHistory(libCTrader::Websock* sock, const libCTrader::Product &product);
    void change_product(const libCTrader::Product &new_product);
    void display_trade_history_window();
};


#endif //CTRADER_TRADEHISTORY_H
