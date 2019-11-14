#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/STL/type_traits>
#include <Filters/TransferFunction.hpp>

/// @addtogroup FilterImplementations
/// @{

/** 
 * @brief   Infinite Impulse Response filter implementation that does not 
 *          normalize the coefficients upon initialization, the division by 
 *          @f$ a_0 @f$ is carried out on each filter iteration.
 * 
 * This class is slower than @ref NormalizingIIRFilter, but it works better
 * for integer types, because it has no rounding error on the coefficients.
 * 
 * Implements the following difference equation using the Direct-Form 1 
 * implementation:
 * 
 * @f[
 * y[n] = \frac{1}{a_0} \left(\sum_{i=0}^{N_b-1} b_i \cdot x[n-i]
 *                          - \sum_{i=1}^{N_a-1} a_i \cdot y[n-i] \right)
 * @f]
 */
template <uint8_t NB, uint8_t NA, class T>
class NonNormalizingIIRFilter {
  public:
    /**
     * @brief   Construct a new Non-Normalizing IIR Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = \frac{b_0 + b_1 z^{-1} + \ldots + b_{N_b} z ^{-N_b}}
     *             {a_0 + a_1 z^{-1} + \ldots + a_{N_b} z ^{-N_a}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     */
    NonNormalizingIIRFilter(const AH::Array<T, NB> &b_coefficients,
                            const AH::Array<T, NA> &a_coefficients)
        : a0(a_coefficients[0]) {
        for (uint8_t i = 0; i < 2 * NB - 1; ++i)
            this->b_coefficients[i] = b_coefficients[(2 * NB - 1 - i) % NB];
        for (uint8_t i = 0; i < 2 * MA - 1; ++i)
            this->a_coefficients[i] = a_coefficients[(2 * MA - 2 - i) % MA + 1];
    }

    NonNormalizingIIRFilter(const TransferFunction<NB, NA, T> &tf)
        : NonNormalizingIIRFilter{tf.b, tf.a} {}

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        // Save the new input to the ring buffer.
        x[index_b] = input;

        // Calculate the offset to the shifted coefficients.
        T *b_coeff_shift = b_coefficients.end() - NB - index_b;
        T *a_coeff_shift = a_coefficients.end() - MA - index_a;

        // Multiply and accumulate the inputs and their respective coefficients.
        T acc = {};
        for (uint8_t i = 0; i < NB; i++)
            acc += x[i] * b_coeff_shift[i];

        // Multiply and accumulate the inputs and their respective coefficients.
        for (uint8_t i = 0; i < MA; i++)
            acc -= y[i] * a_coeff_shift[i];

        // Save the current output
        acc /= a0;
        y[index_a] = acc;

        // Increment and wrap around the index of the ring buffer.
        index_b++;
        if (index_b == NB)
            index_b = 0;

        index_a++;
        if (index_a == MA)
            index_a = 0;

        return acc;
    }

  private:
    constexpr static uint8_t MA = NA - 1;
    uint8_t index_b = 0, index_a = 0;
    AH::Array<T, NB> x = {};                 ///< Previous inputs
    AH::Array<T, MA> y = {};                 ///< Previous outputs
    AH::Array<T, 2 * NB - 1> b_coefficients; ///< Numerator coefficients
    AH::Array<T, 2 * MA - 1> a_coefficients; ///< Denominator coefficients
    T a0;
};

/** 
 * @brief   Infinite Impulse Response filter implementation that normalizes the
 *          coefficients upon initialization.
 * 
 * This class is faster than @ref NonNormalizingIIRFilter, because each filter 
 * iteration involves only addition and multiplication, no divisions.  
 * It works great for floating point numbers, but might be less ideal
 * for integer types, because it can create large rounding errors on the 
 * coefficients.
 * 
 * Implements the following difference equation using the Direct-Form 1 
 * implementation:
 * 
 * @f[
 * y[n] = \frac{1}{a_0} \left(\sum_{i=0}^{N_b-1} b_i \cdot x[n-i]
 *                          - \sum_{i=1}^{N_a-1} a_i \cdot y[n-i] \right)
 * @f]
 */
template <uint8_t NB, uint8_t NA, class T>
class NormalizingIIRFilter {
  public:
    /**
     * @brief   Construct a new Normalizing IIR Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = \frac{b_0 + b_1 z^{-1} + \ldots + b_{N_b} z ^{-N_b}}
     *             {a_0 + a_1 z^{-1} + \ldots + a_{N_b} z ^{-N_a}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     */
    NormalizingIIRFilter(const AH::Array<T, NB> &b_coefficients,
                         const AH::Array<T, NA> &a_coefficients) {
        T a0 = a_coefficients[0];
        for (uint8_t i = 0; i < 2 * NB - 1; ++i)
            this->b_coefficients[i] =
                b_coefficients[(2 * NB - 1 - i) % NB] / a0;
        for (uint8_t i = 0; i < 2 * MA - 1; ++i)
            this->a_coefficients[i] =
                a_coefficients[(2 * MA - 2 - i) % MA + 1] / a0;
    }

    NormalizingIIRFilter(const TransferFunction<NB, NA, T> &tf)
        : NormalizingIIRFilter{tf.b, tf.a} {}

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        // Save the new input to the ring buffer.
        x[index_b] = input;

        // Calculate the offset to the shifted coefficients.
        T *b_coeff_shift = b_coefficients.end() - NB - index_b;
        T *a_coeff_shift = a_coefficients.end() - MA - index_a;

        // Multiply and accumulate the inputs and their respective coefficients.
        T acc = {};
        for (uint8_t i = 0; i < NB; i++)
            acc += x[i] * b_coeff_shift[i];

        // Multiply and accumulate the inputs and their respective coefficients.
        for (uint8_t i = 0; i < MA; i++)
            acc -= y[i] * a_coeff_shift[i];

        // Save the current output
        y[index_a] = acc;

        // Increment and wrap around the index of the ring buffer.
        index_b++;
        if (index_b == NB)
            index_b = 0;

        index_a++;
        if (index_a == MA)
            index_a = 0;

        return acc;
    }

  private:
    constexpr static uint8_t MA = NA - 1;
    uint8_t index_b = 0, index_a = 0;
    AH::Array<T, NB> x = {};
    AH::Array<T, MA> y = {};
    AH::Array<T, 2 * NB - 1> b_coefficients;
    AH::Array<T, 2 * MA - 1> a_coefficients;
};

/// @}

/// Select the @ref NormalizingIIRFilter implementation if @p T is a floating
/// point type, @ref NonNormalizingIIRFilter otherwise.
template <uint8_t NB, uint8_t NA, class T>
using IIRImplementation =
    typename std::conditional<std::is_floating_point<T>::value,
                              NormalizingIIRFilter<NB, NA, T>,
                              NonNormalizingIIRFilter<NB, NA, T>>::type;

/// @addtogroup Filters
/// @{

/** 
 * @brief   Generic Infinite Impulse Response filter class.
 * 
 * Uses the @ref NormalizingIIRFilter implementation for floating point 
 * types, and @ref NonNormalizingIIRFilter for all other types. 
 * 
 * Implements the following difference equation using the Direct-Form 1 
 * implementation:
 * 
 * @f[
 * y[n] = \frac{1}{a_0} \left(\sum_{i=0}^{N_b-1} b_i \cdot x[n-i]
 *                          - \sum_{i=1}^{N_a-1} a_i \cdot y[n-i] \right)
 * @f]
 */
template <uint8_t NB, uint8_t NA = NB, class T = float>
class IIRFilter : public IIRImplementation<NB, NA, T> {
  public:
    /**
     * @brief   Construct a new IIR Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = \frac{b_0 + b_1 z^{-1} + \ldots + b_{N_b} z ^{-N_b}}
     *             {a_0 + a_1 z^{-1} + \ldots + a_{N_b} z ^{-N_a}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     */
    IIRFilter(const AH::Array<T, NB> &b_coefficients,
              const AH::Array<T, NA> &a_coefficients)
        : IIRImplementation<NB, NA, T>{b_coefficients, a_coefficients} {}

    IIRFilter(const TransferFunction<NB, NA, T> &tf)
        : IIRImplementation<NB, NA, T>{tf} {}

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        return IIRImplementation<NB, NA, T>::operator()(input);
    }
};

/// Create an IIRFilter from the given transfer function.
template <size_t NB, size_t NA, class T = float>
IIRFilter<NB, NA, T> makeIIRFilter(const TransferFunction<NB, NA, T> &tf) {
    return tf;
}

/// Create an IIRFilter from the given transfer function coefficients.
template <size_t NB, size_t NA, class T = float>
IIRFilter<NB, NA, T> makeIIRFilter(const AH::Array<T, NB> &b_coefficients,
                                   const AH::Array<T, NA> &a_coefficients) {
    return {b_coefficients, a_coefficients};
}

/// @}