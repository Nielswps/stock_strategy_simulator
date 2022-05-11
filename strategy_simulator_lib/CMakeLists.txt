cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(stock_exchange_lib VERSION 0.2.0 DESCRIPTION "Library for simulating stock market strategies")
include(GNUInstallDirs)

add_library(${PROJECT_NAME} src/StockStrategySimulator.cpp src/StockDataParser.cpp
        src/StochasticOscillatorStrategy.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/StockStrategySimulator.h
        include/Strategy.h include/StochasticOscillatorStrategy.h)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})