cmake_minimum_required(VERSION 3.22)
project(stock_exchange_lib)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(library.cpp src/StockDataParser.cpp)