//
// Created by constexpr_dog on 3/2/21.
//

#include "Accounts.h"
#include "Account.h"
#include "imgui.h"
#include <algorithm>

Accounts::Accounts(libCTrader::Api *api) : api(api) {
    accounts = api->accounts();
    display_account = std::vector<bool>(accounts.size());
    std::fill(display_account.begin(), display_account.end(), false);
    sort_accounts();
}

std::vector<libCTrader::Account> Accounts::get_accounts() {
    return accounts;
}

bool Accounts::display_accounts_window() {
    bool close = false;
    ImGui::Begin("Accounts", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Currency | Balance");
    for (int i = 0; i < accounts.size(); i++) {
        if (ImGui::Button(accounts[i].currency.c_str()))
            display_account[i] = !display_account[i];
        ImGui::SameLine();
        ImGui::Text("| %s", accounts[i].balance.c_str());
        if (display_account[i])
            display_account[i] = !display_account_window(&accounts[i]);
    }
    if (ImGui::Button("Close"))
        close = true;
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
        refresh();
    ImGui::End();
    return close;
}

void Accounts::refresh() {
    accounts = api->accounts();
    display_account = std::vector<bool>(accounts.size());
    std::fill(display_account.begin(), display_account.end(), false);
    sort_accounts();
}

void Accounts::sort_accounts() {
    std::sort(accounts.begin(), accounts.end(), [](const libCTrader::Account &a, const libCTrader::Account &b) {
        return std::stof(a.balance) > std::stof(b.balance);
    });
}
