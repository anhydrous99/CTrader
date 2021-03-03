//
// Created by constexpr_dog on 3/2/21.
//

#ifndef CTRADER_TYPES_H
#define CTRADER_TYPES_H

#include <string>

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
        std::string trade_id;
        std::string product_id;
        std::string price;
        std::string size;
        std::string order_id;
        std::string created_at;
        std::string liquidity;
        std::string fee;
        bool settled;
        std::string side;

        Fill(std::string trade_id,
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
    };
}

#endif //CTRADER_TYPES_H
