/**
 * Performance test for different filters.
 * 
 * @boards  AVR, AVR USB, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Written by PieterP, 2019-11-26  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Filters.h>

#include <AH/STL/cmath>
#include <AH/STL/algorithm>
#include <AH/STL/array>
#include <AH/Timing/MillisMicrosTimer.hpp>
#include <Filters/SMA.hpp>
#include <Filters/MedianFilter.hpp>
#include <AH/Filters/EMA.hpp>

constexpr uint8_t data_length = 100;
constexpr unsigned int iterations = 100;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  std::array<uint16_t, data_length> data;
  std::generate(std::begin(data), std::end(data), 
                []{ return analogRead(A0); });
  {
    Serial << F("EMA (K = 4)") << endl;
    EMA<4, uint16_t> ema;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = ema(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("EMA (K = 6)") << endl;
    EMA<6, uint16_t> ema;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = ema(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("EMA (K = 8)") << endl;
    EMA<8, uint16_t> ema;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = ema(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("SMA (N = 16)") << endl;
    SMA<16, uint16_t, uint16_t> sma;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = sma(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("SMA (N = 10)") << endl;
    SMA<10, uint16_t, uint16_t> sma;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = sma(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("SMA (N = 100)") << endl;
    SMA<100, uint16_t, uint16_t> sma;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = sma(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("Median even (N = 10)") << endl;
    MedianFilter<10, uint16_t> med;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = med(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("Median odd (N = 11)") << endl;
    MedianFilter<11, uint16_t> med;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = med(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("Median even (N = 100)") << endl;
    MedianFilter<100, uint16_t> med;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = med(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
  {
    Serial << F("Median odd (N = 101)") << endl;
    MedianFilter<101, uint16_t> med;
    auto startTime = micros();
    
    for (unsigned int i = 0; i < iterations; ++i)
      for (uint8_t j = 0; j < data_length; ++j)
        data[j] = med(data[j]);

    auto stopTime = micros();
    Serial << (1e6 * data_length * iterations) / (stopTime - startTime) 
           << F(" filter operations per second") << endl << endl;
  }
}

void loop() {}

/*

EMA (K = 4)
358731.53 filter operations per second

EMA (K = 6)
297867.28 filter operations per second

EMA (K = 8)
657375.75 filter operations per second

SMA (N = 16)
162951.37 filter operations per second

SMA (N = 10)
51198.03 filter operations per second

SMA (N = 100)
53278.78 filter operations per second

Median even (N = 10)
16338.37 filter operations per second

Median odd (N = 11)
17778.92 filter operations per second

Median even (N = 100)
2162.23 filter operations per second

Median odd (N = 101)
2658.79 filter operations per second

Observations:

The EMA is much faster than the EMA, even if N is a power of two.

If K = 4, the EMA is very fast, because the AVR instruction set has an 
instruction to swap nibbles in a byte.  
If K = 8, the EMA is even faster, because this just corresponds to removing the 
least significant byte.  
Otherwise, multiple bit shift instructions are used, which is slower.

The SMA with N a power of two is significantly faster than SMA with N not a 
power of two, because the division by N can then be implemented as a series of 
bit shifts. (It should be noted that the library uses special code for signed 
integers, otherwise the compiler can't optimize it.)  
Making N 10 times larger doesn't really impact the performance.

The median filter is a bit slower, because each step involves copying and 
partially sorting the ring buffer. As N increases, the performance drops, 
because the sorting algorithm used has an average complexity of O(N).  
Odd N is faster, as the median requires only one middle element, when N is even,
you have to find the two middle elements, which requires N/2 extra comparisons.

*/
