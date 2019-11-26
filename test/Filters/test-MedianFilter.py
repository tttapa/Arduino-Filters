import numpy as np

def medfilt(signal, N):
    result = np.empty((len(signal)- N + 1,))
    for i in range(len(result)):
        result[i] = np.median(signal[i:i+N])
    return result 

N = 6
signal = np.array((100, 100, 25, 25, 50, 123, 465, 75, 56, 50, 23, 41), 
                  dtype=np.float32)
prepend = np.ones((N-1,), dtype=np.float32) * 3.14
extended = np.concatenate((prepend, signal))
output = medfilt(extended, N)
print('std::array<float, {}> signal = {{'.format(len(signal)))
for i in signal:
    print(i, end=', ')
print('};')
print('std::array<float, {}> expected = {{'.format(len(signal)))
for o in output:
    print(o, end=', ')
print('};')

N = 5
signal = np.array((100, 100, 25, 25, 50, 123, 465, 75, 56, 50, 23, 41), 
                  dtype=np.float32)
prepend = np.ones((N-1,), dtype=np.float32) * 3.14
extended = np.concatenate((prepend, signal))
output = medfilt(extended, N)
print('std::array<float, {}> signal = {{'.format(len(signal)))
for i in signal:
    print(i, end=', ')
print('};')
print('std::array<float, {}> expected = {{'.format(len(signal)))
for o in output:
    print(o, end=', ')
print('};')