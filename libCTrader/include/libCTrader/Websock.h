//
// Created by constexpr_dog on 3/3/21.
//

#ifndef CTRADER_WEBSOCK_H
#define CTRADER_WEBSOCK_H

#include <cpprest/ws_client.h>
#include <functional>
#include <shared_mutex>
#include "types.h"

namespace libCTrader {
    class Websock {
        web::web_sockets::client::websocket_callback_client client;
        void message_handler(const std::string &msg);
        void send_message(const std::string &msg);
        std::map<std::string, std::vector<Product>> channel_products;
        std::function<void(WSTicker)> on_ticker;
        std::string uri;
        bool connected = false;

        std::map<std::string, WSTicker> tickers;
        std::shared_mutex tickers_mutex;

    public:
        explicit Websock(std::string uri);
        ~Websock();
        void Connect();
        void Disconnect();
        void add_channel_product_pair(const std::string &channel, const Product &product);
        void remove_channel_product_pair(const std::string &channel, const Product &product);
        void add_channel(const std::string &channel, const std::vector<Product> &products);
        void on_new_ticker(const std::function<void(WSTicker)>& fun);
        std::map<std::string, WSTicker> get_tickers();
        WSTicker get_ticker(const std::string &product_id);
        void set_uri(const std::string &uri);
    };
}

#endif //CTRADER_WEBSOCK_H
