#include <gtest/gtest.h>
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"

class StrategyFixture : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};

TEST_F(StrategyFixture, StochasticOscillatorStrategyIsConstructed) {
    ASSERT_NO_THROW(auto strategy = StochasticOscillatorStrategy(1, 14););
    ASSERT_NO_THROW(auto strategy = StochasticOscillatorStrategy(6, 2););
}