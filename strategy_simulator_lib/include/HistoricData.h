//
// Created by niels on 5/7/22.
//

#ifndef STOCK_EXCHANGE_HISTORICDATA_H
#define STOCK_EXCHANGE_HISTORICDATA_H


#include <vector>
#include "Strategy.h"

class  HistoricData {
public:
    struct candlestick {
        int period;
        double openingPrice;
        double closingPrice;
        double lowestPrice;
        double highestPrice;
        bool upwardTrend;
    };

    std::vector<candlestick> candlesticks{};
};

#endif //STOCK_EXCHANGE_HISTORICDATA_H
