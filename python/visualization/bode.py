from scipy.signal import freqz
import matplotlib.pyplot as plt
from math import pi
import numpy as np

def bode(b, a, f_s, magnitude_limits = None, phase_limits = None):
    w, H = freqz(b, a, 4096)                  # Calculate the frequency response
    w *= f_s / (2 * pi)                       # Convert from rad/sample to Hz

    # Plot the amplitude response
    plt.subplot(2, 1, 1)            
    plt.suptitle('Bode Plot')
    H_dB = 20 * np.log10(abs(H))              # Convert modulus of H to dB
    plt.plot(w, H_dB)
    plt.ylabel('Magnitude [dB]')
    plt.xlim(0, f_s / 2)
    if (magnitude_limits is not None):
        plt.ylim(*magnitude_limits)
    plt.axhline(0, linewidth=0.8, color='black', linestyle=':')

    # Plot the phase response
    plt.subplot(2, 1, 2)
    phi = np.angle(H)                         # Argument of H
    phi = np.unwrap(phi)                      # Remove discontinuities 
    phi *= 180 / pi                           # and convert to degrees
    plt.plot(w, phi)
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Phase [°]')
    plt.xlim(0, f_s / 2)
    if (phase_limits is not None):
        plt.ylim(*phase_limits)
    plt.axhline(0, linewidth=0.8, color='black', linestyle=':')

    plt.show()