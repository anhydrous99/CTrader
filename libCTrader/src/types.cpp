//
// Created by constexpr_dog on 3/2/21.
//

#include "libCTrader/types.h"

#include <utility>

libCTrader::Account::Account(std::string id, std::string currency, std::string balance, std::string available,
                             std::string hold, std::string profile_id, bool trading_enabled) : id(std::move(id)),
                                                                                               currency(std::move(
                                                                                                       currency)),
                                                                                               balance(std::move(
                                                                                                       balance)),
                                                                                               available(std::move(
                                                                                                       available)),
                                                                                               hold(std::move(hold)),
                                                                                               profile_id(std::move(
                                                                                                       profile_id)),
                                                                                               trading_enabled(
                                                                                                       trading_enabled) {}

libCTrader::Account_Ledger_Entry::Account_Ledger_Entry(std::string id, std::string created_at, std::string amount,
                                                       std::string balance, std::string type) : id(std::move(id)),
                                                       created_at(std::move(created_at)), amount(std::move(amount)),
                                                       balance(std::move(balance)), type(std::move(type)) {}
