cmake_minimum_required(VERSION 3.17)
project(CTrader)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build the tests" OFF)

# Get CPM & CPM packages
set(CPM_DOWNLOAD_VERSION 0.31.1)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})
include(cmake/nlohmann_json.cmake)
include(cmake/yaml_cpp.cmake)
include(cmake/glfw3.cmake)
include(cmake/glew.cmake)
if (NOT WIN32)
    include(cmake/curl.cmake)
endif ()
##

find_package(cpprestsdk REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.75.0 REQUIRED)
if (WIN32)
    find_package(CURL REQUIRED)
endif ()

add_library(libCTrader
        libCTrader/src/Api.cpp
        libCTrader/src/Auth.cpp
        libCTrader/src/types.cpp
        libCTrader/src/Websock.cpp
        libCTrader/src/Client.cpp)
target_include_directories(libCTrader PUBLIC libCTrader/include)
target_link_libraries(libCTrader cpprestsdk::cpprest OpenSSL::SSL OpenSSL::Crypto Boost::boost nlohmann_json)
if (NOT WIN32)
    target_link_libraries(libCTrader libcurl)
else ()
    target_link_libraries(libCTrader CURL::libcurl)
endif ()
set_target_properties(libCTrader PROPERTIES PREFIX "")

# imgui
include_directories("extern/imgui/")
include_directories("extern/imgui/backends")
file(GLOB ImGui
        "extern/imgui/imgui.cpp"
        "extern/imgui/imgui_draw.cpp"
        "extern/imgui/imgui_demo.cpp"
        "extern/imgui/imgui_tables.cpp"
        "extern/imgui/imgui_widgets.cpp"
        "extern/imgui/backends/imgui_impl_glfw.cpp"
        "extern/imgui/backends/imgui_impl_opengl3.cpp")
include_directories("extern/implot/")
file(GLOB ImPlot
        "extern/implot/implot.cpp"
        "extern/implot/implot_demo.cpp"
        "extern/implot/implot_items.cpp")

include_directories(${OPENGL_INCLUDE_DIR})
add_executable(CTrader
        src/main.cpp
        src/MaxQueue.h
        src/Settings.cpp
        src/Settings.h
        src/Widgets.h
        src/widgets/Performance.cpp
        src/widgets/Performance.h
        src/widgets/Accounts.cpp
        src/widgets/Accounts.h
        src/widgets/Account.cpp
        src/widgets/Account.h
        src/widgets/ProductInfo.cpp
        src/widgets/ProductInfo.h
        src/widgets/WatchList.cpp
        src/widgets/WatchList.h
        src/widgets/TradeHistory.cpp
        src/widgets/TradeHistory.h
        src/widgets/OrderBook.cpp
        src/widgets/OrderBook.h
        src/widgets/PriceCharts.cpp
        src/widgets/PriceCharts.h
        src/widgets/Orders.cpp
        src/widgets/Orders.h
        ${ImGui}
        ${ImPlot})
target_link_libraries(CTrader OpenGL::GL glfw libglew_static yaml-cpp libCTrader)
configure_file(${CMAKE_SOURCE_DIR}/settings.yaml ${CMAKE_BINARY_DIR}/settings.yaml COPYONLY)

if (BUILD_TESTS)
    include(cmake/googletest.cmake)
    enable_testing()
    add_subdirectory(tests)
endif ()
