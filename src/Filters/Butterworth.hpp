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
 *          Normalized cut-off frequency in half-cycles per sample.  
 *          @f$ f_n = \frac{2 f_c}{f_s} \in \left[0, 1\right] @f$, where
 *          @f$ f_s @f$ is the sample frequency in @f$ \text{Hz} @f$, and 
 *          @f$ f_c @f$ is the cut-off frequency in @f$ \text{Hz} @f$.
 * @param   normalize
 *          If true, normalize all coefficients such that @f$ a_0 = 1 @f$.
 * @tparam  T
 *          The type of the coefficients.
 * 
 * @see <https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Discretization/Discretization-of-a-fourth-order-Butterworth-filter.html#discretization-using-second-order-sections-sos>
 */
template <uint8_t N, class T = float>
SOSCoefficients<T, (N + 1) / 2> butter_coeff(double f_n,
                                             bool normalize = true) {
    const double gamma = 1 / std::tan(M_PI * f_n / 2); // pre-warp factor

    auto make_sos = [=](uint8_t k) {
        const double gamma2 = gamma * gamma;
        const double alpha = 2 * std::cos(2 * M_PI * (2 * k + N + 1) / (4 * N));
        return BiQuadCoefficients<T>{
            {{T(1.), T(2.), T(1.)}}, // b0, b1, b2
            {{
                T(gamma2 - alpha * gamma + 1), // a0
                T(2 * (1 - gamma2)),           // a1
                T(gamma2 + alpha * gamma + 1), // a2
            }},
        };
    };
    auto make_fos = [=]() {
        return BiQuadCoefficients<T>{
            {{T(1.), T(1.)}}, // b0, b1
            {{
                T(gamma + 1), // a0
                T(1 - gamma), // a1
            }},
        };
    };

    auto make_sos_norm = [=](uint8_t k) {
        const double gamma2 = gamma * gamma;
        const double alpha = 2 * std::cos(2 * M_PI * (2 * k + N + 1) / (4 * N));
        const double a0 = gamma2 - alpha * gamma + 1;
        return BiQuadCoefficients<T>{
            {{T(1. / a0), T(2. / a0), T(1. / a0)}}, // b0, b1, b2
            {{
                T(1.),                                // a0
                T(2 * (1 - gamma2) / a0),             // a1
                T((gamma2 + alpha * gamma + 1) / a0), // a2
            }},
        };
    };
    auto make_fos_norm = [=]() {
        const double a0 = gamma + 1;
        return BiQuadCoefficients<T>{
            {{T(1. / a0), T(1. / a0)}}, // b0, b1
            {{
                T(1.),               // a0
                T((1 - gamma) / a0), // a1
            }},
        };
    };

    SOSCoefficients<T, (N + 1) / 2> sections;

    if (N % 2 == 0) {
        for (uint8_t i = 0; i < sections.length; ++i)
            sections[i] = normalize ? make_sos_norm(i) : make_sos(i);
    } else {
        for (uint8_t i = 0; i < sections.length - 1; ++i)
            sections[i] = normalize ? make_sos_norm(i) : make_sos(i);
        sections.end()[-1] = normalize ? make_fos_norm() : make_fos();
    }
    return sections;
}

/** 
 * @brief   Create a Butterworth filter, implemented as Second Order Sections
 *          (SOS) filter.
 * @tparam  N
 *          Order of the filter.
 * @param   f_n
 *          Normalized cut-off frequency in half-cycles per sample.  
 *          @f$ f_n = \frac{2 f_c}{f_s} \in \left[0, 1\right] @f$, where
 *          @f$ f_s @f$ is the sample frequency in @f$ \text{Hz} @f$, and 
 *          @f$ f_c @f$ is the cut-off frequency in @f$ \text{Hz} @f$.
 * @tparam  T
 *          The type of filter values and coefficients.
 * @param   normalize
 *          If true, normalize all coefficients such that @f$ a_0 = 1 @f$.
 * @tparam  Implementation
 *          The BiQuad implementation to use.
 * 
 * @see <https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Discretization/Discretization-of-a-fourth-order-Butterworth-filter.html#discretization-using-second-order-sections-sos>
 */
template <uint8_t N, class T = float, class Implementation = BiQuadFilterDF1<T>>
SOSFilter<T, (N + 1) / 2, Implementation> butter(double f_n,
                                                 bool normalize = true) {
    return butter_coeff<N, T>(f_n, normalize);
}

/// @}