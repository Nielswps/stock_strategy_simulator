//
// Created by niels on 5/9/22.
//

#ifndef STOCK_EXCHANGE_SIMULATOR_H
#define STOCK_EXCHANGE_SIMULATOR_H


#include <memory>
#include "Strategy.h"

class Simulator {
public:
    struct SimulationResult {
        double profit;
        std::shared_ptr<std::vector<Trade>> trades;
    };

    static SimulationResult simulateStrategy(Strategy& strategy, const std::string& pathToFile, int candleStickPeriodInDays, double startingCapital);
};


#endif //STOCK_EXCHANGE_SIMULATOR_H
