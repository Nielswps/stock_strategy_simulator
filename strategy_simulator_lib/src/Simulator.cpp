//
// Created by niels on 5/9/22.
//

#include "../include/Simulator.h"
#include "../include/StockDataParser.h"
#include <fstream>
#include <iostream>

Simulator::SimulationResult
Simulator::simulateStrategy(Strategy& strategy, const std::string& pathToFile, int candleStickPeriodInDays, double startMoney) {
    auto input = new std::ifstream{pathToFile, std::ios_base::in};
    auto stockData = StockDataParser(*input, candleStickPeriodInDays).data;
    auto pointerToTrades = this->trades;

    auto makeTrade = [pointerToTrades](Trade t) {
        pointerToTrades->emplace_back(t);
    };

    auto finalCapital = strategy.simulateOnData(&stockData, startMoney, static_cast<const std::function<void(Trade)> &>(makeTrade));

    return SimulationResult{finalCapital - startMoney, trades};
}