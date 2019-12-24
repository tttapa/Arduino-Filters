#pragma once

#include <AH/STL/cmath>
#include <AH/STL/cstdint>
#include <AH/STL/limits>
#include <AH/STL/type_traits>

#include <AH/Error/Error.hpp>

template <class T>
struct DoubleWidthInt {
    using type = void;
};

template <>
struct DoubleWidthInt<uint8_t> {
    using type = uint16_t;
};

template <>
struct DoubleWidthInt<int8_t> {
    using type = int16_t;
};

template <>
struct DoubleWidthInt<uint16_t> {
    using type = uint32_t;
};

template <>
struct DoubleWidthInt<int16_t> {
    using type = int32_t;
};

template <>
struct DoubleWidthInt<uint32_t> {
    using type = uint64_t;
};

template <>
struct DoubleWidthInt<int32_t> {
    using type = int64_t;
};

/// @addtogroup FixedPoint
/// @{

/// Get the integer type that has twice the number of bits as the given type.
template <class T>
using DoubleWidthInt_t = typename DoubleWidthInt<T>::type;

/**
 * @brief   Very basic fixed-point integer implementation.
 * 
 * @tparam  T 
 *          The integer type to use.
 * @tparam  N 
 *          The number of fractional bits.
 * @tparam  T2 
 *          The integer type to use for intermediate values when mutliplying or
 *          dividing.
 */
template <class T, uint8_t N, class T2 = DoubleWidthInt_t<T>>
class FixedPoint {
  public:
    /// Fixed-point representation of the number one.
    constexpr static T one = 1 << N;

    /// Default constructor
    FixedPoint() : val(0) {}

    /// Initialize U from a numeric value.
    template <class U>
    FixedPoint(U f) : val(std::llrint(f * one)) {}

    /// Convert a raw integer representation to fixed point type.
    static constexpr FixedPoint raw(T t) {
        FixedPoint res;
        res.val = t;
        return res;
    }

    /// Addition.
    FixedPoint operator+(FixedPoint rhs) const {
        // if (rhs.val > 0 &&
        //     this->val > std::numeric_limits<T>::max() - rhs.val) {
        //     ERROR("Integer overflow", 0x0FF0);
        //     return raw(std::numeric_limits<T>::max());
        // }
        // if (rhs.val < 0 &&
        //     this->val < std::numeric_limits<T>::min() - rhs.val) {
        //     ERROR("Integer underflow", 0x0FF1);
        //     return raw(std::numeric_limits<T>::min());
        // }
        return raw(static_cast<T>(
            static_cast<typename std::make_unsigned<T>::type>(this->val) +
            static_cast<typename std::make_unsigned<T>::type>(rhs.val)));
    }

    /// Subtraction.
    FixedPoint operator-(FixedPoint rhs) const {
        // return raw(this->val - rhs.val);
        return *this + -rhs;
    }

    /// Invert.
    FixedPoint operator-() const { return raw(-this->val); }

    /// Multiplication.
    FixedPoint operator*(FixedPoint rhs) const {
        return raw(div_N(T2(this->val) * T2(rhs.val)));
    }

    /// Multiplication with normal integer.
    T2 operator*(T2 rhs) const { return div_N(T2(this->val) * T2(rhs)); }

    /// Division.
    FixedPoint operator/(FixedPoint rhs) const {
        if (rhs.val == one)
            return raw(this->val);
        return raw(T2(this->val) * one / rhs.val);
    }

    /// Divide the given integer by @f$ 2^N @f$.
    static T div_N(T2 val) {
        static_assert(std::is_unsigned<T2>::value || (-97 * 2) >> 1 == -97,
                      "Negative signed right shift incorrect");
        int neg = val < 0 ? 1 : 0;
        return (val + (1 << (N - 1)) - neg) >> N;
    }

    explicit operator long double() const { return (long double)val / one; }
    explicit operator double() const { return (double)val / one; }
    explicit operator float() const { return (float)val / one; }

  private:
    T val;
};

/// Multiply normal integer with fixed point integer.
template <class T, uint8_t N, class T2>
T2 operator*(T lhs, FixedPoint<T, N, T2> rhs) {
    return rhs * lhs;
}

/// Multiply normal integer with fixed point integer.
template <class T, uint8_t N, class T2>
T2 operator*(T2 lhs, FixedPoint<T, N, T2> rhs) {
    return rhs * lhs;
}

#ifndef ARDUINO

#include <iosfwd>

/// Printing a fixed-point integer.
template <class T, class T2, uint8_t N>
std::ostream &operator<<(std::ostream &os, FixedPoint<T, N, T2> fp) {
    return os << double(fp);
}

#else

#include <AH/PrintStream/PrintStream.hpp>

/// Printing a fixed-point integer.
template <class T, uint8_t N, class T2>
Print &operator<<(Print &os, FixedPoint<T, N, T2> fp) {
    return os << double(fp);
}

#endif

/// @}