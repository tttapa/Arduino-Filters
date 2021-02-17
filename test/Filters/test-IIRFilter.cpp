#include <gtest/gtest.h>

#include <Filters/IIRFilter.hpp>

TEST(IIRFilter, IIRFilterRandomInt) {
    using namespace std;
    IIRFilter<5, 3, int> filter = {{1, 10, 2, -3, -1}, {-1, 2, -3}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = {-100,     -1210,    -2522,   -2177,    2857,
                               12004,    15506,    -4673,   -55360,   -97524,
                               -28437,   235155,   555311,  405266,   -855481,
                               -2926992, -3287961, 2203823, 14271183, 21930362};
    for_each(signal.begin(), signal.end(), [&](int &s) { s = filter(s); });
    EXPECT_EQ(signal, expected);
}

TEST(IIRFilter, IIRFilterRandomDouble) {
    using namespace std;
    IIRFilter<5, 3, double> filter = {{1, 10, 2, -3, -1}, {-1, 2, -3}};
    array<double, 20> signal = {
        100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
        100, -10, 10,  11, 20, 30, 123, 12,  90,  10,
    };
    array<double, 20> expected = {
        -100,    -1210,    -2522,    -2177,   2857,     12004,    15506,
        -4673,   -55360,   -97524,   -28437,  235155,   555311,   405266,
        -855481, -2926992, -3287961, 2203823, 14271183, 21930362,
    };
    for_each(signal.begin(), signal.end(), [&](double &s) { s = filter(s); });
    EXPECT_EQ(signal, expected);
}