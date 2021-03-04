//
// Created by Armando Herrera on 3/4/21.
//

#include "WatchList.h"
#include "imgui.h"

WatchList::WatchList(libCTrader::Websock *websock, const std::vector<libCTrader::Product> &av_products) {
    websock_ptr = websock;
    available_products = av_products;
}

bool WatchList::display_watch_list_window() {
    bool close = false;
    ImGui::Begin("WatchList");
    if (ImGui::Button("Close"))
        close = true;
    ImGui::End();
    return close;
}
