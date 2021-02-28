cmake_minimum_required(VERSION 3.17)
project(CTrader)

set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# imgui
include_directories("extern/imgui/")
file(GLOB ImGui
        "extern/imgui/imgui.cpp"
        "extern/imgui/imgui_draw.cpp"
        "extern/imgui/imgui_demo.cpp"
        "extern/imgui/imgui_tables.cpp"
        "extern/imgui/imgui_widgets.cpp"
        "extern/imgui/backends/imgui_impl_glfw.cpp"
        "extern/imgui/backends/imgui_impl_opengl3.cpp"
        "extern/imgui/backends/imgui_impl_glfw.h"
        "extern/imgui/backends/imgui_impl_opengl3.h")
include_directories("extern/implot/")
file(GLOB ImPlot
        "extern/implot/implot.cpp"
        "extern/implot/implot_demo.cpp"
        "extern/implot/implot_items.cpp")

include_directories(${OPENGL_INCLUDE_DIR})
add_executable(CTrader src/main.cpp ${ImGui} ${ImPlot})
target_link_libraries(CTrader cpprestsdk::cpprest OpenGL::GL glfw GLEW::glew)