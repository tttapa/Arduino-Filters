#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/STL/cmath>
#include <Filters/TransferFunction.hpp>

/// @addtogroup Filters
/// @{

/// BiQuadratic transfer function coefficients.
template <class T = float>
using BiQuadCoefficients = TransferFunction<3, 3, T>;

/// @}

/// @addtogroup FilterImplementations
/// @{

// Direct Form 1 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/** 
 * @brief   BiQuad filter Direct Form 1 implementation that normalizes the
 *          coefficients upon initialization.
 * 
 * This class is faster than @ref NonNormalizingBiQuadFilterDF1, because each 
 * filter iteration involves only addition and multiplication, no divisions.  
 * It works great for floating point numbers, but might be less ideal
 * for integer types, because it can create large rounding errors on the 
 * coefficients.
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \frac{b_0 \cdot x[n] + b_1 \cdot x[n-1] + b_2 \cdot x[n-2] 
 *            - a_1 \cdot y[n-1] - a_2 \cdot y[n-2]}{a_0}
 * @f]
 */
template <class T>
class NormalizingBiQuadFilterDF1 {
  public:
    NormalizingBiQuadFilterDF1() = default;

    NormalizingBiQuadFilterDF1(const AH::Array<T, 3> &b,
                               const AH::Array<T, 3> &a)
        : b(b / a[0]), a(-a.template slice<1, 2>() / a[0]) {}

    NormalizingBiQuadFilterDF1(const BiQuadCoefficients<T> &coefficients)
        : NormalizingBiQuadFilterDF1{coefficients.b, coefficients.a} {}

    NormalizingBiQuadFilterDF1(const AH::Array<T, 3> &b,
                               const AH::Array<T, 3> &a, T gain)
        : b(b * gain / a[0]), a(-a.template slice<1, 2>() / a[0]) {}

    NormalizingBiQuadFilterDF1(const BiQuadCoefficients<T> &coefficients,
                               T gain)
        : NormalizingBiQuadFilterDF1{coefficients.b, coefficients.a, gain} {}

    template <bool Enable = true>
    static std::enable_if_t<std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &x, AH::Array<T, 2> &y,
           const AH::Array<T, 3> &b, const AH::Array<T, 2> &a) {
        T acc = input * b[0];
        acc = std::fma(x[0], b[1], acc);
        acc = std::fma(x[1], b[2], acc);
        acc = std::fma(y[0], a[0], acc);
        acc = std::fma(y[1], a[1], acc);
        x[1] = x[0];
        x[0] = input;
        y[1] = y[0];
        y[0] = acc;
        return y[0];
    }

    template <bool Enable = true>
    static std::enable_if_t<!std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &x, AH::Array<T, 2> &y,
           const AH::Array<T, 3> &b, const AH::Array<T, 2> &a) {
        T acc = input * b[0];
        acc = x[0] * b[1] + acc;
        acc = x[1] * b[2] + acc;
        acc = y[0] * a[0] + acc;
        acc = y[1] * a[1] + acc;
        x[1] = x[0];
        x[0] = input;
        y[1] = y[0];
        y[0] = acc;
        return y[0];
    }

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) { return update(input, x, y, b, a); }

  private:
    AH::Array<T, 2> x = {{}}; ///< Previous inputs
    AH::Array<T, 2> y = {{}}; ///< Previous outputs
    AH::Array<T, 3> b = {{}}; ///< Numerator coefficients
    AH::Array<T, 2> a = {{}}; ///< Denominator coefficients
};

/** 
 * @brief   BiQuad filter Direct Form 1 implementation that does not 
 *          normalize the coefficients upon initialization, the division by 
 *          @f$ a_0 @f$ is carried out on each filter iteration.
 * 
 * This class is slower than @ref NormalizingBiQuadFilterDF1, but it works 
 * better for integer types, because it has no rounding error on the 
 * coefficients.
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \frac{b_0 \cdot x[n] + b_1 \cdot x[n-1] + b_2 \cdot x[n-2] 
 *            - a_1 \cdot y[n-1] - a_2 \cdot y[n-2]}{a_0}
 * @f]
 */
template <class T>
class NonNormalizingBiQuadFilterDF1 {
  public:
    NonNormalizingBiQuadFilterDF1() = default;

    NonNormalizingBiQuadFilterDF1(const AH::Array<T, 3> &b,
                                  const AH::Array<T, 3> &a)
        : b(b), a(-a.template slice<1, 2>()), a0(a[0]) {}

    NonNormalizingBiQuadFilterDF1(const BiQuadCoefficients<T> &coefficients)
        : NonNormalizingBiQuadFilterDF1{coefficients.b, coefficients.a} {}

    NonNormalizingBiQuadFilterDF1(const AH::Array<T, 3> &b,
                                  const AH::Array<T, 3> &a, T gain)
        : b(b * gain), a(-a.template slice<1, 2>()), a0(a[0]) {}

    NonNormalizingBiQuadFilterDF1(const BiQuadCoefficients<T> &coefficients,
                                  T gain)
        : NonNormalizingBiQuadFilterDF1{coefficients.b, coefficients.a, gain} {}

    template <bool Enable = true>
    static std::enable_if_t<std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &x, AH::Array<T, 2> &y,
           const AH::Array<T, 3> &b, const AH::Array<T, 2> &a, T a0) {
        T acc = input * b[0];
        acc = std::fma(x[0], b[1], acc);
        acc = std::fma(x[1], b[2], acc);
        acc = std::fma(y[0], a[0], acc);
        acc = std::fma(y[1], a[1], acc);
        x[1] = x[0];
        x[0] = input;
        y[1] = y[0];
        y[0] = acc / a0;
        return y[0];
    }

    template <bool Enable = true>
    static std::enable_if_t<!std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &x, AH::Array<T, 2> &y,
           const AH::Array<T, 3> &b, const AH::Array<T, 2> &a, T a0) {
        T acc = input * b[0];
        acc = x[0] * b[1] + acc;
        acc = x[1] * b[2] + acc;
        acc = y[0] * a[0] + acc;
        acc = y[1] * a[1] + acc;
        x[1] = x[0];
        x[0] = input;
        y[1] = y[0];
        y[0] = acc / a0;
        return y[0];
    }

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) { return update(input, x, y, b, a, a0); }

  private:
    AH::Array<T, 2> x = {{}}; ///< Previous inputs
    AH::Array<T, 2> y = {{}}; ///< Previous outputs
    AH::Array<T, 3> b = {{}}; ///< Numerator coefficients
    AH::Array<T, 2> a = {{}}; ///< Denominator coefficients
    T a0 = T(1.);             ///< First denominator coefficient
};

/// @}

/// Select the @ref NormalizingIIRFilter implementation if @p T is a floating
/// point type, @ref NonNormalizingIIRFilter otherwise.
template <class T>
using BiQuadDF1Implementation =
    typename std::conditional<std::is_floating_point<T>::value,
                              NormalizingBiQuadFilterDF1<T>,
                              NonNormalizingBiQuadFilterDF1<T>>::type;

/// @addtogroup Filters
/// @{

/** 
 * @brief   Generic BiQuad (Bi-Quadratic) filter class, Direct Form 1 
 *          implementation.
 * 
 * Uses the @ref NormalizingBiQuadFilterDF1 implementation for floating point 
 * types, and @ref NonNormalizingBiQuadFilterDF1 for all other types. 
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \frac{b_0 \cdot x[n] + b_1 \cdot x[n-1] + b_2 \cdot x[n-2] 
 *            - a_1 \cdot y[n-1] - a_2 \cdot y[n-2]}{a_0}
 * @f]
 */
template <class T = float>
class BiQuadFilterDF1 : public BiQuadDF1Implementation<T> {
  public:
    BiQuadFilterDF1() = default;

    /**
     * @brief   Construct a new BiQuad (Bi-Quadratic) Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = \frac{b_0 + b_1 z^{-1} + b_2 z ^{-2}}
     *             {a_0 + a_1 z^{-1} + a_2 z ^{-2}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     */
    BiQuadFilterDF1(const AH::Array<T, 3> &b_coefficients,
                    const AH::Array<T, 3> &a_coefficients)
        : BiQuadDF1Implementation<T>{b_coefficients, a_coefficients} {}

    BiQuadFilterDF1(const BiQuadCoefficients<T> &coefficients)
        : BiQuadDF1Implementation<T>{coefficients} {}

    /**
     * @brief   Construct a new BiQuad (Bi-Quadratic) Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = K \frac{b_0 + b_1 z^{-1} + b_2 z ^{-2}}
     *               {a_0 + a_1 z^{-1} + a_2 z ^{-2}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     * @param   gain
     *          Gain factor @f$ K @f$.
     */
    BiQuadFilterDF1(const AH::Array<T, 3> &b_coefficients,
                    const AH::Array<T, 3> &a_coefficients, T gain)
        : BiQuadDF1Implementation<T>{b_coefficients, a_coefficients, gain} {}

    BiQuadFilterDF1(const BiQuadCoefficients<T> &coefficients, T gain)
        : BiQuadDF1Implementation<T>{coefficients, gain} {}

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        return BiQuadDF1Implementation<T>::operator()(input);
    }
};

/// @}

// Direct Form 2 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// @addtogroup FilterImplementations
/// @{

/** 
 * @brief   BiQuad filter Direct Form 2 implementation that normalizes the
 *          coefficients upon initialization.
 * 
 * This class is faster than @ref NonNormalizingBiQuadFilterDF2, because each 
 * filter iteration involves only addition and multiplication, no divisions.  
 * It works great for floating point numbers, but might be less ideal
 * for integer types, because it can create large rounding errors on the 
 * coefficients.
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \frac{b_0 \cdot x[n] + b_1 \cdot x[n-1] + b_2 \cdot x[n-2] 
 *            - a_1 \cdot y[n-1] - a_2 \cdot y[n-2]}{a_0}
 * @f]
 */
template <class T>
class NormalizingBiQuadFilterDF2 {
  public:
    NormalizingBiQuadFilterDF2() = default;

    NormalizingBiQuadFilterDF2(const AH::Array<T, 3> &b,
                               const AH::Array<T, 3> &a)
        : b(b / a[0]), a(-a.template slice<1, 2>() / a[0]) {}

    NormalizingBiQuadFilterDF2(const BiQuadCoefficients<T> &coefficients)
        : NormalizingBiQuadFilterDF2{coefficients.b, coefficients.a} {}

    NormalizingBiQuadFilterDF2(const AH::Array<T, 3> &b,
                               const AH::Array<T, 3> &a, T gain)
        : b(b * gain / a[0]), a(-a.template slice<1, 2>() / a[0]) {}

    NormalizingBiQuadFilterDF2(const BiQuadCoefficients<T> &coefficients,
                               T gain)
        : NormalizingBiQuadFilterDF2{coefficients.b, coefficients.a, gain} {}

    template <bool Enable = true>
    static std::enable_if_t<std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &w, const AH::Array<T, 3> &b,
           const AH::Array<T, 2> &a) {
        input = std::fma(a[0], w[0], input);
        input = std::fma(a[1], w[1], input);
        T result = b[0] * input;
        result = std::fma(b[1], w[0], result);
        result = std::fma(b[2], w[1], result);
        w[1] = w[0];
        w[0] = input;
        return result;
    }

    template <bool Enable = true>
    static std::enable_if_t<!std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &w, const AH::Array<T, 3> &b,
           const AH::Array<T, 2> &a) {
        input += a[0] * w[0];
        input += a[1] * w[1];
        T result = b[0] * input;
        result += b[1] * w[0];
        result += b[2] * w[1];
        w[1] = w[0];
        w[0] = input;
        return result;
    }

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) { return update(input, w, b, a); }

  private:
    AH::Array<T, 2> w = {{}}; ///< Internal state
    AH::Array<T, 3> b = {{}}; ///< Numerator coefficients
    AH::Array<T, 2> a = {{}}; ///< Denominator coefficients
};

/** 
 * @brief   BiQuad filter Direct Form 2 implementation that does not 
 *          normalize the coefficients upon initialization, the division by 
 *          @f$ a_0 @f$ is carried out on each filter iteration.
 * 
 * This class is slower than @ref NormalizingBiQuadFilterDF2, but it works 
 * better for integer types, because it has no rounding error on the 
 * coefficients.
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \frac{b_0 \cdot x[n] + b_1 \cdot x[n-1] + b_2 \cdot x[n-2] 
 *            - a_1 \cdot y[n-1] - a_2 \cdot y[n-2]}{a_0}
 * @f]
 */
template <class T>
class NonNormalizingBiQuadFilterDF2 {
  public:
    NonNormalizingBiQuadFilterDF2() = default;

    NonNormalizingBiQuadFilterDF2(const AH::Array<T, 3> &b,
                                  const AH::Array<T, 3> &a)
        : b(b), a(-a.template slice<1, 2>()), a0(a[0]) {}

    NonNormalizingBiQuadFilterDF2(const BiQuadCoefficients<T> &coefficients)
        : NonNormalizingBiQuadFilterDF2{coefficients.b, coefficients.a} {}

    NonNormalizingBiQuadFilterDF2(const AH::Array<T, 3> &b,
                                  const AH::Array<T, 3> &a, T gain)
        : b(b * gain), a(-a.template slice<1, 2>()), a0(a[0]) {}

    NonNormalizingBiQuadFilterDF2(const BiQuadCoefficients<T> &coefficients,
                                  T gain)
        : NonNormalizingBiQuadFilterDF2{coefficients.b, coefficients.a, gain} {}

    template <bool Enable = true>
    static std::enable_if_t<std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &w, const AH::Array<T, 3> &b,
           const AH::Array<T, 2> &a, T a0) {
        input = std::fma(a[0], w[0], input);
        input = std::fma(a[1], w[1], input);
        input /= a0;
        T result = b[0] * input;
        result = std::fma(b[1], w[0], result);
        result = std::fma(b[2], w[1], result);
        w[1] = w[0];
        w[0] = input;
        return result;
    }

    template <bool Enable = true>
    static std::enable_if_t<!std::is_floating_point<T>::value && Enable, T>
    update(T input, AH::Array<T, 2> &w, const AH::Array<T, 3> &b,
           const AH::Array<T, 2> &a, T a0) {
        input += a[0] * w[0];
        input += a[1] * w[1];
        input /= a0;
        T result = b[0] * input;
        result += b[1] * w[0];
        result += b[2] * w[1];
        w[1] = w[0];
        w[0] = input;
        return result;
    }

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) { return update(input, w, b, a, a0); }

  private:
    AH::Array<T, 2> w = {{}}; ///< Internal state
    AH::Array<T, 3> b = {{}}; ///< Numerator coefficients
    AH::Array<T, 2> a = {{}}; ///< Denominator coefficients
    T a0 = T(1.);             ///< First denominator coefficient
};

/// @}

/// Select the @ref NormalizingIIRFilter implementation if @p T is a floating
/// point type, @ref NonNormalizingIIRFilter otherwise.
template <class T>
using BiQuadDF2Implementation =
    typename std::conditional<std::is_floating_point<T>::value,
                              NormalizingBiQuadFilterDF2<T>,
                              NonNormalizingBiQuadFilterDF2<T>>::type;

/// @addtogroup Filters
/// @{

/** 
 * @brief   Generic BiQuad (Bi-Quadratic) filter class, Direct Form 2 
 *          implementation.
 * 
 * Uses the @ref NormalizingBiQuadFilterDF2 implementation for floating point 
 * types, and @ref NonNormalizingBiQuadFilterDF2 for all other types. 
 * 
 * Implements the following difference equation:
 * 
 * @f[
 * y[n] = \frac{b_0 \cdot x[n] + b_1 \cdot x[n-1] + b_2 \cdot x[n-2] 
 *            - a_1 \cdot y[n-1] - a_2 \cdot y[n-2]}{a_0}
 * @f]
 */
template <class T = float>
class BiQuadFilterDF2 : public BiQuadDF2Implementation<T> {
  public:
    BiQuadFilterDF2() = default;

    /**
     * @brief   Construct a new BiQuad (Bi-Quadratic) Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = \frac{b_0 + b_1 z^{-1} + b_2 z ^{-2}}
     *             {a_0 + a_1 z^{-1} + a_2 z ^{-2}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     */
    BiQuadFilterDF2(const AH::Array<T, 3> &b_coefficients,
                    const AH::Array<T, 3> &a_coefficients)
        : BiQuadDF2Implementation<T>{b_coefficients, a_coefficients} {}

    BiQuadFilterDF2(const BiQuadCoefficients<T> &coefficients)
        : BiQuadDF2Implementation<T>{coefficients} {}

    /**
     * @brief   Construct a new BiQuad (Bi-Quadratic) Filter object.
     * 
     * The coefficients @f$ b @f$ and @f$ a @f$ can be derived from the transfer
     * function:
     * 
     * @f[
     * H(z) = K \frac{b_0 + b_1 z^{-1} + b_2 z ^{-2}}
     *               {a_0 + a_1 z^{-1} + a_2 z ^{-2}}
     * @f]
     * 
     * @param   b_coefficients 
     *          The coefficients of the transfer function numerator.
     * @param   a_coefficients 
     *          The coefficients of the transfer function denominator.
     * @param   gain
     *          Gain factor @f$ K @f$.
     */
    BiQuadFilterDF2(const AH::Array<T, 3> &b_coefficients,
                    const AH::Array<T, 3> &a_coefficients, T gain)
        : BiQuadDF2Implementation<T>{b_coefficients, a_coefficients, gain} {}

    BiQuadFilterDF2(const BiQuadCoefficients<T> &coefficients, T gain)
        : BiQuadDF2Implementation<T>{coefficients, gain} {}

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        return BiQuadDF2Implementation<T>::operator()(input);
    }
};

/// @}