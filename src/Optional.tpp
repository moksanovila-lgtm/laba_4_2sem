#include "Optional.hpp"

template <typename T>
Optional<T>::Optional() : hasValue(false) {}

template <typename T>
Optional<T>::Optional(const T& val) : value(val), hasValue(true) {}

template <typename T>
Optional<T>::Optional(T&& val) : value(std::move(val)), hasValue(true) {}

template <typename T>
Optional<T>::Optional(const Optional& other) : hasValue(other.hasValue) {
    if (hasValue) {
        value = other.value;
    }
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional& other) {
    if (this != &other) {
        hasValue = other.hasValue;
        if (hasValue) {
            value = other.value;
        }
    }
    return *this;
}

template <typename T>
bool Optional<T>::HasValue() const {
    return hasValue;
}

template <typename T>
T Optional<T>::GetValue() const {
    if (!hasValue) {
        throw IteratorStateException("Optional has no value");
    }
    return value;
}

template <typename T>
T Optional<T>::GetValueOrDefault(const T& defaultValue) const {
    return hasValue ? value : defaultValue;
}

template <typename T>
void Optional<T>::SetValue(const T& val) {
    value = val;
    hasValue = true;
}

template <typename T>
void Optional<T>::Reset() {
    hasValue = false;
}

template <typename T>
Optional<T>::operator bool() const {
    return hasValue;
}

template <typename T>
T& Optional<T>::operator*() {
    if (!hasValue) {
        throw IteratorStateException("Optional has no value");
    }
    return value;
}

template <typename T>
const T& Optional<T>::operator*() const {
    if (!hasValue) {
        throw IteratorStateException("Optional has no value");
    }
    return value;
}

template <typename T>
T* Optional<T>::operator->() {
    if (!hasValue) {
        throw IteratorStateException("Optional has no value");
    }
    return &value;
}

template <typename T>
const T* Optional<T>::operator->() const {
    if (!hasValue) {
        throw IteratorStateException("Optional has no value");
    }
    return &value;
}