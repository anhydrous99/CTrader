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

libCTrader::Fill::Fill(std::string trade_id,
                       std::string product_id,
                       std::string price,
                       std::string size,
                       std::string order_id,
                       std::string created_at,
                       std::string liquidity,
                       std::string fee,
                       bool settled,
                       std::string side) :
                       trade_id(std::move(trade_id)),
                       product_id(std::move(product_id)),
                       price(std::move(price)),
                       size(std::move(size)),
                       order_id(std::move(order_id)),
                       created_at(std::move(created_at)),
                       liquidity(std::move(liquidity)),
                       fee(std::move(fee)),
                       settled(settled),
                       side(std::move(side)) {}

libCTrader::Product::Product(std::string id,
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
                             bool trading_disabled) :
                             id(std::move(id)),
                             display_name(std::move(display_name)),
                             base_currency(std::move(base_currency)),
                             quote_currency(std::move(quote_currency)),
                             base_increment(std::move(base_increment)),
                             quote_increment(std::move(quote_increment)),
                             base_min_size(std::move(base_min_size)),
                             base_max_size(std::move(base_max_size)),
                             min_market_funds(std::move(min_market_funds)),
                             max_market_funds(std::move(max_market_funds)),
                             status(std::move(status)),
                             status_messages(std::move(status_messages)),
                             cancel_only(cancel_only),
                             limit_only(limit_only),
                             post_only(post_only),
                             trading_disabled(trading_disabled) {}