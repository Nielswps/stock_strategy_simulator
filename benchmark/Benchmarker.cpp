#include <chrono>
#include <iostream>
#include <fstream>
#include "../strategy_simulator_lib/include/StockStrategySimulator.h"
#include "../strategy_simulator_lib/include/StockDataParser.h"
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"

void benchMarkDataParser();

void benchmarkParallelSimulation();

int main() {
    benchMarkDataParser();
    benchmarkParallelSimulation();

    return 0;
}

void benchMarkDataParser() {
    auto startTime = std::chrono::system_clock::now();
    auto input = std::fstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                              std::ios_base::in};
    auto sdp = StockDataParser{input, 3};

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;

    std::cout << "Time for parsing PAALB.json (748.2 kB)" << elapsedTime.count() << std::endl;

    startTime = std::chrono::system_clock::now();
    input = std::fstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/MAERSKB.json",
                         std::ios_base::in};
    sdp = StockDataParser{input, 3};
    endTime = std::chrono::system_clock::now();
    elapsedTime = endTime - startTime;

    std::cout << "Time for parsing MAERSKB.json " << elapsedTime.count() << std::endl;
}

void benchmarkParallelSimulation() {
    auto s = StochasticOscillatorStrategy{14, 3};

    auto startTime = std::chrono::system_clock::now();
    auto path = "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/";
    auto res = StockStrategySimulator::simulateStrategy(s, path, 1, 10000);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;

    std::cout << "Time for parsing 5 files with parallelism: " << elapsedTime.count() << std::endl;

    auto individualPaths = new std::vector<std::string>{
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/HMB.json",
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/MAERSKB.json"
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json"
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/three_trade_test.json"
            "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/TRYG.json"
    };

    startTime = std::chrono::system_clock::now();
    for (const auto& p : *individualPaths) {
        res = StockStrategySimulator::simulateStrategy(s, p, 1, 10000);
    }

    endTime = std::chrono::system_clock::now();
    elapsedTime = endTime - startTime;

    std::cout << "Time for parsing 5 files sequential: " << elapsedTime.count() << std::endl;
}
