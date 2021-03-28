//
// Created by arman on 3/17/2021.
//

#include "PriceCharts.h"
#include "implot.h"
#include "imgui.h"

using namespace boost::posix_time;

PriceCharts::PriceCharts(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product) : api(api), websock(websock),
                                                                                                            current_product(std::move(current_product)) {
    //update_candle_vector();
}

void PriceCharts::update_candle_vector() {
    auto now = second_clock::universal_time();
    auto start = now - interval_duration;
    candles = api->get_historical_candles(current_product, start, now, granularity.total_seconds());
}

void PriceCharts::display_price_charts_window() {
    static bool show_EMA12 = false;
    static bool show_EMA26 = false;
    static int local_granularity = 3;
    static int local_graph = 0;
    ImGui::Begin("Price Graph", nullptr, ImGuiWindowFlags_MenuBar);

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Granularity")) {
            ImGui::RadioButton("1m", &local_granularity, 0);
            ImGui::RadioButton("5m", &local_granularity, 1);
            ImGui::RadioButton("15m", &local_granularity, 2);
            ImGui::RadioButton("1h", &local_granularity, 3);
            ImGui::RadioButton("6h", &local_granularity, 4);
            ImGui::RadioButton("1d", &local_granularity, 5);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Graph")) {
            ImGui::RadioButton("Candle", &local_graph, 0);
            ImGui::RadioButton("Line", &local_graph, 1);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Overlay")) {
            ImGui::Checkbox("EMA12", &show_EMA12);
            ImGui::Checkbox("EMA26", &show_EMA26);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Spacing();
    ImPlot::CreateContext();
    if (ImPlot::BeginPlot("Price Charts", "Time", "Price")) {
        ImPlot::EndPlot();
    }
    ImPlot::DestroyContext();
    ImGui::End();
}
