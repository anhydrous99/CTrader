//
// Created by Armando Herrera on 3/4/21.
//

#ifndef CTRADER_WATCHLIST_H
#define CTRADER_WATCHLIST_H

#include <libCTrader/Websock.h>
#include <unordered_map>
#include <utility>
#include <chrono>

class WatchList {
    libCTrader::Websock *websock_ptr;
    std::vector<libCTrader::Product> available_products;
    std::vector<libCTrader::Product> selected_products;
    std::unordered_map<std::string, libCTrader::WSTicker> latest_tickers;
    std::shared_mutex selected_products_mut, latest_tickers_mut;
    bool first = true;

    static std::string aggregator(float num);
    void add_product(const libCTrader::Product &p);
    void remove_product(const libCTrader::Product &p);
public:
    WatchList(libCTrader::Websock *websock, std::vector<libCTrader::Product> available_products,
              std::vector<libCTrader::Product> sel_products);
    bool display_watch_list_window();
};


#endif //CTRADER_WATCHLIST_H
