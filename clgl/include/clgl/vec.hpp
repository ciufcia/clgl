#pragma once

#include "typedefs.hpp"

namespace clgl
{
template<typename Number>
class Vec2
{
public:

    Vec2() = default;
    Vec2(Number x, Number y) : x(x), y(y) {}

    template<typename OtherNumber>
    operator Vec2<OtherNumber>() const;

    [[nodiscard]] Vec2<Number> operator+(const Vec2<Number> &other) const;
    void                       operator+=(const Vec2<Number> &other);
    void                       operator+=(Number number);
    [[nodiscard]] Vec2<Number> operator-(const Vec2<Number> &other) const;
    void                       operator-=(const Vec2<Number> &other);
    void                       operator-=(Number number);
    [[nodiscard]] Vec2<Number> operator*(Number number) const;
    void                       operator*=(Number number);
    [[nodiscard]] Vec2<Number> operator/(Number number) const;
    void                       operator/=(Number number);
    [[nodiscard]] bool         operator==(const Vec2<Number> &other) const;
    [[nodiscard]] bool         operator!=(const Vec2<Number> &other) const;

public:

    Number x;
    Number y;
};

typedef Vec2<I32> Vec2I;
typedef Vec2<U32> Vec2U;
typedef Vec2<F32> Vec2F;

}

template<typename Number>
template<typename OtherNumber>
clgl::Vec2<Number>::operator Vec2<OtherNumber>() const {
    return Vec2<OtherNumber>(static_cast<OtherNumber>(x), static_cast<OtherNumber>(y));
}

template<typename Number>
clgl::Vec2<Number> clgl::Vec2<Number>::operator+(const Vec2<Number> &other) const {
    return { x + other.x, y + other.y };
}

template<typename Number>
void clgl::Vec2<Number>::operator+=(const Vec2<Number> &other) {
    x += other.x; y += other.y;
}

template<typename Number>
void clgl::Vec2<Number>::operator+=(Number number) {
    x += number; y += number;
}

template<typename Number>
clgl::Vec2<Number> clgl::Vec2<Number>::operator-(const Vec2<Number> &other) const {
    return { x - other.x, y - other.y };
}

template<typename Number>
void clgl::Vec2<Number>::operator-=(const Vec2<Number> &other) {
    x -= other.x; y -= other.y;
}

template<typename Number>
void clgl::Vec2<Number>::operator-=(Number number) {
    x -= number; y -= number;
}

template<typename Number>
clgl::Vec2<Number> clgl::Vec2<Number>::operator*(Number number) const {
    return { x * number, y * number };
}

template<typename Number>
void clgl::Vec2<Number>::operator*=(Number number) {
    x *= number; y *= number;
}

template<typename Number>
clgl::Vec2<Number> clgl::Vec2<Number>::operator/(Number number) const {
    return { x / number, y / number };
}

template<typename Number>
void clgl::Vec2<Number>::operator/=(Number number) {
    x /= number; y /= number;
}

template<typename Number>
bool clgl::Vec2<Number>::operator==(const Vec2<Number> &other) const {
    return x == other.x && y == other.y;
}

template<typename Number>
bool clgl::Vec2<Number>::operator!=(const Vec2<Number> &other) const {
    return x != other.x || y != other.y;
}