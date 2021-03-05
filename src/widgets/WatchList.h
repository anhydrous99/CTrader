//
// Created by Armando Herrera on 3/4/21.
//

#ifndef CTRADER_WATCHLIST_H
#define CTRADER_WATCHLIST_H

#include <libCTrader/Websock.h>
#include <utility>
#include <chrono>

class WatchList {
    struct ProductDetails {
        libCTrader::Product product;
        std::chrono::high_resolution_clock::time_point timePoint;
        libCTrader::WSTicker ticker;

        ProductDetails(libCTrader::Product p, const std::chrono::high_resolution_clock::time_point &t,
                       libCTrader::WSTicker tick);
    };


    libCTrader::Websock *websock_ptr;
    std::vector<libCTrader::Product> available_products;
    std::vector<ProductDetails> selected_products;
    bool first = true;

    static std::string aggregator(float num);
public:
    WatchList(libCTrader::Websock *websock, const std::vector<libCTrader::Product> &av_products,
              const std::vector<int> &active);
    bool display_watch_list_window();
    void add_product(const libCTrader::Product &p);
    void remove_product(const libCTrader::Product &p);
};


#endif //CTRADER_WATCHLIST_H
