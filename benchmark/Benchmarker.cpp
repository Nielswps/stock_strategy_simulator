#include <chrono>
#include <iostream>
#include <fstream>
#include "../strategy_simulator_lib/include/StockDataParser.h"

int main() {
    auto startTime = std::chrono::system_clock::now();

    auto input = std::fstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json", std::ios_base::in};
    auto sdp = StockDataParser{input, 3};

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;

    std::cout << "Time for parsing PAALB.json (748.2 kB)" << elapsedTime.count() << std::endl;

    startTime = std::chrono::system_clock::now();

    input = std::fstream{"/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/MAERSKB.json", std::ios_base::in};
    sdp = StockDataParser{input, 3};

    endTime = std::chrono::system_clock::now();
    elapsedTime = endTime - startTime;

    std::cout << "Time for parsing MAERSKB.json (145.5 MB)" << elapsedTime.count() << std::endl;

    return 0;
}