#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>

#include "../include/StochasticOscillatorStrategy.h"


StochasticOscillatorStrategy::StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator) {
    this->periodsForFastIndicator = periodsForFastIndicator;
    this->periodsForSlowIndicator = periodsForSlowIndicator;
}

double StochasticOscillatorStrategy::simulateOnData(const HistoricData *data, double availableCapital,
                                                    std::function<void(Trade)> makeTrade) {
    double currentCapital = availableCapital;
    int ownedStocks = 0;

    // Calculate whether the fast indicator is on top at the beginning of the data
    // (not ideal as this is not correctly calculated in accordance with formulas)
    bool fastIndicatorOnTop = [&data, this]() {
        double sum = 0;
        for (int i = 0; i < this->periodsForFastIndicator; i++) {
            sum += data->candlesticks[i].closingPrice;
        }
        return (sum / this->periodsForFastIndicator) < data->candlesticks[this->periodsForFastIndicator].closingPrice;
    }();

    double fastStochasticIndicator;
    double slowStochasticIndicator = 0;
    IterableQueue<double> fastStochasticIndicatorHistory{};

    for (int i = this->periodsForFastIndicator; i < data->candlesticks.size(); i++) {
        // Get the lowest and highest price across all X previous candlesticks
        double lowestPriceAcrossPeriods = min_element(data->candlesticks.begin() + i - this->periodsForFastIndicator,
                                                      data->candlesticks.begin() + i,
                                                      [&](HistoricData::candlestick a, HistoricData::candlestick b) {
                                                          return a.lowestPrice < b.lowestPrice;
                                                      })->lowestPrice;
        double highestPriceAcrossPeriods = max_element(data->candlesticks.begin() + i - this->periodsForFastIndicator,
                                                       data->candlesticks.begin() + i,
                                                       [&](HistoricData::candlestick a, HistoricData::candlestick b) {
                                                           return a.highestPrice > b.highestPrice;
                                                       })->highestPrice;

        // Calculate fast stochastic indicator
        double highLowDiff = highestPriceAcrossPeriods - lowestPriceAcrossPeriods;
        fastStochasticIndicator =
                (data->candlesticks[i].closingPrice - lowestPriceAcrossPeriods) / (highLowDiff == 0 ? 1 : highLowDiff);

        // Update values used for slow stochastic indicator
        if (fastStochasticIndicatorHistory.size() >= periodsForSlowIndicator) {
            fastStochasticIndicatorHistory.pop();
        }
        fastStochasticIndicatorHistory.push(fastStochasticIndicator);

        // Skip calculating slow indicator before enough fast indicators have been calculated
        if (fastStochasticIndicatorHistory.size() == periodsForSlowIndicator) {
            slowStochasticIndicator = std::accumulate(fastStochasticIndicatorHistory.begin(),
                                                      fastStochasticIndicatorHistory.end(), 0.);
        }

        if (!fastIndicatorOnTop) {
            if (fastStochasticIndicator > slowStochasticIndicator) {
                // Buy stock for all capital available
                int amount = static_cast<int>(floor(currentCapital / data->candlesticks[i].closingPrice));
                if (amount > 0) {
                    makeTrade(Trade{true, amount, data->candlesticks[i].closingPrice,
                                    data->candlesticks[i].timeSpan.second});
                    currentCapital -= data->candlesticks[i].closingPrice * amount;
                    ownedStocks = amount;
                }

                fastIndicatorOnTop = true;
            }
        } else {
            if (fastStochasticIndicator < slowStochasticIndicator) {
                // Sell all owned stocks
                if (ownedStocks > 0) {
                    makeTrade(Trade{false, ownedStocks, data->candlesticks[i].closingPrice,
                                    data->candlesticks[i].timeSpan.second});
                    currentCapital += data->candlesticks[i].closingPrice * ownedStocks;
                    ownedStocks = 0;
                }

                fastIndicatorOnTop = false;
            }
        }
    }

    // Sell all remaining stocks if any
    if (ownedStocks > 0) {
        makeTrade(Trade{false, ownedStocks, data->candlesticks.back().closingPrice,
                        data->candlesticks.back().timeSpan.second});
        currentCapital += data->candlesticks.back().closingPrice * ownedStocks;
    }

    return currentCapital;
}