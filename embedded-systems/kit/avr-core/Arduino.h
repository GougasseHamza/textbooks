// Minimal Arduino API declarations for *syntax checking* sketches with avr-g++ (-fsyntax-only).
// This is not the real Arduino core (no implementation): it lets avr-g++ type-check your sketch
// for the ATmega328P, where int is 16 bits, pointers are 16 bits and float == double.
#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define LED_BUILTIN 13
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define F(x) (x)
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define bitRead(v, b) (((v) >> (b)) & 1)
#define bitSet(v, b) ((v) |= (1UL << (b)))
#define bitClear(v, b) ((v) &= ~(1UL << (b)))
#define bit(b) (1UL << (b))
typedef bool boolean;
typedef uint8_t byte;
typedef unsigned int word;

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int value);
unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
void tone(uint8_t pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t pin);
long map(long, long, long, long, long);
long random(long);
long random(long, long);
void randomSeed(unsigned long);
void attachInterrupt(uint8_t, void (*)(), int);
void detachInterrupt(uint8_t);
#define digitalPinToInterrupt(p) (p)
#define CHANGE 1
#define FALLING 2
#define RISING 3
template <class A, class B> auto min(A a, B b) -> decltype(a < b ? a : b) { return a < b ? a : b; }
template <class A, class B> auto max(A a, B b) -> decltype(a > b ? a : b) { return a > b ? a : b; }
template <class T, class L, class H> T constrain(T x, L lo, H hi) { return x < lo ? (T)lo : (x > hi ? (T)hi : x); }

class String {
public:
    String(); String(const char*); String(char); String(int, int = DEC); String(unsigned int, int = DEC);
    String(long, int = DEC); String(unsigned long, int = DEC); String(double, int = 2);
    unsigned int length() const; const char* c_str() const; char charAt(unsigned int) const; char operator[](unsigned int) const;
    long toInt() const; float toFloat() const; String& operator+=(const String&); String operator+(const String&) const;
    bool operator==(const String&) const; bool operator!=(const String&) const; bool equals(const String&) const;
    bool startsWith(const String&) const; int indexOf(char) const; int indexOf(const String&) const;
    String substring(unsigned int) const; String substring(unsigned int, unsigned int) const;
    void trim(); void toUpperCase(); void toLowerCase();
};
String operator+(const char*, const String&);

class Print {
public:
    virtual size_t write(uint8_t) = 0;
    size_t write(const char*); size_t write(const uint8_t*, size_t);
    size_t write(int v) { return write((uint8_t)v); }
    size_t write(long v) { return write((uint8_t)v); }
    size_t write(unsigned int v) { return write((uint8_t)v); }
    size_t write(unsigned long v) { return write((uint8_t)v); }
    size_t print(const char*); size_t print(const String&); size_t print(char); size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC); size_t print(long, int = DEC); size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2); size_t print(bool);
    size_t println(const char*); size_t println(const String&); size_t println(char); size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC); size_t println(long, int = DEC); size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2); size_t println(bool); size_t println();
};
class HardwareSerial : public Print {
public:
    void begin(unsigned long); void end(); operator bool() const;
    int available(); int read(); int peek(); void flush();
    size_t write(uint8_t) override; using Print::write;
    long parseInt(); float parseFloat(); String readStringUntil(char); String readString();
    size_t readBytes(char*, size_t); size_t readBytes(uint8_t*, size_t);
};
extern HardwareSerial Serial;
