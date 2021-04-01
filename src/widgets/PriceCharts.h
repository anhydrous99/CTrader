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
    std::vector<double> closing_prices;
    std::vector<double> ema12_prices;
    std::vector<double> ema26_prices;
    std::vector<double> volume;
    std::vector<double> times;
    float max_value = -INFINITY, min_value = INFINITY;
    std::string current_product;

    bool show_EMA12 = false;
    bool show_EMA26 = false;
    bool fit_graph = false;
    int local_granularity = 2;
    int local_graph = 0;

    void update_candle_vector();
public:
    PriceCharts(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product);
    void display_price_charts_window();
    void change_product(const std::string &new_product_id);
};


#endif //CTRADER_PRICECHARTS_H
