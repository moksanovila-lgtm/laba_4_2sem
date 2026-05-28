#pragma once

#include <string>
#include "..\third_party\Lab_2\library\include\exceptions.hpp"

class Cardinal {
private:
    bool isInfinite;
    size_t value;
    
public:

    Cardinal() : isInfinite(false), value(0) {}
    explicit Cardinal(size_t val) : isInfinite(false), value(val) {}
    
    static Cardinal Infinity() {
        Cardinal c;
        c.isInfinite = true;
        return c;
    }
    
    bool IsInfinite() const { return isInfinite; }
    bool IsFinite() const { return !isInfinite; }
    
    size_t GetValue() const {
        if (isInfinite) {
            throw InvalidArgumentException("Cannot get value of infinite cardinal");
        }
        return value;
    }
    
    bool operator==(const Cardinal& other) const {
        if (isInfinite && other.isInfinite) return true;
        if (isInfinite || other.isInfinite) return false;
        return value == other.value;
    }
    
    bool operator!=(const Cardinal& other) const {
        return !(*this == other);
    }
    
    std::string ToString() const {
        if (isInfinite) return "infinity";
        return std::to_string(value);
    }
    
    operator std::string() const {
        return ToString();
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Cardinal& c) {
        os << c.ToString();
        return os;
    }
};