//
// Created by constexpr_dog on 3/3/21.
//

#ifndef CTRADER_WEBSOCK_H
#define CTRADER_WEBSOCK_H

#include <cpprest/ws_client.h>
#include "types.h"

namespace libCTrader {
    class Websock {
        web::web_sockets::client::websocket_callback_client client;
        void message_handler(const std::string &msg);
        void send_message(const std::string &msg);
        std::map<std::string, std::vector<Product>> channel_products;
        std::string uri;
        bool connected = false;

    public:
        explicit Websock(std::string uri);
        ~Websock();
        void Connect();
        void Disconnect();
        void add_channel_product_pair(const std::string &channel, const Product &product);
        void remove_channel_product_pair(const std::string &channel, const Product &product);
        void add_channel(const std::string &channel, const std::vector<Product> &products);
        void set_uri(const std::string &uri);
    };
}

#endif //CTRADER_WEBSOCK_H
