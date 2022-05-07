//
// Created by niels on 5/7/22.
//

#ifndef STOCK_EXCHANGE_DATA_H
#define STOCK_EXCHANGE_DATA_H

class Data {
public:
    struct candlestick {
        int period;
        double openingPrice;
        double closingPrice;
        double lowestPrice;
        double highestPrice;
        bool upwardTrend;
    };
};

#endif //STOCK_EXCHANGE_DATA_H
