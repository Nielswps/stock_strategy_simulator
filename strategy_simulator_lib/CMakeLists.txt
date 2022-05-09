cmake_minimum_required(VERSION 3.22)
project(stock_exchange_lib)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(library.cpp src/StockDataParser.cpp src/StochasticOscillatorStrategy.cpp include/Strategy.h include/StockDataParser.h include/StochasticOscillatorStrategy.h include/IterableQueue.h include/HistoricData.h)