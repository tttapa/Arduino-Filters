from bode import bode
from scipy.signal import butter

f_s = 360    # Sample frequency Hz
f_c = 45     # Cut-off frequency in Hz
order = 4    # Order of the butterworth filter

# Design the digital Butterworth filter
b, a = butter(order, 2 * f_c / f_s)

# Display the bode plot of the filter
bode(b, a, f_s, magnitude_limits=[-100, 10], phase_limits=[-360, 45])