#pragma once

#include "OnlineStatistics.hpp"

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
        for (size_t i = 1; i < allValues.GetCount(); ++i) {
            T key = allValues.Get(i);
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && allValues.Get(j) > key) {
                allValues.Set(j + 1, allValues.Get(j));
                j--;
            }
            allValues.Set(j + 1, key);
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
    allValues.Append(value);
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
void OnlineStatistics<T>::Print() const {
    std::cout << "Total elements: " << count << std::endl;
    if (count == 0) {
        std::cout << "No data collected.\n";
        return;
    }
    std::cout << "Minimum: " << currentMin << std::endl;
    std::cout << "Maximum: " << currentMax << std::endl;
    std::cout << "Average: " << GetAverage() << std::endl;
    std::cout << "Variance: " << GetVariance() << std::endl;
    std::cout << "Std Deviation: " << GetStdDeviation() << std::endl;
    std::cout << "Median: " << const_cast<OnlineStatistics<T>*>(this)->GetMedian() << std::endl;
}


template <typename T>
typename OnlineStatistics<T>::PartialStats OnlineStatistics<T>::GetPartialStats(size_t elementsProcessed) const {
    PartialStats stats;
    stats.min = currentMin;
    stats.max = currentMax;
    stats.average = GetAverage();
    stats.processedCount = count;
    
    if (count > 0 && elementsProcessed <= count) {
        DynamicArray<T> temp;
        for (size_t i = 0; i < elementsProcessed; ++i) {
            temp.Append(allValues.Get(i));
        }
        for (size_t i = 1; i < temp.GetCount(); ++i) {
            T key = temp.Get(i);
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && temp.Get(j) > key) {
                temp.Set(j + 1, temp.Get(j));
                j--;
            }
            temp.Set(j + 1, key);
        }
        if (elementsProcessed % 2 == 1) {
            stats.median = temp.Get(elementsProcessed / 2);
        } else {
            T left = temp.Get(elementsProcessed / 2 - 1);
            T right = temp.Get(elementsProcessed / 2);
            stats.median = (left + right) / 2;
        }
    }
    return stats;
}