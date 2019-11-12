/**
 * Example of Butterworth filter.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Written by PieterP, 2019-11-12
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Filters.h>

#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Filters/Butterworth.hpp>

void setup() {
  Serial.begin(115200);
}

auto filter = butter<6, float>(0.4 * M_PI);
Timer<micros> timer = 10000; // 100 Hz

void loop() {
  if (timer) {
    Serial.println(filter(analogRead(A0)));
  }
}