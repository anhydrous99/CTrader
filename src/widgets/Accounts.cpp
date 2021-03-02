//
// Created by constexpr_dog on 3/2/21.
//

#include "Accounts.h"
#include "imgui.h"

Accounts::Accounts(libCTrader::Api *api) : api(api) {
    accounts = api->accounts();
    selection = &accounts[0];
    selection_ptr = &selection;
}

std::vector<libCTrader::Account> Accounts::get_accounts() {
    return accounts;
}

libCTrader::Account **Accounts::get_selection_ptr() {
    return selection_ptr;
}

bool Accounts::display_accounts_window() {
    static int e = 0;
    bool close = false;
    ImGui::Begin("Accounts");
    for (int i = 0; i < accounts.size(); i++)
        ImGui::RadioButton(accounts[i].currency.c_str(), &e, i);
    if (ImGui::Button("Submit")) {
        selection = &accounts[e];
        close = true;
    }
    ImGui::End();
    return close;
}
