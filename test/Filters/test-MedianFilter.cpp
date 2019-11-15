#include <gtest/gtest.h>

#include <Filters/MedianFilter.hpp>

#include <algorithm>

TEST(MedianFilter, med) {
    MedianFilter<5> med = 3.14;
    std::array<float, 12> signal = {
        100.0, 100.0, 25.0, 25.0, 50.0, 123.0,
        465.0, 75.0,  56.0, 50.0, 23.0, 41.0,
    };
    std::array<float, 12> expected = {
        3.140000104904175,
        3.140000104904175,
        25.0,
        25.0,
        50.0,
        50.0,
        50.0,
        75.0,
        75.0,
        75.0,
        56.0,
        50.0,
    };
    std::transform(signal.begin(), signal.end(), signal.begin(), med);
    // ASSERT_EQ(signal, expected);
    for (size_t i = 0; i < signal.size(); ++i)
        ASSERT_FLOAT_EQ(signal[i], expected[i]);
}