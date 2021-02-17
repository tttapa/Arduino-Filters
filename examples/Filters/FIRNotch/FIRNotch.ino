/**
 * Example of 50 Hz (+harmonics) notch filter.
 * 
 * @boards  AVR, AVR USB, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * @see <https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/FIR-Notch.html>
 * 
 * Be careful when selecting a sampling frequency that's a multiple of 50 Hz, as
 * this will alias 50 Hz harmonics to 0 Hz (DC), and this might introduce a more
 * or less constant error to your measurements.  
 * It's best to add an analog anti-aliasing filter as well.
 * 
 * ![Filtered mains power noise signal (blue is unfilterd, red is filtered)](50Hz-notch.png)
 * 
 * Written by PieterP, 2019-11-22  
 * https://github.com/tttapa/Arduino-Filters
 */

#include <Filters.h>

#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Filters/Notch.hpp>

void setup() {
  Serial.begin(1e6);
}

// Sampling frequency
const double f_s = 250; // Hz
// Notch frequency (-∞ dB)
const double f_c = 50; // Hz
// Normalized notch frequency
const double f_n = 2 * f_c / f_s;

// Sample timer
Timer<micros> timer = std::round(1e6 / f_s);

// Very simple Finite Impulse Response notch filter
auto filter1 = simpleNotchFIR(f_n);     // fundamental
auto filter2 = simpleNotchFIR(2 * f_n); // second harmonic

void loop() {
  if (timer) {
    auto raw = analogRead(A0);
    Serial.print(raw);
    Serial.print('\t');
    Serial.println(filter2(filter1(raw)));
  }
}
