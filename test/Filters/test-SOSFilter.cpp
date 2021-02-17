#include <gtest/gtest.h>

#include <Filters/IIRFilter.hpp>
#include <Filters/SOSFilter.hpp>

/*
 *  (1 + 2 s⁻¹ + 3 s⁻²) (4 + 5 s⁻1 + 6 s⁻²) 
 * ----------------------------------------  =
 * (-1 - 2 s⁻¹ + 3 s⁻²) (1 - 3 s⁻¹ + 2 s⁻²)
 * 
 * (4 + 13 s⁻¹ + 28 s⁻² + 27 s⁻³ + 28 s⁻⁴)
 * ---------------------------------------
 *  (-1 + 1 s⁻¹ + 7 s⁻² - 13 s⁻³ + 6 s⁻⁴)
 * 
 */
TEST(SOSFilter, SOSFilter) {
    using namespace std;
    IIRFilter<5, 5, int> reference = {{4, 13, 28, 27, 18}, {-1, 1, 7, -13, 6}};
    SOSFilter<int, 2> sos = {{{
        {{1, 2, 3}, {-1, -2, 3}},
        {{4, 5, 6}, {1, -3, 2}},
    }}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = signal;
    transform(signal.begin(), signal.end(), signal.begin(), sos);
    transform(expected.begin(), expected.end(), expected.begin(), reference);
    EXPECT_EQ(signal, expected);
}