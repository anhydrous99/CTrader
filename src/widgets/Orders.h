//
// Created by constexpr_dog on 4/4/21.
//

#ifndef CTRADER_ORDERS_H
#define CTRADER_ORDERS_H

#include <libCTrader/Api.h>

class Orders {
    libCTrader::Api* api;
    int buy_sell = 0;

public:
    explicit Orders(libCTrader::Api *api);
    void display_orders_window();
};


#endif //CTRADER_ORDERS_H
