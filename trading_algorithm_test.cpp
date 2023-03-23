#include <gtest/gtest.h>

TEST(RunAlgorithmTest, ReturnsZeroIfPriceBelowMovingAverageOrSupportLevel) {
  double currentPrice = 10.0;
  double highPrice = 20.0;
  double lowPrice = 5.0;
  double riskAmount = 0.01;
  double accountBalance = 1000.0;
  double result = runAlgorithm(currentPrice, highPrice, lowPrice, riskAmount, accountBalance);
  EXPECT_EQ(result, 0);
}

TEST(RunAlgorithmTest, ReturnsZeroIfTradeNotProfitable) {
  double currentPrice = 50.0;
  double highPrice = 55.0;
  double lowPrice = 45.0;
  double riskAmount = 0.01;
  double accountBalance = 1000.0;
  double result = runAlgorithm(currentPrice, highPrice, lowPrice, riskAmount, accountBalance);
  EXPECT_EQ(result, 0);
}

TEST(RunAlgorithmTest, ReturnsProfitLossIfTradeIsProfitable) {
  double currentPrice = 55.0;
  double highPrice = 60.0;
  double lowPrice = 50.0;
  double riskAmount = 0.01;
  double accountBalance = 1000.0;
  double result = runAlgorithm(currentPrice, highPrice, lowPrice, riskAmount, accountBalance);
  EXPECT_GT(result, 0);
}
