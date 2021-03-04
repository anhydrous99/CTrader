//
// Created by constexpr_dog on 3/3/21.
//

#include "libCTrader/Websock.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace web::websockets::client;

libCTrader::Websock::Websock(libCTrader::Product product, std::vector<std::string> channels, std::string uri) :
    current_product(std::move(product)), channels(std::move(channels)), uri(std::move(uri)) {
    client.set_message_handler([this](const websocket_incoming_message& msg) {
        message_handler(msg.extract_string().get());
    });
}

void libCTrader::Websock::send_message(const std::string &msg) {
    websocket_outgoing_message out_msg;
    out_msg.set_utf8_message(msg);
    client.send(out_msg).wait();
}

void libCTrader::Websock::Connect() {
    client.connect(web::uri(utility::conversions::to_string_t(uri))).wait();
    std::string sub = get_sub_string(true);
    send_message(sub);
}

void libCTrader::Websock::message_handler(const std::string &msg) {
    std::cout << msg << std::endl;
}

void libCTrader::Websock::Disconnect() {
    std::string unsub = get_sub_string(false);
    send_message(unsub);
    client.close().wait();
}

std::string libCTrader::Websock::get_sub_string(bool sub) {
    json o;
    o["type"] = sub ? "subscribe" : "unsubscribe";
    o["product_ids"] = {current_product.id};
    o["channels"] = channels;
    return o.dump();
}

void libCTrader::Websock::add_channel(const std::string &channel) {
    json o;
    o["type"] = "subscribe";
    o["product_ids"] = {current_product.id};
    o["channels"] = {channel};
    channels.push_back(channel);
    send_message(o.dump());
}

void libCTrader::Websock::remove_channel(const std::string &channel) {
    auto itr = std::find(channels.begin(), channels.end(), channel);
    if (itr == channels.end())
        throw std::runtime_error("Websock, can't remove channel that doesn't exist.");
    json o;
    o["type"] = "unsubscribe";
    o["channels"] = {channel};
    channels.erase(itr);
    send_message(o.dump());
}

void libCTrader::Websock::change_product(const libCTrader::Product &product) {
    send_message(get_sub_string(false));
    current_product = product;
    send_message(get_sub_string(true));
}
