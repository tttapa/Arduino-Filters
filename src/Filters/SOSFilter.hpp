#pragma once

#include <AH/Containers/ArrayHelpers.hpp>
#include <Filters/BiQuad.hpp>

/// @addtogroup Filters
/// @{

template <class T, size_t N>
using SOSCoefficients = AH::Array<BiQuadCoefficients<T>, N>;

/**
 * @brief   Second Order Sections filter. Higher cost than direct 
 *          implementation, but better numerical stability.
 * 
 * @tparam  T 
 *          The type of the signals and filter coefficients.
 * @tparam  N 
 *          The number of sections.
 */
template <class T, size_t N, class Implementation = BiQuadFilterDF1<T>>
class SOSFilter {
  public:
    /// Constructor.
    SOSFilter(const SOSCoefficients<T, N> &sectionCoefficients)
        : sections(AH::copyAs<Implementation>(sectionCoefficients)) {}

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

template <class T, size_t M, size_t N>
TransferFunction<M + N * 2, M + N * 2, T>
sos2tf_helper(const TransferFunction<M, M, T> &tf,
              AH::ArraySlice<BiQuadCoefficients<T>, N, false, true> sos) {
    auto sub_tf = sos2tf_helper(tf, sos.template slice<0, N - 2>());
    return TransferFunction<M + N * 2, M + N * 2, T>{
        AH::distribute(sub_tf.b, sos[N - 1].b),
        AH::distribute(sub_tf.a, sos[N - 1].a),
    };
}

template <class T, size_t M>
TransferFunction<M + 2, M + 2, T>
sos2tf_helper(const TransferFunction<M, M, T> &tf,
              AH::ArraySlice<BiQuadCoefficients<T>, 1, false, true> sos) {
    return TransferFunction<M + 2, M + 2, T>{
        AH::distribute(tf.b, sos[0].b),
        AH::distribute(tf.a, sos[0].a),
    };
}

/**
 * @brief   Convert Second Order Section (SOS) coefficients to an equivalent 
 *          tranfer function representation.
 * @ingroup    FilterDesign
 */
template <class T, size_t N>
TransferFunction<N * 2 + 1, N * 2 + 1, T>
sos2tf(const SOSCoefficients<T, N> &sos) {
    return sos2tf_helper(sos[N - 1], sos.template slice<0, N - 2>());
}

template <class T>
TransferFunction<3, 3, T> sos2tf(const SOSCoefficients<T, 1> &sos) {
    return sos[0];
}
