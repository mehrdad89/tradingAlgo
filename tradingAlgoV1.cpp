#include <iostream>

int main()
{
    // Set initial values
    double position = 0;
    double entryPrice = 0;
    double exitPrice = 0;
    double stopLoss = 0;
    double takeProfit = 0;
    double riskPerTrade = 0.01; // 1% risk per trade
    double accountSize = 10000; // $10,000 account size
    double riskAmount = accountSize * riskPerTrade;

    // Get market data
    double currentPrice = 50;
    double highPrice = 55;
    double lowPrice = 45;

    // Calculate entry price, stop loss, and take profit
    entryPrice = currentPrice + 0.5 * (highPrice - lowPrice);
    stopLoss = entryPrice - 0.5 * (highPrice - lowPrice);
    takeProfit = entryPrice + (entryPrice - stopLoss);

    // Calculate position size
    double riskPerContract = entryPrice - stopLoss;
    int numContracts = static_cast<int>(riskAmount / riskPerContract);
    position = numContracts * 1000; // Each contract is worth $1000

    // Check if trade is profitable
    if (currentPrice >= takeProfit)
    {
        exitPrice = takeProfit;
        std::cout << "Take profit hit" << std::endl;
    }
    else if (currentPrice <= stopLoss)
    {
        exitPrice = stopLoss;
        std::cout << "Stop loss hit" << std::endl;
    }
    else
    {
        std::cout << "Trade still open" << std::endl;
    }

    // Calculate profit/loss
    double profitLoss = (exitPrice - entryPrice) * position;

    std::cout << "Entry price: " << entryPrice << std::endl;
    std::cout << "Stop loss: " << stopLoss << std::endl;
    std::cout << "Take profit: " << takeProfit << std::endl;
    std::cout << "Position size: " << position << std::endl;
    std::cout << "Profit/loss: " << profitLoss << std::endl;

    return 0;
}
