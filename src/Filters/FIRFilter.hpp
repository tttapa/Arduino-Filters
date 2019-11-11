#pragma once

#include <AH/Containers/Array.hpp>

/// @addtogroup Filters
/// @{

/** 
 * @brief   Finite Impulse Response filter implementation.
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \sum_{i=0}^{N-1} b_i \cdot x[n-i]
 * @f]
 */
template <uint8_t N, class T = float>
class FIRFilter {
  public:
    /**
     * @brief   Construct a new FIR Filter object.
     * 
     * The coefficients @f$ b @f$ can be derived from the transfer function:
     * 
     * @f[
     * H(z) = b_0 + b_1 z^{-1} + \ldots + b_{N_b} z ^{-N_b}
     * @f]
     * 
     * @param   coefficients 
     *          The coefficients of the transfer function numerator.
     */
    FIRFilter(const AH::Array<T, N> &coefficients) {
        for (uint8_t i = 0; i < 2 * N - 1; ++i)
            this->coefficients[i] = coefficients[(2 * N - 1 - i) % N];
    }

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        // Save the new value to the ring buffer.
        x[index_b] = input;

        // Calculate the offset to the shifted coefficients.
        T *coeff_shift = coefficients.end() - N - index_b;

        // Multiply and accumulate the inputs and their respective coefficients.
        T acc = {};
        for (uint8_t i = 0; i < N; i++)
            acc += x[i] * coeff_shift[i];

        // Increment and wrap around the index of the ring buffer.
        index_b++;
        if (index_b == N)
            index_b = 0;

        return acc;
    }

  private:
    uint8_t index_b = 0;
    AH::Array<T, N> x = {};
    AH::Array<T, 2 * N - 1> coefficients;
};

/// @}
