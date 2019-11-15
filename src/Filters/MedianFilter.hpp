#pragma once

#include <AH/STL/algorithm> // std::partial_sort_copy
#include <AH/STL/array>     // std::array
#include <AH/STL/cstdint>   // uint8_t

/// @addtogroup Filters
/// @{

/** 
 * @brief  Class for Median Filters. 
 * 
 * Use the parenthesis or call operator (`operator()`) with the next input of 
 * the filter as an argument to update the Median filter. 
 * This operator returns the next output of the filter.
 * 
 * The output equation is:
 * @f$ y[n] = \text{median}\Big(x[n], x[n-1],\ \ldots,\ x[n-N+1]\Big) @f$
 * 
 * @tparam  N
 *          The number of previous values to take the median of.
 * @tparam  T 
 *          The type of the input and output values of the filter.
 */
template <uint8_t N, class T = float>
class MedianFilter {
  public:
    /**
     * @brief   Construct a new Median Filter (zero initialized).
     */
    MedianFilter() = default;

    /** 
     * @brief   Construct a new Median Filter, and initialize it with the given
     *          value.
     * 
     * @param   initialValue
     *          Determines the initial state of the filter:  
     *          @f$ x[-N] =\ \ldots\ = x[-2] = x[-1] = \text{initialValue} @f$
     */
    MedianFilter(T initialValue) {
        std::fill(previousInputs.begin(), previousInputs.end(), initialValue);
    }

    /** 
     * @brief   Calculate the output @f$ y[n] @f$ for a given input 
     *          @f$ x[n] @f$.
     * 
     * @f$ y[n] = \text{median}\Big(x[n], x[n-1],\ \ldots,\ x[n-N+1]\Big) @f$
     * 
     * @param   x
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T x) {
        // Insert the new input into the ring buffer, overwriting the oldest
        // input.
        previousInputs[index] = x;
        if (++index == N)
            index = 0;

        // Calculate the median of the buffer by sorting the first half. A copy
        // should be made to keep the order of the buffer intact.
        const uint8_t halfSize = (N + 1) / 2;
        std::array<T, halfSize> sorted;
        std::partial_sort_copy(previousInputs.begin(), previousInputs.end(),
                               sorted.begin(), sorted.end());

        // If the length of the ring buffer is odd, then the median is its
        // center element, which is the final element of `sorted`.
        // If the length of the ring buffer is even, then we need to take the
        // average of its two center elements, which are the final two elements
        // of `sorted`.
        if (N % 2 == 0)
            return (sorted.end()[-2] + sorted.end()[-1]) / 2;
        else
            return sorted.end()[-1];
    }

  private:
    /// The last index in the ring buffer.
    uint8_t index = 0;
    /// A ring buffer to keep track of the N last inputs.
    std::array<T, N> previousInputs = {};
};

/// @}