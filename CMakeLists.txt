cmake_minimum_required(VERSION 3.17)
project(CTrader)

set(CMAKE_CXX_STANDARD 17)

# Get CPM & CPM packages
set(CPM_DOWNLOAD_VERSION 0.31.1)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})
include(cmake/nlohmann_json.cmake)
include(cmake/glfw3.cmake)
include(cmake/glew.cmake)
##

find_package(cpprestsdk REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(libCTrader libCTrader/Api.cpp libCTrader/Auth.cpp)
target_include_directories(libCTrader PUBLIC libCTrader)
target_link_libraries(libCTrader cpprestsdk::cpprest OpenSSL::SSL OpenSSL::Crypto nlohmann_json)
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
add_executable(CTrader src/main.cpp src/MaxQueue.h ${ImGui} ${ImPlot})
target_link_libraries(CTrader OpenGL::GL glfw libglew_static libCTrader)