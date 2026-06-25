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