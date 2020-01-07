from scipy.signal import lfilter
import numpy as np

N = 10
b = np.ones((N,), dtype=np.float32)
a = np.array((N,), dtype=np.float32)

signal = np.array((100, 100, 25, 25, 50, 123, 465, 75, 56, 50, 23, 41), 
                  dtype=np.float32)
output = lfilter(b, a, signal)
print('std::array<float, {}> signal = {{'.format(len(signal)))
for i in signal:
    print(i, end=', ')
print('};')
print('std::array<float, {}> expected = {{'.format(len(signal)))
for o in output:
    print(o, end=', ')
print('};')

signal = np.concatenate((np.array((100,) * N, dtype=np.float32), signal))
output = lfilter(b, a, signal)[N:]
print('std::array<float, {}> expected = {{'.format(len(output)))
for o in output:
    print(o, end=', ')
print('};')