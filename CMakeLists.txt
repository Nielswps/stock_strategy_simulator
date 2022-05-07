cmake_minimum_required(VERSION 3.22)
project(stock_exchange)

set(CMAKE_CXX_STANDARD 20)

add_library(stock_exchange strategy_simulator_lib/library.cpp strategy_simulator_lib/include/Strategy.h strategy_simulator_lib/include/Data.h)
add_subdirectory(Google_tests)
add_subdirectory(strategy_simulator_lib)