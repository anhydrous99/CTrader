cmake_minimum_required(VERSION 3.17)
project(CTrader)

set(CMAKE_CXX_STANDARD 17)

find_package(cpprestsdk REQUIRED)

add_executable(CTrader src/main.cpp)
target_link_libraries(CTrader cpprestsdk::cpprest)