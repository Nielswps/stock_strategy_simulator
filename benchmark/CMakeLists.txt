cmake_minimum_required(VERSION 3.22)
project(Benchmark)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} Benchmarker.cpp ../strategy_simulator_lib/src/StockDataParser.cpp
        ../strategy_simulator_lib/src/StockStrategySimulator.cpp
        ../strategy_simulator_lib/src/StochasticOscillatorStrategy.cpp
        ../strategy_simulator_lib/include/StockStrategySimulator.h
        ../strategy_simulator_lib/include/StochasticOscillatorStrategy.h)