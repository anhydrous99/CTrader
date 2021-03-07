//
// Created by constexpr_dog on 3/2/21.
//

#ifndef CTRADER_TYPES_H
#define CTRADER_TYPES_H

#include <string>
#include <cinttypes>
#include <vector>
#include <map>

namespace libCTrader {
    struct Account {
        std::string id;
        std::string currency;
        std::string balance;
        std::string available;
        std::string hold;
        std::string profile_id;
        bool trading_enabled;

        Account(std::string id, std::string currency, std::string balance, std::string available, std::string hold,
                std::string profile_id, bool trading_enabled);
    };

    struct Account_Ledger_Entry {
        std::string id;
        std::string created_at;
        std::string amount;
        std::string balance;
        std::string type;

        Account_Ledger_Entry(std::string id, std::string created_at, std::string amount, std::string balance,
                             std::string type);
    };

    struct Order {
        std::string id;
        std::string price;
        std::string size;
        std::string product_id;
        std::string side;
        std::string stp;
        std::string funds;
        std::string specific_funds;
        std::string type;
        std::string time_in_force;
        bool post_only;
        std::string created_at;
        std::string fill_fees;
        std::string fill_size;
        std::string executed_value;
        std::string status;
        bool settled;

        Order(std::string id,
             std::string price,
             std::string size,
             std::string product_id,
             std::string side,
             std::string stp,
             std::string funds,
             std::string specific_funds,
             std::string type,
             std::string time_in_force,
             bool post_only,
             std::string created_at,
             std::string fill_fees,
             std::string fill_size,
             std::string executed_value,
             std::string status,
             bool settled);
    };

    struct Fill {
        int trade_id;
        std::string product_id;
        std::string price;
        std::string size;
        std::string order_id;
        std::string created_at;
        std::string liquidity;
        std::string fee;
        bool settled;
        std::string side;

        Fill(int trade_id,
             std::string product_id,
             std::string price,
             std::string size,
             std::string order_id,
             std::string created_at,
             std::string liquidity,
             std::string fee,
             bool settled,
             std::string side);
    };

    struct Product {
        std::string id;
        std::string display_name;
        std::string base_currency;
        std::string quote_currency;
        std::string base_increment;
        std::string quote_increment;
        std::string base_min_size;
        std::string base_max_size;
        std::string min_market_funds;
        std::string max_market_funds;
        std::string status;
        std::string status_messages;
        bool cancel_only;
        bool limit_only;
        bool post_only;
        bool trading_disabled;

        Product(std::string id,
                std::string display_name,
                std::string base_currency,
                std::string quote_currency,
                std::string base_increment,
                std::string quote_increment,
                std::string base_min_size,
                std::string base_max_size,
                std::string min_market_funds,
                std::string max_market_funds,
                std::string status,
                std::string status_messages,
                bool cancel_only,
                bool limit_only,
                bool post_only,
                bool trading_disabled
                );
        bool operator==(const Product &product) const;
        bool operator<(const Product &product) const;
    };

    struct Page {
        std::string price;
        std::string size;
        std::string side;
        int num_orders = 0;

        Page(std::string price, std::string size, std::string side, int num_orders);
        Page() = default;
    };

    struct PageLVL3 {
        std::string price;
        std::string size;
        std::string side;
        std::string order_id;

        PageLVL3(std::string price, std::string size, std::string side, std::string order_id);
        PageLVL3() = default;
    };

    struct Ticker {
        int trade_id;
        std::string price;
        std::string size;
        std::string time;
        std::string bid;
        std::string ask;
        std::string volume;

        Ticker(int trade_id, std::string price, std::string size, std::string time, std::string bid,
               std::string ask, std::string volume);
    };

    struct WSTicker {
        int trade_id = 0;
        int sequence = 0;
        std::string time;
        std::string product_id;
        std::string price;
        std::string side;
        std::string last_size;
        std::string best_bid;
        std::string best_ask;
        std::string open_24h;
        std::string volume_24h;
        std::string low_24h;
        std::string high_24h;
        std::string volume_30d;

        WSTicker(int trade_id, int sequence, std::string time, std::string product_id, std::string price,
                 std::string side, std::string last_size, std::string best_bid, std::string best_ask,
                 std::string open_24h, std::string volume_24h, std::string low_24h, std::string high_24h,
                 std::string volume_30d);
        WSTicker() = default;
    };

    struct Trade {
        std::string time;
        int trade_id;
        std::string price;
        std::string size;
        std::string side;

        Trade(std::string time, int trade_id, std::string price, std::string size, std::string side);
    };

    struct Candle {
        uint64_t time;
        float low;
        float high;
        float open;
        float close;
        float volume;

        Candle(uint64_t time, float low, float high, float open, float close, float volume);
    };

    struct LVL2Snapshot {
        std::string product_id;
        std::map<std::string, std::string> bids;
        std::map<std::string, std::string> asks;

        LVL2Snapshot(std::string p, std::map<std::string, std::string> b, std::map<std::string, std::string> a);
    };

    struct LVL2Update {
        std::string product_id;
        std::string time;
        std::vector<std::tuple<std::string, std::string, std::string>> changes;

        LVL2Update(std::string product_id, std::string time, std::vector<std::tuple<std::string, std::string, std::string>> changes);
    };
}

#endif //CTRADER_TYPES_H
