#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/STL/cstdint>
#include <AH/STL/type_traits>

template <uint8_t N, class T>
struct round_div_default {
    static T div(T val) { return val / N; }
};

template <uint8_t N, class T>
struct round_div_unsigned_int {
    static T div(T val) {
        return (val + (N / 2)) / N;
        static_assert(std::is_unsigned<T>::value && std::is_integral<T>::value,
                      "This function is only valid for unsigned integers");
    }
};

template <uint8_t N, class T>
struct round_div_signed_int {
    static T div(T val) {
        T offset = val >= 0 ? (N / 2) : (-N / 2);
        return (val + offset) / N;
    }
};

template <uint8_t N, class T>
struct round_div_int
    : std::conditional<std::is_signed<T>::value, round_div_signed_int<N, T>,
                       round_div_unsigned_int<N, T>>::type {};

template <uint8_t N, class T>
struct round_div_helper
    : std::conditional<std::is_integral<T>::value, round_div_int<N, T>,
                       round_div_default<N, T>>::type {};

template <size_t N, class T>
T round_div(T val) {
    return round_div_helper<N, T>::div(val);
}

template <uint8_t N, class input_t = uint16_t, class sum_t = uint32_t>
class SMA {
  public:
    input_t operator()(input_t input) {
        sum -= previousInputs[index];
        sum += input;
        previousInputs[index] = input;
        if (++index == N)
            index = 0;
        return round_div<N>(sum);
    }

    static_assert(
        sum_t(0) < sum_t(-1), // Check that `sum_t` is an unsigned type
        "Error: sum data type should be an unsigned integer, otherwise, "
        "the rounding operation in the return statement is invalid.");

  private:
    uint8_t index = 0;
    AH::Array<input_t, N> previousInputs = {};
    sum_t sum = 0;
};