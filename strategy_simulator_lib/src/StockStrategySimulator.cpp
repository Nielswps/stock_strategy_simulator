#include "../include/StockStrategySimulator.h"
#include "../include/StockDataParser.h"
#include <fstream>
#include <iostream>

StockStrategySimulator::SimulationResult StockStrategySimulator::simulateStrategy(Strategy& strategy, const std::string& pathToFile, int candleStickPeriodInDays, double startingCapital) {
    std::shared_ptr<std::vector<Trade>> trades = std::make_shared<std::vector<Trade>>();
    auto input = new std::ifstream{pathToFile, std::ios_base::in};
    auto stockData = StockDataParser(*input, candleStickPeriodInDays).data;

    // Define method for making a trade
    auto makeTrade = [trades](Trade t) {
        trades->emplace_back(t);
    };

    // Simulate strategy on loaded data and return final capital and trade history
    auto finalCapital = strategy.simulateOnData(&stockData, startingCapital, static_cast<const std::function<void(Trade)> &>(makeTrade));
    return SimulationResult{finalCapital - startingCapital, trades};
}