#pragma once

#include <Filters/BiQuad.hpp>

/// @addtogroup Filters
/// @{

template <class T, uint8_t N, class Implementation = BiQuadFilterDF1<T>>
class SOSFilter {
  public:
    SOSFilter(const AH::Array<Implementation, N> &sections)
        : sections{sections} {}

    /**
     * @brief   Update the internal state with the new input @f$ x[n] @f$ and
     *          return the new output @f$ y[n] @f$.
     * 
     * @param   input 
     *          The new input @f$ x[n] @f$.
     * @return  The new output @f$ y[n] @f$.
     */
    T operator()(T input) {
        for (auto &section : sections)
            input = section(input);
        return input;
    }

  private:
    AH::Array<Implementation, N> sections;
};

/// @}