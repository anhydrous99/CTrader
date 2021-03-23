//
// Created by arman on 3/17/2021.
//

#ifndef CTRADER_PRICEGRAPH_H
#define CTRADER_PRICEGRAPH_H

#include <libCTrader/Api.h>
#include <libCTrader/Websock.h>


class PriceGraph {
    libCTrader::Api *api;
    libCTrader::Websock *websock;
    boost::posix_time::time_duration granularity;
    boost::posix_time::time_duration interval_duration;
    std::vector<libCTrader::Candle> candles;
    std::string current_product;

    void update_candle_vector();
public:
    PriceGraph(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product);

    void display_price_graph_window();
};


#endif //CTRADER_PRICEGRAPH_H
