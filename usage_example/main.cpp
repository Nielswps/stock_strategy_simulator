#include <iostream>
#include "StochasticOscillatorStrategy.h"
#include "StockStrategySimulator.h"

int main() {
    auto s = StochasticOscillatorStrategy{14, 3};
    auto res = StockStrategySimulator::simulateStrategy(s, "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json", 1, 10000);

    std::cout << "Profit from strategy: " << res.profit << std::endl;

    for (const Trade& t : *res.trades) {
        auto time = gmtime(&t.date);
        std::cout << "Trade made: " << time->tm_year + 1900 << "-" << time->tm_mon << "-" << time->tm_mday << " for " << t.price << std::endl;
    }

    return 0;
}
