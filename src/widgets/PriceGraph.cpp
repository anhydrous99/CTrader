//
// Created by arman on 3/17/2021.
//

#include "PriceGraph.h"

PriceGraph::PriceGraph(libCTrader::Api *api, libCTrader::Websock *websock) : api(api), websock(websock) {
    update_candle_vector();
}

void PriceGraph::update_candle_vector() {

}

void PriceGraph::display_price_graph_window() {

}
