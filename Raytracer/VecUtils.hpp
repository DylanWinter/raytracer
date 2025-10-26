#pragma once
#include <string>
#include <sstream>

template <typename T, size_t N>
struct vec;

template <typename T>
struct vec<T, 2> {
    T x, y;

    constexpr vec() : x(T{}), y(T{}) {}
    constexpr vec(T x, T y) : x(x), y(y) {}

    T& operator[](size_t i) { return *(&x + i); }
    const T& operator[](size_t i) const { return *(&x + i); }
};

template <typename T>
struct vec<T, 3> {
    T x, y, z;

    constexpr vec() : x(T{}), y(T{}), z(T{}) {}
    constexpr vec(T x, T y, T z) : x(x), y(y), z(z) {}

    T& operator[](size_t i) { return *(&x + i); }
    const T& operator[](size_t i) const { return *(&x + i); }
};

template <typename T>
struct vec<T, 4> {
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };

    constexpr vec() : x(T{}), y(T{}), z(T{}), w(T{}) {}
    constexpr vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    T& operator[](size_t i) { return *(&x + i); }
    const T& operator[](size_t i) const { return *(&x + i); }
};

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;
using ivec2 = vec<int, 2>;
using ivec3 = vec<int, 3>;
using ivec4 = vec<int, 4>;
using color4 = vec4;

constexpr vec2 VEC2_ZERO = vec2{ 0, 0 };
constexpr vec3 VEC3_ZERO = vec3{ 0, 0, 0 };
constexpr vec4 VEC4_ZERO = vec4{ 0, 0, 0, 0 };

// Used to restrict to vector types
template <typename>
struct is_vec : std::false_type {};
template <typename T, size_t N>
struct is_vec<vec<T, N>> : std::true_type {};
template <typename T>
inline constexpr bool is_vec_v = is_vec<T>::value;
template <typename T>
concept VecType = is_vec_v<T>;

// Addition
template <VecType Vec>
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
template <VecType Vec>
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
template <VecType Vec>
auto operator-(const Vec& a)
{
    using T = decltype(a.x);
    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i)
        result[i] = -a[i];
    return result;
}

// Equality
template <VecType Vec>
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
template <VecType Vec>
bool operator!=(const Vec& a, const Vec& b)
{
    return !(a == b);
}

// Scalar multiplication
template <VecType Vec>
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
template <VecType Vec>
auto operator*(const decltype(std::declval<Vec>().x)& s, const Vec& v)
{
    return v * s; 
}

// Scalar division
template <VecType Vec>
auto operator/(const Vec& a, const decltype(a.x)& s)
{
    using T = decltype(a.x);
    Vec result{};
    for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i)
        result[i] = a[i] / s;
    return result;
}

// Add assign
template <VecType Vec>
Vec& operator+=(Vec& a, const Vec& b)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] += b[i];
    return a;
}

// Sub assign
template <VecType Vec>
Vec& operator-=(Vec& a, const Vec& b)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] -= b[i];
    return a;
}

// Multiply assign
template <VecType Vec>
Vec& operator*=(Vec& a, const decltype(a.x)& s)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] *= s;
    return a;
}

// Divide assign
template <VecType Vec>
Vec& operator/=(Vec& a, const decltype(a.x)& s)
{
    for (size_t i = 0; i < sizeof(Vec) / sizeof(decltype(a.x)); ++i)
        a[i] /= s;
    return a;
}

namespace VecUtils 
{
    // Length
    template <VecType Vec>
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
    template <VecType Vec>
    auto length2(const Vec& a)
    {
        using T = decltype(a.x);
        T sum = 0;
        for (size_t i = 0; i < sizeof(Vec) / sizeof(T); ++i)
            sum += a[i] * a[i];
        return sum;
    }

    // Distance
    template <VecType Vec>
    float distance(const Vec& a, const Vec& b)
    {
        return length(a - b);
    }

    // Distance squared; useful to avoid expensive sqrt
    template <VecType Vec>
    float distance2(const Vec& a, const Vec& b)
    {
        return length2(a - b);
    }

    // Normalize
    template <VecType Vec>
    Vec normalize(const Vec& a)
    {
        auto len = VecUtils::length(a);
        if (len == 0) return a; 
        return a / len;
    }

    // Dot product
    template <VecType Vec>
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
    template <VecType T>
    vec<T, 3> cross(const vec<T, 3>& a, const vec<T, 3>& b)
    {
        return vec<T, 3>{
            a.y* b.z - a.z * b.y,
                a.z* b.x - a.x * b.z,
                a.x* b.y - a.y * b.x
        };
    }
}


