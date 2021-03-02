//
// Created by constexpr_dog on 3/2/21.
//

#include "Account.h"
#include "imgui.h"

bool display_account_window(libCTrader::Account *account_ptr) {
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
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}
