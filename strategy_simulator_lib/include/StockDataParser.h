//
// Created by niels on 5/5/22.
//

#ifndef STOCK_EXCHANGE_STOCKDATAPARSER_H
#define STOCK_EXCHANGE_STOCKDATAPARSER_H

#include <vector>
#include <ctime>
#include "istream"

class StockDataParser {
public:
    explicit StockDataParser(std::istream& istream);



    struct trade {
        std::tm time;
        float price;
    };
    std::vector<trade> trades{};
private:
    std::string error;
    enum parsable {object, list};

    bool loadPointersToTrades(std::istream &istream);
};


#endif //STOCK_EXCHANGE_STOCKDATAPARSER_H
