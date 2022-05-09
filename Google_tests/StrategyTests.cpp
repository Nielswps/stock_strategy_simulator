#include <gtest/gtest.h>
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"

class StrategyFixture : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};

TEST_F(StrategyFixture, StochasticOscillatorStrategyInitializationWithFastLesserThanSlowCausesException) {
    EXPECT_THROW({
                     try {
                         auto strategy = StochasticOscillatorStrategy(20, 2);
                     } catch (const std::invalid_argument &e) {
                         EXPECT_STREQ("Periods for fast stochastic indicator must be smaller than periods for slow indicator", e.what());
                         throw;
                     }
                 }, std::invalid_argument);
}

TEST_F(StrategyFixture, StochasticOscillatorStrategyIsConstructedWithFastGreaterThanSlowCausesException) {
    ASSERT_NO_THROW(auto strategy = StochasticOscillatorStrategy(1, 14););
}