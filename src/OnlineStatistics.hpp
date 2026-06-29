#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include <concepts>
#include "ArraySequence.hpp"  
#include "IStatisticsStrategy.hpp"
#include "exceptions.hpp"

template <typename T>
concept Statisticable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;   
    { a - b } -> std::convertible_to<T>;   
    { a * b } -> std::convertible_to<T>;  
    { a / b } -> std::convertible_to<T>;  
    { a < b } -> std::convertible_to<bool>; 
    { a > b } -> std::convertible_to<bool>; 
};

template <Statisticable T>  
class OnlineStatistics {
private:
    T minVal;
    T maxVal;
    double sum;
    double sumSq;
    size_t count;

    ArraySequence<IStatisticsStrategy<T>*> strategies;
    void UpdateStrategies(const T& value);

public:
    OnlineStatistics();
    ~OnlineStatistics(); 
    
    void Update(const T& value);
    void Reset();
    
    T GetMin() const;
    T GetMax() const;
    double GetAverage() const;
    double GetVariance() const;
    double GetStdDeviation() const;
    double GetRMS() const;
    double GetRange() const;
    double GetCoefficientOfVariation() const;
    size_t GetCount() const;
    
    std::string ToString() const;
    operator std::string() const;

    void AddStrategy(IStatisticsStrategy<T>* strategy);
    double GetStrategyResult(const std::string& name) const;
    ArraySequence<std::string> GetStrategyNames() const;
    ArraySequence<double> GetStrategyValues() const;
    void ResetAllStrategies();
};

#include "OnlineStatistics.tpp"

