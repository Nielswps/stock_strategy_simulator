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
    auto input = new std::ifstream{"/home/niels/Documents/uni/8. Semester/sP/exam_project/json-2y_2010-11-01_2012-11-01/json-2y/HMB.json",std::ios_base::in};
    try {
        auto stockData = StockDataParser(*input);
        ASSERT_EQ(stockData.trades.size(), 4030697);
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}