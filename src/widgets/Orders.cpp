//
// Created by constexpr_dog on 4/4/21.
//

#include "Orders.h"
#include "imgui.h"

Orders::Orders(libCTrader::Api *api) : api(api) {}

void Orders::display_orders_window() {
    ImGui::Begin("Orders");

    // Placing Orders
    ImGui::Text("Order Form");
    ImGui::RadioButton("Buy", &buy_sell, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Sell", &buy_sell, 1);
    if (ImGui::BeginTabBar("Order Form")) {
        if (ImGui::BeginTabItem("Market")) {
            ImGui::Text("1");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Limit")) {
            ImGui::Text("2");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Stop")) {
            ImGui::Text("3");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();

    // Showing Orders
    ImGui::Text("Hello, world!");

    ImGui::End();
}
