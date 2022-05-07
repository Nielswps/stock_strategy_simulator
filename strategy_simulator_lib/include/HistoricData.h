//
// Created by niels on 5/7/22.
//

#ifndef STOCK_EXCHANGE_HISTORICDATA_H
#define STOCK_EXCHANGE_HISTORICDATA_H


#include <vector>
#include "Strategy.h"

class  HistoricData : Data {
public:
    std::vector<candlestick> candlesticks{};

    void accept(Strategy *visitor);
};


#endif //STOCK_EXCHANGE_HISTORICDATA_H
