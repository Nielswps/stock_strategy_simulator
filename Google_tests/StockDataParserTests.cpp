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
        auto stockData = StockDataParser(*input);

        ASSERT_EQ(stockData.trades.size(), 1);
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
        auto stockData = StockDataParser(*input);
        ASSERT_EQ(stockData.trades.size(), 3);

        std::vector<StockDataParser::trade> expectedTrades{
                StockDataParser::trade{getTm("2012-10-31T08:00:00.000+0100"), 1278},
                StockDataParser::trade{getTm("2012-10-31T08:00:00.000+0100"), 1278},
                StockDataParser::trade{getTm("2012-10-31T08:00:00.000+0100"), 1269},
        };

        for (int i = 0; i < stockData.trades.size(); i++) {
            ASSERT_EQ(mktime(&stockData.trades[i].time), mktime(&expectedTrades[i].time));
            ASSERT_EQ(stockData.trades[i].price, expectedTrades[i].price);
        }
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}

TEST_F(StockDataParserFixture, WhiteSpaceCharactersConstructsObject) {
    auto input = new std::istringstream{
            "{\"trades\":\r[{\"time\":\"2012-10-31T08:00:00.000+0100\",\n\t\"price\":1278}]})"
    };

    try {
        auto stockData = StockDataParser(*input);
        ASSERT_EQ(stockData.trades.size(), 1);
    } catch (std::exception const &err) {
        FAIL() << err.what();
    }
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedWhenParsingAcceptedTradeData) {
    auto input = new std::ifstream{
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/three_trade_test.json",
            std::ios_base::in};
    auto stockData = StockDataParser(*input).data;

    ASSERT_FALSE(stockData.candlesticks.empty());
    ASSERT_EQ(stockData.candlesticks.size(), 1);
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedForPAALB) {
    auto input = new std::ifstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                   std::ios_base::in};
    auto stockData = StockDataParser(*input).data;

    ASSERT_FALSE(stockData.candlesticks.empty());
    ASSERT_EQ(stockData.candlesticks.size(), 489);
}

TEST_F(StockDataParserFixture, CandleSticksAreCreatedForPAALBWithTwoDayPeriod) {
    auto input = new std::ifstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                   std::ios_base::in};

    auto startTime = std::chrono::system_clock::now();

    auto stockData = StockDataParser(*input, 2).data;

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    std::cout << "Execution took: " << elapsed_seconds.count() << std::endl;

    ASSERT_FALSE(stockData.candlesticks.empty());
    ASSERT_EQ(stockData.candlesticks.size(), 299);
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
    auto input = new std::istringstream{R"({"trades": [{"time":"2012-10-31T08:00:00.000+0100","price":1278}]})"};
    auto stockData = StockDataParser(*input);

    auto expectedDate = tm{
            0,
            0,
            8,
            31,
            9,
            112
    };

    ASSERT_EQ(stockData.trades.back().time.tm_year, expectedDate.tm_year);
    ASSERT_EQ(stockData.trades.back().time.tm_mon, expectedDate.tm_mon);
    ASSERT_EQ(stockData.trades.back().time.tm_mday, expectedDate.tm_mday);
    ASSERT_EQ(stockData.trades.back().time.tm_hour, expectedDate.tm_hour);
    ASSERT_EQ(stockData.trades.back().time.tm_min, expectedDate.tm_min);
    ASSERT_EQ(stockData.trades.back().time.tm_sec, expectedDate.tm_sec);
}

TEST_F(StockDataParserFixture, GeneratedTradesHaveCorrectDateAndTimeAtAlmostNoonWithSeconds) {
    auto input = new std::istringstream{R"({"trades": [{"time":"2010-6-4T11:45:30.000+0100","price":1278}]})"};
    auto stockData = StockDataParser(*input);

    auto expectedDate = tm{
            30,
            45,
            11,
            4,
            5,
            110
    };

    ASSERT_EQ(stockData.trades.back().time.tm_year, expectedDate.tm_year);
    ASSERT_EQ(stockData.trades.back().time.tm_mon, expectedDate.tm_mon);
    ASSERT_EQ(stockData.trades.back().time.tm_mday, expectedDate.tm_mday);
    ASSERT_EQ(stockData.trades.back().time.tm_hour, expectedDate.tm_hour);
    ASSERT_EQ(stockData.trades.back().time.tm_min, expectedDate.tm_min);
    ASSERT_EQ(stockData.trades.back().time.tm_sec, expectedDate.tm_sec);
}