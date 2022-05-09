#include <gtest/gtest.h>
#include <fstream>
#include "../strategy_simulator_lib/include/StockDataParser.h"

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
        auto stockData = StockDataParser(*input);
        ASSERT_EQ(stockData.trades.size(), 4030697);
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}