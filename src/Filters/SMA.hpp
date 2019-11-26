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

/// @addtogroup Filters
/// @{

/** 
 * @brief   Simple Moving Average filter.
 * 
 * @f[
 * y[n] = \frac{1}{N} \sum_{i=0}^{N-1}x[n-i]
 * @f]
 * 
 * @see     https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Simple%20Moving%20Average/Simple-Moving-Average.html
 * 
 * @tparam  N
 *          The number of samples to average.
 * @tparam  input_t
 *          The type of the input (and output) of the filter.
 * @tparam  sum_t
 *          The type to use for the accumulator, must be large enough to fit
 *          N times the maximum input value.
 */
template <uint8_t N, class input_t = uint16_t, class sum_t = uint32_t>
class SMA {
  public:
    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    input_t operator()(input_t input) {
        sum -= previousInputs[index];
        sum += input;
        previousInputs[index] = input;
        if (++index == N)
            index = 0;
        return round_div<N>(sum);
    }

  private:
    uint8_t index = 0;
    AH::Array<input_t, N> previousInputs = {};
    sum_t sum = 0;
};

/// @}
