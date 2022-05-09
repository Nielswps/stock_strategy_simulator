cmake_minimum_required(VERSION 3.22)
project(usage_example)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../strategy_simulator_lib/src ../strategy_simulator_lib/include)

link_directories(${CMAKE_BINARY_DIR}/res)
target_link_libraries(${PROJECT_NAME} libstock_exchange_lib.a)