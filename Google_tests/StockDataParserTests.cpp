#include <fstream>
#include "gtest/gtest.h"
#include "istream"
#include "../strategy_simulator_lib/include/StockDataParser.h"

class StockDataParserFixture : public ::testing::Test {

protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

TEST_F(StockDataParserFixture, IStreamContainingWordThrowsException) {
    auto input = new std::istringstream{"hee"};

    ASSERT_THROW(auto stockData = StockDataParser(*input), std::invalid_argument);
}

TEST_F(StockDataParserFixture, IStreamContainingEmptyKeyThrowsException) {
    auto input = new std::istringstream{R"({"": "value"})"};

    EXPECT_THROW({
                     try {
                         auto stockData = StockDataParser(*input);
                     } catch (const std::invalid_argument &e) {
                         EXPECT_STREQ("':' encounter without key", e.what());
                         throw;
                     }
                 }, std::invalid_argument);
}

TEST_F(StockDataParserFixture, IStreamContainingClosingBracketBeforeOpeningBracketThrowsException) {
    auto input = new std::istringstream{R"({"key": ]})"};

    EXPECT_THROW({
                     try {
                         auto stockData = StockDataParser(*input);
                     } catch (const std::invalid_argument &e) {
                         EXPECT_STREQ("']' encounter without matching '['", e.what());
                         throw;
                     }
                 }, std::invalid_argument);
}

TEST_F(StockDataParserFixture, IStreamContainingNoDataThrowsException) {
    auto input = new std::istringstream{R"({})"};

    EXPECT_THROW({
                     try {
                         auto stockData = StockDataParser(*input);
                     } catch (const std::invalid_argument &e) {
                         EXPECT_STREQ("Invalid stock data", e.what());
                         throw;
                     }
                 }, std::invalid_argument);
}

TEST_F(StockDataParserFixture, IStreamNotContainingTradesKeyThrowsException) {
    auto input = new std::istringstream{R"({"key": "value"})"};

    ASSERT_THROW(auto stockData = StockDataParser(*input), std::invalid_argument);
}

TEST_F(StockDataParserFixture, IStreamWithExampleDataConstructsObject) {
    auto input = new std::istringstream{R"({"trades": [ { "time":"2012-10-31T08:00:00.000+0100", "price":1278 } ]})"};

    try {
        auto stockData = StockDataParser(*input);

        ASSERT_EQ(stockData.trades.size(), 2);

    }
    catch (std::exception const & err) {
        FAIL() << err.what();
    }
}