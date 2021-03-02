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
}

#endif //CTRADER_TYPES_H
