//
// Created by arman on 3/17/2021.
//

#ifndef CTRADER_PRICEGRAPH_H
#define CTRADER_PRICEGRAPH_H

#include <libCTrader/Api.h>
#include <libCTrader/Websock.h>


class PriceGraph {
    libCTrader::Websock *websock;
    libCTrader::Api *api;
    std::vector<libCTrader::Candle> candles;

    void update_candle_vector();
public:
    PriceGraph(libCTrader::Api *api, libCTrader::Websock *websock);

    void display_price_graph_window();
};


#endif //CTRADER_PRICEGRAPH_H
