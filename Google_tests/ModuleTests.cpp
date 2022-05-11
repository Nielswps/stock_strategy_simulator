#include <gtest/gtest.h>
#include <fstream>
#include <numeric>
#include <cmath>
#include "../strategy_simulator_lib/include/StockDataParser.h"
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"
#include "../strategy_simulator_lib/include/StockStrategySimulator.h"

class ModuleFixture : public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

TEST_F(ModuleFixture, LargestDataFileCanBeParsedWithoutCrash) {
    auto input = new std::ifstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/HMB.json",std::ios_base::in};
    try {
        auto stockData = StockDataParser(*input).data;
        ASSERT_EQ(stockData.candlesticks.size(), 495);
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}

TEST_F(ModuleFixture, SimulationOfStochasticOscillatorOnPAALBGivesLoss) {
    auto s = StochasticOscillatorStrategy{14, 3};
    auto res = StockStrategySimulator::simulateStrategy(s, "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json", 1, 10000);

    ASSERT_LT(res[0].profit, 0);
}

TEST_F(ModuleFixture, SimulateStrategyOnDirectory) {
    auto s = StochasticOscillatorStrategy{14, 3};
    auto path = "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/";
    auto res = StockStrategySimulator::simulateStrategy(s, path, 1, 10000);

    ASSERT_FALSE(res.empty());
}