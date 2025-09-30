#pragma once
#include <string>
#include <sstream>

template <typename T, size_t N>
struct vec;

template <typename T>
struct vec<T, 2> {
    T x, y;

    vec() : x(T{}), y(T{}) {}
    vec(T x, T y) : x(x), y(y) {}
    T& operator[](size_t i) { return *(&x + i); }
    const T& operator[](size_t i) const { return *(&x + i); }
};

template <typename T>
struct vec<T, 3> {
    T x, y, z;

    vec() : x(T{}), y(T{}), z(T{}) {}
    vec(T x, T y, T z) : x(x), y(y), z(z) {}

    T& operator[](size_t i) { return *(&x + i); }
    const T& operator[](size_t i) const { return *(&x + i); }
};

template <typename T>
struct vec<T, 4> {
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };

    vec() : x(T{}), y(T{}), z(T{}), w(T{}) {}
    vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    T& operator[](size_t i) { return *(&x + i); }
    const T& operator[](size_t i) const { return *(&x + i); }
};

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;
using ivec2 = vec<int, 2>;
using ivec3 = vec<int, 3>;
using ivec4 = vec<int, 4>;
using color4 = ivec4;

// Addition
template <typename Vec>
auto operator+(const Vec& a, const Vec& b) 
{
    using T = decltype(a.x);

    const T* pa = reinterpret_cast<const T*>(&a);
    const T* pb = reinterpret_cast<const T*>(&b);
   
    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); i++) {
        result[i] = static_cast<T>(pa[i]) + static_cast<T>(pb[i]);
    }
    return result;
}

// Subtraction
template <typename Vec>
auto operator-(const Vec& a, const Vec& b)
{
    using T = decltype(a.x);

    const T* pa = reinterpret_cast<const T*>(&a);
    const T* pb = reinterpret_cast<const T*>(&b);

    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); i++) {
        result[i] = static_cast<T>(pa[i]) - static_cast<T>(pb[i]);
    }
    return result;
}

// Negation
template <typename Vec>
auto operator-(const Vec& a)
{
    using T = decltype(a.x);
    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i)
        result[i] = -a[i];
    return result;
}

// Equality
template <typename Vec>
bool operator==(const Vec& a, const Vec& b)
{
    using T = decltype(a.x);

    const T* pa = reinterpret_cast<const T*>(&a);
    const T* pb = reinterpret_cast<const T*>(&b);

    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); i++) {
        if (pa[i] != pb[i]) return false;
    }
    return true;
}

// Inequality
template <typename Vec>
bool operator!=(const Vec& a, const Vec& b)
{
    return !(a == b);
}

// Scalar multiplication
template <typename Vec>
auto operator*(const Vec& a, const decltype(a.x)& s)
{
    using T = decltype(a.x);
    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i) {
        result[i] = a[i] * s;
    }
    return result;
}
// Commutative
template <typename Vec>
auto operator*(const decltype(std::declval<Vec>().x)& s, const Vec& v)
{
    return v * s; 
}

// Scalar division
template <typename Vec>
auto operator/(const Vec& a, const decltype(a.x)& s)
{
    using T = decltype(a.x);
    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i)
        result[i] = a[i] / s;
    return result;
}

// Add assign
template <typename Vec>
Vec& operator+=(Vec& a, const Vec& b)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] += b[i];
    return a;
}

// Sub assign
template <typename Vec>
Vec& operator-=(Vec& a, const Vec& b)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] -= b[i];
    return a;
}

// Multiply assign
template <typename Vec>
Vec& operator*=(Vec& a, const decltype(a.x)& s)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] *= s;
    return a;
}

// Divide assign
template <typename Vec>
Vec& operator/=(Vec& a, const decltype(a.x)& s)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] /= s;
    return a;
}

namespace VecUtils 
{
    // Length
    template <typename Vec>
    auto length(const Vec& a) 
    {
        using T = decltype(a.x);
        T sum = 0;
        for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i) {
            sum += a[i] * a[i];
        }
        return std::sqrt(sum);
    }

    // Length squared
    template <typename Vec>
    auto length2(const Vec& a)
    {
        using T = decltype(a.x);
        T sum = 0;
        for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i)
            sum += a[i] * a[i];
        return sum;
    }

    // Distance
    template <typename Vec>
    float distance(const Vec& a, const Vec& b)
    {
        return length(a - b);
    }

    // Distance squared; useful to avoid expensive sqrt
    template <typename Vec>
    float distance2(const Vec& a, const Vec& b)
    {
        return length2(a - b);
    }

    // Normalize
    template <typename Vec>
    Vec normalize(const Vec& a)
    {
        auto len = VecUtils::length(a);
        if (len == 0) return a; 
        return a / len;
    }

    // Dot product
    template <typename Vec>
    auto dot(const Vec& a, const Vec& b) {
        using T = decltype(a.x);

        const T* pa = reinterpret_cast<const T*>(&a);
        const T* pb = reinterpret_cast<const T*>(&b);

        T result = 0;
        for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i) {
            result += static_cast<T>(pa[i]) * static_cast<T>(pb[i]);
        }
        return result;
    }

    // Cross product
    template <typename T>
    vec<T, 3> cross(const vec<T, 3>& a, const vec<T, 3>& b)
    {
        return vec<T, 3>{
            a.y* b.z - a.z * b.y,
                a.z* b.x - a.x * b.z,
                a.x* b.y - a.y * b.x
        };
    }
}


