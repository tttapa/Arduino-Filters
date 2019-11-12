#pragma once

#include <AH/STL/cmath>
#include <Filters/SOSFilter.hpp>

/// @addtogroup Filters
/// @{

/** 
 * @brief   Create a butterworth filter.
 * @tparam  N
 *          Order of the filter.
 * @param   omega
 *          Normalized angular cut-off frequency.
 *          @f$ \omega \in \left(0, \pi\right] @f$
 *
 * @todo    Optimize omega and K
 */
template <uint8_t N, class T = double,
          class Implementation = BiQuadFilterDF1<T>>
SOSFilter<T, (N + 1) / 2, Implementation> butter(double omega) {
    T K = omega / std::tan(omega / 2);
    auto make_sos = [=](uint8_t i) {
        T b0 = 0;
        T b1 = 0;
        T b2 = 1;
        T a0 = 1 / (omega * omega);
        T a1 = -2 * std::cos(2 * M_PI * (2 * i + N + 1) / (4 * N)) / omega;
        T a2 = 1;
        T denom = a0 * K * K + a1 * K + a2;
        return Implementation{
            {{
                (b0 * K * K + b1 * K + b2) / denom,
                (2 * b2 - 2 * b0 * K * K) / denom,
                (b0 * K * K - b1 * K + b2) / denom,
            }},
            {{
                (a0 * K * K + a1 * K + a2) / denom,
                (2 * a2 - 2 * a0 * K * K) / denom,
                (a0 * K * K - a1 * K + a2) / denom,
            }},
        };
    };

    auto make_fos = [=]() {
        T b0 = 0;
        T b1 = 1;
        T a0 = 1 / omega;
        T a1 = 1;
        T denom = a0 * K + a1;
        return Implementation{
            {{
                (b0 * K + b1) / denom,
                (-b0 * K + b1) / denom,
                0,
            }},
            {{
                (a0 * K + a1) / denom,
                (-a0 * K + a1) / denom,
                0,
            }},
        };
    };

    AH::Array<Implementation, (N + 1) / 2> sections;

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

/// @}