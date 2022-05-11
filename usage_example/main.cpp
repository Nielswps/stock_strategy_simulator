#include <iostream>
#include <StochasticOscillatorStrategy.h>
#include <StockStrategySimulator.h>

int main() {
    auto s = StochasticOscillatorStrategy{14, 3};
    auto res = StockStrategySimulator::simulateStrategy(s, "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json", 1, 10000);

    std::cout << "Profit from strategy: " << res[0].profit << std::endl;
    std::cout << "Trades made during simulation:" << std::endl;

    for (const Trade& t : res[0].trades) {
        auto time = gmtime(&t.date);
        std::cout << time->tm_year + 1900 << "-" << time->tm_mon << "-" << time->tm_mday << " at " << t.price << "DKK" << std::endl;
    }



    return 0;
}
