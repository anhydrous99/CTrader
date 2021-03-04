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
        std::string get_sub_string(bool sub);
        void message_handler(const std::string &msg);
        void send_message(const std::string &msg);

        std::vector<std::string> channels;
        Product current_product;
        std::string uri;

    public:
        Websock(Product product, std::vector<std::string> channels, std::string uri);
        void Connect();
        void Disconnect();
        void add_channel(const std::string &channel);
        void remove_channel(const std::string &channel);
        void change_product(const Product &product);
    };
}

#endif //CTRADER_WEBSOCK_H
