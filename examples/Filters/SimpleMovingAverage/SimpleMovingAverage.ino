/**
 * Example of a Simple Moving Average (SMA, also known as Running Average) 
 * filter.
 * 
 * @boards  AVR, AVR USB, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Written by PieterP, 2020-01-07  
 * https://github.com/tttapa/Arduino-Filters
 */

#include <Filters.h>

#include <AH/STL/cmath>                    // std::round
#include <AH/Timing/MillisMicrosTimer.hpp> // Timer
#include <Filters/SMA.hpp>                 // SMA (Simple Moving Average)

void setup() {
  Serial.begin(115200);
}

// Sampling frequency
const double f_s = 100; // Hz

// Sample timer
Timer<micros> timer = std::round(1e6 / f_s);
// Simple Moving Average filter of length 10, initialized with a value of 512.
SMA<10> average = {512};

void loop() {
  if (timer) // returns true once every 10 ms, determines sampling frequency
    Serial.println(average(analogRead(A0)));
}

// By default, SMA uses `uint16_t` as the type for the input and output values,
// and `uint32_t` for the accumulator (the sum of previous inputs).
// This works fine for most use cases, but you can specifically set the types 
// as well. The first type is the type of the in- and outputs, and the second
// type is the type of the accumulator.  
// 
// For example:
//
//   SMA<10, uint32_t, uint64_t>  // for very large inputs (larger than 65,535)
//   SMA<10, int16_t, int32_t>    // for inputs that can go negative
//   SMA<10, float, float>        // for decimal (floating point) values
//   SMA<10, uint16_t, uint16_t>  // for small input values and short averages
//
// Some important things to note:
//  - Unsigned integer types are slightly faster than signed integer types
//  - Integer types are significantly faster than floating point types.
//    Since most sensors and ADCs return an integer, it's often a good idea to
//    filter the raw integer measurement, before converting it to a floating 
//    point number (e.g. voltage or temperature).
//  - The accumulator has to be large enough to fit N times the maximum input 
//    value. If your maximum input value is 1023 (e.g. from analogRead), and if
//    the accumulator type is `uint16_t`, as in the last example above, the 
//    maximum length N of the SMA will be ⌊(2¹⁶ - 1) / 1023⌋ = 64.
//    Alternatively, analogRead returns a 10-bit number, so the maximum length
//    is 2¹⁶⁻¹⁰ = 2⁶ = 64, which is the same result as before.