//
// Created by arman on 3/17/2021.
//

#ifndef CTRADER_PRICECHARTS_H
#define CTRADER_PRICECHARTS_H

#include <libCTrader/Api.h>
#include <libCTrader/Websock.h>
#include <map>


class PriceCharts {
    libCTrader::Api *api;
    libCTrader::Websock *websock;
    boost::posix_time::time_duration granularity;
    std::map<uint64_t, libCTrader::Candle> candles;
    float max_value = -INFINITY, min_value = INFINITY;
    std::string current_product;

    bool show_EMA12 = false;
    bool show_EMA26 = false;
    int local_granularity = 2;
    int local_graph = 0;

    void update_candle_vector();
public:
    PriceCharts(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product);

    void display_price_charts_window();
};


#endif //CTRADER_PRICECHARTS_H
