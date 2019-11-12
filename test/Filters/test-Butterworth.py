from scipy.signal import lfilter, butter
import numpy as np

type = 'double'

b, a = butter(7, 0.4)

signal = np.array((100, 10, 102, 23, 51, 1, -10, -53, 100, -100, 100, -10, 10,
                   11, 20, 30, 123, 12, 90, 10),
                  dtype=np.float64)
output = lfilter(b, a, signal)
print(f'array<{type}, {len(signal)}> signal = {{')
print(' ', ', '.join(map(lambda x: str(x), signal)))
print('};')
print(f'array<{type}, {len(signal)}> expected = {{')
print(' ', ', '.join(map(lambda x: str(x), output)))
print('};')
print(f'transform(signal.begin(), signal.end(), signal.begin(), butterworth);')
print('EXPECT_EQ(signal, expected);')