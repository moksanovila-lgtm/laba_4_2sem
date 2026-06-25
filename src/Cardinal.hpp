#pragma once

#include <string>
#include "..\third_party\Lab_2\library\include\exceptions.hpp"

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
    
    std::string ToString() const;
    
    operator std::string() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Cardinal& c);
};

// class cardinal_t {
//     size_t value_;
//     public:
//     cardinal_t(size_t value) : value_(value) {}
// };

// void test() {
//     cardinal_t c = 5;
// }