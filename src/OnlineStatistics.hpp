#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include "exceptions.hpp"

template <typename T>
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