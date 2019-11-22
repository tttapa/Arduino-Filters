#pragma once

#include <AH/STL/cmath>
#include <Filters/FIRFilter.hpp>

/// @addtogroup FilterDesign
/// @{

/**
 * @brief   Create a very simple second-order FIR notch filter.
 * 
 * @f[
 * H(z) = \frac{z^2 - 2\cos\left(\omega_{c,d}\right)z + 1}
 *             {2 - 2\cos\left(\omega_{c,d}\right)}
 * @f]
 * where @f$ \omega_{c,d} = \pi f_n = 2\pi \frac{f_c}{f_n} @f$
 * 
 * @see <https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/FIR-Notch.html>
 * 
 * @param   f_n
 *          Normalized notch frequency in half-cycles per sample.  
 *          @f$ f_n = \frac{2 f_c}{f_s} \in \left[0, 1\right] @f$, where
 *          @f$ f_s @f$ is the sample frequency in @f$ \text{Hz} @f$, and 
 *          @f$ f_c @f$ is the analog notch frequency in @f$ \text{Hz} @f$.
 */
template <class T = float>
FIRFilter<3, T> simpleNotchFIR(double f_n) {
    double cos_omega = std::cos(M_PI * f_n);
    double normalize = 2 - 2 * cos_omega;
    return {{
        T(1. / normalize),
        T(-2 * cos_omega / normalize),
        T(1. / normalize),
    }};
}

/// @}