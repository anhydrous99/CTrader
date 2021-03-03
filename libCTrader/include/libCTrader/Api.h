//
// Created by Armando Herrera on 3/1/21.
//

#ifndef CTRADER_API_H
#define CTRADER_API_H

#include <string>
#include <vector>
#include <map>
#include "Auth.h"
#include "types.h"

namespace libCTrader {
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
    };
}


#endif //CTRADER_API_H
