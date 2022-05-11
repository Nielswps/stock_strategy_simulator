#include <iostream>
#include "../strategy_simulator_lib/include/StockStrategySimulator.h"
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"
#include <matplotlibcpp.h>

void simulateStochasticOscillatorForPAALB();

namespace plt = matplotlibcpp;

int main() {
    simulateStochasticOscillatorForPAALB();

    return 0;
}

void simulateStochasticOscillatorForPAALB() {
    auto s = StochasticOscillatorStrategy{4, 3};
    auto res = StockStrategySimulator::simulateStrategy(s,
                                                        "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/HMB.json",
                                                        2, 10000);

    std::cout << "Trades made during simulation:" << std::endl;
    for (const auto &t: res[0].trades) {
        auto time = gmtime(&t.date);
        std::cout << (t.buy ? "Bought " : "Sold ") << t.amount << " shares at " << t.price
                  << " each at " << time->tm_year + 1900 << "-" << time->tm_mon << "-" << time->tm_mday << " "
                  << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << std::endl;
    }

    std::cout << (res[0].profit < 0 ? "Loss from strategy:" : "Profit from strategy: ") << std::abs(res[0].profit)
              << std::endl;
}
