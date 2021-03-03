//
// Created by constexpr_dog on 3/3/21.
//

#include "ProductInfo.h"
#include "imgui.h"

bool DisplayProductInfoWindow(const std::vector<libCTrader::Product> &products) {
    bool close = false;
    ImGui::Begin("Product Info", nullptr, ImGuiWindowFlags_NoResize);
    if (ImGui::BeginTable("Info", 7)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Name");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Base Incr");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("Quote Incr");
        ImGui::TableSetColumnIndex(3);
        ImGui::Text("Min Size");
        ImGui::TableSetColumnIndex(4);
        ImGui::Text("Max Size");
        ImGui::TableSetColumnIndex(5);
        ImGui::Text("Min Funds");
        ImGui::TableSetColumnIndex(6);
        ImGui::Text("Max Funds");
        for (const auto& product : products) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", product.display_name.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", product.base_increment.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", product.quote_increment.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", product.base_min_size.c_str());
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", product.base_max_size.c_str());
            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%s", product.min_market_funds.c_str());
            ImGui::TableSetColumnIndex(6);
            ImGui::Text("%s", product.max_market_funds.c_str());
        }
        ImGui::EndTable();
    }
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}
