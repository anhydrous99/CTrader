//
// Created by constexpr_dog on 3/2/21.
//

#ifndef CTRADER_ACCOUNTS_H
#define CTRADER_ACCOUNTS_H

#include <libCTrader/Api.h>

class Accounts {
    libCTrader::Api *api;
    std::vector<libCTrader::Account> accounts;
    std::vector<bool> display_account;

    void refresh();
    void sort_accounts();
public:
    explicit Accounts(libCTrader::Api *api);
    std::vector<libCTrader::Account> get_accounts();
    bool display_accounts_window();
};


#endif //CTRADER_ACCOUNTS_H
