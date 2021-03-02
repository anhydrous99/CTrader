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
}

#endif //CTRADER_TYPES_H
