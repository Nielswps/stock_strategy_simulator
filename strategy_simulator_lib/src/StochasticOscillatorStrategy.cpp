#include <vector>
#include <numeric>
#include "Strategy.h"

#include "StochasticOscillatorStrategy.h"


void StochasticOscillatorStrategy::visitData(const HistoricData *data) {
    for (int i = this->periodsForSlowIndicator; i<data->candlesticks.size(); i++) {
        // Get the lowest and highest price across all X previous candlesticks
        double lowestPriceAcrossPeriods = min_element (data->candlesticks.begin() + i - this->periodsForFastIndicator, data->candlesticks.begin() + i,
                                                        [&](Data::candlestick a, Data::candlestick b) { return a.lowestPrice < b.lowestPrice; })->lowestPrice;
        double highestPriceAcrossPeriods = max_element (data->candlesticks.begin() + i - this->periodsForFastIndicator, data->candlesticks.begin() + i,
                                                       [&](Data::candlestick a, Data::candlestick b) { return a.highestPrice > b.highestPrice; })->highestPrice;

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
            }
        } else {
            if (fastStochasticIndicator < slowStochasticIndicator) {
                // Sell Sell Sell!!!
            }
        }
    }
}

StochasticOscillatorStrategy::StochasticOscillatorStrategy(int periodsForFastIndicator, int periodsForSlowIndicator) {
    this->periodsForFastIndicator = periodsForFastIndicator;
    this->periodsForSlowIndicator = periodsForSlowIndicator;
}

