//
// Created by arman on 3/17/2021.
//

#include "PriceCharts.h"
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

using namespace boost::posix_time;

PriceCharts::PriceCharts(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product) : api(api), websock(websock),
                                                                                                            current_product(std::move(current_product)) {
    update_candle_vector();
}

void PriceCharts::update_candle_vector() {

    switch (local_granularity) {
        case 0:
            granularity = boost::posix_time::time_duration(0, 1, 0); // hour, min, sec
            break;
        case 1:
            granularity = boost::posix_time::time_duration(0, 5, 0);
            break;
        case 2:
            granularity = boost::posix_time::time_duration(0, 15, 0);
            break;
        case 3:
            granularity = boost::posix_time::time_duration(1, 0, 0);
            break;
        case 4:
            granularity = boost::posix_time::time_duration(6, 0, 0);
            break;
        case 5:
            granularity = boost::posix_time::time_duration(24, 0, 0);
            break;
        default:
            granularity = boost::posix_time::time_duration(0, 5, 0);
    }
    candles = api->get_latest_historical_candles(current_product, granularity.total_seconds());
    std::sort(candles.begin(), candles.end(),
              [](const libCTrader::Candle& a, const libCTrader::Candle& b) {
        return a.time < b.time;
    });
}

void PriceCharts::display_price_charts_window() {
    const static ImVec4 bearCol{1.0f, 0.0f, 0.0f, 1.0f}; // Red
    const static ImVec4 bullCol{0.0f, 1.0f, 0.0f, 1.0f}; // Green
    const static double width_percent = 0.25f;
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

    const auto min = std::min_element(candles.begin(), candles.end(),
                                                 [](const libCTrader::Candle &a, const libCTrader::Candle &b){
        return a.low < b.low;
    });
    const auto max = std::max_element(candles.begin(), candles.end(),
                                      [](const libCTrader::Candle &a, const libCTrader::Candle &b) {
        return a.high < b.high;
    });

    ImPlot::SetNextPlotLimits(candles.front().time, candles.back().time, min->low, max->high);
    if (ImPlot::BeginPlot("Price Charts", "Time", "Price", ImVec2(-1, 0), 0, ImPlotAxisFlags_Time)) {
        ImDrawList* draw_list = ImPlot::GetPlotDrawList();
        if (ImPlot::BeginItem("PriceCharts")) {
            double half_width = candles.size() > 1 ? static_cast<double>(candles[1].time - candles[0].time) * width_percent : width_percent;
            ImPlot::GetCurrentItem();
            // TODO: custom tool

            // fit data if requested
            if (ImPlot::FitThisFrame()) {
                for (const libCTrader::Candle &candle : candles) {
                    ImPlot::FitPoint(ImPlotPoint(candle.time, candle.low));
                    ImPlot::FitPoint(ImPlotPoint(candle.time, candle.high));
                }
            }
            // render data
            for (const libCTrader::Candle &candle : candles) {
                ImVec2 open_pos = ImPlot::PlotToPixels(candle.time - half_width, candle.open);
                ImVec2 close_pos = ImPlot::PlotToPixels(candle.time + half_width, candle.close);
                ImVec2 low_pos = ImPlot::PlotToPixels(candle.time, candle.low);
                ImVec2 high_pos = ImPlot::PlotToPixels(candle.time, candle.high);
                auto color = ImGui::GetColorU32(candle.open > candle.close ? bearCol : bullCol);
                draw_list->AddLine(low_pos, high_pos, color);
                draw_list->AddRectFilled(open_pos, close_pos, color);
            }

            ImPlot::EndItem();
        }
        ImPlot::EndPlot();
    }
    ImGui::End();
}
