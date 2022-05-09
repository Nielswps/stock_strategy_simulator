#ifndef STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H
#define STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H

#include <queue>
#include "Strategy.h"
#include "IterableQueue.h"

class StochasticOscillatorStrategy : public Strategy {
public:
    StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator);

    double simulateOnData(const HistoricData *data, double availableCapital, std::function<void(Trade)> makeTrade) override;

private:
    int periodsForFastIndicator;
    int periodsForSlowIndicator;
};

#endif //STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H
