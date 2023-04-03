#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cmath>

struct MarketData {
    double currentPrice;
    double highPrice;
    double lowPrice;
};

double runAlgorithm(const double& randomCurrentPrice, const double& randomHighPrice, const double& randomLowPrice, const double& riskAmount, const double& accountBalance)
{
    // Set initial values
    double positionSize = 0;
    double entryPrice = 0;
    double exitPrice = 0;
    double stopLoss = 0;
    double takeProfit = 0;
    double maxRiskAmount = 0.02 * accountBalance; // Maximum 2% risk per trade
    double profitLoss = 0;

    // Determine the 20-period simple moving average
    double sma = (randomCurrentPrice + randomHighPrice + randomLowPrice) / 3.0;

    // Determine the support level as the low of the last 10 periods
    double support = randomLowPrice;

    // Check if the current price is above the moving average and the support level
    if (randomCurrentPrice > sma && randomCurrentPrice > support)
    {
        // Calculate position size based on Kelly Criterion
        double winProbability = (takeProfit - randomCurrentPrice) / (takeProfit - stopLoss);
        double lossProbability = 1 - winProbability;
        double winLossRatio = (takeProfit - randomCurrentPrice) / (randomCurrentPrice - stopLoss);
        double expectedWin = winProbability * winLossRatio - lossProbability;
        double edge = expectedWin / winLossRatio;
        positionSize = (maxRiskAmount * edge) / (riskAmount / accountBalance);

        // Calculate entry price, stop loss, and take profit
        entryPrice = randomCurrentPrice;
        stopLoss = randomCurrentPrice - 2 * (randomCurrentPrice - support);
        takeProfit = randomCurrentPrice + 2 * (takeProfit - randomCurrentPrice);

        // Update stop loss and take profit based on position size
        stopLoss = entryPrice - (riskAmount * entryPrice / positionSize) * (entryPrice - stopLoss) / accountBalance;
        takeProfit = entryPrice + (entryPrice - stopLoss);

        // Check if trade is profitable
        if (randomCurrentPrice >= takeProfit)
        {
            exitPrice = takeProfit;
            profitLoss = positionSize * (takeProfit - entryPrice);
        }
        else if (randomCurrentPrice <= stopLoss)
        {
            exitPrice = stopLoss;
            profitLoss = positionSize * (stopLoss - entryPrice);
        }
        else
        {
            return 0; // Trade still open
        }
    }
    else
    {
        return 0; // Trade not taken    - price below moving average or support level
    }

    return profitLoss;
}

// Generate random market data
void generateRandomMarketData(const double& currentPrice, const double& highPrice, const double& lowPrice, const double& priceRange, double& randomCurrentPrice, double& randomHighPrice, double& randomLowPrice)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-priceRange, priceRange);

    randomCurrentPrice = std::max(lowPrice, std::min(highPrice, currentPrice + dis(gen)));
    randomHighPrice = std::max(randomCurrentPrice, std::max(highPrice, currentPrice + dis(gen)));
    randomLowPrice = std::min(randomCurrentPrice, std::min(lowPrice, currentPrice + dis(gen)));
}


// Generate random risk per trade
void generateRandomRiskPerTrade(const double& riskRange, double& randomRiskPerTrade)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, riskRange);

    randomRiskPerTrade = dis(gen);
}

int main()
{
    // Get market data
	MarketData marketData{0.0, 0.0, 0.0};  // Initialize with default values
	marketData.currentPrice = 50;
	marketData.highPrice = 55;
	marketData.lowPrice = 45;
    // starting balance of $10,000
    double accountBalance = 10000.0;

    // Define the number of simulations
    const int numSimulations = 10000;

    // Define the range of values for the market data
    const double priceRange = 10.0;

    // Define the range of values for the risk per trade
    const double riskRange = 0.05; // 5% maximum risk per trade
    // Create vector to store results
    std::vector<double> results;

    // Run the simulations and store the results
    for (int i = 0; i < numSimulations; i++)
    {
        // Generate random market data and risk per trade for each simulation
        double randomCurrentPrice, randomHighPrice, randomLowPrice;
        generateRandomMarketData(marketData.currentPrice + i, marketData.highPrice + i, marketData.lowPrice + i, priceRange, randomCurrentPrice, randomHighPrice, randomLowPrice);
        double randomRiskPerTrade;
        generateRandomRiskPerTrade(riskRange, randomRiskPerTrade);

        // Run the algorithm and store the result
        double result = runAlgorithm(randomCurrentPrice, randomHighPrice, randomLowPrice, randomRiskPerTrade, accountBalance);
        results.push_back(result);
    }

    // Print the results
    for (int i = 0; i < numSimulations; i++)
    {
        std::cout << "Simulation " << i << ": " << results[i] << std::endl;
    }

    std::cout << "All simulations completed.\n";

    return 0;
}

