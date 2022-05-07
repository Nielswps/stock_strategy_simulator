#ifndef STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H
#define STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H

#include <queue>
#include "Strategy.h"
#include "IterableQueue.h"

class StochasticOscillatorStrategy : public Strategy {
public:
    StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator);

    void simulateOnData(const HistoricData *data);

private:
    bool fastIndicatorOnTop = false;
    int periodsForFastIndicator;
    int periodsForSlowIndicator;
    double fastStochasticIndicator;
    double slowStochasticIndicator;
    IterableQueue<double> fastStochasticIndicatorHistory{};
};

#endif //STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H
