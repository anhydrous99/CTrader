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
    for (const auto &product_channel : channel_products) {
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
    auto j = json::parse(msg);
    std::cout << msg << std::endl;
    if (j["type"] == "ticker" && j.count("sequence") == 1) {
        std::string product_id = j["product_id"];
        WSTicker ticker{
                j["trade_id"],
                j["sequence"],
                j["time"],
                j["product_id"],
                j["price"],
                j["side"],
                j["last_size"],
                j["best_bid"],
                j["best_ask"],
                j["open_24h"],
                j["volume_24h"],
                j["low_24h"],
                j["high_24h"],
                j["volume_30d"]
        };
        {
            std::unique_lock lock(tickers_mutex);
            tickers[product_id] = ticker;
        }
        if (on_ticker)
            on_ticker(ticker);
    }
}

void libCTrader::Websock::Disconnect() {
    std::vector<std::string> channels;
    for (auto &product_channel : channel_products)
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
    auto itr = channel_products.find(channel);
    bool found = false;
    if (itr != channel_products.end()) {
        auto sub_itr = std::find(itr->second.begin(), itr->second.end(), product);
        if (sub_itr != itr->second.end())
            found = true;
        else
            itr->second.push_back(product);
    } else
        channel_products[channel] = std::vector<Product>{product};

    if (connected && !found) {
        json o, i;
        i["name"] = channel;
        i["product_ids"].push_back(product.id);

        o["type"] = "subscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::remove_channel_product_pair(const std::string &channel, const libCTrader::Product &product) {
    auto itr = channel_products.find(channel);
    if (itr == channel_products.end())
        throw std::runtime_error("Can remove a channel product pair that doesn't exist.");
    auto sub_itr = std::find(itr->second.begin(), itr->second.end(), product);
    if (sub_itr == itr->second.end())
        throw std::runtime_error("Can remove a channel product pair that doesn't exist.");
    itr->second.erase(sub_itr);
    if (itr->second.empty())
        channel_products.erase(itr);

    if (connected && itr != channel_products.end()) {
        json o, i;
        i["name"] = channel;
        i["product_ids"].push_back(product.id);

        o["type"] = "unsubscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::add_channel(const std::string &channel, const std::vector<Product> &products) {
    auto itr = channel_products.find(channel);
    if (itr != channel_products.end()) {
        for (const auto& product : products)
            itr->second.push_back(product);
    } else
        channel_products[channel] = products;

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

void libCTrader::Websock::set_uri(const std::string &u) {
    this->uri = u;
}

libCTrader::Websock::~Websock() {
    if (connected)
        Disconnect();
}

libCTrader::WSTicker libCTrader::Websock::get_ticker(const std::string &product_id) {
    std::shared_lock lock(tickers_mutex);
    auto itr = tickers.find(product_id);
    if (itr == tickers.end())
        return WSTicker();
    else
        return itr->second;
}

std::map<std::string, libCTrader::WSTicker> libCTrader::Websock::get_tickers() {
    std::shared_lock lock(tickers_mutex);
    return tickers;
}

void libCTrader::Websock::on_new_ticker(const std::function<void(const WSTicker &)> &handler) {
    on_ticker = handler;
}

bool libCTrader::Websock::is_connected(const std::string &channel, const libCTrader::Product &product) {
    auto itr = channel_products.find(channel);
    if (itr == channel_products.end())
        return false;
    auto sub_itr = std::find(channel_products[channel].begin(), channel_products[channel].end(), product);
    if (sub_itr == channel_products[channel].end())
        return false;
    return true;
}
