#include <gtest-wrapper.h>

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
        auto result = round_div<divisor>(dividends[i]);
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
        auto result = round_div<divisor>(dividends[i]);
        EXPECT_NEAR(result, quotients[i], 0.5);
        cout << i << ": " << dividends[i] << " / " << divisor << " = "
             << quotients[i] << " ≃ " << result << '\n';
    }
}
