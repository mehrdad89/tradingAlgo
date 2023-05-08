cmake_minimum_required(VERSION 3.7)
project(tradingAlgorithm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# ... Define targets, etc. as usual

add_executable(tradingAlgorithm tradingAlgo.cpp)

add_executable(trading_algorithm_test trading_algorithm_test.cpp)
target_link_libraries(trading_algorithm_test gtest)
