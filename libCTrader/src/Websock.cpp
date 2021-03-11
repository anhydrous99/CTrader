//
// Created by constexpr_dog on 3/3/21.
//

#include "libCTrader/Websock.h"
#include <nlohmann/json.hpp>
#include <set>

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
    std::map<std::string, std::vector<Product>> channel_products;
    for (const auto &channel_product_id : channel_product_ids) {
        const auto &product_channel = channel_product_id.first;
        auto itr = channel_products.find(product_channel.first);
        if (itr == channel_products.end())
            channel_products[product_channel.first] = {product_channel.second};
        else
            channel_products[product_channel.first].push_back(product_channel.second);
    }

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
    // std::cout << msg << std::endl;
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
    } else if (j["type"] == "snapshot" && on_lvl2_book_snapshot) {
        std::map<std::string, std::string> bids;
        std::map<std::string, std::string> asks;
        for (const auto& bid : j["bids"])
            bids[bid[0]] = bid[1];
        for (const auto& ask : j["asks"])
            asks[ask[0]] = ask[1];
        LVL2Snapshot snapshot{
            j["product_id"],
            bids,
            asks
        };
        on_lvl2_book_snapshot(snapshot);
    } else if (j["type"] == "l2update" && on_lvl2_book_update) {
        std::vector<std::tuple<std::string, std::string, std::string>> changes;
        for (const auto &change : j["changes"])
            changes.emplace_back(change[0], change[1], change[2]);
        LVL2Update update{
            j["product_id"],
            j["time"],
            changes
        };
        on_lvl2_book_update(update);
    }
}

void libCTrader::Websock::Disconnect() {
    std::set<std::string> channels;
    for (const auto& channel_product_id : channel_product_ids)
        channels.insert(channel_product_id.first.first);
    json o;
    o["type"] = "unsubscribe";
    o["channels"] = channels;
    send_message(o.dump());
    client.close().wait();
    connected = false;
    std::cout << "WS: Disconnected\n";
}

void libCTrader::Websock::add_channel_product_pair(const std::string &channel, const libCTrader::Product &product, int id) {
    bool found = false;
    const auto chpr_pair = std::make_pair(channel, product);
    auto itr = channel_product_ids.find(chpr_pair);
    if (itr != channel_product_ids.end()) {
        channel_product_ids[chpr_pair].push_back(id);
        found = true;
    } else
        channel_product_ids[chpr_pair] = {id};

    if (connected && !found) {
        json o, i;
        i["name"] = channel;
        i["product_ids"].push_back(product.id);

        o["type"] = "subscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::remove_channel_product_pair(const std::string &channel, const libCTrader::Product &product, int id) {
    bool unsub = false;
    auto itr = channel_product_ids.find(std::make_pair(channel, product));
    if (itr != channel_product_ids.end()) {
        auto sub_itr = std::find(itr->second.begin(), itr->second.end(), id);
        if (sub_itr != itr->second.end())
            itr->second.erase(sub_itr);
        if (itr->second.empty()) {
            channel_product_ids.erase(itr);
            unsub = true;
        }
    }

    if (connected && unsub) {
        json o, i;
        i["name"] = channel;
        i["product_ids"].push_back(product.id);

        o["type"] = "unsubscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::add_channel(const std::string &channel, const std::vector<Product> &products, int id) {
    std::vector<Product> to_add;
    for (const auto& product : products) {
        const auto chpr_pair = std::make_pair(channel, product);
        auto itr = channel_product_ids.find(chpr_pair);
        if (itr != channel_product_ids.end()) {
            channel_product_ids[chpr_pair].push_back(id);
        } else {
            channel_product_ids[chpr_pair] = {id};
            to_add.push_back(product);
        }
    }

    if (connected && !to_add.empty()) {
        json o, i;
        i["name"] = channel;
        for (const auto& product : to_add)
            i["product_ids"].push_back(product.id);

        o["type"] = "subscribe";
        o["channels"].push_back(i);
        send_message(o.dump());
    }
}

void libCTrader::Websock::remove_channel(const std::string &channel) {
    bool found = false;
    for (auto & channel_product_id : channel_product_ids) {
        if (channel_product_id.first.first == channel) {
            found = true;
            break;
        }
    }

    if (connected && found) {
        json o;
        o["type"] = "unsubscribe";
        o["channels"] = channel;
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
    auto itr = channel_product_ids.find(std::make_pair(channel, product));
    return itr != channel_product_ids.end();
}

void libCTrader::Websock::on_lvl2_snapshot(const std::function<void(const LVL2Snapshot &)> &handler) {
    on_lvl2_book_snapshot = handler;
}

void libCTrader::Websock::on_lvl2_update(const std::function<void(const LVL2Update &)> &handler) {
    on_lvl2_book_update = handler;
}
