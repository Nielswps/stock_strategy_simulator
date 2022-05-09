#include <gtest/gtest.h>
#include "../strategy_simulator_lib/include/Simulator.h"
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
                Trade{false, static_cast<int>(data->candlesticks.size()), 2, data->candlesticks.back().timeSpan.second});
        currentCapital += static_cast<int>(data->candlesticks.size() * 2);
        return currentCapital;
    };
};

TEST_F(SimulatorFixture, SimulatorCanBeConstructed) {
    ASSERT_NO_THROW(auto sim = Simulator{});
}

TEST_F(SimulatorFixture, SimulatorCanSimulateTransactionWithoutException) {
    auto strat = TestStrategy{};
    Simulator::SimulationResult res;

    ASSERT_NO_THROW(res = Simulator::simulateStrategy(strat,
                                         "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                         1, 100000.));
    ASSERT_EQ(res.trades->size(), 490);
}

TEST_F(SimulatorFixture, SimulationByingSotkcsAtOneAndSellingAtTwoReturnsTheDoubleProfit) {
    auto strat = TestStrategy{};
    auto res = Simulator::simulateStrategy(strat,
                                         "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                         1, 100000.);

    ASSERT_EQ(res.profit, 489);
    ASSERT_EQ(res.trades->size(), 490);
}

TEST_F(SimulatorFixture, TradesAreBuyAndSellAsExpected) {
    auto strat = TestStrategy{};
    auto res = Simulator::simulateStrategy(strat,
                                    "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                    1, 100000.);

    for (int i = 0; i < res.trades->size() - 1; i++) {
        ASSERT_TRUE((res.trades->data() + i)->buy);
    }

    ASSERT_FALSE((res.trades->back()).buy);
}