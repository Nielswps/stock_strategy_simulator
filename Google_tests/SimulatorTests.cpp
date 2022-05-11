#include <gtest/gtest.h>
#include "../strategy_simulator_lib/include/StockStrategySimulator.h"
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"

class SimulatorFixture : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};

class TestStrategy : public Strategy {
    double
    simulateOnData(const HistoricData *data, double availableMoney, std::function<void(Trade)> makeTrade) override {
        auto currentCapital = availableMoney;
        for (const auto &candlestick: data->candlesticks) {
            makeTrade(
                    Trade{true, 1, 1, candlestick.timeSpan.first});
            currentCapital -= 1;
        }

        makeTrade(
                Trade{false, static_cast<int>(data->candlesticks.size()), 2,
                      data->candlesticks.back().timeSpan.second});
        currentCapital += static_cast<int>(data->candlesticks.size() * 2);
        return currentCapital;
    };
};

TEST_F(SimulatorFixture, SimulatorCanBeConstructed) {
    ASSERT_NO_THROW(auto sim = StockStrategySimulator{});
}

TEST_F(SimulatorFixture, SimulatorCanSimulateTransactionWithoutException) {
    auto strat = TestStrategy{};
    std::vector<StockStrategySimulator::SimulationResult> res;

    ASSERT_NO_THROW(res = StockStrategySimulator::simulateStrategy(strat,
                                                                   "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                                                   1, 100000));
    ASSERT_EQ(res[0].trades.size(), 429);
}

TEST_F(SimulatorFixture, SimulationByingSotkcsAtOneAndSellingAtTwoReturnsTheDoubleProfit) {
    auto strat = TestStrategy{};
    auto res = StockStrategySimulator::simulateStrategy(strat,
                                                        "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                                        1, 100000.);

    ASSERT_EQ(res[0].profit, 350);
    ASSERT_EQ(res[0].trades.size(), 351);
}

TEST_F(SimulatorFixture, BuyAndSellAsExpected) {
    auto strat = TestStrategy{};
    auto res = StockStrategySimulator::simulateStrategy(strat,
                                                        "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                                        1, 100000.);

    for (int i = 0; i < res[0].trades.size() - 1; i++) {
        ASSERT_TRUE((res[0].trades.data() + i)->buy);
    }

    ASSERT_FALSE((res[0].trades.back()).buy);
}