#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include <concepts>
#include <type_traits>
#include "exceptions.hpp"

template <typename T>
concept Statisticable = requires(T a, T b) {
    a + b;      
    a - b;     
    a * b;      
    a / b;      
    a < b;      
    a > b;
};

template <Statisticable T>  
class OnlineStatistics {
private:
    T minVal;
    T maxVal;
    double sum;
    double sumSq;
    size_t count;
    
public:
    OnlineStatistics();
    
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
};

#include "OnlineStatistics.tpp"
