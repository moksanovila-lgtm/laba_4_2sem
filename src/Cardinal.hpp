#pragma once

#include <string>
#include "exceptions.hpp"

class Cardinal {
private:
    bool isInfinite;
    size_t value;
    
public:
    Cardinal();
    Cardinal(size_t val);
    
    static Cardinal Infinity();
    
    bool IsInfinite() const;
    bool IsFinite() const;
    
    size_t GetValue() const;

    Cardinal operator+(const Cardinal& other) const;
    Cardinal operator-(const Cardinal& other) const;
    Cardinal operator*(const Cardinal& other) const;
    bool operator==(const Cardinal& other) const;
    bool operator!=(const Cardinal& other) const;
    bool operator<(const Cardinal& other) const;
    bool operator>(const Cardinal& other) const;
    bool operator<=(const Cardinal& other) const;
    bool operator>=(const Cardinal& other) const;
    
    std::string ToString() const;
    
    operator std::string() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Cardinal& c);
};
