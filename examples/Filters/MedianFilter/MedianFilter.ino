/**
 * Example of median filter.
 * 
 * @boards  AVR, AVR USB, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Written by PieterP, 2019-11-16  
 * https://github.com/tttapa/Arduino-Filters
 */

#include <Filters.h>

#include <AH/STL/cmath>                    // std::round
#include <AH/Timing/MillisMicrosTimer.hpp> // Timer
#include <Filters/MedianFilter.hpp>        // MedianFilter

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
  if (timer) // returns true once every 10 ms, determines sampling frequency
    Serial.println(medfilt(analogRead(A0)));
}