from scipy.signal import lfilter
import numpy as np

b = np.array((1, 0, 0, 0), dtype=np.float64)
a = np.array((1, ), dtype=np.float64)

signal = np.array((100, 10, 102, 23, 51, 1, -10, -53, 100, -100, 100, -10, 10,
                   11, 20, 30, 123, 12, 90, 10),
                  dtype=np.float64)
output = lfilter(b, a, signal)
print(f'FIRFilter<{len(b)}, int> filter = {{{{')
print(' ', ', '.join(map(lambda x: str(x), np.round(b).astype(np.int))))
print('}};')
print(f'array<int, {len(signal)}> signal = {{')
print(' ', ', '.join(map(lambda x: str(x), np.round(signal).astype(np.int))))
print('};')
print(f'array<int, {len(signal)}> expected = {{')
print(' ', ', '.join(map(lambda x: str(x), np.round(signal).astype(np.int))))
print('};')
print(
    'for_each(signal.begin(), signal.end(), [&](int &s) { s = filter(s); });')
print('EXPECT_EQ(signal, expected);')