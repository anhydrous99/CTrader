//
// Created by Armando Herrera on 3/4/21.
//

#ifndef CTRADER_WATCHLIST_H
#define CTRADER_WATCHLIST_H

#include <libCTrader/Websock.h>

class WatchList {
    libCTrader::Websock *websock_ptr;
    std::vector<libCTrader::Product> available_products;
    std::vector<libCTrader::Product> selected_products;

public:
    WatchList(libCTrader::Websock *websock, const std::vector<libCTrader::Product> &av_products);
    bool display_watch_list_window();
};


#endif //CTRADER_WATCHLIST_H
