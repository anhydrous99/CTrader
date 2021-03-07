//
// Created by constexpr_dog on 3/3/21.
//

#ifndef CTRADER_WEBSOCK_H
#define CTRADER_WEBSOCK_H

#include <cpprest/ws_client.h>
#include <map>
#include <functional>
#include <shared_mutex>
#include "types.h"

namespace libCTrader {
    class Websock {
        web::web_sockets::client::websocket_callback_client client;
        void message_handler(const std::string &msg);
        void send_message(const std::string &msg);
        std::map<std::pair<std::string, Product>, std::vector<int>> channel_product_ids;
        std::function<void(const WSTicker &)> on_ticker;
        std::function<void(const LVL2Snapshot&)> on_lvl2_book_snapshot;
        std::function<void(const LVL2Update&)> on_lvl2_book_update;
        std::string uri;
        bool connected = false;

        std::map<std::string, WSTicker> tickers;
        std::shared_mutex tickers_mutex;

    public:
        explicit Websock(std::string uri);
        ~Websock();
        void Connect();
        void Disconnect();
        void add_channel_product_pair(const std::string &channel, const Product &product, int id=0);
        void remove_channel_product_pair(const std::string &channel, const Product &product, int id=0);
        void add_channel(const std::string &channel, const std::vector<Product> &products, int id=0);
        void remove_channel(const std::string &channel);
        void on_new_ticker(const std::function<void(const WSTicker&)> &handler);
        void on_lvl2_snapshot(const std::function<void(const LVL2Snapshot&)> &handler);
        void on_lvl2_update(const std::function<void(const LVL2Update&)> &handler);
        bool is_connected(const std::string &channel, const Product &product);
        std::map<std::string, WSTicker> get_tickers();
        WSTicker get_ticker(const std::string &product_id);
        void set_uri(const std::string &u);
    };
}

#endif //CTRADER_WEBSOCK_H
