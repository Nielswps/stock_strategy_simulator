cmake_minimum_required(VERSION 3.22)
project(usage_example)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)
find_library(STOCKEXCHANGE
        NAMES stock_exchange_lib
        PATHS ../build/strategy_simulator_lib
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH)
include_directories(../strategy_simulator_lib/include)
target_link_libraries(${PROJECT_NAME} ${STOCKEXCHANGE})
