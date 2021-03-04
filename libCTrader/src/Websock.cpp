//
// Created by constexpr_dog on 3/3/21.
//

#include "libCTrader/Websock.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace web::websockets::client;

libCTrader::Websock::Websock(std::string uri) : uri(std::move(uri)) {
    client.set_message_handler([this](const websocket_incoming_message& msg) {
        message_handler(msg.extract_string().get());
    });
}

void libCTrader::Websock::send_message(const std::string &msg) {
    if (!connected)
        throw std::runtime_error("Websock::send_message, can't send message without connecting first.");
    websocket_outgoing_message out_msg;
    out_msg.set_utf8_message(msg);
    client.send(out_msg).wait();
}

void libCTrader::Websock::Connect() {
    json o;
    o["type"] = "subscribe";
    for (const auto &product_channel : products_channels) {
        json i;
        i["name"] = product_channel.first;
        for (const auto &product : product_channel.second)
            i["product_ids"].push_back(product.id);
        o["channels"].push_back(i);
    }
    client.connect(web::uri(utility::conversions::to_string_t(uri))).wait();
    connected = true;
    send_message(o.dump());
    std::cout << "WS: Connected\n";
}

void libCTrader::Websock::message_handler(const std::string &msg) {
    std::cout << msg << std::endl;
}

void libCTrader::Websock::Disconnect() {
    std::vector<std::string> channels;
    for (auto &product_channel : products_channels)
        channels.push_back(product_channel.first);
    json o;
    o["type"] = "unsubscribe";
    o["channels"] = channels;
    send_message(o.dump());
    client.close().wait();
    connected = false;
    std::cout << "WS: Disconnected\n";
}

void libCTrader::Websock::add_channel_product_pair(const std::string &channel, const libCTrader::Product &product) {
    auto itr = products_channels.find(channel);
    if (itr != products_channels.end())
        itr->second.push_back(product);
    else
        products_channels[channel] = std::vector<Product>{ product };

    if (connected) {
        json o, i;
        i["name"] = channel;
        i["product_ids"].push_back(product.id);

        o["type"] = "subscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::remove_channel_product_pair(const std::string &channel, const libCTrader::Product &product) {
    auto itr = products_channels.find(channel);
    if (itr == products_channels.end())
        throw std::runtime_error("Can remove a channel product pair that doesn't exist.");
    auto sub_itr = std::find(itr->second.begin(), itr->second.end(), product);
    if (sub_itr == itr->second.end())
        throw std::runtime_error("Can remove a channel product pair that doesn't exist.");
    itr->second.erase(sub_itr);
    if (itr->second.empty())
        products_channels.erase(itr);

    if (connected) {
        json o, i;
        i["name"] = channel;
        i["product_ids"].push_back(product.id);

        o["type"] = "unsubscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::add_channel(const std::string &channel, const std::vector<Product> &products) {
    auto itr = products_channels.find(channel);
    if (itr != products_channels.end()) {
        for (const auto& product : products)
            itr->second.push_back(product);
    } else
        products_channels[channel] = products;

    if (connected) {
        json o, i;
        i["name"] = channel;
        for (const auto& product : products)
            i["product_ids"].push_back(product.id);

        o["type"] = "subscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::set_uri(const std::string &uri) {
    this->uri = uri;
}

libCTrader::Websock::~Websock() {
    if (connected)
        Disconnect();
}
