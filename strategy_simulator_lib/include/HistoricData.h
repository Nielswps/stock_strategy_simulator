#ifndef STOCK_EXCHANGE_HISTORICDATA_H
#define STOCK_EXCHANGE_HISTORICDATA_H


#include <vector>

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
};

#endif //STOCK_EXCHANGE_HISTORICDATA_H
