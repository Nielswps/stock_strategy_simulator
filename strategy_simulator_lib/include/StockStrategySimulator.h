#ifndef STOCK_EXCHANGE_STOCKSTRATEGYSIMULATOR_H
#define STOCK_EXCHANGE_STOCKSTRATEGYSIMULATOR_H


#include <memory>
#include "Strategy.h"

class StockStrategySimulator {
public:
    struct SimulationResult {
        double profit;
        std::vector<Trade> trades;
    };

    static std::vector<SimulationResult>
    simulateStrategy(Strategy &strategy, const std::string &path, int candleStickPeriodInDays, double startingCapital);

private:
    static SimulationResult
    getResultForFile(Strategy &strategy, const std::string &path, const int &candleStickPeriodInDays,
                     const double &startingCapital);
};


#endif //STOCK_EXCHANGE_STOCKSTRATEGYSIMULATOR_H
