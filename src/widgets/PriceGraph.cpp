//
// Created by arman on 3/17/2021.
//

#include "PriceGraph.h"
#include "imgui.h"

using namespace boost::posix_time;

PriceGraph::PriceGraph(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product) : api(api), websock(websock),
    current_product(std::move(current_product)) {
    //update_candle_vector();
}

void PriceGraph::update_candle_vector() {
    auto now = second_clock::universal_time();
    auto start = now - interval_duration;
    candles = api->get_historical_candles(current_product, start, now, granularity.total_seconds());
}

void PriceGraph::display_price_graph_window() {
    ImGui::Begin("Price Graph");

    ImGui::End();
}
