//
// Created by niels on 5/6/22.
//

#ifndef STOCK_EXCHANGE_STRATEGY_H
#define STOCK_EXCHANGE_STRATEGY_H


#include <string>
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
    virtual void simulateOnData(const HistoricData *data, double availableMoney, void (*makeTrade)(Trade)) = 0;
protected:
    double availableMoney = 1000;
};


#endif //STOCK_EXCHANGE_STRATEGY_H
