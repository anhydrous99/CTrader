//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_API_H
#define CTRADER_API_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>
#include <map>
#include "Auth.h"
#include "types.h"

namespace libCTrader {
    using ptime = boost::posix_time::ptime;

    class Api {
        Auth *auth;
        std::string uri;

        std::string call(const std::string &method, bool authed, const std::string &path, const std::string &body);
        std::string call(const std::string &method, bool authed, const std::string &path);
        static std::string GetTimestamp();
        static std::string build_url_args(const std::map<std::string, std::string> &args);

    public:
        Api(std::string uri, Auth *auth);
        ~Api();

        void set_auth(Auth *au);
        void set_uri(std::string u);

        std::vector<Account> accounts();
        Account account(const std::string& account_id);
        std::vector<Account_Ledger_Entry> account_ledger(const std::string& account_id);
        std::string account_holds(const std::string& account_id);
        Order place_market_order(const std::string& product_id, const std::string& side, const std::string &size,
                                 const std::string &funds);
        Order place_limit_order(const std::string& product_id, const std::string& side, const std::string &price,
                                const std::string &size);
        Order place_stop_order(const std::string& product_id, const std::string& side, const std::string &price,
                               const std::string &size, const std::string &funds);
        void cancel_all_orders(const std::string *product_id=nullptr);
        std::vector<Order> list_orders(const std::string *status=nullptr, const std::string *product_id=nullptr);
        Order get_order(const std::string &order_id);
        std::vector<Fill> list_fills(const std::string* order_id=nullptr, const std::string* product_id=nullptr);
        std::string exchange_limits();

        // Public API
        std::vector<Product> get_products();
        Product get_product(const std::string &product_id);
        std::map<std::string, Page> get_order_book(const std::string &product_id, int level=1);
        std::map<std::string, PageLVL3> get_lvl3_order_book(const std::string &product_id);
        Ticker get_product_ticker(const std::string &product_id);
        std::vector<Trade> list_latest_trades(const std::string &product_id);
        std::vector<Candle> get_historical_candles(const std::string &product_id, const std::string &start,
                                                 const std::string &end, int granularity);
        std::vector<Candle> get_historical_candles(const std::string &product_id, const ptime &start, const ptime &end,
                                                   int granularity);
        std::string get_24hr_stats(const std::string &product_id);
    };
}


#endif //CTRADER_API_H
