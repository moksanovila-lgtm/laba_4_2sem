#pragma once

#include "OnlineStatistics.hpp"
#include "..\third_party\Lab_2\library\include\exceptions.hpp"

// template <typename T>
// OnlineStatistics<T>::OnlineStatistics()
//     : minVal(0)
//     , maxVal(0)
//     , sum(0.0)
//     , sumSq(0.0)
//     , count(0) {}

// template <typename T>
// void OnlineStatistics<T>::Update(const T& value) {
//     if (count == 0) {
//         minVal = value;
//         maxVal = value;
//     } else {
//         if (value < minVal) minVal = value;
//         if (value > maxVal) maxVal = value;
//     }
//     sum += static_cast<double>(value);
//     sumSq += static_cast<double>(value * value);
//     count++;
// }

// template <typename T>
// void OnlineStatistics<T>::Reset() {
//     minVal = T();
//     maxVal = T();
//     sum = 0.0;
//     sumSq = 0.0;
//     count = 0;
// }

// template <typename T>
// T OnlineStatistics<T>::GetMin() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     return minVal;
// }

// template <typename T>
// T OnlineStatistics<T>::GetMax() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     return maxVal;
// }

// template <typename T>
// double OnlineStatistics<T>::GetSum() const {
//     return sum;
// }

// template <typename T>
// double OnlineStatistics<T>::GetAverage() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     return sum / count;
// }

// template <typename T>
// double OnlineStatistics<T>::GetVariance() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     double avg = GetAverage();
//     return (sumSq / count) - (avg * avg);
// }

// template <typename T>
// double OnlineStatistics<T>::GetStdDeviation() const {
//     return std::sqrt(GetVariance());
// }

// template <typename T>
// double OnlineStatistics<T>::GetRMS() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     return std::sqrt(sumSq / count);
// }

// template <typename T>
// double OnlineStatistics<T>::GetRange() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     return static_cast<double>(maxVal) - static_cast<double>(minVal);
// }

// template <typename T>
// double OnlineStatistics<T>::GetCoefficientOfVariation() const {
//     if (count == 0) throw EmptySequenceException("No data");
//     double avg = GetAverage();
//     if (avg == 0) return 0;
//     return GetStdDeviation() / avg;
// }

// template <typename T>
// size_t OnlineStatistics<T>::GetCount() const {
//     return count;
// }

// template <typename T>
// std::string OnlineStatistics<T>::ToString() const {
//     if (count == 0) {
//         return "No data collected";
//     }
    
//     std::ostringstream oss;
//     oss << "Statistics:\n";
//     oss << "  Count: " << count << "\n";
//     oss << "  Min: " << minVal << "\n";
//     oss << "  Max: " << maxVal << "\n";
//     oss << "  Range: " << GetRange() << "\n";
//     oss << "  Sum: " << sum << "\n";
//     oss << "  Average: " << GetAverage() << "\n";
//     oss << "  Variance: " << GetVariance() << "\n";
//     oss << "  StdDeviation: " << GetStdDeviation() << "\n";
//     oss << "  RMS: " << GetRMS() << "\n";
//     oss << "  CoeffOfVariation: " << GetCoefficientOfVariation();
//     return oss.str();
// }

// template <typename T>
// OnlineStatistics<T>::operator std::string() const {
//     return ToString();
// }

template <typename T>
OnlineStatistics<T>::OnlineStatistics()
    : minVal(0)
    , maxVal(0)
    , sum(0.0)
    , sumSq(0.0)
    , count(0) {}

template <typename T>
void OnlineStatistics<T>::Update(const T& value) {
    if (count == 0) {
        minVal = value;
        maxVal = value;
    } else {
        if (value < minVal) minVal = value;
        if (value > maxVal) maxVal = value;
    }
    sum += static_cast<double>(value);
    sumSq += static_cast<double>(value * value);
    count++;
}

template <typename T>
void OnlineStatistics<T>::Reset() {
    minVal = T();
    maxVal = T();
    sum = 0.0;
    sumSq = 0.0;
    count = 0;
}

template <typename T>
T OnlineStatistics<T>::GetMin() const {
    if (count == 0) throw EmptySequenceException("No data");
    return minVal;
}

template <typename T>
T OnlineStatistics<T>::GetMax() const {
    if (count == 0) throw EmptySequenceException("No data");
    return maxVal;
}

template <typename T>
double OnlineStatistics<T>::GetAverage() const {
    if (count == 0) throw EmptySequenceException("No data");
    return sum / count;
}

template <typename T>
double OnlineStatistics<T>::GetVariance() const {
    if (count == 0) throw EmptySequenceException("No data");
    double avg = GetAverage();
    return (sumSq / count) - (avg * avg);
}

template <typename T>
double OnlineStatistics<T>::GetStdDeviation() const {
    return std::sqrt(GetVariance());
}

template <typename T>
double OnlineStatistics<T>::GetRMS() const {
    if (count == 0) throw EmptySequenceException("No data");
    return std::sqrt(sumSq / count);
}

template <typename T>
double OnlineStatistics<T>::GetRange() const {
    if (count == 0) throw EmptySequenceException("No data");
    return static_cast<double>(maxVal) - static_cast<double>(minVal);
}

template <typename T>
double OnlineStatistics<T>::GetCoefficientOfVariation() const {
    if (count == 0) throw EmptySequenceException("No data");
    double avg = GetAverage();
    if (avg == 0) return 0;
    return GetStdDeviation() / avg;
}

template <typename T>
size_t OnlineStatistics<T>::GetCount() const {
    return count;
}

template <typename T>
std::string OnlineStatistics<T>::ToString() const {
    if (count == 0) {
        return "No data collected";
    }
    
    std::ostringstream oss;
    oss << "Statistics:\n";
    oss << "  Count: " << count << "\n";
    oss << "  Min: " << minVal << "\n";
    oss << "  Max: " << maxVal << "\n";
    oss << "  Range: " << GetRange() << "\n";
    oss << "  Average: " << GetAverage() << "\n";
    oss << "  Variance: " << GetVariance() << "\n";
    oss << "  StdDeviation: " << GetStdDeviation() << "\n";
    oss << "  RMS: " << GetRMS() << "\n";
    oss << "  CoeffOfVariation: " << GetCoefficientOfVariation();
    return oss.str();
}

template <typename T>
OnlineStatistics<T>::operator std::string() const {
    return ToString();
}