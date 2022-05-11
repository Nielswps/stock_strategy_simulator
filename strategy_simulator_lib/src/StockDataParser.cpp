#include <iostream>
#include <map>
#include <stack>
#include "../include/StockDataParser.h"

StockDataParser::StockDataParser(std::istream &istream, int candlestickPeriod) {
    if (loadTrades(istream)) {
        // Trades have been successfully loaded, created historic data with candlesticks
        auto initialTrade = trades.top();
        auto currentStartTime = (std::time_t) mktime(&initialTrade.time);
        double currentOpeningPrice = initialTrade.price, currentClosingPrice = initialTrade.price,
                currentLowestPrice = initialTrade.price, currentHighestPrice = initialTrade.price;
        bool allDataCandlesticksSaved = false;
        trades.pop();

        while (!trades.empty()) {
            trade &t = trades.top();

            std::time_t time = std::mktime(&t.time);
            if (time != (std::time_t) (-1)) {
                double difference = std::difftime(currentStartTime, time) / (60 * 60 * 24);
                if (std::abs(difference) < candlestickPeriod) {
                    currentLowestPrice = std::min(currentLowestPrice, t.price);
                    currentHighestPrice = std::max(currentHighestPrice, t.price);
                    currentClosingPrice = t.price;

                    allDataCandlesticksSaved = false;
                } else {
                    // save candle stick
                    data.candlesticks.push_back(HistoricData::candlestick{
                            currentOpeningPrice,
                            currentClosingPrice,
                            currentLowestPrice,
                            currentHighestPrice,
                            currentOpeningPrice < currentClosingPrice,
                            std::pair<time_t, time_t>{currentStartTime, time}
                    });

                    // Prepare next candlestick with current trade
                    currentStartTime = mktime(&t.time);
                    currentLowestPrice = t.price;
                    currentHighestPrice = t.price;
                    currentOpeningPrice = t.price;
                    currentClosingPrice = t.price;

                    allDataCandlesticksSaved = true;
                }
            }

            trades.pop();
        }

        // Make sure to push any dangling candlestick to the vector
        if (!allDataCandlesticksSaved) {
            data.candlesticks.push_back(HistoricData::candlestick{
                    currentOpeningPrice,
                    currentClosingPrice,
                    currentLowestPrice,
                    currentHighestPrice,
                    currentOpeningPrice < currentClosingPrice,
                    std::pair<time_t, time_t>{currentStartTime, reinterpret_cast<const long>(time)}
            });
        }

    } else {
        throw std::invalid_argument(error);
    }
}

bool StockDataParser::loadTrades(std::istream &istream) {
    std::stack<parsable> parsedObjectStack;
    char currentChar;
    std::string loadedChars;
    bool isParsingString, isParsingValue;

    std::string key;
    std::map<std::string, std::string> currentObject{};

    if (istream.bad()) {
        error.append("Input stream is bad");
        return false;
    }

    while ((currentChar = istream.get()) != -1) {
        switch (currentChar) {
            case '"':
                if (!isParsingString) {
                    isParsingString = true;

                    if (!key.empty()) {
                        isParsingValue = true;
                    }

                    break;
                }

                if (key.empty()) {
                    // We have finished parsing key
                    key = loadedChars;
                    loadedChars.clear();
                } else if (parsedObjectStack.top() != object) {
                    // String value parsed outside of object
                }

                isParsingString = false;
                break;

            case ':':
                if (key.empty()) {
                    error.append("':' encounter without key");
                    return false;
                }

                if (!(isParsingString || loadedChars.empty())) {
                    error.append("Trailing '\"' missing from key");
                    return false;
                }

                if (isParsingString) {
                    loadedChars.append(&currentChar);
                }

                isParsingValue = true;
                break;

            case ',':
                if (!(isParsingValue || parsedObjectStack.top() == list)) {
                    error.append("',' encountered unexpectedly");
                    return false;
                }

                currentObject[key] = loadedChars;
                isParsingValue = false;
                loadedChars.clear();
                key.clear();
                break;

            case '[':
                if (parsedObjectStack.top() == list && key.empty()) {
                    error.append("List encountered within list without a key");
                    return false;
                };

                parsedObjectStack.push(list);
                key.clear();
                break;

            case ']':
                if (parsedObjectStack.top() != list) {
                    error.append("']' encounter without matching '['");
                    return false;
                }

                parsedObjectStack.pop();
                key.clear();
                break;

            case '{':
                parsedObjectStack.push(object);
                key.clear();
                break;

            case '}': {
                if (parsedObjectStack.top() != object) {
                    error.append("'}' encountered before '{'");
                    return false;
                }
                parsedObjectStack.pop();

                if (!loadedChars.empty()) {
                    currentObject[key] = loadedChars;
                    loadedChars.clear();
                }

                if (currentObject.contains("time") && currentObject.contains("price")) {
                    std::string time_details = currentObject["time"];
                    struct tm tm{};
                    strptime(time_details.c_str(), "%Y-%m-%dT%H:%M:%S", &tm);

                    try {
                        this->trades.push(trade{tm, std::stof(currentObject["price"])});
                    } catch (std::exception &e) {
                        error.append(e.what());
                        return false;
                    }
                    currentObject.clear();
                }

                isParsingValue = false;
                key.clear();
                break;
            }

            case ' ':
            case '\n':
            case '\r':
            case '\t':
                break;

            default:
                loadedChars.push_back(currentChar);
                break;
        }
    }

    if (trades.empty() || !parsedObjectStack.empty()) {
        error.append("Invalid stock data");
        return false;
    }

    return true;
}