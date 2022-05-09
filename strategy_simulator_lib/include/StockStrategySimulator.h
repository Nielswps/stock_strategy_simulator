#ifndef STOCK_EXCHANGE_STOCKSTRATEGYSIMULATOR_H
#define STOCK_EXCHANGE_STOCKSTRATEGYSIMULATOR_H


#include <memory>
#include "Strategy.h"

class StockStrategySimulator {
public:
    struct SimulationResult {
        double profit;
        std::shared_ptr<std::vector<Trade>> trades;
    };

    static SimulationResult simulateStrategy(Strategy& strategy, const std::string& pathToFile, int candleStickPeriodInDays, double startingCapital);
};


#endif //STOCK_EXCHANGE_STOCKSTRATEGYSIMULATOR_H
