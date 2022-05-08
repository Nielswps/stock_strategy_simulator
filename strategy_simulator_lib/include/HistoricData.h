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
        double openingPrice;
        double closingPrice;
        double lowestPrice;
        double highestPrice;
        bool upwardTrend;
        std::pair<time_t, time_t> timeSpan;
    };

    std::vector<candlestick> candlesticks{};
    int candlestickPeriod;
};

#endif //STOCK_EXCHANGE_HISTORICDATA_H
