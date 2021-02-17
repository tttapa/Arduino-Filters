#include <gtest/gtest.h>

#include <Filters/FixedPoint.hpp>

#include <Filters/Butterworth.hpp>
#include <random>

TEST(FixedPoint, butter32) {
    using namespace std;

    constexpr uint8_t NBits = 24;
    using fp = FixedPoint<int32_t, NBits>;

    constexpr uint8_t N = 32;
    auto f_n = 0.4;
    auto filter = butter<N, fp>(f_n);
    auto reference = butter<N, double>(f_n);

    mt19937 gen(1); // Standard mersenne_twister_engine seeded with 1
    uniform_int_distribution<> dis(0, (1 << NBits) - 1);

    AH::Array<fp, N * 4> signal;
    generate(begin(signal), end(signal), [&]() { return fp::raw(dis(gen)); });
    AH::Array<double, N * 4> ref_signal = AH::copyAs<double>(signal);

    transform(begin(signal), end(signal), begin(signal), filter);
    transform(begin(ref_signal), end(ref_signal), begin(ref_signal), reference);

    // for (size_t i = 0; i < signal.length; ++i)
    //     cout << signal[i] << '\t' << ref_signal[i] << '\t'
    //          << (double(signal[i]) - ref_signal[i]) << '\n';

    for (size_t i = 0; i < signal.length; ++i)
        EXPECT_NEAR(double(signal[i]), ref_signal[i], 5 * pow(2, -NBits));
}

TEST(FixedPoint, butter16) {
    using namespace std;

    constexpr uint8_t NBits = 13;
    using fp = FixedPoint<int16_t, NBits>;

    constexpr uint8_t N = 16;
    auto f_n = 0.4;
    auto filter = butter<N, fp>(f_n);
    auto reference = butter<N, float>(f_n);

    mt19937 gen(1); // Standard mersenne_twister_engine seeded with 1
    uniform_int_distribution<> dis(0, (1 << NBits) - 1);

    AH::Array<fp, N * 4> signal;
    generate(begin(signal), end(signal), [&]() { return fp::raw(dis(gen)); });
    AH::Array<float, N * 4> ref_signal = AH::copyAs<float>(signal);

    transform(begin(signal), end(signal), begin(signal), filter);
    transform(begin(ref_signal), end(ref_signal), begin(ref_signal), reference);

    // for (size_t i = 0; i < signal.length; ++i)
    //     cout << signal[i] << '\t' << ref_signal[i] << '\t'
    //          << (float(signal[i]) - ref_signal[i]) << '\n';

    for (size_t i = 0; i < signal.length; ++i)
        EXPECT_NEAR(float(signal[i]), ref_signal[i], 5 * pow(2, -NBits));
}

TEST(FixedPoint, butter8) {
    using namespace std;

    constexpr uint8_t NBits = 6;
    using fp = FixedPoint<int8_t, NBits>;

    constexpr uint8_t N = 12;
    auto f_n = 0.4;
    auto filter = butter<N, fp>(f_n);
    auto reference = butter<N, float>(f_n);

    mt19937 gen(1); // Standard mersenne_twister_engine seeded with 1
    uniform_int_distribution<> dis(0, (1 << NBits) - 1);

    AH::Array<fp, N * 4> signal;
    generate(begin(signal), end(signal), [&]() { return fp::raw(dis(gen)); });
    AH::Array<float, N * 4> ref_signal = AH::copyAs<float>(signal);

    transform(begin(signal), end(signal), begin(signal), filter);
    transform(begin(ref_signal), end(ref_signal), begin(ref_signal), reference);

    // for (size_t i = 0; i < signal.length; ++i)
    //     cout << signal[i] << '\t' << ref_signal[i] << '\t'
    //          << (float(signal[i]) - ref_signal[i]) << '\n';

    for (size_t i = 0; i < signal.length; ++i)
        EXPECT_NEAR(float(signal[i]), ref_signal[i], 5 * pow(2, -NBits));
}

TEST(FixedPoint, divide) {
    using namespace std;

    constexpr uint8_t NBits = 2;
    using fp = FixedPoint<int8_t, NBits>;

    AH::Array<fp, 32> dividends;
    generate(begin(dividends), end(dividends),
             [i = 0]() mutable { return fp::raw(i++); });
    fp divisor = 5;

    AH::Array<double, dividends.length> quotients =
        AH::copyAs<double>(dividends) / double(divisor);

    for (size_t i = 0; i < dividends.length; ++i) {
        EXPECT_NEAR(double(dividends[i] / divisor), quotients[i], pow(2., -2));
        // cout << (quotients[i] - double(dividends[i] / divisor)) << '\n';
    }
}

TEST(FixedPoint, mult) {
    using namespace std;

    constexpr uint8_t NBits = 6;
    using fp = FixedPoint<int8_t, NBits>;

    AH::Array<fp, 65> input;
    generate(begin(input), end(input),
             [i = 0]() mutable { return fp::raw(i++); });
    fp fac = fp::raw(12);

    AH::Array<double, input.length> expected =
        AH::copyAs<double>(input) * double(fac);

    for (size_t i = 0; i < input.length; ++i) {
        EXPECT_NEAR(double(input[i] * fac), expected[i], pow(2., -6));
        // cout << (expected[i] - double(input[i] * fac)) << '\n';
    }
}

TEST(FixedPoint, multNormalInt) {
    using namespace std;

    constexpr uint8_t NBits = 30;
    using fp = FixedPoint<uint32_t, NBits>;

    double ffac = 2.1;
    fp fac = ffac;

    uint32_t integer = 1'000'000'000;

    EXPECT_NEAR(double(integer * fac), integer *ffac, pow(2., -6));
    EXPECT_NEAR(double(fac * integer), integer *ffac, pow(2., -6));
}

TEST(FixedPoint, add) {
    using namespace std;

    constexpr uint8_t NBits = 6;
    using fp = FixedPoint<int8_t, NBits>;

    AH::Array<fp, 65> input;
    generate(begin(input), end(input),
             [i = 0]() mutable { return fp::raw(i++); });
    fp add = fp::raw(13);

    AH::Array<double, input.length> expected =
        AH::copyAs<double>(input) +
        AH::fillArray<double, input.length>(double(add));

    for (size_t i = 0; i < input.length; ++i) {
        EXPECT_FLOAT_EQ(double(input[i] + add), expected[i]);
        // cout << (expected[i] - double(input[i] + add)) << '\n';
    }
}

TEST(FixedPoint, sub) {
    using namespace std;

    constexpr uint8_t NBits = 6;
    using fp = FixedPoint<int8_t, NBits>;

    AH::Array<fp, 65> input;
    generate(begin(input), end(input),
             [i = 0]() mutable { return fp::raw(i++); });
    fp sub = fp::raw(13);

    AH::Array<double, input.length> expected =
        AH::copyAs<double>(input) -
        AH::fillArray<double, input.length>(double(sub));

    for (size_t i = 0; i < input.length; ++i) {
        EXPECT_FLOAT_EQ(double(input[i] - sub), expected[i]);
        // cout << (expected[i] - double(input[i] - sub)) << '\n';
    }
}