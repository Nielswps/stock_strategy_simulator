project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run StockDataParserTests.cpp ../strategy_simulator_lib/src/StockDataParser.cpp ../strategy_simulator_lib/src/StochasticOscillatorStrategy.cpp ../strategy_simulator_lib/src/StockStrategySimulator.cpp  ModuleTests.cpp StrategyTests.cpp SimulatorTests.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)