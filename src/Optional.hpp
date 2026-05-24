#pragma once

#include <stdexcept>

template <typename T>
class Optional {
private:
    T value;
    bool hasValue;
    
public:
    Optional();
    Optional(const T& val);
    Optional(T&& val);
    Optional(const Optional& other);
    
    Optional& operator=(const Optional& other);
    
    bool HasValue() const;
    
    T GetValue() const;
    T GetValueOrDefault(const T& defaultValue) const;
    
    void SetValue(const T& val);
    void Reset();
    
    explicit operator bool() const;
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;
};

#include "Optional.tpp"