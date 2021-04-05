add_executable(tests src/tests.cpp src/test_Auth.cpp src/test_Api.cpp)
target_link_libraries(tests libCTrader gtest gmock)