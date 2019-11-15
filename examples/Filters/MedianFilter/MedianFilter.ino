/**
 * Example of median filter.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Written by PieterP, 2019-11-16
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Filters.h>

#include <AH/STL/cmath>
#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Filters/MedianFilter.hpp>

void setup() {
  Serial.begin(115200);
}

// Sampling frequency
const double f_s = 100; // Hz

// Sample timer
Timer<micros> timer = std::round(1e6 / f_s);
// Median filter of length 10, initialized with a value of 512.
MedianFilter<10, uint16_t> medfilt = {512};

void loop() {
  if (timer)
    Serial.println(medfilt(analogRead(A0)));
}