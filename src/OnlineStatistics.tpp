#include "OnlineStatistics.hpp"
#include "exceptions.hpp"

template <Statisticable T>  
OnlineStatistics<T>::OnlineStatistics()
    : minVal(0)
    , maxVal(0)
    , sum(0.0)
    , sumSq(0.0)
    , count(0) {}

template <Statisticable T>  
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
    
    UpdateStrategies(value);
}

template <Statisticable T>  
void OnlineStatistics<T>::Reset() {
    minVal = T();
    maxVal = T();
    sum = 0.0;
    sumSq = 0.0;
    count = 0;
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        strategies.Get(i)->Reset();
    }
}

template <Statisticable T>  
T OnlineStatistics<T>::GetMin() const {
    if (count == 0) throw EmptySequenceException("No data");
    return minVal;
}

template <Statisticable T>  
T OnlineStatistics<T>::GetMax() const {
    if (count == 0) throw EmptySequenceException("No data");
    return maxVal;
}

template <Statisticable T>  
double OnlineStatistics<T>::GetAverage() const {
    if (count == 0) throw EmptySequenceException("No data");
    return sum / count;
}

template <Statisticable T>  
double OnlineStatistics<T>::GetVariance() const {
    if (count == 0) throw EmptySequenceException("No data");
    double avg = GetAverage();
    return (sumSq / count) - (avg * avg);
}

template <Statisticable T>  
double OnlineStatistics<T>::GetStdDeviation() const {
    return std::sqrt(GetVariance());
}

template <Statisticable T>  
double OnlineStatistics<T>::GetRMS() const {
    if (count == 0) throw EmptySequenceException("No data");
    return std::sqrt(sumSq / count);
}

template <Statisticable T>  
double OnlineStatistics<T>::GetRange() const {
    if (count == 0) throw EmptySequenceException("No data");
    return static_cast<double>(maxVal) - static_cast<double>(minVal);
}

template <Statisticable T>  
double OnlineStatistics<T>::GetCoefficientOfVariation() const {
    if (count == 0) throw EmptySequenceException("No data");
    double avg = GetAverage();
    if (avg == 0) return 0;
    return GetStdDeviation() / avg;
}

template <Statisticable T>  
size_t OnlineStatistics<T>::GetCount() const {
    return count;
}

template <Statisticable T>  
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

    auto strategyResults = GetAllStrategyResults();
    if (!strategyResults.empty()) {
        oss << "\n Additional statistics:\n";
        for (const auto& [name, value] : strategyResults) {
            oss << "  " << name << ": " << value << "\n";
        }
    }

    return oss.str();
}

template <Statisticable T>  
OnlineStatistics<T>::operator std::string() const {
    return ToString();
}

template <Statisticable T>
void OnlineStatistics<T>::AddStrategy(IStatisticsStrategy<T>* strategy) {
    strategies.Append(strategy);
}

template <Statisticable T>
double OnlineStatistics<T>::GetStrategyResult(const std::string& name) const {
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        if (strategies.Get(i)->GetName() == name) {
            return strategies.Get(i)->GetResult();
        }
    }
    throw InvalidArgumentException("Strategy not found: " + name);
}

template <Statisticable T>
std::map<std::string, double> OnlineStatistics<T>::GetAllStrategyResults() const {
    std::map<std::string, double> results;
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        IStatisticsStrategy<T>* s = strategies.Get(i);
        results[s->GetName()] = s->GetResult();
    }
    return results;
}

template <Statisticable T>
ArraySequence<std::string> OnlineStatistics<T>::GetStrategyNames() const {
    ArraySequence<std::string> names;
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        names.Append(strategies.Get(i)->GetName());
    }
    return names;
}

template <Statisticable T>
void OnlineStatistics<T>::ResetAllStrategies() {
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        strategies.Get(i)->Reset();
    }
}

template <Statisticable T>
void OnlineStatistics<T>::UpdateStrategies(const T& value) {
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        strategies.Get(i)->Update(value);
    }
}

template <Statisticable T>  
OnlineStatistics<T>::~OnlineStatistics() {
    for (size_t i = 0; i < strategies.GetCount(); ++i) {
        delete strategies.Get(i);
    }
}