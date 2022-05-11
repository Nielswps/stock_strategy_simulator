#include "gtest/gtest.h"
#include "istream"
#include <fstream>
#include "../strategy_simulator_lib/include/StockDataParser.h"

class StockDataParserFixture : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }

    static tm getTm(const char *timeAsString) {
        const char *time_details = timeAsString;
        struct tm tm{};
        strptime(time_details, "%Y-%m-%dT%H:%M:%S", &tm);
        return tm;
    }
};

TEST_F(StockDataParserFixture, IStreamContainingWordThrowsException) {
    auto input = new std::istringstream{"hee"};
    ASSERT_THROW(auto stockData = StockDataParser(*input), std::invalid_argument);
}

TEST_F(StockDataParserFixture, EmptyKeyThrowsException) {
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

TEST_F(StockDataParserFixture, ClosingBracketBeforeOpeningBracketThrowsException) {
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

TEST_F(StockDataParserFixture, NoDataThrowsException) {
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

TEST_F(StockDataParserFixture, NoTradesKeyThrowsException) {
    auto input = new std::istringstream{R"({"key": "value"})"};
    ASSERT_THROW(auto stockData = StockDataParser(*input), std::invalid_argument);
}

TEST_F(StockDataParserFixture, IStreamWithExampleDataConstructsObject) {
    auto input = new std::istringstream{R"({"trades": [{"time":"2012-10-31T08:00:00.000+0100","price":1278}]})"};
    try {
        auto stockData = StockDataParser(*input).data;

        ASSERT_EQ(stockData.candlesticks.size(), 1);
    }
    catch (std::exception const &err) {
        FAIL() << err.what();
    }
}

TEST_F(StockDataParserFixture, IFStreamWithExampleDataConstructsObject) {
    auto input = new std::ifstream{
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/three_trade_test.json",
            std::ios_base::in};
    try {
        auto stockData = StockDataParser(*input).data;
        ASSERT_EQ(stockData.candlesticks.size(), 1);

        std::vector<StockDataParser::trade> expectedTrades{
                StockDataParser::trade{getTm("2012-10-31T08:00:00.000+0100"), 1269},
                StockDataParser::trade{getTm("2012-10-31T08:00:00.000+0100"), 1278},
                StockDataParser::trade{getTm("2012-10-31T08:00:00.000+0100"), 1278},
        };

        ASSERT_EQ(stockData.candlesticks[0].timeSpan.first, mktime(&expectedTrades[0].time));
        ASSERT_EQ(stockData.candlesticks[0].openingPrice, expectedTrades[0].price);
        ASSERT_EQ(stockData.candlesticks[0].closingPrice, expectedTrades[2].price);
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}

TEST_F(StockDataParserFixture, WhiteSpaceCharactersConstructsObject) {
    auto input = new std::istringstream{
            "{\"trades\":\r[{\"time\":\"2012-10-31T08:00:00.000+0100\",\n\t\"price\":1278}]})"
    };

    try {
        auto stockData = StockDataParser(*input).data;
        ASSERT_EQ(stockData.candlesticks.size(), 1);
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedWhenParsingAcceptedTradeData) {
    auto input = new std::ifstream{
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/three_trade_test.json",
            std::ios_base::in};
    auto stockData = StockDataParser(*input).data;

    ASSERT_EQ(stockData.candlesticks.size(), 1);
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedForPAALB) {
    auto input = new std::ifstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                   std::ios_base::in};
    auto stockData = StockDataParser(*input).data;

    ASSERT_EQ(stockData.candlesticks.size(), 350);
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedForPAALBWithTwoDayPeriod) {
    auto input = new std::ifstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                   std::ios_base::in};
    auto stockData = StockDataParser(*input, 2).data;

    ASSERT_EQ(stockData.candlesticks.size(), 225);
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedInIncreasingTimeForPAALB) {
    auto input = new std::ifstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                   std::ios_base::in};
    auto stockData = StockDataParser(*input).data;

    ASSERT_TRUE(std::is_sorted(stockData.candlesticks.begin(), stockData.candlesticks.end(),
                               [](HistoricData::candlestick a, HistoricData::candlestick b) {
                                   return a.timeSpan.first < b.timeSpan.first;
                               }));

    for (auto &candlestick: stockData.candlesticks) {
        ASSERT_TRUE([&candlestick]() { return candlestick.timeSpan.first < candlestick.timeSpan.second; }());
    }
}

TEST_F(StockDataParserFixture, GeneratedTradesHaveCorrectDateAndTimeAtEightAM) {
    auto input = new std::istringstream{R"({"trades": [{"time":"2012-10-31T08:00:00.000","price":1278}]})"};
    auto stockData = StockDataParser(*input).data;

    auto expectedDate = tm{
            0,
            0,
            8,
            31,
            9,
            112
    };

    ASSERT_EQ(stockData.candlesticks[0].timeSpan.first, mktime(&expectedDate));
}

TEST_F(StockDataParserFixture, GeneratedTradesHaveCorrectDateAndTimeAtAlmostNoonWithSeconds) {
    auto input = new std::istringstream{R"({"trades": [{"time":"2010-6-4T11:45:30.000","price":1278}]})"};
    auto stockData = StockDataParser(*input).data;

    auto expectedDate = tm{
            30,
            45,
            11,
            4,
            5,
            110
    };

    ASSERT_EQ(stockData.candlesticks[0].timeSpan.first, mktime(&expectedDate));

}