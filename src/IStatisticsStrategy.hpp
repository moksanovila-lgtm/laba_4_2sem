#pragma once

#include <string>

template <typename T>
class IStatisticsStrategy {
public:
    virtual ~IStatisticsStrategy() = default;
    
    virtual void Update(const T& value) = 0;      
    virtual double GetResult() const = 0;          
    virtual void Reset() = 0;                    
    virtual std::string GetName() const = 0;      
};