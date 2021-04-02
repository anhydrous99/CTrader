//
// Created by arman on 3/17/2021.
//

#include "PriceCharts.h"
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

using namespace boost::posix_time;

void plot_candlestick_graph(const std::map<uint64_t, libCTrader::Candle> &candles, int local_granularity, double width_percent, bool fit, const ImVec4 &bullCol, const ImVec4 &bearCol);

PriceCharts::PriceCharts(libCTrader::Api *api, libCTrader::Websock *websock, std::string current_product) : api(api), websock(websock),
                                                                                                            _current_product(std::move(current_product)) {
    update_candle_vector();
    websock->on_new_ticker(
        [&](const libCTrader::WSTicker &ticker) {
            if (ticker.product_id == _current_product) {
                std::unique_lock lock(grand_mutex);
                double last_ticker_time = times.back();
                auto current_time = libCTrader::Api::get_timestamp<uint64_t>();
                double price = std::stod(ticker.price);
                double volume = std::stod(ticker.last_size);
                if (current_time - last_ticker_time > granularity.total_seconds()) {
                    // Start new ticker
                    candles[current_time] = libCTrader::Candle(current_time, price, price, price, price, volume);
                    times.push_back(current_time);
                    closing_prices.push_back(price);
                    volumes.push_back(volume);
                    ema12_prices.push_back(2 * (price - ema12_prices.back()) / 13 + ema12_prices.back());
                    ema26_prices.push_back(2 * (price - ema26_prices.back()) / 27 + ema26_prices.back());
                } else {
                    // Update current ticker
                    candles[last_ticker_time].close = closing_prices.back() = price;
                    if (price > candles[last_ticker_time].high)
                        candles[last_ticker_time].high = price;
                    if (price < candles[last_ticker_time].low)
                        candles[last_ticker_time].low = price;
                    candles[last_ticker_time].volume += volume;
                    volumes.back() += volume;
                    ema12_prices.back() = 2 * (price - ema12_prices.rbegin()[1]) / 13 + ema12_prices.rbegin()[1];
                    ema26_prices.back() = 2 * (price - ema26_prices.rbegin()[1]) / 27 + ema26_prices.rbegin()[1];
                }
            }
        });
}

void PriceCharts::update_candle_vector() {
    std::unique_lock lock(grand_mutex);
    min_value = static_cast<double>(-INFINITY);
    max_value = INFINITY;
    candles.clear();
    closing_prices.clear();
    ema12_prices.clear();
    ema26_prices.clear();
    volumes.clear();
    times.clear();
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
    auto candles_vec = api->get_latest_historical_candles(_current_product, granularity.total_seconds());
    std::sort(candles_vec.begin(), candles_vec.end(),
              [](const libCTrader::Candle &a, const libCTrader::Candle &b) {
        return a.time < b.time;
    });
    for (std::size_t i = 0; i < candles_vec.size(); i++) {
        candles[candles_vec[i].time] = candles_vec[i];
        closing_prices.push_back(candles_vec[i].close);
        volumes.push_back(candles_vec[i].volume);
        times.push_back(candles_vec[i].time);

        if (i == 0) {
            ema12_prices.push_back(candles_vec[i].close);
            ema26_prices.push_back(candles_vec[i].close);
        } else {
            ema12_prices.push_back(2 * (candles_vec[i].close - ema12_prices[i - 1]) / 13 + ema12_prices[i - 1]);
            ema26_prices.push_back(2 * (candles_vec[i].close - ema26_prices[i - 1]) / 27 + ema26_prices[i - 1]);
        }

        if (candles_vec[i].low < min_value)
            min_value = candles_vec[i].low;
        if (candles_vec[i].high > max_value)
            max_value = candles_vec[i].high;
    }
    fit_graph = true;
}

void PriceCharts::display_price_charts_window() {
    const static ImVec4 bearCol{1.0f, 0.0f, 0.0f, 1.0f}; // Red
    const static ImVec4 bullCol{0.0f, 1.0f, 0.0f, 1.0f}; // Green
    const static double width_percent = 0.333f;
    static int last_local_granularity = local_granularity;
    ImGui::SetNextWindowPos(ImVec2(199.f,19.f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(866.f,362.f), ImGuiCond_FirstUseEver);
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

    // If granularity has been changed
    if (last_local_granularity != local_granularity) {
        last_local_granularity = local_granularity;
        update_candle_vector();
    }

    bool fit = false;
    if (fit_graph) {
        ImPlot::FitNextPlotAxes();
        fit = true;
        fit_graph = false;
    }

    ImGui::Spacing();

    std::shared_lock lock(grand_mutex);
    ImPlot::SetNextPlotLimits(static_cast<double>(candles.begin()->first), static_cast<double>(candles.rbegin()->first), min_value, max_value);
    if (ImPlot::BeginPlot("Price Charts", "Time", "Price", ImVec2(-1, 0), 0, ImPlotAxisFlags_Time, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit)) {
        switch (local_graph) {
            case 1:
                // Plot Line Graph
                ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                ImPlot::PlotLine("Price", times.data(), closing_prices.data(), closing_prices.size());
                break;
            case 0:
            default:
                // Plot Candlestick Graph
                plot_candlestick_graph(candles, local_granularity, width_percent, fit, bullCol, bearCol);
        }

        if (show_EMA12)
            ImPlot::PlotLine("EMA12", times.data(), ema12_prices.data(), ema12_prices.size());
        if (show_EMA26)
            ImPlot::PlotLine("EMA26", times.data(), ema26_prices.data(), ema26_prices.size());

        ImPlot::EndPlot();
    }
    ImGui::End();
}

void PriceCharts::change_product(const std::string &new_product_id) {
    _current_product = new_product_id;
    update_candle_vector();
}

void plot_candlestick_graph(const std::map<uint64_t, libCTrader::Candle> &candles, int local_granularity, double width_percent, bool fit, const ImVec4 &bullCol, const ImVec4 &bearCol) {
    // Get draw list
    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    // Get current viewport range
    static ImPlotLimits range;
    range = ImPlot::GetPlotLimits();

    if (ImPlot::BeginItem("")) {
        double half_width = candles.size() > 1 ? static_cast<double>(std::next(candles.begin())->first - candles.begin()->first) * width_percent : width_percent;
        ImPlot::GetCurrentItem();

        // custom tool
        if (ImPlot::IsPlotHovered()) {
            ImPlotPoint mouse = ImPlot::GetPlotMousePos();

            // Pick the unit
            ImPlotTimeUnit_ unit;
            switch (local_granularity) {
                case 0:
                case 1:
                case 2:
                    unit = ImPlotTimeUnit_Min;
                    break;
                case 3:
                case 4:
                    unit = ImPlotTimeUnit_Hr;
                    break;
                case 5:
                    unit = ImPlotTimeUnit_Day;
                    break;
                default:
                    unit = ImPlotTimeUnit_Min;
            }

            mouse.x = ImPlot::RoundTime(ImPlotTime::FromDouble(mouse.x), unit).ToDouble();
            float tool_l = ImPlot::PlotToPixels(mouse.x - half_width * 1.5, mouse.y).x;
            float tool_r = ImPlot::PlotToPixels(mouse.x + half_width * 1.5, mouse.y).x;
            float tool_t = ImPlot::GetPlotPos().y;
            float tool_b = tool_t + ImPlot::GetPlotSize().y;
            ImPlot::PushPlotClipRect();
            draw_list->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128, 128, 128, 64));
            ImPlot::PopPlotClipRect();
            // find mouse location index
            const auto itr = std::lower_bound(candles.begin(), candles.end(), mouse.x,
                                              [](const std::pair<uint64_t, libCTrader::Candle>& a, double value) {
                                                  return a.first < value;
                                              });
            if (itr != candles.end()) {
                ImGui::BeginTooltip();
                char buff[32];
                ImPlot::FormatDateTime(ImPlotTime::FromDouble(static_cast<double>(itr->first)), buff, 32, ImPlotDateTimeFmt(ImPlotDateFmt_DayMoYr, ImPlotTimeFmt_HrMin, false, true));
                ImGui::Text("D:  %s", buff);
                ImGui::Text("O: $%.2f", itr->second.open);
                ImGui::Text("C: $%.2f", itr->second.close);
                ImGui::Text("L: $%.2f", itr->second.low);
                ImGui::Text("H: $%.2f", itr->second.high);
                ImGui::Text("V:  %.3f", itr->second.volume);
                ImGui::EndTooltip();
            }
        }

        // fit data if requested
        if (ImPlot::FitThisFrame()) {
            for (const auto &candle_pair : candles) {
                // Only fit data in the viewport
                if (!fit) {
                    if (range.X.Min < static_cast<double>(candle_pair.first) &&
                        static_cast<double>(candle_pair.first) < range.X.Max) {
                        ImPlot::FitPoint(
                                ImPlotPoint(static_cast<double>(candle_pair.first), candle_pair.second.low));
                        ImPlot::FitPoint(
                                ImPlotPoint(static_cast<double>(candle_pair.first), candle_pair.second.high));
                    }
                } else {
                    ImPlot::FitPoint(
                            ImPlotPoint(static_cast<double>(candle_pair.first), candle_pair.second.low));
                    ImPlot::FitPoint(
                            ImPlotPoint(static_cast<double>(candle_pair.first), candle_pair.second.high));
                }
            }
        }

        // render data
        for (const auto& candle_pair : candles) {
            ImVec2 open_pos = ImPlot::PlotToPixels(static_cast<double>(candle_pair.first) - half_width, candle_pair.second.open);
            ImVec2 close_pos = ImPlot::PlotToPixels(static_cast<double>(candle_pair.first) + half_width, candle_pair.second.close);
            ImVec2 low_pos = ImPlot::PlotToPixels(static_cast<double>(candle_pair.first), candle_pair.second.low);
            ImVec2 high_pos = ImPlot::PlotToPixels(static_cast<double>(candle_pair.first), candle_pair.second.high);
            auto color = ImGui::GetColorU32(candle_pair.second.open > candle_pair.second.close ? bearCol : bullCol);
            draw_list->AddLine(low_pos, high_pos, color);
            draw_list->AddRectFilled(open_pos, close_pos, color);
        }

        ImPlot::EndItem();
    }
}
