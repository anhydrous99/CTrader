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

libCTrader::Order::Order(std::string id,
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
                         bool settled) :
                         id(std::move(id)),
                         price(std::move(price)),
                         size(std::move(size)),
                         product_id(std::move(product_id)),
                         side(std::move(side)),
                         stp(std::move(stp)),
                         funds(std::move(funds)),
                         specific_funds(std::move(specific_funds)),
                         type(std::move(type)),
                         time_in_force(std::move(time_in_force)),
                         post_only(post_only),
                         created_at(std::move(created_at)),
                         fill_fees(std::move(fill_fees)),
                         fill_size(std::move(fill_size)),
                         executed_value(std::move(executed_value)),
                         status(std::move(status)),
                         settled(settled) {}
