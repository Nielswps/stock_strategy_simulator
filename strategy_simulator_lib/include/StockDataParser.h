//
// Created by niels on 5/5/22.
//

#ifndef STOCK_EXCHANGE_STOCKDATAPARSER_H
#define STOCK_EXCHANGE_STOCKDATAPARSER_H

#include <vector>
#include <ctime>
#include "istream"
#include "HistoricData.h"

class StockDataParser {
public:
    explicit StockDataParser(std::istream& istream) : StockDataParser::StockDataParser(istream, 1) {};
    explicit StockDataParser(std::istream& istream, int candlestickPeriod);

    HistoricData data{};

    struct trade {
        std::tm time;
        double price;
    };

    std::vector<trade> trades{};
private:
    std::string error;
    enum parsable {object, list};

    bool loadPointersToTrades(std::istream &istream);
};


#endif //STOCK_EXCHANGE_STOCKDATAPARSER_H
