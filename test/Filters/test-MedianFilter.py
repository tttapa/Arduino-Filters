from scipy.signal import medfilt
import numpy as np

signal = np.array((100, 100, 25, 25, 50, 123, 465, 75, 56, 50, 23, 41), 
                  dtype=np.float32)
prepend = np.ones((5,), dtype=np.float32) * 3.14
output = medfilt(np.concatenate((prepend, signal)), 5)
print('std::array<float, {}> signal = {{'.format(len(signal)))
for i in signal:
    print(i, end=', ')
print('};')
print('std::array<float, {}> expected = {{'.format(len(signal)))
for o in output[3:-2]:
    print(o, end=', ')
print('};')