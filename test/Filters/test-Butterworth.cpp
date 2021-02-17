#include <gtest/gtest.h>

#include <Filters/Butterworth.hpp>
#include <Filters/IIRFilter.hpp>

#include <iomanip>

TEST(Butterworth, evenOrder) {
    using namespace std;

    auto butterworth = butter<6, double>(0.4);

    array<double, 20> signal = {100.0, 10.0,  102.0,  23.0,  51.0,  1.0,  -10.0,
                                -53.0, 100.0, -100.0, 100.0, -10.0, 10.0, 11.0,
                                20.0,  30.0,  123.0,  12.0,  90.0,  10.0};
    array<double, 20> expected = {
        1.0312874762664406,  7.5156113134959055,  24.719492782414466,
        48.96419197331465,   65.94168547692809,   64.64286189231214,
        47.611566793390764,  24.459114479787313,  2.30670651957257,
        -12.501024944555455, -12.471303690929604, 1.1900356191047718,
        16.094778615441065,  22.424208472570697,  19.737752153200365,
        13.049801559138237,  11.18792332879074,   23.200472458577526,
        48.392807471527775,  71.09457974808586};
    transform(signal.begin(), signal.end(), signal.begin(), butterworth);
    for (size_t i = 0; i < signal.size(); ++i)
        EXPECT_FLOAT_EQ(signal[i], expected[i]) << "at index " << i;
}

TEST(Butterworth, oddOrder) {
    using namespace std;

    auto butterworth = butter<7, double>(0.4);

    array<double, 20> signal = {100.0, 10.0,  102.0,  23.0,  51.0,  1.0,  -10.0,
                                -53.0, 100.0, -100.0, 100.0, -10.0, 10.0, 11.0,
                                20.0,  30.0,  123.0,  12.0,  90.0,  10.0};
    array<double, 20> expected = {
        0.48421152393466566, 4.110607277163259,   15.90100516824798,
        37.248782223463,     59.24082887044678,   67.86896938263345,
        57.57371920434696,   35.23288182688867,   11.224568208632098,
        -7.1755802352153415, -14.039847708901476, -6.495413585543419,
        9.106139776920436,   20.900070928261584,  22.518746478860155,
        16.559558467508843,  11.188799404184095,  16.064770949249294,
        35.58427055190113,   61.332940440798545};
    transform(signal.begin(), signal.end(), signal.begin(), butterworth);
    for (size_t i = 0; i < signal.size(); ++i)
        EXPECT_FLOAT_EQ(signal[i], expected[i]) << "at index " << i;
}

TEST(Butterworth, evenOrderNonNormalized) {
    using namespace std;

    auto butterworth = butter<6, double>(0.4, false);

    array<double, 20> signal = {100.0, 10.0,  102.0,  23.0,  51.0,  1.0,  -10.0,
                                -53.0, 100.0, -100.0, 100.0, -10.0, 10.0, 11.0,
                                20.0,  30.0,  123.0,  12.0,  90.0,  10.0};
    array<double, 20> expected = {
        1.0312874762664406,  7.5156113134959055,  24.719492782414466,
        48.96419197331465,   65.94168547692809,   64.64286189231214,
        47.611566793390764,  24.459114479787313,  2.30670651957257,
        -12.501024944555455, -12.471303690929604, 1.1900356191047718,
        16.094778615441065,  22.424208472570697,  19.737752153200365,
        13.049801559138237,  11.18792332879074,   23.200472458577526,
        48.392807471527775,  71.09457974808586};
    transform(signal.begin(), signal.end(), signal.begin(), butterworth);
    for (size_t i = 0; i < signal.size(); ++i)
        EXPECT_FLOAT_EQ(signal[i], expected[i]) << "at index " << i;
}

TEST(Butterworth, oddOrderNonNormalized) {
    using namespace std;

    auto butterworth = butter<7, double>(0.4, false);

    array<double, 20> signal = {100.0, 10.0,  102.0,  23.0,  51.0,  1.0,  -10.0,
                                -53.0, 100.0, -100.0, 100.0, -10.0, 10.0, 11.0,
                                20.0,  30.0,  123.0,  12.0,  90.0,  10.0};
    array<double, 20> expected = {
        0.48421152393466566, 4.110607277163259,   15.90100516824798,
        37.248782223463,     59.24082887044678,   67.86896938263345,
        57.57371920434696,   35.23288182688867,   11.224568208632098,
        -7.1755802352153415, -14.039847708901476, -6.495413585543419,
        9.106139776920436,   20.900070928261584,  22.518746478860155,
        16.559558467508843,  11.188799404184095,  16.064770949249294,
        35.58427055190113,   61.332940440798545};
    transform(signal.begin(), signal.end(), signal.begin(), butterworth);
    for (size_t i = 0; i < signal.size(); ++i)
        EXPECT_FLOAT_EQ(signal[i], expected[i]) << "at index " << i;
}

/* 
 * https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Discretization/Discretization-of-a-fourth-order-Butterworth-filter.html#discretizing-the-analog-filter
 */
TEST(Butterworth, PietersPages) {
    using namespace std;

    auto butterworth = butter<4, double>(45. / 360.);

    double alpha = -2 * cos(M_PI * 5 / 8);
    double beta = -2 * cos(M_PI * 7 / 8);
    double gamma = 1. / tan(M_PI * 45. / 360. / 2);
    IIRFilter<5, 5, double> reference = {
        {{1, 4, 6, 4, 1}},
        {{
            pow(gamma, 4) + pow(gamma, 3) * (alpha + beta) +
                pow(gamma, 2) * (alpha * beta + 2) + gamma * (alpha + beta) + 1,

            -4 * pow(gamma, 4) - 2 * pow(gamma, 3) * (alpha + beta) +
                2 * gamma * (alpha + beta) + 4,
            6 * pow(gamma, 4) - 2 * pow(gamma, 2) * (alpha * beta + 2) + 6,

            -4 * pow(gamma, 4) + 2 * pow(gamma, 3) * (alpha + beta) -
                2 * gamma * (alpha + beta) + 4,

            pow(gamma, 4) - pow(gamma, 3) * (alpha + beta) +
                pow(gamma, 2) * (alpha * beta + 2) - gamma * (alpha + beta) + 1,
        }}};
    array<double, 20> signal = {100.0, 10.0,  102.0,  23.0,  51.0,  1.0,  -10.0,
                                -53.0, 100.0, -100.0, 100.0, -10.0, 10.0, 11.0,
                                20.0,  30.0,  123.0,  12.0,  90.0,  10.0};
    array<double, 20> expected = signal;
    transform(signal.begin(), signal.end(), signal.begin(), butterworth);
    transform(expected.begin(), expected.end(), expected.begin(), reference);
    for (size_t i = 0; i < signal.size(); ++i)
        EXPECT_FLOAT_EQ(signal[i], expected[i]) << "at index " << i;
}

TEST(Butterworth, sos2tf) {
    using namespace std;

    auto sos = butter_coeff<7, double>(0.4);
    auto tf = sos2tf(sos);
    auto butterworth = makeIIRFilter(tf);

    array<double, 20> signal = {100.0, 10.0,  102.0,  23.0,  51.0,  1.0,  -10.0,
                                -53.0, 100.0, -100.0, 100.0, -10.0, 10.0, 11.0,
                                20.0,  30.0,  123.0,  12.0,  90.0,  10.0};
    array<double, 20> expected = {
        0.48421152393466566, 4.110607277163259,   15.90100516824798,
        37.248782223463,     59.24082887044678,   67.86896938263345,
        57.57371920434696,   35.23288182688867,   11.224568208632098,
        -7.1755802352153415, -14.039847708901476, -6.495413585543419,
        9.106139776920436,   20.900070928261584,  22.518746478860155,
        16.559558467508843,  11.188799404184095,  16.064770949249294,
        35.58427055190113,   61.332940440798545};
    transform(signal.begin(), signal.end(), signal.begin(), butterworth);
    for (size_t i = 0; i < signal.size(); ++i)
        EXPECT_FLOAT_EQ(signal[i], expected[i]) << "at index " << i;
}