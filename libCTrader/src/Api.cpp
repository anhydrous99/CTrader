//
// Created by Armando Herrera on 3/1/21.
//

#include "libCTrader/Api.h"
#include <thread>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

auto parse_json(const std::string &input_string) {
    auto ret = json::parse(input_string);
    if (ret.count("message") == 1)
        throw std::runtime_error(ret["message"].get<std::string>());
    return ret;
}

std::string libCTrader::Api::build_url_args(const std::map<std::string, std::string> &args) {
    std::string output;
    if (!args.empty())
        output += "?";
    for (const auto& arg : args) {
        output += arg.first;
        output += "=";
        output += arg.second;
        output += "&";
    }
    if (!args.empty())
        output.pop_back();
    return output;
}

libCTrader::Api::Api(std::string uri, Auth *auth) {
    client = new Client(auth, uri);
    built_client = true;
}

libCTrader::Api::Api(Client *client) : client(client) {}

libCTrader::Api::~Api() {
    if (built_client)
        delete client;
}

std::vector<libCTrader::Account> libCTrader::Api::accounts() {
    auto j = parse_json(client->call("GET", true, "/accounts"));
    std::vector<Account> accounts;
    for (const auto& acc : j) {
        accounts.emplace_back(
                acc["id"].get<std::string>(),
                acc["currency"].get<std::string>(),
                acc["balance"].get<std::string>(),
                acc["available"].get<std::string>(),
                acc["hold"].get<std::string>(),
                acc["profile_id"].get<std::string>(),
                acc["trading_enabled"].get<bool>());
    }
    return accounts;
}

libCTrader::Account libCTrader::Api::account(const std::string& account_id) {
    auto j = parse_json(client->call("GET", true, "/accounts/" + account_id));
    return Account(j["id"].get<std::string>(),
                   j["currency"].get<std::string>(),
                   j["balance"].get<std::string>(),
                   j["available"].get<std::string>(),
                   j["hold"].get<std::string>(),
                   "",
                   true);
}

std::vector<libCTrader::Account_Ledger_Entry> libCTrader::Api::account_ledger(const std::string &account_id) {
    auto j = parse_json(client->call("GET", true, "/accounts/" + account_id + "/ledger"));
    std::vector<Account_Ledger_Entry> ledger;
    for (const auto& j_e : j) {
        ledger.emplace_back(
                j_e["id"],
                j_e["created_at"],
                j_e["amount"],
                j_e["balance"],
                j_e["type"]);
    }
    return ledger;
}

std::string libCTrader::Api::account_holds(const std::string &account_id) {
    auto j = parse_json(client->call("GET", true, "/accounts/" + account_id + "/holds"));
    return j.dump(2);
}

libCTrader::Order
libCTrader::Api::place_market_order(const std::string &product_id, const std::string &side, const std::string &size,
                                    const std::string &funds) {
    json o;
    o["type"] = "market";
    o["side"] = side;
    o["product_id"] = product_id;
    if (size.empty() ^ funds.empty()) {
        if (!size.empty())
            o["size"] = size;
        if (!funds.empty())
            o["funds"] = funds;
    } else
        throw std::runtime_error("Either size or funds must be set.");
    auto j = parse_json(client->call("POST", true, "/orders", o.dump()));
    return {
        j["id"],
        j.count("price") == 0 ?  "" : j["price"],
        j.count("size") == 0 ? "" : j["size"],
        j["product_id"],
        j["side"],
        j["stp"],
        j.count("funds") == 0 ? "" : j["funds"],
        j.count("specific_funds") == 0 ? "" : j["specific_funds"],
        j["type"],
        j["time_in_force"],
        j["post_only"],
        j["created_at"],
        j.count("fill_fees") == 0 ? "" : j["fill_fees"],
        j.count("fill_size") == 0 ? "" : j["fill_size"],
        j.count("executed_value") == 0 ? "" : j["executed_value"],
        j["status"],
        j["settled"]
    };
}

libCTrader::Order
libCTrader::Api::place_limit_order(const std::string &product_id, const std::string &side, const std::string &price,
                                   const std::string &size) {
    json o;
    o["type"] = "limit";
    o["product_id"] = product_id;
    o["side"] = side;
    o["price"] = price;
    o["size"] = size;
    o["post_only"] = true;
    auto j = parse_json(client->call("POST", true, "/orders", o.dump()));
    return {
            j["id"],
            j.count("price") == 0 ?  "" : j["price"],
            j.count("size") == 0 ? "" : j["size"],
            j["product_id"],
            j["side"],
            j["stp"],
            j.count("funds") == 0 ? "" : j["funds"],
            j.count("specific_funds") == 0 ? "" : j["specific_funds"],
            j["type"],
            j["time_in_force"],
            j["post_only"],
            j["created_at"],
            j.count("fill_fees") == 0 ? "" : j["fill_fees"],
            j.count("fill_size") == 0 ? "" : j["fill_size"],
            j.count("executed_value") == 0 ? "" : j["executed_value"],
            j["status"],
            j["settled"]
    };
}

libCTrader::Order
libCTrader::Api::place_stop_order(const std::string &product_id, const std::string &side, const std::string &price,
                                  const std::string &size, const std::string &funds) {
    json o;
    o["type"] = "stop";
    o["side"] = side;
    o["product_id"] = product_id;
    o["price"] = price;
    if (size.empty() ^ funds.empty()) {
        if (!size.empty())
            o["size"] = size;
        if (!funds.empty())
            o["funds"] = funds;
    } else
        throw std::runtime_error("Either size or funds must be set.");
    auto j = parse_json(client->call("POST", true, "/orders", o.dump()));
    return {
            j["id"],
            j.count("price") == 0 ?  "" : j["price"],
            j.count("size") == 0 ? "" : j["size"],
            j["product_id"],
            j["side"],
            j["stp"],
            j.count("funds") == 0 ? "" : j["funds"],
            j.count("specific_funds") == 0 ? "" : j["specific_funds"],
            j["type"],
            j["time_in_force"],
            j["post_only"],
            j["created_at"],
            j.count("fill_fees") == 0 ? "" : j["fill_fees"],
            j.count("fill_size") == 0 ? "" : j["fill_size"],
            j.count("executed_value") == 0 ? "" : j["executed_value"],
            j["status"],
            j["settled"]
    };
}

void libCTrader::Api::cancel_all_orders(const std::string *product_id) {
    std::string path = "/orders";
    if (product_id != nullptr)
        path += "?product_id" + *product_id;
    client->call("DELETE", true, path);
}

std::vector<libCTrader::Order> libCTrader::Api::list_orders(const std::string *status, const std::string *product_id) {
    std::string path = "/orders";
    std::map<std::string, std::string> args;
    if (status != nullptr)
        args["status"] = *status;
    if (product_id != nullptr)
        args["product_id"] = *product_id;
    path += build_url_args(args);
    auto json = parse_json(client->call("GET", true, path));

    std::vector<Order> output;
    for (const auto& j : json) {
        output.emplace_back(
                j["id"],
                j.count("price") == 0 ?  "" : j["price"],
                j.count("size") == 0 ? "" : j["size"],
                j["product_id"],
                j["side"],
                j["stp"],
                j.count("funds") == 0 ? "" : j["funds"],
                j.count("specific_funds") == 0 ? "" : j["specific_funds"],
                j["type"],
                j["time_in_force"],
                j["post_only"],
                j["created_at"],
                j.count("fill_fees") == 0 ? "" : j["fill_fees"],
                j.count("fill_size") == 0 ? "" : j["fill_size"],
                j.count("executed_value") == 0 ? "" : j["executed_value"],
                j["status"],
                j["settled"]
                );
    }
    return output;
}

libCTrader::Order libCTrader::Api::get_order(const std::string &order_id) {
    auto j = parse_json(client->call("GET", true, "/orders/" + order_id));
    return {
            j["id"],
            j.count("price") == 0 ?  "" : j["price"],
            j.count("size") == 0 ? "" : j["size"],
            j["product_id"],
            j["side"],
            j["stp"],
            j.count("funds") == 0 ? "" : j["funds"],
            j.count("specific_funds") == 0 ? "" : j["specific_funds"],
            j["type"],
            j["time_in_force"],
            j["post_only"],
            j["created_at"],
            j.count("fill_fees") == 0 ? "" : j["fill_fees"],
            j.count("fill_size") == 0 ? "" : j["fill_size"],
            j.count("executed_value") == 0 ? "" : j["executed_value"],
            j["status"],
            j["settled"]
    };
}

std::vector<libCTrader::Fill> libCTrader::Api::list_fills(const std::string *order_id, const std::string *product_id) {
    std::map<std::string, std::string> args;
    if (order_id != nullptr)
        args["order_id"] = *order_id;
    if (product_id != nullptr)
        args["product_id"] = *product_id;
    std::string path = "/fills" + build_url_args(args);
    auto json = parse_json(client->call("GET", true, path));
    std::vector<Fill> fills;
    for (const auto& j : json) {
        fills.emplace_back(
                j["trade_id"],
                j["product_id"],
                j["price"],
                j["size"],
                j["order_id"],
                j["created_at"],
                j["liquidity"],
                j["fee"],
                j["settled"],
                j["side"]);
    }
    return fills;
}

std::string libCTrader::Api::exchange_limits() {
    return client->call("GET", true, "/users/self/exchange-limits");
}

std::vector<libCTrader::Product> libCTrader::Api::get_products() {
    json json1 = parse_json(client->call("GET", false, "/products"));
    std::vector<Product> products;
    for (const auto& j : json1) {
        products.emplace_back(
                j["id"],
                j["display_name"],
                j["base_currency"],
                j["quote_currency"],
                j["base_increment"],
                j["quote_increment"],
                j["base_min_size"],
                j["base_max_size"],
                j["min_market_funds"],
                j["max_market_funds"],
                j["status"],
                j["status_message"],
                j["cancel_only"],
                j["limit_only"],
                j["post_only"],
                j["trading_disabled"]
                );
    }
    std::sort(products.begin(), products.end());
    return products;
}

libCTrader::Product libCTrader::Api::get_product(const std::string &product_id) {
    json j = parse_json(client->call("GET", false, "/products/" + product_id));
    return {
            j["id"],
            j["display_name"],
            j["base_currency"],
            j["quote_currency"],
            j["base_increment"],
            j["quote_increment"],
            j["base_min_size"],
            j["base_max_size"],
            j["min_market_funds"],
            j["max_market_funds"],
            j["status"],
            j["status_message"],
            j["cancel_only"],
            j["limit_only"],
            j["post_only"],
            j["trading_disabled"]
    };
}

std::map<std::string, libCTrader::Page> libCTrader::Api::get_order_book(const std::string &product_id, int level) {
    json j = parse_json(client->call("GET", false,
                              "/products/" + product_id + "/book?level=" + std::to_string(level)));
    std::map<std::string, Page> output;
    auto bids = j["bids"];
    auto asks = j["asks"];
    for (const auto& bid : bids) {
        output[bid[0].get<std::string>()] = {bid[0], bid[1], "bids", bid[3]};
    }
    for (const auto& ask : asks)
        output[ask[0].get<std::string>()] = {ask[0], ask[1], "asks", ask[3]};
    return output;
}

std::map<std::string, libCTrader::PageLVL3> libCTrader::Api::get_lvl3_order_book(const std::string &product_id) {
    json j = parse_json(client->call("GET", false,
                              "/products/" + product_id + "/book?level=3"));
    std::map<std::string, PageLVL3> output;
    auto bids = j["bids"];
    auto asks = j["asks"];
    for (const auto& bid : bids)
        output[bid[0].get<std::string>()] = {bid[0], bid[1], "bids", bid[3]};
    for (const auto& ask : asks)
        output[ask[0].get<std::string>()] = {ask[0], ask[1], "asks", ask[3]};
    return output;
}

libCTrader::Ticker libCTrader::Api::get_product_ticker(const std::string &product_id) {
    json j = parse_json(client->call("GET", false, "/products/" + product_id + "/ticker"));
    return {
        j["trade_id"],
        j["price"],
        j["size"],
        j["bid"],
        j["ask"],
        j["volume"],
        j["time"]
    };
}

std::vector<libCTrader::Trade> libCTrader::Api::list_latest_trades(const std::string &product_id) {
    json json1 = parse_json(client->call("GET", false, "/products" + product_id + "/products"));
    std::vector<Trade> output;
    for (const auto& j : json1) {
        output.emplace_back(
                j["time"],
                j["trade_id"],
                j["price"],
                j["size"],
                j["side"]
                );
    }
    return output;
}

std::vector<libCTrader::Candle>
libCTrader::Api::get_historical_candles(const std::string &product_id, const std::string &start, const std::string &end,
                                      int granularity) {
    std::string path = "/products/" + product_id + "/candles?start=" + start + "&end=" + end + "&granularity=" +
            std::to_string(granularity);
    json json1 = parse_json(client->call("GET", false, path));
    std::vector<Candle> candles;
    for (const auto& j : json1)
        candles.emplace_back(j[0], j[1], j[2], j[3], j[4], j[5]);
    return candles;
}

std::vector<libCTrader::Candle>
libCTrader::Api::get_historical_candles(const std::string &product_id, const ptime &start, const ptime &end,
                                        int granularity) {
    return get_historical_candles(product_id, boost::posix_time::to_iso_string(start), boost::posix_time::to_iso_string(end), granularity);
}

std::string libCTrader::Api::get_24hr_stats(const std::string &product_id) {
    return client->call("GET", false, "/products/" + product_id + "/stats");
}

std::vector<libCTrader::Candle> libCTrader::Api::get_latest_historical_candles(const std::string &product_id, int granularity) {
    json json1 = parse_json(client->call("GET", false, "/products/" + product_id + "/candles?granularity=" + std::to_string(granularity)));
    std::vector<Candle> candles;
    for (const auto& j : json1)
        candles.emplace_back(j[0], j[1], j[2], j[3], j[4], j[5]);
    return candles;
}

void libCTrader::Api::set_auth(libCTrader::Auth *au) {
    client->set_auth(au);
}

void libCTrader::Api::set_uri(std::string u) {
    client->set_uri(std::move(u));
}
