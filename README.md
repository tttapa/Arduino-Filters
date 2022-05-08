[![Build Status](https://github.com/tttapa/Arduino-Filters/workflows/CI%20Tests/badge.svg#)](https://github.com/tttapa/Arduino-Filters/actions)
[![Test Coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/tttapa/Arduino-Filters/gh-pages/Coverage/shield.io.coverage.json)](https://tttapa.github.io/Arduino-Filters/Coverage/index.html)
[![GitHub](https://img.shields.io/github/stars/tttapa/Arduino-Filters?label=GitHub&logo=github)](https://github.com/tttapa/Arduino-Filters)

# Arduino Filters

## Features

- **Infinite Impulse Response Filters**
- **Finite Impulse Response Filters**
- **BiQuad Filters**
- **Butterworth Filters**
- **Notch Filters**
- **Median Filters**
- **Simple and Exponential Moving Average Filters**
- **Hysteresis**

These filters were originally part of the
[old Filters library](https://github.com/tttapa/Filters).  
It has been updated completely, with continuous integration, unit tests, etc.

## Documentation

The automatically generated Doxygen documentation for this library can be found 
here:  
[**Documentation**](https://tttapa.github.io/Arduino-Filters/Doxygen/index.html)  
Test coverage information can be found here:  
[**Code Coverage**](https://tttapa.github.io/Arduino-Filters/Coverage/index.html)  
Arduino examples can be found here:  
[**Examples**](https://tttapa.github.io/Arduino-Filters/Doxygen/examples.html)

You might want to start with the 
[**Filters Module**](https://tttapa.github.io/Arduino-Filters/Doxygen/d2/d5a/group__Filters.html).

## Example: Butterworth filter

```cpp
#include <Filters.h>

#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Filters/Butterworth.hpp>

void setup() {
  Serial.begin(115200);
}

// Sampling frequency
const double f_s = 100; // Hz
// Cut-off frequency (-3 dB)
const double f_c = 40; // Hz
// Normalized cut-off frequency
const double f_n = 2 * f_c / f_s;

// Sample timer
Timer<micros> timer = std::round(1e6 / f_s);

// Sixth-order Butterworth filter
auto filter = butter<6>(f_n);

void loop() {
  if (timer)
    Serial.println(filter(analogRead(A0)));
}
```

## Tools

The [`python`](python) folder contains some Python scripts to visualize the
frequency response of the filters used in the examples.  

### Butterworth Filter
![Butterworth.ino](python/butterworth.svg)

### FIR Notch Filter
![FIRNotch.ino](python/firnotch.svg)

## Related Projects

This library uses the
[**Arduino Helpers utility library**](https://github.com/tttapa/Arduino-Helpers)

## Supported boards

For each commit, the continuous integration tests compile the examples for the
following boards:

- Arduino UNO
- Arduino Mega
- Arduino Leonardo
- Teensy 3.2
- Teensy 4.1
- Arduino Due
- Arduino Nano 33 IoT
- Arduino Nano 33 BLE
- Arduino Nano Every
- ESP8266
- ESP32

This covers a very large part of the Arduino platform, and similar boards will
also work (e.g. Arduino Nano, Arduino Mega, etc.).

If you have a board that's not supported, please 
[open an issue](https://github.com/tttapa/Arduino-Helpers/issues/new)
and let me know!

### Known limitations

The Arduino Due toolchain provided by Arduino is very old, and has some 
configuration problems. As a result, some math functions are not available.  
There is nothing I can do about it in this library, it's a bug in the Arduino 
Due Core.
