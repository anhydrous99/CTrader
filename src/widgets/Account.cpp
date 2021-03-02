//
// Created by constexpr_dog on 3/2/21.
//

#include "Account.h"
#include "imgui.h"
#include <map>

static std::map<std::string, std::vector<libCTrader::Account_Ledger_Entry>> ledger_info;

bool display_account_ledger_window(const std::string &id, const std::string &currency) {
    bool close = false;
    auto entry = ledger_info[id];
    std::string ledger_window_name = "Ledger (" + currency + ")";
    ImGui::Begin(ledger_window_name.c_str(), nullptr, ImGuiWindowFlags_NoResize);
    if (ImGui::BeginTable("Ledger", 5)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("ID");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Created At");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("Amount");
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("Balance");
        ImGui::TableSetColumnIndex(4);
        ImGui::Text("Type");
        for (auto & row : entry) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", row.id.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", row.created_at.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", row.amount.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", row.balance.c_str());
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", row.type.c_str());
        }
        ImGui::EndTable();
    }
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}

bool display_account_window(libCTrader::Account *account_ptr, libCTrader::Api *api) {
    bool close = false;
    std::string account_window_name = "Account (" + account_ptr->currency + ")";
    ImGui::Begin(account_window_name.c_str(), nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text("ID: %s", account_ptr->id.c_str());
    ImGui::Text("Currency: %s", account_ptr->currency.c_str());
    ImGui::Text("Balance: %s", account_ptr->balance.c_str());
    ImGui::Text("Available: %s", account_ptr->available.c_str());
    ImGui::Text("Hold: %s", account_ptr->hold.c_str());
    ImGui::Text("Profile ID: %s", account_ptr->profile_id.c_str());
    ImGui::Text("Trading Enabled: %s", account_ptr->trading_enabled ? "true" : "false");
    if (ImGui::Button("Ledger"))
        ledger_info[account_ptr->id] = api->account_ledger(account_ptr->id);
    ImGui::SameLine();
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    auto itr = ledger_info.find(account_ptr->id);
    if (itr != ledger_info.end()) {
        if (display_account_ledger_window(account_ptr->id, account_ptr->currency))
            ledger_info.erase(itr);
    }
    return close;
}
