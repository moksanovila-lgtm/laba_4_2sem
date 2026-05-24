#pragma once

#include "..\third_party\Lab_2\library\include\Sequence.hpp"
#include "..\third_party\Lab_2\library\include\ArraySequence.hpp"
#include "..\third_party\Lab_2\library\include\DynamicArray.hpp"
#include "..\third_party\Lab_2\library\include\exceptions.hpp"
#include "ReadOnlyStream.hpp"
#include <cmath>
#include <limits>

template <typename T>
class OnlineStatistics {
private:
    T currentMin;
    T currentMax;
    double currentSum;
    double currentSumSquares;
    size_t count;
    
    DynamicArray<T> allValues;
    bool isSorted;
    
    void EnsureSorted();
    void UpdateSorted(const T& value);
    
public:
    OnlineStatistics();
    
    void Update(const T& value);
    void ProcessStream(ReadOnlyStream<T>& stream);
    void ProcessStream(ReadOnlyStream<T>& stream, size_t elementsCount);
    
    T GetMin() const;
    T GetMax() const;
    double GetAverage() const;
    double GetVariance() const;
    double GetStdDeviation() const;
    T GetMedian();
    size_t GetCount() const;
    
    void Reset();
    void Print() const;
    
    struct PartialStats {
        T min;
        T max;
        double average;
        T median;
        size_t processedCount;
    };
    
    PartialStats GetPartialStats(size_t elementsProcessed) const;
};

#include "OnlineStatistics.tpp"