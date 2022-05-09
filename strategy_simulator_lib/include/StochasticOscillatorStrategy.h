#ifndef STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H
#define STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H

#include <queue>
#include "Strategy.h"
#include "IterableQueue.h"

class StochasticOscillatorStrategy : public Strategy {
public:
    StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator);

    void simulateOnData(const HistoricData *data, double availableMoney, void (*makeTrade)(Trade)) override;

private:
    bool fastIndicatorOnTop = false;
    int periodsForFastIndicator;
    int periodsForSlowIndicator;
    double fastStochasticIndicator = 0;
    double slowStochasticIndicator = 0;
    IterableQueue<double> fastStochasticIndicatorHistory{};
};

#endif //STOCK_EXCHANGE_STOCHASTICOSCILLATORSTRATEGY_H
