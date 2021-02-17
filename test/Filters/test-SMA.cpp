#include <gtest/gtest.h>

#include <AH/Containers/ArrayHelpers.hpp>
#include <Filters/SMA.hpp>
#include <numeric>

TEST(SMA, divRoundSigned) {
    using namespace std;

    AH::Array<int16_t, 65> dividends;
    iota(begin(dividends), end(dividends), -32);

    constexpr uint8_t divisor = 8;

    AH::Array<double, dividends.length> quotients =
        AH::copyAs<double>(dividends) / double(divisor);

    for (size_t i = 0; i < dividends.length; ++i) {
        auto result = AH::round_div<divisor>(dividends[i]);
        EXPECT_NEAR(result, quotients[i], 0.5);
        cout << i << ": " << dividends[i] << " / " << divisor << " = "
             << quotients[i] << " ≃ " << result << '\n';
    }
}

TEST(SMA, divRoundUnsigned) {
    using namespace std;

    AH::Array<int16_t, 65> dividends;
    iota(begin(dividends), end(dividends), -32);

    constexpr uint8_t divisor = 8;

    AH::Array<double, dividends.length> quotients =
        AH::copyAs<double>(dividends) / double(divisor);

    for (size_t i = 0; i < dividends.length; ++i) {
        auto result = AH::round_div<divisor>(dividends[i]);
        EXPECT_NEAR(result, quotients[i], 0.5);
        cout << i << ": " << dividends[i] << " / " << divisor << " = "
             << quotients[i] << " ≃ " << result << '\n';
    }
}

#include <algorithm>

TEST(SMA, smaFloat) {
    SMA<10, float, float> sma;
    std::array<float, 12> signal = {
        100.0, 100.0, 25.0, 25.0, 50.0, 123.0,
        465.0, 75.0,  56.0, 50.0, 23.0, 41.0,
    };
    std::array<float, 12> expected = {
        10.0, 20.0, 22.5,  25.0,  30.0,      42.3,
        88.8, 96.3, 101.9, 106.9, 99.200005, 93.3,
    };
    std::for_each(signal.begin(), signal.end(), [&](float &s) { s = sma(s); });
    // ASSERT_EQ(signal, expected);
    for (size_t i = 0; i < signal.size(); ++i)
        ASSERT_FLOAT_EQ(signal[i], expected[i]);
}

TEST(SMA, smaFloatInitial) {
    SMA<10, float, float> sma = 100.0;
    std::array<float, 12> signal = {
        100.0, 100.0, 25.0, 25.0, 50.0, 123.0,
        465.0, 75.0,  56.0, 50.0, 23.0, 41.0,
    };
    std::array<float, 12> expected = {
        100.0, 100.0, 92.5,  85.0,  80.0, 82.3,
        118.8, 116.3, 111.9, 106.9, 99.2, 93.3,
    };
    std::for_each(signal.begin(), signal.end(), [&](float &s) { s = sma(s); });
    // ASSERT_EQ(signal, expected);
    for (size_t i = 0; i < signal.size(); ++i)
        ASSERT_FLOAT_EQ(signal[i], expected[i]);
}

TEST(SMA, smaUintInitial) {
    SMA<4, uint16_t, uint32_t> sma = 8;
    std::array<uint16_t, 6> signal = {
        8, 4, 8, 4, 0, 16,
    };
    std::array<uint16_t, 6> expected = {
        8, 7, 7, 6, 4, 7,
    };
    std::for_each(signal.begin(), signal.end(),
                  [&](uint16_t &s) { s = sma(s); });
    ASSERT_EQ(signal, expected);
}