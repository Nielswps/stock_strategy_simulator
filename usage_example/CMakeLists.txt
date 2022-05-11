cmake_minimum_required(VERSION 3.22)
project(usage_example)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp ../strategy_simulator_lib/src/StockDataParser.cpp
        ../strategy_simulator_lib/src/StockStrategySimulator.cpp
        ../strategy_simulator_lib/src/StochasticOscillatorStrategy.cpp
        ../strategy_simulator_lib/include/StockStrategySimulator.h
        ../strategy_simulator_lib/include/StochasticOscillatorStrategy.h)

find_package(Python COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON3_INCLUDE_DIRS} ${NumPy_INCLUDE_DIRS})

# populate matplotlib repository
include(FetchContent)
FetchContent_Declare(
        matplotlib
        GIT_REPOSITORY https://github.com/lava/matplotlib-cpp.git
        GIT_TAG        f23347fca25219d1c42cbb91608b5556814bf572
)
FetchContent_GetProperties(matplotlib)
if(NOT matplotlib_POPULATED)
    FetchContent_Populate(matplotlib)
endif()
include_directories(SYSTEM ${matplotlib_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PYTHON_LIBRARIES} Python::NumPy)