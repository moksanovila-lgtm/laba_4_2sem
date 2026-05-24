#pragma once

#include "OnlineStatistics.hpp"
#include <algorithm>

template <typename T>
OnlineStatistics<T>::OnlineStatistics()
    : currentMin(0)
    , currentMax(0)
    , currentSum(0.0)
    , currentSumSquares(0.0)
    , count(0)
    , isSorted(true)
{
}

template <typename T>
void OnlineStatistics<T>::EnsureSorted() {
    if (!isSorted && allValues.GetCount() > 0) {
        for (size_t i = 0; i < allValues.GetCount() - 1; ++i) {
            for (size_t j = 0; j < allValues.GetCount() - i - 1; ++j) {
                if (allValues.Get(j) > allValues.Get(j + 1)) {
                    T temp = allValues.Get(j);
                    allValues.Set(j, allValues.Get(j + 1));
                    allValues.Set(j + 1, temp);
                }
            }
        }
        isSorted = true;
    }
}

template <typename T>
void OnlineStatistics<T>::UpdateSorted(const T& value) {
    (void)value;
    isSorted = false;
}

template <typename T>
void OnlineStatistics<T>::Update(const T& value) {
    if (count == 0) {
        currentMin = value;
        currentMax = value;
    } else {
        if (value < currentMin) currentMin = value;
        if (value > currentMax) currentMax = value;
    }
    
    currentSum += static_cast<double>(value);
    currentSumSquares += static_cast<double>(value * value);
    
    allValues.Resize(count + 1);
    allValues.Set(count, value);
    
    UpdateSorted(value);
    count++;
}

template <typename T>
void OnlineStatistics<T>::ProcessStream(ReadOnlyStream<T>& stream) {
    stream.Open();
    while (!stream.IsEndOfStream()) {
        T value = stream.Read();
        Update(value);
    }
    stream.Close();
}

template <typename T>
void OnlineStatistics<T>::ProcessStream(ReadOnlyStream<T>& stream, size_t elementsCount) {
    stream.Open();
    size_t processed = 0;
    while (!stream.IsEndOfStream() && processed < elementsCount) {
        T value = stream.Read();
        Update(value);
        processed++;
    }
    stream.Close();
}

template <typename T>
T OnlineStatistics<T>::GetMin() const {
    if (count == 0) throw EmptySequenceException("No data");
    return currentMin;
}

template <typename T>
T OnlineStatistics<T>::GetMax() const {
    if (count == 0) throw EmptySequenceException("No data");
    return currentMax;
}

template <typename T>
double OnlineStatistics<T>::GetAverage() const {
    if (count == 0) throw EmptySequenceException("No data");
    return currentSum / count;
}

template <typename T>
double OnlineStatistics<T>::GetVariance() const {
    if (count == 0) throw EmptySequenceException("No data");
    double mean = GetAverage();
    return (currentSumSquares / count) - (mean * mean);
}

template <typename T>
double OnlineStatistics<T>::GetStdDeviation() const {
    return std::sqrt(GetVariance());
}

template <typename T>
T OnlineStatistics<T>::GetMedian() {
    if (count == 0) throw EmptySequenceException("No data");
    EnsureSorted();
    
    if (count % 2 == 1) {
        return allValues.Get(count / 2);
    } else {
        T left = allValues.Get(count / 2 - 1);
        T right = allValues.Get(count / 2);
        return (left + right) / 2;
    }
}

template <typename T>
size_t OnlineStatistics<T>::GetCount() const {
    return count;
}

template <typename T>
void OnlineStatistics<T>::Reset() {
    currentMin = T();
    currentMax = T();
    currentSum = 0.0;
    currentSumSquares = 0.0;
    count = 0;
    allValues.Clear();
    isSorted = true;
}

template <typename T>
typename OnlineStatistics<T>::StatsData OnlineStatistics<T>::GetStatsData() {
    StatsData data;
    data.min = GetMin();
    data.max = GetMax();
    data.average = GetAverage();
    data.variance = GetVariance();
    data.stdDeviation = GetStdDeviation();
    data.median = GetMedian();
    data.count = count;
    return data;
}

template <typename T>
typename OnlineStatistics<T>::PartialStats OnlineStatistics<T>::GetPartialStats(size_t elementsProcessed) const {
    PartialStats stats;
    stats.min = currentMin;
    stats.max = currentMax;
    stats.average = GetAverage();
    stats.processedCount = count;
    
    if (count > 0 && elementsProcessed <= count && elementsProcessed > 0) {
        T* temp = new T[elementsProcessed];
        for (size_t i = 0; i < elementsProcessed; ++i) {
            temp[i] = allValues.Get(i);
        }
        
        for (size_t i = 0; i < elementsProcessed - 1; ++i) {
            for (size_t j = 0; j < elementsProcessed - i - 1; ++j) {
                if (temp[j] > temp[j + 1]) {
                    T t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }
        
        if (elementsProcessed % 2 == 1) {
            stats.median = temp[elementsProcessed / 2];
        } else {
            stats.median = (temp[elementsProcessed / 2 - 1] + temp[elementsProcessed / 2]) / 2;
        }
        
        delete[] temp;
    }
    
    return stats;
}

template <typename T>
std::string OnlineStatistics<T>::ToString() const {
    if (count == 0) {
        return "No data collected";
    }
    
    std::ostringstream oss;
    oss << "Statistics {\n"
        << "  count: " << count << "\n"
        << "  min: " << currentMin << "\n"
        << "  max: " << currentMax << "\n"
        << "  average: " << GetAverage() << "\n"
        << "  variance: " << GetVariance() << "\n"
        << "  stdDeviation: " << GetStdDeviation() << "\n"
        << "  median: " << const_cast<OnlineStatistics<T>*>(this)->GetMedian() << "\n"
        << "}";
    return oss.str();
}

template <typename T>
OnlineStatistics<T>::operator std::string() const {
    return ToString();
}