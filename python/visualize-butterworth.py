#!/usr/bin/env python3

"""
Visualization of the filter from the Butterworth.ino example.
"""

from visualization.bode import bode
from scipy.signal import butter

f_s = 100            # Sample frequency Hz
f_c = 25             # Cut-off frequency in Hz
f_n = 2 * f_c / f_s  # Normalized cut-off frequency
order = 6            # Order of the butterworth filter

# Design the digital Butterworth filter
b, a = butter(order, f_n)

# Display the bode plot of the filter
bode(b, a, f_s, magnitude_limits=[-100, 10], phase_limits=[-540, 45])
