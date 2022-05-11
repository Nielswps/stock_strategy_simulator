#include "../include/StockStrategySimulator.h"
#include "../include/StockDataParser.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include <future>

namespace fs = std::filesystem;

std::vector<StockStrategySimulator::SimulationResult>
StockStrategySimulator::simulateStrategy(Strategy &strategy, const std::string &path, int candleStickPeriodInDays,
                                         double startingCapital) {
    struct stat s{};
    if (stat(path.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            auto directoryIterator = fs::directory_iterator(path,
                                                            fs::directory_options::skip_permission_denied);
            std::vector<std::future<SimulationResult>> results;

            // Start threads for each file to run the simulation on and execute
            for (const auto &entry: directoryIterator) {
                if (!fs::is_regular_file(entry.path())) continue;

                auto future = std::async(std::launch::async, [&]() {
                    return getResultForFile(strategy, entry.path(), candleStickPeriodInDays, startingCapital);
                });
                results.push_back(std::move(future));
            }

            // Get results
            std::vector<SimulationResult> simulationResults{};
            for (std::future<SimulationResult> &f: results) {
                if (f.valid())
                    simulationResults.push_back(std::move(f.get()));
            }

            return simulationResults;
        } else if (s.st_mode & S_IFREG) {
            std::vector<SimulationResult> res;
            res.push_back(getResultForFile(strategy, path, candleStickPeriodInDays, startingCapital));
            return res;
        } else {
            throw std::invalid_argument("Given path is not a file or directory");
        }
    }
    return std::vector<SimulationResult>{};
}

StockStrategySimulator::SimulationResult
StockStrategySimulator::getResultForFile(Strategy &strategy, const std::string &path,
                                         const int &candleStickPeriodInDays,
                                         const double &startingCapital) {
    std::shared_ptr<std::vector<Trade>> trades = std::make_shared<std::vector<Trade>>();
    auto input = new std::ifstream{path, std::ios_base::in};
    auto stockData = StockDataParser(*input, candleStickPeriodInDays).data;

    // Define method for making a trade
    auto makeTrade = [trades](Trade t) {
        trades->emplace_back(t);
    };

    // Simulate strategy on loaded data and return final capital and trade history
    auto finalCapital = strategy.simulateOnData(&stockData, startingCapital,
                                                static_cast<const std::function<void(Trade)> &>(makeTrade));
    return SimulationResult{finalCapital - startingCapital, *trades};
}
