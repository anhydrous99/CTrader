//
// Created by Armando Herrera on 3/7/21.
//

#ifndef CTRADER_ORDERBOOK_H
#define CTRADER_ORDERBOOK_H

#include <libCTrader/Websock.h>
#include <chrono>

class OrderBook {
    struct cmpByStringDoubleValue {
        bool operator()(const std::string& a, const std::string& b) const {
            return std::stod(a) < std::stod(b);
        }
    };

    libCTrader::Websock *websock;
    libCTrader::Product current_product;
    std::map<std::string, std::string, cmpByStringDoubleValue> bids;
    std::map<std::string, std::string, cmpByStringDoubleValue> asks;
    std::shared_mutex bids_mutex;
    std::shared_mutex asks_mutex;
    std::shared_mutex product_mutex;
    std::chrono::high_resolution_clock::time_point last_book_t = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point last_hist_t = std::chrono::high_resolution_clock::now();

    std::map<float, float> displayed_bids;
    std::map<float, float> displayed_asks;
    int book_grouping = 1;
    int book_count = 17;
    bool book_first = true;
    bool hist_first = true;

    std::map<float, float> get_best_bids(int n, int grping);
    std::map<float, float> get_best_asks(int n, int grping);
    std::map<double, double> get_best_asks_hist(double stop);
    std::map<double, double> get_best_bids_hist(double stop);
    double mid_market_price();
    int get_bids_size();
    int get_asks_size();

public:
    explicit OrderBook(libCTrader::Websock *websock, libCTrader::Product product);
    void change_product(const libCTrader::Product &product);

    void display_order_book_window();
    void display_order_histogram_window();
};


#endif //CTRADER_ORDERBOOK_H
