#include <gtest/gtest.h>

#include <Filters/FIRFilter.hpp>

TEST(FIRFilter, FIRFilter1) {
    using namespace std;

    FIRFilter<1, int> filter = {{1}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                               100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    for_each(signal.begin(), signal.end(), [&](int &s) { s = filter(s); });
    EXPECT_EQ(signal, expected);
}

TEST(FIRFilter, FIRFilter1000) {
    using namespace std;

    FIRFilter<4, int> filter = {{1, 0, 0, 0}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                               100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    for_each(signal.begin(), signal.end(), [&](int &s) { s = filter(s); });
    EXPECT_EQ(signal, expected);
}

TEST(FIRFilter, FIRFilterRandom) {
    using namespace std;
    FIRFilter<11, int> filter = {{1, 2, 3, -4, -4, 5, 6, 1, 2, 1, -2}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = {100,  210, 422, -143, -37,  224,  295,
                               304,  693, 592, 800,  -163, -113, 180,
                               -362, -15, 776, 320,  288,  70};
    for_each(signal.begin(), signal.end(), [&](int &s) { s = filter(s); });
    EXPECT_EQ(signal, expected);
}