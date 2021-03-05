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

libCTrader::Fill::Fill(int trade_id,
                       std::string product_id,
                       std::string price,
                       std::string size,
                       std::string order_id,
                       std::string created_at,
                       std::string liquidity,
                       std::string fee,
                       bool settled,
                       std::string side) :
                       trade_id(trade_id),
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

bool libCTrader::Product::operator==(const libCTrader::Product &product) const {
    return id == product.id;
}

libCTrader::Page::Page(std::string price, std::string size, std::string side, int num_orders) :
    price(std::move(price)), size(std::move(size)), side(std::move(side)), num_orders(num_orders) {}

libCTrader::PageLVL3::PageLVL3(std::string price, std::string size, std::string side, std::string order_id) :
    price(std::move(price)), size(std::move(size)), side(std::move(side)), order_id(std::move(order_id)) {}

libCTrader::Ticker::Ticker(int trade_id, std::string price, std::string size, std::string time,
                           std::string bid, std::string ask, std::string volume) :
                                           trade_id(trade_id), price(std::move(price)), size(std::move(size)),
                                           time(std::move(time)), bid(std::move(bid)), ask(std::move(ask)),
                                           volume(std::move(volume)) {}

libCTrader::Trade::Trade(std::string time, int trade_id, std::string price, std::string size, std::string side) :
    time(std::move(time)), trade_id(trade_id), price(std::move(price)), size(std::move(size)), side(std::move(side)) {}

libCTrader::Candle::Candle(uint64_t time, float low, float high, float open, float close, float volume):
    time(time), low(low), high(high), open(open), close(close), volume(volume) {}

libCTrader::WSTicker::WSTicker(int trade_id, int sequence, std::string time, std::string product_id, std::string price,
                               std::string side, std::string last_size, std::string best_bid, std::string best_ask,
                               std::string open_24h, std::string volume_24h, std::string low_24h, std::string high_24h,
                               std::string volume_30d) :
                               trade_id(trade_id), sequence(sequence), time(std::move(time)),
                               product_id(std::move(product_id)), price(std::move(price)), side(std::move(side)),
                               last_size(std::move(last_size)), best_bid(std::move(best_bid)),
                               best_ask(std::move(best_ask)), open_24h(std::move(open_24h)),
                               volume_24h(std::move(volume_24h)), low_24h(std::move(low_24h)),
                               high_24h(std::move(high_24h)), volume_30d(std::move(volume_30d))
                               {}
