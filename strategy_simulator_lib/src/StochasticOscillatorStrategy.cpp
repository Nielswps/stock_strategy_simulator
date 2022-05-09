#include <vector>
#include <numeric>
#include <stdexcept>
#include <cmath>

#include "../include/StochasticOscillatorStrategy.h"


StochasticOscillatorStrategy::StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator) {
    if (periodsForFastIndicator > periodsForSlowIndicator) {
        throw std::invalid_argument{"Periods for fast stochastic indicator must be smaller than periods for slow indicator"};
    }

    this->periodsForFastIndicator = periodsForFastIndicator;
    this->periodsForSlowIndicator = periodsForSlowIndicator;
}

void StochasticOscillatorStrategy::simulateOnData(const HistoricData *data, double availableMoney, void (*makeTrade)(Trade)) {
    double money = availableMoney;
    int ownedStocks = 0;

    for (int i = this->periodsForSlowIndicator; i<data->candlesticks.size(); i++) {
        // Get the lowest and highest price across all X previous candlesticks
        double lowestPriceAcrossPeriods = min_element (data->candlesticks.begin() + i - this->periodsForFastIndicator, data->candlesticks.begin() + i,
                                                        [&](HistoricData::candlestick a, HistoricData::candlestick b) { return a.lowestPrice < b.lowestPrice; })->lowestPrice;
        double highestPriceAcrossPeriods = max_element (data->candlesticks.begin() + i - this->periodsForFastIndicator, data->candlesticks.begin() + i,
                                                       [&](HistoricData::candlestick a, HistoricData::candlestick b) { return a.highestPrice > b.highestPrice; })->highestPrice;

        // Calculate fast stochastic indicator
        fastStochasticIndicator = (data->candlesticks[i].closingPrice - lowestPriceAcrossPeriods) / (highestPriceAcrossPeriods - lowestPriceAcrossPeriods);

        // Update values used for slow stochastic indicator
        if (fastStochasticIndicatorHistory.size() > periodsForSlowIndicator) {
            fastStochasticIndicatorHistory.pop();
        }
        fastStochasticIndicatorHistory.push(fastStochasticIndicator);
        slowStochasticIndicator = std::accumulate(fastStochasticIndicatorHistory.begin(),
                                                  fastStochasticIndicatorHistory.end(), 0.);

        if (!fastIndicatorOnTop) {
            if (fastStochasticIndicator > slowStochasticIndicator) {
                // Buy stock for all money available
                int amount = static_cast<int>(floor(money / data->candlesticks[i].closingPrice));
                if (amount > 0) {
                    makeTrade(Trade{true, amount, data->candlesticks[i].closingPrice, data->candlesticks[i+1].timeSpan.first});
                    money -= data->candlesticks[i].closingPrice * amount;
                }

                fastIndicatorOnTop = true;
            }
        } else {
            if (fastStochasticIndicator < slowStochasticIndicator) {
                // Sell all owned stocks
                if (ownedStocks > 0) {
                    makeTrade(Trade{false, ownedStocks, data->candlesticks[i].closingPrice, data->candlesticks[i+1].timeSpan.first});
                    money += data->candlesticks[i].closingPrice * ownedStocks;
                    ownedStocks = 0;
                }
                fastIndicatorOnTop = false;
            }
        }
    }
}