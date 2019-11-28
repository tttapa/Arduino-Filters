#pragma once

/// @addtogroup Filters
/// @{

#include <AH/Containers/Array.hpp>

/** 
 * @brief   Class for transfer function coefficients.
 * 
 * @f[
 * H(z) = \frac{b_0 + b_1 z^{-1} + \ldots + b_{N_b} z ^{-N_b}}
 *             {a_0 + a_1 z^{-1} + \ldots + a_{N_b} z ^{-N_a}}
 * @f]
 */
template <size_t NB, size_t NA = NB, class T = float>
struct TransferFunction {
    TransferFunction() = default;

    /// Construct a new Transfer Function object.
    TransferFunction(const AH::Array<T, NB> &b, const AH::Array<T, NA> &a)
        : b(b), a(a) {}

    AH::Array<T, NB> b = {{}};
    AH::Array<T, NA> a = {{}};
};

/// @}