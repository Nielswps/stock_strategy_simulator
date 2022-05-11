#ifndef STOCK_EXCHANGE_STOCKDATAPARSER_H
#define STOCK_EXCHANGE_STOCKDATAPARSER_H

#include <vector>
#include <ctime>
#include <stack>
#include "istream"
#include "HistoricData.h"
#include "IterableStack.h"

class StockDataParser {
public:
    explicit StockDataParser(std::istream& istream) : StockDataParser::StockDataParser(istream, 1) {};
    explicit StockDataParser(std::istream& istream, int candlestickPeriod);

    HistoricData data{};

    struct trade {
        std::tm time;
        double price;
    };

    IterableStack<trade> trades{};
private:
    std::string error;
    enum parsable {object, list};

    bool loadTrades(std::istream &istream);
};


#endif //STOCK_EXCHANGE_STOCKDATAPARSER_H
