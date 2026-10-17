// Arduino.h  --  host-side emulation of the Arduino Uno API for the offline checker.
//
// A sketch (.ino) is compiled unchanged against this header with clang++.
// Every call is routed to the simulator core (sim.cpp), which keeps a virtual
// clock (delay() advances it, millis() reads it), the state of all pins, a
// serial link, and a trace of everything the sketch does.
//
// Two boards can run in one process (Serial lab, Arduino A <-> Arduino B):
// each board's sketch is included inside its own namespace with BOARD_ID set,
// so unqualified calls resolve to the board-local wrappers declared below.
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string>

// ---------- constants ----------
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
#define PI 3.1415926535897932384626433832795
#define PROGMEM
#define F(x) (x)
#define _BV(b) (1 << (b))
#define bitRead(v, b) (((v) >> (b)) & 1)
#define bitSet(v, b) ((v) |= (1UL << (b)))
#define bitClear(v, b) ((v) &= ~(1UL << (b)))
#define bitWrite(v, b, x) ((x) ? bitSet(v, b) : bitClear(v, b))
#define bit(b) (1UL << (b))
#define lowByte(w) ((uint8_t)((w) & 0xff))
#define highByte(w) ((uint8_t)((w) >> 8))
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

typedef bool boolean;
typedef uint8_t byte;
typedef unsigned int word;

// ---------- simulator core (board-indexed) ----------
namespace sim {
void pinMode(int board, int pin, int mode);
void digitalWrite(int board, int pin, int value);
int  digitalRead(int board, int pin);
void analogWrite(int board, int pin, int value);
int  analogRead(int board, int pin);
unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
void tone(int board, int pin, unsigned int freq, unsigned long dur);
void noTone(int board, int pin);
void serialBegin(int board, unsigned long baud);
void serialWriteByte(int board, uint8_t b);
void serialWriteStr(int board, const char* s);
int  serialAvailable(int board);
int  serialRead(int board);
int  serialPeek(int board);
void yieldTick();          // called by the scheduler between loop() iterations
uint8_t* regAddr(int board, const char* name);   // DDRB, PORTB, PINB, ...
}

// ---------- String (small subset of the Arduino String class) ----------
class String {
public:
    std::string s;
    String() {}
    String(const char* c) : s(c ? c : "") {}
    String(const std::string& x) : s(x) {}
    String(char c) : s(1, c) {}
    String(int v) : s(std::to_string(v)) {}
    String(unsigned int v) : s(std::to_string(v)) {}
    String(long v) : s(std::to_string(v)) {}
    String(unsigned long v) : s(std::to_string(v)) {}
    String(double v, int digits = 2) { char b[32]; snprintf(b, sizeof b, "%.*f", digits, v); s = b; }
    unsigned int length() const { return (unsigned)s.size(); }
    const char* c_str() const { return s.c_str(); }
    char charAt(unsigned i) const { return i < s.size() ? s[i] : 0; }
    char operator[](unsigned i) const { return charAt(i); }
    int toInt() const { return atoi(s.c_str()); }
    float toFloat() const { return (float)atof(s.c_str()); }
    String& operator+=(const String& o) { s += o.s; return *this; }
    String operator+(const String& o) const { return String(s + o.s); }
    bool operator==(const String& o) const { return s == o.s; }
    bool operator!=(const String& o) const { return s != o.s; }
    bool equals(const String& o) const { return s == o.s; }
    bool startsWith(const String& p) const { return s.rfind(p.s, 0) == 0; }
    int indexOf(char c) const { auto p = s.find(c); return p == std::string::npos ? -1 : (int)p; }
    int indexOf(const String& x) const { auto p = s.find(x.s); return p == std::string::npos ? -1 : (int)p; }
    String substring(unsigned a) const { return a < s.size() ? String(s.substr(a)) : String(); }
    String substring(unsigned a, unsigned b) const { return a < s.size() ? String(s.substr(a, b - a)) : String(); }
    void trim() { size_t a = s.find_first_not_of(" \t\r\n"); size_t b = s.find_last_not_of(" \t\r\n");
                  s = (a == std::string::npos) ? "" : s.substr(a, b - a + 1); }
    void toUpperCase() { for (auto& c : s) c = toupper(c); }
    void toLowerCase() { for (auto& c : s) c = tolower(c); }
};
inline String operator+(const char* a, const String& b) { return String(a) + b; }

// ---------- HardwareSerial ----------
class HardwareSerial {
    int board_;
public:
    explicit HardwareSerial(int board) : board_(board) {}
    void begin(unsigned long baud) { sim::serialBegin(board_, baud); }
    void end() {}
    operator bool() const { return true; }
    int available() { return sim::serialAvailable(board_); }
    int read() { return sim::serialRead(board_); }
    int peek() { return sim::serialPeek(board_); }
    void flush() {}
    // write: raw bytes
    size_t write(uint8_t b) { sim::serialWriteByte(board_, b); return 1; }
    size_t write(int b) { return write((uint8_t)b); }
    size_t write(char b) { return write((uint8_t)b); }
    size_t write(unsigned long b) { return write((uint8_t)b); }
    size_t write(long b) { return write((uint8_t)b); }
    size_t write(unsigned int b) { return write((uint8_t)b); }
    size_t write(const char* s) { size_t n = strlen(s); sim::serialWriteStr(board_, s); return n; }
    size_t write(const uint8_t* b, size_t n) { for (size_t i = 0; i < n; i++) write(b[i]); return n; }
    // print: human-readable text
    size_t print(const char* s) { return write(s); }
    size_t print(const String& s) { return write(s.c_str()); }
    size_t print(char c) { char b[2] = {c, 0}; return write(b); }
    size_t print(int v, int base = DEC) { return printLong((long)v, base); }
    size_t print(unsigned int v, int base = DEC) { return printULong((unsigned long)v, base); }
    size_t print(long v, int base = DEC) { return printLong(v, base); }
    size_t print(unsigned long v, int base = DEC) { return printULong(v, base); }
    size_t print(long long v, int base = DEC) { return printLong((long)v, base); }
    size_t print(unsigned long long v, int base = DEC) { return printULong((unsigned long)v, base); }
    size_t print(double v, int digits = 2) { char b[40]; snprintf(b, sizeof b, "%.*f", digits, v); return write(b); }
    size_t print(bool v) { return write(v ? "1" : "0"); }
    template <class T> size_t println(T v) { size_t n = print(v); return n + write("\r\n"); }
    size_t println(int v, int base) { size_t n = print(v, base); return n + write("\r\n"); }
    size_t println(unsigned long v, int base) { size_t n = print(v, base); return n + write("\r\n"); }
    size_t println(double v, int digits) { size_t n = print(v, digits); return n + write("\r\n"); }
    size_t println() { return write("\r\n"); }
    // parsing (same semantics as the Arduino core: skip non-digits, read digits, 1 s timeout)
    long parseInt() {
        unsigned long start = sim::millis();
        auto wait = [&]() { while (sim::serialAvailable(board_) == 0) { if (sim::millis() - start >= 1000) return false; sim::delay(1); } return true; };
        bool neg = false; long val = 0; bool got = false;
        for (;;) {
            if (!wait()) break;
            int c = sim::serialPeek(board_);
            if (c == '-' && !got) { neg = true; sim::serialRead(board_); continue; }
            if (c >= '0' && c <= '9') { got = true; val = val * 10 + (c - '0'); sim::serialRead(board_); continue; }
            if (!got) { sim::serialRead(board_); neg = false; continue; }   // skip junk before the number
            break;                                                             // stop at first non-digit after the number
        }
        return neg ? -val : val;
    }
    float parseFloat() {
        String t; unsigned long start = sim::millis();
        while (sim::millis() - start < 1000) {
            if (sim::serialAvailable(board_) == 0) { sim::delay(1); continue; }
            int c = sim::serialPeek(board_);
            if ((c >= '0' && c <= '9') || c == '.' || (c == '-' && t.length() == 0)) { t += String((char)c); sim::serialRead(board_); }
            else if (t.length() == 0) sim::serialRead(board_);
            else break;
        }
        return t.toFloat();
    }
    String readStringUntil(char term) {
        String out; unsigned long start = sim::millis();
        while (sim::millis() - start < 1000) {
            if (sim::serialAvailable(board_) == 0) { sim::delay(1); continue; }
            int c = sim::serialRead(board_);
            if (c == term) return out;
            out += String((char)c);
        }
        return out;
    }
    String readString() { return readStringUntil('\0'); }
    size_t readBytes(char* buf, size_t len) {
        size_t n = 0; unsigned long start = sim::millis();
        while (n < len && sim::millis() - start < 1000) {
            if (sim::serialAvailable(board_) == 0) { sim::delay(1); continue; }
            buf[n++] = (char)sim::serialRead(board_);
        }
        return n;
    }
    size_t readBytes(uint8_t* buf, size_t len) { return readBytes((char*)buf, len); }
private:
    size_t printLong(long v, int base) {
        if (base == DEC) { char b[24]; snprintf(b, sizeof b, "%ld", v); return write(b); }
        if (v < 0 && base == DEC) { write('-'); v = -v; }
        return printULong((unsigned long)v, base);
    }
    size_t printULong(unsigned long v, int base) {
        char b[40]; int i = 39; b[i] = 0;
        if (v == 0) b[--i] = '0';
        while (v) { int d = v % base; b[--i] = (char)(d < 10 ? '0' + d : 'A' + d - 10); v /= base; }
        return write(b + i);
    }
};

// ---------- math helpers ----------
template <class A, class B> auto min(A a, B b) -> decltype(a < b ? a : b) { return a < b ? a : b; }
template <class A, class B> auto max(A a, B b) -> decltype(a > b ? a : b) { return a > b ? a : b; }
template <class T> T sq(T x) { return x * x; }
template <class T, class L, class H> T constrain(T x, L lo, H hi) { return x < lo ? (T)lo : (x > hi ? (T)hi : x); }
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
inline long random(long hi) { return hi <= 0 ? 0 : rand() % hi; }
inline long random(long lo, long hi) { return lo + random(hi - lo); }
inline void randomSeed(unsigned long s) { srand((unsigned)s); }

// ---------- global (board-independent) time functions ----------
inline unsigned long millis() { return sim::millis(); }
inline unsigned long micros() { return sim::micros(); }
inline void delay(unsigned long ms) { sim::delay(ms); }
inline void delayMicroseconds(unsigned int us) { sim::delayMicroseconds(us); }
inline void yield() { sim::yieldTick(); }

// ---------- per-board API ----------
// BOARD_API(id) declares the board-local wrappers. The runner includes each
// sketch inside `namespace boardN { BOARD_API(N) #include "sketch.ino" }`.
#define BOARD_API(ID)                                                                   \
    static const int __board_id = ID;                                                   \
    inline void pinMode(int pin, int mode) { sim::pinMode(ID, pin, mode); }             \
    inline void digitalWrite(int pin, int value) { sim::digitalWrite(ID, pin, value); } \
    inline int  digitalRead(int pin) { return sim::digitalRead(ID, pin); }              \
    inline void analogWrite(int pin, int value) { sim::analogWrite(ID, pin, value); }   \
    inline int  analogRead(int pin) { return sim::analogRead(ID, pin); }                \
    inline void tone(int pin, unsigned int f, unsigned long d = 0) { sim::tone(ID, pin, f, d); } \
    inline void noTone(int pin) { sim::noTone(ID, pin); }                               \
    static HardwareSerial Serial(ID);                                                   \
    static uint8_t& DDRB  = *sim::regAddr(ID, "DDRB");                                  \
    static uint8_t& PORTB = *sim::regAddr(ID, "PORTB");                                 \
    static uint8_t& PINB  = *sim::regAddr(ID, "PINB");                                  \
    static uint8_t& DDRD  = *sim::regAddr(ID, "DDRD");                                  \
    static uint8_t& PORTD = *sim::regAddr(ID, "PORTD");                                 \
    static uint8_t& PIND  = *sim::regAddr(ID, "PIND");                                  \
    static uint8_t& DDRC  = *sim::regAddr(ID, "DDRC");                                  \
    static uint8_t& PORTC = *sim::regAddr(ID, "PORTC");                                 \
    static uint8_t& PINC  = *sim::regAddr(ID, "PINC");

// Bit names of the ATmega328P ports (PB5 = pin 13, PD2 = pin 2, ...)
enum { PB0 = 0, PB1, PB2, PB3, PB4, PB5, PB6, PB7 };
enum { PD0 = 0, PD1, PD2, PD3, PD4, PD5, PD6, PD7 };
enum { PC0 = 0, PC1, PC2, PC3, PC4, PC5 };
