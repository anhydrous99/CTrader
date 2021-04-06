#include "imgui.h"
#include "implot.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Settings.h"
#include "Widgets.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "Glfw Error " << error << ": " << description << "\n";
}


int main() {
    // Get settings
    Settings settings;
    libCTrader::Api *api = settings.get_api();
    libCTrader::Websock *websock = settings.get_websock();
    Accounts accounts(api);
    auto products = api->get_products();
    auto current_product = products[0];
    int product_selection = 0;

    // Connect websocket to starting product
    websock->add_channel_product_pair("ticker", current_product, 0);
    websock->add_channel_product_pair("level2", current_product, 2);
    // websock->Connect();

    // Create WatchList class
    WatchList watchList(websock, products, {current_product});

    // Create tradehistory class
    TradeHistory tradeHistory(websock, current_product);

    // Create OrderBook class
    OrderBook orderBook(websock, current_product);

    // Create PriceCharts class
    PriceCharts priceGraph(api, websock, current_product.id);

    // Create Orders class
    Orders orders(api);

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "CTrader", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize OpenGL loader\n";
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_accounts_window = false;
    bool show_product_info_window = false;
    bool show_performance_window = false;
    bool show_settings_window = false;
    bool show_watchlist_window = false;
    bool show_trade_history_window = false;
    bool show_order_book_window = false;
    bool show_order_histogram_window = false;
    bool show_price_charts_window = false;
    bool show_orders_window = false;

    bool new_product_selected = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    while(!glfwWindowShouldClose(window)) {
        // Poll events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create top menu
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("View")) {
                ImGui::Checkbox("Orders", & show_orders_window);
                ImGui::Checkbox("Price Graph", & show_price_charts_window);
                ImGui::Checkbox("Order Histogram", & show_order_histogram_window);
                ImGui::Checkbox("Order Book", & show_order_book_window);
                ImGui::Checkbox("Trade History", & show_trade_history_window);
                ImGui::Checkbox("WatchList", & show_watchlist_window);
                ImGui::Checkbox("Accounts", & show_accounts_window);
                ImGui::Checkbox("Product Info", & show_product_info_window);
                ImGui::Checkbox("Performance", & show_performance_window);
                ImGui::Checkbox("Settings", & show_settings_window);
                ImGui::EndMenu();
            }
            ImGui::Text("|");
            if (ImGui::BeginMenu("Products")) {
                for (int i = 0; i < products.size(); i++) {
                    if (ImGui::RadioButton(products[i].display_name.c_str(), &product_selection, i))
                        new_product_selected = true;
                }
                ImGui::EndMenu();
            }
            ImGui::Text("| %s", current_product.display_name.c_str());
            ImGui::EndMainMenuBar();
        }

        // Show performance window
        if (show_performance_window)
            DisplayPerformanceWindow();
        // Show product info window
        if (show_product_info_window)
            show_product_info_window = !DisplayProductInfoWindow(products);
        // Show Settings Window
        if (show_settings_window)
            show_settings_window = !settings.create_settings_window();
        // Show accounts window
        if (show_accounts_window)
            show_accounts_window = !accounts.display_accounts_window();
        if (new_product_selected) {
            current_product = products[product_selection];
            tradeHistory.change_product(current_product);
            orderBook.change_product(current_product);
            priceGraph.change_product(current_product.id);
            new_product_selected = false;
        }
        // Show watch list window
        if (show_watchlist_window)
            show_watchlist_window = !watchList.display_watch_list_window();
        // Show trade history window
        if (show_trade_history_window)
            tradeHistory.display_trade_history_window();
        if (show_order_book_window)
            orderBook.display_order_book_window();
        if (show_order_histogram_window)
            orderBook.display_order_histogram_window();
        if (show_price_charts_window)
            priceGraph.display_price_charts_window();
        if (show_orders_window)
            orders.display_orders_window();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    // websock->Disconnect();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
