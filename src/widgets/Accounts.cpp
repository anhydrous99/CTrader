//
// Created by constexpr_dog on 3/2/21.
//

#include "Accounts.h"
#include "imgui.h"

Accounts::Accounts(libCTrader::Api *api) : api(api) {
    accounts = api->accounts();
    selection = &accounts[0];
    selection_ptr = &selection;
    e = 0;
}

std::vector<libCTrader::Account> Accounts::get_accounts() {
    return accounts;
}

libCTrader::Account **Accounts::get_selection_ptr() {
    return selection_ptr;
}

bool Accounts::display_accounts_window() {
    bool close = false;
    ImGui::Begin("Accounts");
    for (int i = 0; i < accounts.size(); i++) {
        ImGui::RadioButton(accounts[i].currency.c_str(), &e, i);
        ImGui::SameLine();
        ImGui::Text("%s", accounts[i].balance.c_str());
    }
    if (ImGui::Button("Submit")) {
        selection = &accounts[e];
        close = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
        refresh();
    ImGui::End();
    return close;
}

void Accounts::refresh() {
    accounts = api->accounts();
    selection = &accounts[e];
}
