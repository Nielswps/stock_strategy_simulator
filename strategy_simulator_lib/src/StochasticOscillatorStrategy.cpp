#include <vector>
#include <numeric>
#include <stdexcept>

#include "../include/StochasticOscillatorStrategy.h"


void StochasticOscillatorStrategy::simulateOnData(const HistoricData *data) {
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
                // Buy Buy Buy!!!
                fastIndicatorOnTop = true;
            }
        } else {
            if (fastStochasticIndicator < slowStochasticIndicator) {
                // Sell Sell Sell!!!s
                fastIndicatorOnTop = false;
            }
        }
    }
}

StochasticOscillatorStrategy::StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator) {
    if (periodsForFastIndicator > periodsForSlowIndicator) {
        throw std::invalid_argument{"Periods for fast stochastic indicator must be smaller than periods for slow indicator"};
    }

    this->periodsForFastIndicator = periodsForFastIndicator;
    this->periodsForSlowIndicator = periodsForSlowIndicator;
}

