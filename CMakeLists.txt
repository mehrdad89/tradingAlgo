cmake_minimum_required(VERSION 3.7)
project(tradingAlgorithm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add path to gtest headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gtest)

# Define targets, etc. as usual
add_executable(tradingAlgorithm tradingAlgo.cpp)

# Link against gtest library
target_link_libraries(tradingAlgorithm gtest)

# Add test executable
add_executable(trading_algorithm_test trading_algorithm_test.cpp)

# Link against gtest library and tradingAlgorithm
target_link_libraries(trading_algorithm_test gtest tradingAlgorithm)

# Add tests to be run by CTest
add_test(NAME RunAlgorithmTest COMMAND trading_algorithm_test)

