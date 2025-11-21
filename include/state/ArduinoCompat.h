/*
 * Arduino Compatibility Layer for Simulator
 */

#ifndef ARDUINO_COMPAT_H
#define ARDUINO_COMPAT_H

#ifdef ARDUINO
// Real Arduino environment - use real functions
#include <Arduino.h>
#else
// Simulator environment - provide stub
#include <stdio.h>
#include <sys/time.h>

// Stub Serial object for simulator
class SerialClass {
public:
    void printf(const char* format, ...) {}
    void println(const char* msg) {}
};
extern SerialClass Serial;

// Stub millis() for simulator
static inline unsigned long millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL);
}
#endif

#endif

