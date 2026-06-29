#include "Cardinal.hpp"
#include <string>

Cardinal::Cardinal() : isInfinite(false), value(0) {}

Cardinal::Cardinal(size_t val) : isInfinite(false), value(val) {}

Cardinal Cardinal::Infinity() {
    Cardinal c;
    c.isInfinite = true;
    return c;
}

bool Cardinal::IsInfinite() const {
    return isInfinite;
}

bool Cardinal::IsFinite() const {
    return !isInfinite;
}

size_t Cardinal::GetValue() const {
    if (isInfinite) {
        throw InvalidArgumentException("Cannot get value of infinite cardinal");
    }
    return value;
}

std::string Cardinal::ToString() const {
    if (isInfinite) return "infinity";
    return std::to_string(value);
}

Cardinal::operator std::string() const {
    return ToString();
}

std::ostream& operator<<(std::ostream& os, const Cardinal& c) {
    os << c.ToString();
    return os;
}

Cardinal Cardinal::operator+(const Cardinal& other) const {
    if (isInfinite || other.isInfinite) return Infinity();
    return Cardinal(value + other.value);
}

Cardinal Cardinal::operator-(const Cardinal& other) const {
    if (isInfinite && other.isInfinite) {
        throw InvalidArgumentException("Infinity - infinity is undefined");
    }
    if (isInfinite) return Infinity();
    if (other.isInfinite) {
        throw InvalidArgumentException("Cannot subtract infinity from finite");
    }
    if (value < other.value) {
        throw InvalidArgumentException("Cannot subtract larger from smaller");
    }
    return Cardinal(value - other.value);
}

Cardinal Cardinal::operator*(const Cardinal& other) const {
    if (isInfinite || other.isInfinite) return Infinity();
    return Cardinal(value * other.value);
}

bool Cardinal::operator==(const Cardinal& other) const {
    if (isInfinite || other.isInfinite) return false;
    return value == other.value;
}

bool Cardinal::operator!=(const Cardinal& other) const {
    return !(*this == other);
}

bool Cardinal::operator<(const Cardinal& other) const {
    if (isInfinite && other.isInfinite) return false;
    if (isInfinite) return false;
    if (other.isInfinite) return true;
    return value < other.value;
}

bool Cardinal::operator>(const Cardinal& other) const {
    return other < *this;
}

bool Cardinal::operator<=(const Cardinal& other) const {
    return !(*this > other);
}

bool Cardinal::operator>=(const Cardinal& other) const {
    return !(*this < other);
}