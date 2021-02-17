#include <gtest/gtest.h>

#include <Filters/BiQuad.hpp>
#include <Filters/IIRFilter.hpp>

TEST(BiQuad, BiQuadDF1RandomInt) {
    using namespace std;
    IIRFilter<3, 3, int> reference = {{1, 10, -2}, {-1, 2, -3}};
    BiQuadFilterDF1<int> biquad = {{1, 10, -2}, {-1, 2, -3}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = signal;
    transform(signal.begin(), signal.end(), signal.begin(), biquad);
    transform(expected.begin(), expected.end(), expected.begin(), reference);
    EXPECT_EQ(signal, expected);
}

TEST(BiQuad, BiQuadDF1RandomFloat) {
    using namespace std;
    IIRFilter<3, 3, float> reference = {{1, 10, -2}, {-1, 2, -3}};
    BiQuadFilterDF1<float> biquad = {{1, 10, -2}, {-1, 2, -3}};
    array<float, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                               100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<float, 20> expected = signal;
    transform(signal.begin(), signal.end(), signal.begin(), biquad);
    transform(expected.begin(), expected.end(), expected.begin(), reference);
    EXPECT_EQ(signal, expected);
}

TEST(BiQuad, BiQuadDF2RandomInt) {
    using namespace std;
    IIRFilter<3, 3, int> reference = {{1, 10, -2}, {-1, 2, -3}};
    BiQuadFilterDF2<int> biquad = {{1, 10, -2}, {-1, 2, -3}};
    array<int, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                             100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<int, 20> expected = signal;
    transform(signal.begin(), signal.end(), signal.begin(), biquad);
    transform(expected.begin(), expected.end(), expected.begin(), reference);
    EXPECT_EQ(signal, expected);
}

TEST(BiQuad, BiQuadDF2RandomFloat) {
    using namespace std;
    IIRFilter<3, 3, float> reference = {{1, 10, -2}, {-1, 2, -3}};
    BiQuadFilterDF2<float> biquad = {{1, 10, -2}, {-1, 2, -3}};
    array<float, 20> signal = {100, 10,  102, 23, 51, 1,  -10, -53, 100, -100,
                               100, -10, 10,  11, 20, 30, 123, 12,  90,  10};
    array<float, 20> expected = signal;
    transform(signal.begin(), signal.end(), signal.begin(), biquad);
    transform(expected.begin(), expected.end(), expected.begin(), reference);
    EXPECT_EQ(signal, expected);
}