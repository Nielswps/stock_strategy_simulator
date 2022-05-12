#include <iostream>
#include "../strategy_simulator_lib/include/StockStrategySimulator.h"
#include "../strategy_simulator_lib/include/StochasticOscillatorStrategy.h"
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;

void simulateStochasticOscillatorForPAALB() {
    auto s = StochasticOscillatorStrategy{4, 3};
    auto res = StockStrategySimulator::simulateStrategy(s,
                                                        "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                                        2, 10000);

    std::cout << "Trades made during simulation:" << std::endl;
    for (const auto &t: res[0].trades) {
        auto time = gmtime(&t.date);
        std::cout << (t.buy ? "Bought " : "Sold ") << t.amount << " shares at " << t.price
                  << " each at " << time->tm_year + 1900 << "-" << time->tm_mon << "-" << time->tm_mday << " "
                  << time->tm_hour << ":" << time->tm_min << ":" << time->tm_sec << std::endl;
    }

    std::cout << (res[0].profit < 0 ? "Loss from strategy:" : "Profit from strategy: ") << std::abs(res[0].profit)
              << std::endl;
}

void plotStochasticOscillatorForPAALB(int X, int Y) {
    class PlottingStochasticOscillatorStrategy : public Strategy {
    public:
        PlottingStochasticOscillatorStrategy(int X, int Y) {
            this->periodsForFastIndicator = X;
            this->periodsForSlowIndicator = Y;
        }

        double simulateOnData(const HistoricData *data, double availableCapital,
                              std::function<void(Trade)> makeTrade) override {
            double currentCapital = availableCapital;
            int ownedStocks = 0;
            std::vector<double> dates(data->candlesticks.size());
            std::transform(data->candlesticks.begin(), data->candlesticks.end(), dates.begin(),
                      [](HistoricData::candlestick c) { return c.timeSpan.first; });

            std::vector<double> highs(data->candlesticks.size());
            std::transform(data->candlesticks.begin(), data->candlesticks.end(), highs.begin(),
                      [](HistoricData::candlestick c) { return c.highestPrice; });

            std::vector<double> opening(data->candlesticks.size());
            std::transform(data->candlesticks.begin(), data->candlesticks.end(), opening.begin(),
                      [](HistoricData::candlestick c) { return c.openingPrice; });

            std::vector<double> closing(data->candlesticks.size());
            std::transform(data->candlesticks.begin(), data->candlesticks.end(), closing.begin(),
                      [](HistoricData::candlestick c) { return c.closingPrice; });

            std::vector<double> lows(data->candlesticks.size());
            std::transform(data->candlesticks.begin(), data->candlesticks.end(), lows.begin(),
                      [](HistoricData::candlestick c) { return c.lowestPrice; });

            std::vector<double> fastIndicatorValues{};
            std::vector<double> slowIndicatorValues{};

            // Calculate whether the fast indicator is on top at the beginning of the data
            // (not ideal as this is not correctly calculated in accordance with formulas)
            bool fastIndicatorOnTop = [&data, this]() {
                double sum = 0;
                for (int i = 0; i < this->periodsForFastIndicator; i++) {
                    sum += data->candlesticks[i].closingPrice;
                }
                return (sum / this->periodsForFastIndicator) <
                       data->candlesticks[this->periodsForFastIndicator].closingPrice;
            }();

            double fastStochasticIndicator;
            double slowStochasticIndicator = 0;
            IterableQueue<double> fastStochasticIndicatorHistory{};

            for (int i = this->periodsForFastIndicator; i < data->candlesticks.size(); i++) {
                // Get the lowest and highest price across all X previous candlesticks
                double lowestPriceAcrossPeriods = min_element(
                        data->candlesticks.begin() + i - this->periodsForFastIndicator,
                        data->candlesticks.begin() + i,
                        [&](HistoricData::candlestick a, HistoricData::candlestick b) {
                            return a.lowestPrice < b.lowestPrice;
                        })->lowestPrice;
                double highestPriceAcrossPeriods = max_element(
                        data->candlesticks.begin() + i - this->periodsForFastIndicator,
                        data->candlesticks.begin() + i,
                        [&](HistoricData::candlestick a, HistoricData::candlestick b) {
                            return a.highestPrice > b.highestPrice;
                        })->highestPrice;

                // Calculate fast stochastic indicator
                double highLowDiff = highestPriceAcrossPeriods - lowestPriceAcrossPeriods;
                fastStochasticIndicator =
                        (data->candlesticks[i].closingPrice - lowestPriceAcrossPeriods) /
                        (highLowDiff == 0 ? 1 : highLowDiff);
                fastIndicatorValues.push_back(fastStochasticIndicator);

                // Update values used for slow stochastic indicator
                if (fastStochasticIndicatorHistory.size() >= periodsForSlowIndicator) {
                    fastStochasticIndicatorHistory.pop();
                }
                fastStochasticIndicatorHistory.push(fastStochasticIndicator);

                // Skip calculating slow indicator before enough fast indicators have been calculated
                if (fastStochasticIndicatorHistory.size() == periodsForSlowIndicator) {
                    slowStochasticIndicator = std::accumulate(fastStochasticIndicatorHistory.begin(),
                                                              fastStochasticIndicatorHistory.end(), 0.);
                    slowIndicatorValues.push_back(slowStochasticIndicator);
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

            // Pad fast and slow indicator value vectors to ensure correct length
            while (fastIndicatorValues.size() < dates.size()) fastIndicatorValues.insert(fastIndicatorValues.begin(), 0);
            while (slowIndicatorValues.size() < dates.size()) slowIndicatorValues.insert(slowIndicatorValues.begin(), 0);

            plt::plot(dates, highs, {{"label", "High"}});
            plt::plot(dates, opening, {{"label", "Opening"}});
            plt::plot(dates, closing, {{"label", "Closing"}});
            plt::plot(dates, lows, {{"label", "Low"}});
            plt::legend();
            plt::ylabel("Price");
            plt::xlabel("Time");
            plt::show();
            plt::close();

            plt::plot(dates, fastIndicatorValues, {{"label", "Fast Indicator"}});
            plt::plot(dates, slowIndicatorValues, {{"label", "Slow Indicator"}});
            plt::legend();
            plt::ylabel("Indicator value");
            plt::xlabel("Time");
            plt::show();

            return currentCapital;
        }

    private:
        int periodsForFastIndicator;
        int periodsForSlowIndicator;
        std::string destination;
    };

    auto s = PlottingStochasticOscillatorStrategy(4, 3);
    auto res = StockStrategySimulator::simulateStrategy(s,
                                                        "/home/niels/Documents/gitHub/stock_exchange/Google_tests/test_data/PAALB.json",
                                                        2, 10000);
}

int main() {
    simulateStochasticOscillatorForPAALB();
    plotStochasticOscillatorForPAALB(4, 3);
    return 0;
}
