#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cmath>

// Define a function that runs the algorithm with one set of market data and returns the profit/loss
double runAlgorithmMonteCarlo(double currentPrice, double highPrice, double lowPrice, double positionSize, double riskAmount)
{
    // Set initial values
    double position = positionSize;
    double entryPrice = 0;
    double exitPrice = 0;
    double stopLoss = 0;
    double takeProfit = 0;
    double maxRiskAmount = riskAmount;

    // Determine the 20-period simple moving average
    double sma = (currentPrice + highPrice + lowPrice) / 3.0;

    // Determine the support level as the low of the last 10 periods
    double support = lowPrice;

    // Check if the current price is above the moving average and the support level
    if (currentPrice > sma && currentPrice > support)
    {
        // Calculate entry price, stop loss, and take profit
        entryPrice = currentPrice;
        stopLoss = currentPrice - 2 * (currentPrice - support);
        takeProfit = currentPrice + 2 * (currentPrice - stopLoss);

        // Update stop loss and take profit based on position size
        stopLoss = entryPrice - (maxRiskAmount / position) * (entryPrice - stopLoss);
        takeProfit = entryPrice + (entryPrice - stopLoss);
    }
    else
    {
        return 0; // Trade not taken - price below moving average or support level
    }

    // Check if trade is profitable
    if (currentPrice >= takeProfit)
    {
        exitPrice = takeProfit;
    }
    else if (currentPrice <= stopLoss)
    {
        exitPrice = stopLoss;
    }
    else
    {
        return 0; // Trade still open
    }

    // Calculate profit/loss
    double profitLoss = (exitPrice - entryPrice) * position;

    return profitLoss;
}

double runAlgorithmKellyCriterion(double currentPrice, double highPrice, double lowPrice, double riskAmount, double accountBalance)
{
    // Set initial values
    double positionSize = 0;
    double entryPrice = 0;
    double stopLoss = 0;
    double takeProfit = 0;
    double maxRiskAmount = 0.02 * accountBalance; // Maximum 2% risk per trade
    double profitLoss = 0;

    // Determine the 20-period simple moving average
    double sma = (currentPrice + highPrice + lowPrice) / 3.0;

    // Determine the support level as the low of the last 10 periods
    double support = lowPrice;

    // Check if the current price is above the moving average and the support level
    if (currentPrice > sma && currentPrice > support)
    {
        // Calculate position size based on Kelly Criterion
        double winProbability = (takeProfit - currentPrice) / (takeProfit - stopLoss);
        double lossProbability = 1 - winProbability;
        double winLossRatio = (takeProfit - currentPrice) / (currentPrice - stopLoss);
        double expectedWin = winProbability * winLossRatio - lossProbability;
        double edge = expectedWin / winLossRatio;
        positionSize = (maxRiskAmount * edge) / (riskAmount / accountBalance);

        // Calculate entry price, stop loss, and take profit
        entryPrice = currentPrice;
        stopLoss = currentPrice - 2 * (currentPrice - support);
        takeProfit = currentPrice + 2 * (takeProfit - currentPrice);

        // Update stop loss and take profit based on position size
        stopLoss = entryPrice - (riskAmount * entryPrice / positionSize) * (entryPrice - stopLoss) / accountBalance;
        takeProfit = entryPrice + (entryPrice - stopLoss);

        // Check if trade is profitable
        if (currentPrice >= takeProfit)
        {
            profitLoss = positionSize * (takeProfit - entryPrice);
        }
        else if (currentPrice <= stopLoss)
        {
            profitLoss = positionSize * (stopLoss - entryPrice);
        }
        else
        {
            profitLoss = 0; // Trade still open
        }
    }
    else
    {
        return 0; // Trade not taken - price below moving average or support level
    }

    return profitLoss;
}

// Generate random market data
void generateRandomMarketData(double currentPrice, double highPrice, double lowPrice, double priceRange, double& randomCurrentPrice, double& randomHighPrice, double& randomLowPrice)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-priceRange, priceRange);

    randomCurrentPrice = currentPrice + dis(gen);
    randomHighPrice = highPrice + dis(gen);
    randomLowPrice = lowPrice + dis(gen);
}

// Generate random risk per trade
void generateRandomRiskPerTrade(double riskRange, double& randomRiskPerTrade)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, riskRange);

    randomRiskPerTrade = dis(gen);
}

int main()
{
    // Get market data
    double currentPrice = 50;
    double highPrice = 55;
    double lowPrice = 45;
    // starting balance of $10,000
    double accountBalance = 10000.0;

    // Define the number of simulations
    const int numSimulations = 10000;

    // Define the range of values for the market data
    const double priceRange = 10.0;

    // Define the range of values for the risk per trade
    const double riskRange = 0.05; // 5% maximum risk per trade

    // Create vector of threads
    std::vector<std::thread> threads;

    // Create 4 threads and run the algorithm with different market data on each thread
    for (int i = 0; i < 4; i++)
    {
        // Generate random market data for each simulation
        double randomCurrentPrice, randomHighPrice, randomLowPrice;
        generateRandomMarketData(currentPrice + i, highPrice + i, lowPrice + i, priceRange, randomCurrentPrice, randomHighPrice, randomLowPrice);

        // Generate random risk per trade for each simulation
        double randomRiskPerTrade;
        generateRandomRiskPerTrade(riskRange, randomRiskPerTrade);

        // Run the algorithm for each simulation
        threads.emplace_back(runAlgorithmMonteCarlo, randomCurrentPrice, randomHighPrice, randomLowPrice, 1, randomRiskPerTrade);
    }

    for (int i = 0; i < 4; i++)
    {
        // Generate random market data for each simulation
        double randomCurrentPrice, randomHighPrice, randomLowPrice;
        generateRandomMarketData(currentPrice + i, highPrice + i, lowPrice + i, priceRange, randomCurrentPrice, randomHighPrice, randomLowPrice);

        // Run the algorithm for each simulation
        threads.emplace_back(runAlgorithmKellyCriterion, randomCurrentPrice, randomHighPrice, randomLowPrice, randomRiskPerTrade, accountBalance);
    }

    // Join all threads
    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "All simulations completed.\n";

    return 0;
}
