//
// Created by niels on 5/6/22.
//

#ifndef STOCK_EXCHANGE_STRATEGY_H
#define STOCK_EXCHANGE_STRATEGY_H


#include <string>
#include <functional>
#include "map"
#include "HistoricData.h"

struct Trade {
    bool buy;
    int amount;
    double price;
    time_t date;
};

class Strategy {
public:
    virtual double simulateOnData(const HistoricData *data, double availableMoney, std::function<void(Trade)> makeTrade) = 0;
};


#endif //STOCK_EXCHANGE_STRATEGY_H
