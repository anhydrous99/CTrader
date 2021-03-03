//
// Created by Armando Herrera on 3/1/21.
//

#include "libCTrader/Api.h"
#include <iostream>
#include <utility>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/* Used by API::Call to put websource into a string type */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string libCTrader::Api::call(const std::string &method, bool authed, const std::string &path, const std::string &body) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = nullptr;
        curl_easy_setopt(curl, CURLOPT_URL, (uri + path).c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl/7.75.0");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        chunk = curl_slist_append(chunk, "Content-Type: application/json");
        if (authed) {
            std::string time_stamp = GetTimestamp();
            std::string sign = auth->Sign(time_stamp, method, path, body);
            chunk = curl_slist_append(chunk, ("CB-ACCESS-KEY: " + auth->Key).c_str());
            chunk = curl_slist_append(chunk, ("CB-ACCESS-SIGN: " + sign).c_str());
            chunk = curl_slist_append(chunk, ("CB-ACCESS-TIMESTAMP: " + time_stamp).c_str());
            chunk = curl_slist_append(chunk, ("CB-ACCESS-PASSPHRASE: " + auth->Passphrase).c_str());
        }
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        /* always cleanup */
        curl_easy_cleanup(curl);
        /* free the custom headers */
        curl_slist_free_all(chunk);
    }
    return readBuffer;
}

std::string libCTrader::Api::call(const std::string &method, bool authed, const std::string &path) {
    return call(method, authed, path, "");
}

std::string libCTrader::Api::GetTimestamp() {
    time_t t = time(nullptr);
    return std::to_string(t);
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

libCTrader::Api::Api(std::string uri, Auth *auth): uri(std::move(uri)), auth(auth) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

libCTrader::Api::~Api() {
    curl_global_cleanup();
}

std::vector<libCTrader::Account> libCTrader::Api::accounts() {
    auto j = json::parse(call("GET", true, "/accounts"));
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

void libCTrader::Api::set_auth(libCTrader::Auth *au) {
    auth = au;
}

void libCTrader::Api::set_uri(std::string u) {
    uri = std::move(u);
}

libCTrader::Account libCTrader::Api::account(const std::string& account_id) {
    auto j = json::parse(call("GET", true, "/accounts/" + account_id));
    return Account(j["id"].get<std::string>(),
                   j["currency"].get<std::string>(),
                   j["balance"].get<std::string>(),
                   j["available"].get<std::string>(),
                   j["hold"].get<std::string>(),
                   "",
                   true);
}

std::vector<libCTrader::Account_Ledger_Entry> libCTrader::Api::account_ledger(const std::string &account_id) {
    auto j = json::parse(call("GET", true, "/accounts/" + account_id + "/ledger"));
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
    auto j = json::parse(call("GET", true, "/accounts/" + account_id + "/holds"));
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
    auto j = json::parse(call("POST", true, "/orders", o.dump()));
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
    auto j = json::parse(call("POST", true, "/orders", o.dump()));
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
    auto j = json::parse(call("POST", true, "/orders", o.dump()));
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
    call("DELETE", true, path);
}

std::vector<libCTrader::Order> libCTrader::Api::list_orders(const std::string *status, const std::string *product_id) {
    std::string path = "/orders";
    std::map<std::string, std::string> args;
    if (status != nullptr)
        args["status"] = *status;
    if (product_id != nullptr)
        args["product_id"] = *product_id;
    path += build_url_args(args);
    auto json = json::parse(call("GET", true, path));

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
    auto j = json::parse(call("GET", true, "/orders/" + order_id));
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
    auto json = json::parse(call("GET", true, path));
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
    return call("GET", true, "/users/self/exchange-limits");
}
