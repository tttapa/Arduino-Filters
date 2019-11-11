from scipy.signal import lfilter
import numpy as np

type = 'int'
b = np.array((1, 10, 2, -3, -1), dtype=np.float64)
a = np.array((-1, 2, -3), dtype=np.float64)

signal = np.array((100, 10, 102, 23, 51, 1, -10, -53, 100, -100, 100, -10, 10,
                   11, 20, 30, 123, 12, 90, 10),
                  dtype=np.float64)
output = lfilter(b, a, signal)
print(f'IIRFilter<{len(b)}, {len(a)}, {type}> filter = {{{{')
print(' ', ', '.join(map(lambda x: str(x), np.round(b).astype(np.int))))
print('},')
print('{')
print(' ', ', '.join(map(lambda x: str(x), np.round(a).astype(np.int))))
print('}};')
print(f'array<{type}, {len(signal)}> signal = {{')
print(' ', ', '.join(map(lambda x: str(x), np.round(signal).astype(np.int))))
print('};')
print(f'array<{type}, {len(signal)}> expected = {{')
print(' ', ', '.join(map(lambda x: str(x), np.round(output).astype(np.int))))
print('};')
print(
    f'for_each(signal.begin(), signal.end(), [&]({type} &s) {{ s = filter(s); }});'
)
print('EXPECT_EQ(signal, expected);')