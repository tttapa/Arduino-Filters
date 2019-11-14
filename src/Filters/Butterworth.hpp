#pragma once

#include <AH/STL/cmath>
#include <Filters/SOSFilter.hpp>

/// @addtogroup FilterDesign
/// @{

/** 
 * @brief   Compute Butterworth filter coefficients.
 * @tparam  N
 *          Order of the filter.
 * @param   f_n
 *          Normalized cut-off frequency.
 *          @f$ f_n = \frac{f_c}{f_s} \in \left[0, \frac{1}{2}\right] @f$, where
 *          @f$ f_s @f$ is the sample frequency in @f$ \text{Hz} @f$, and 
 *          @f$ f_c @f$ is the cut-off frequency in @f$ \text{Hz} @f$.
 * @tparam  T
 *          The type of the coefficients.
 * 
 * @see <https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Discretization/Discretization-of-a-fourth-order-Butterworth-filter.html#discretization-using-second-order-sections-sos>
 */
template <uint8_t N, class T = float>
SOSCoefficients<T, (N + 1) / 2> butter_coeff(double f_n) {
    const T gamma = 1 / std::tan(M_PI * f_n / 2); // pre-warp factor
    auto make_sos = [=](uint8_t k) {
        const T gamma2 = gamma * gamma;
        const T alpha = 2 * std::cos(2 * M_PI * (2 * k + N + 1) / (4 * N));
        return BiQuadCoefficients<T>{
            {{1, 2, 1}}, // b0, b1, b2
            {{
                gamma2 - alpha * gamma + 1, // a0
                2 * (1 - gamma2),           // a1
                gamma2 + alpha * gamma + 1, // a2
            }},
        };
    };

    auto make_fos = [=]() {
        return BiQuadCoefficients<T>{
            {{1, 1}}, // b0, b1
            {{
                gamma + 1, // a0
                1 - gamma, // a1
            }},
        };
    };

    SOSCoefficients<T, (N + 1) / 2> sections;


    if (N % 2 == 0) {
        for (uint8_t i = 0; i < sections.length; ++i)
            sections[i] = make_sos(i);
    } else {
        for (uint8_t i = 0; i < sections.length - 1; ++i)
            sections[i] = make_sos(i);
        sections.end()[-1] = make_fos();
    }
    return sections;
}

/** 
 * @brief   Create a Butterworth filter, implemented as Second Order Sections
 *          (SOS) filter.
 * @tparam  N
 *          Order of the filter.
 * @param   f_n
 *          Normalized cut-off frequency.
 *          @f$ f_n = \frac{f_c}{f_s} \in \left[0, \frac{1}{2}\right] @f$, where
 *          @f$ f_s @f$ is the sample frequency in @f$ \text{Hz} @f$, and 
 *          @f$ f_c @f$ is the cut-off frequency in @f$ \text{Hz} @f$.
 * @tparam  T
 *          The type of filter values and coefficients.
 * @tparam  Implementation
 *          The BiQuad implementation to use.
 * 
 * @see <https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Discretization/Discretization-of-a-fourth-order-Butterworth-filter.html#discretization-using-second-order-sections-sos>
 */
template <uint8_t N, class T = float, class Implementation = BiQuadFilterDF1<T>>
SOSFilter<T, (N + 1) / 2, Implementation> butter(double f_n) {
    return butter_coeff<N, T>(f_n);
}

/// @}