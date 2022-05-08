#include <iostream>
#include <map>
#include <stack>
#include "../include/StockDataParser.h"
#include "../include/HistoricData.h"

StockDataParser::StockDataParser(std::istream &istream, int candlestickPeriod) {
    if (loadPointersToTrades(istream)) {
        // Trades have been successfully loaded, created historic data with candlesticks
        auto currentStartTime = (std::time_t) (0);
        double currentOpeningPrice, currentClosingPrice, currentLowestPrice, currentHighestPrice;

        data.candlestickPeriod = candlestickPeriod;

        std::sort(trades.begin(), trades.end(), [](trade a, trade b){ return mktime(&a.time) < mktime(&b.time); });

        for (trade t: trades) {
            std::time_t time = std::mktime(&t.time);
            if (time != (std::time_t) (-1)) {
                double difference = std::difftime(currentStartTime, time) / (60 * 60 * 24);
                if (std::abs(difference) < candlestickPeriod) {
                    currentLowestPrice = std::max(currentLowestPrice, t.price);
                    currentHighestPrice = std::max(currentHighestPrice, t.price);
                    currentClosingPrice = t.price;
                } else {
                    if (currentOpeningPrice != 0) {
                        // save candle stick
                        data.candlesticks.push_back(HistoricData::candlestick{currentOpeningPrice,
                                                                            currentClosingPrice,
                                                                            currentLowestPrice,
                                                                            currentHighestPrice,
                                                                            currentOpeningPrice < currentClosingPrice,
                                                                              std::pair<time_t, time_t>{currentStartTime, time}});
                    }

                    // Prepare next candlestick with current trade
                    currentStartTime = mktime(&t.time);
                    currentLowestPrice = t.price;
                    currentHighestPrice = t.price;
                    currentOpeningPrice = t.price;
                    currentClosingPrice = t.price;
                }
            }
        }
    } else {
        throw std::invalid_argument(error);
    }
}

bool StockDataParser::loadPointersToTrades(std::istream &istream) {
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

                isParsingValue = true;
                break;

            case ',':
                if (!(isParsingValue || parsedObjectStack.top() == list)) {
                    error.append("',' encountered unexpectedly");
                    return false;
                }

                currentObject[key] = loadedChars;
                loadedChars.clear();
                isParsingValue = false;
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
                    const char *time_details = currentObject["time"].c_str();
                    struct tm tm{};
                    strptime(time_details, "%Y-%m-%dT%H:%M:%S", &tm);
                    try {
                        this->trades.push_back(trade{tm, std::stof(currentObject["price"])});
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