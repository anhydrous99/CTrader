//
// Created by constexpr_dog on 3/2/21.
//

#ifndef CTRADER_ACCOUNTS_H
#define CTRADER_ACCOUNTS_H

#include <Api.h>

class Accounts {
    libCTrader::Api *api;
    std::vector<libCTrader::Account> accounts;
    libCTrader::Account **selection_ptr;
    libCTrader::Account *selection;
    int e;

public:
    explicit Accounts(libCTrader::Api *api);
    std::vector<libCTrader::Account> get_accounts();
    libCTrader::Account **get_selection_ptr();
    bool display_accounts_window();
    void refresh();
};


#endif //CTRADER_ACCOUNTS_H
