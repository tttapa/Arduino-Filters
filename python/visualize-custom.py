from bode import bode
from math import pi, cos

f_s = 360    # Sample frequency Hz

f_s = 360    # Sample frequency in Hz
f_c = 50     # Notch frequency in Hz

omega_c = 2 * pi * f_c       # Notch angular frequency
omega_c_d = omega_c / f_s    # Normalized notch frequency (digital)

h0 = 2 - 2 * cos(omega_c_d) # DC gain

b = [1 / h0, -2 * cos(omega_c_d) / h0, 1 / h0]   # Calculate coefficients
a = [1]

# Display the bode plot of the filter
bode(b, a, f_s, magnitude_limits=[-60, 20], phase_limits=[-90, 180])