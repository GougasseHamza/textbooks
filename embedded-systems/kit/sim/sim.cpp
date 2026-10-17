// sim.cpp -- the simulator core behind Arduino.h.
//
// Discrete-event simulation with a virtual clock. Each board runs its sketch
// in its own thread; exactly one board thread runs at a time (the one whose
// wake-up time is the smallest). delay(), the end of loop() and every I/O call
// advance the clock. Scenario events (button presses, serial input) are applied
// when the clock reaches their timestamp. Everything observable is written to
// a JSON trace on stdout when the simulated duration is reached.
//
// Scenario (text, one directive per line, read from the file given as argv[1]):
//   duration <ms>                 total simulated time (default 10000)
//   tick <us>                     time consumed by one loop() iteration (default 100)
//   link <a> <b>                  cross-connect Serial TX/RX of boards a and b
//   seed <n>                      seed for floating-pin noise and bounce
//   at <ms> pin <board> <pin> <0|1> [bounce <ms>]     external drive (button, sensor)
//   at <ms> release <board> <pin>                     stop driving (pin floats / pull-up)
//   at <ms> analog <board> <pin> <0..1023>            analog input value
//   at <ms> serial <board> <C-escaped string>         bytes arriving from the PC
#include "Arduino.h"
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>

namespace sim {

struct Event { uint64_t t_us; int board; std::string kind; int pin; int value; std::string text; };

struct Board {
    int id = 0;
    void (*setupFn)() = nullptr;
    void (*loopFn)() = nullptr;
    uint64_t wake_at = 0;
    bool started = false;
    // pins 0..19 (14..19 = A0..A5)
    int mode[20];        // -1 unset, INPUT, OUTPUT, INPUT_PULLUP
    int out[20];         // value driven when OUTPUT (PORT bit)
    int ext[20];         // external drive: -1 none, 0, 1
    int analog[20];      // analog input value
    int pwm[20];         // last analogWrite value, -1 none
    int lastLevel[20];   // last traced logical level of the pin, -1 unknown
    unsigned toneFreq[20];
    uint8_t regs[9];     // DDRB PORTB PINB DDRD PORTD PIND DDRC PORTC PINC
    uint8_t regsSeen[9];
    bool serialOn = false;
    unsigned long baud = 0;
    std::deque<uint8_t> rx;
    int linkTo = -1;
    std::thread th;
    Board() {
        for (int i = 0; i < 20; i++) { mode[i] = -1; out[i] = 0; ext[i] = -1; analog[i] = 0; pwm[i] = -1; lastLevel[i] = -1; toneFreq[i] = 0; }
        memset(regs, 0, sizeof regs); memset(regsSeen, 0, sizeof regsSeen);
    }
};

static std::mutex M;
static std::condition_variable CV;
static std::vector<Board*>& boardsRef() { static std::vector<Board*> v; return v; }
#define boards (boardsRef())
static std::vector<Event> events;
static size_t nextEvent = 0;
static uint64_t now_us = 0;
static uint64_t duration_us = 10000ull * 1000;
static uint64_t tick_us = 100;
static int running = 0;
static bool finished = false;
static uint32_t rng = 12345;
static std::string trace;   // JSON lines accumulated here
static long traceCount = 0;
static int warnings = 0;

static uint32_t nextRand() { rng = rng * 1664525u + 1013904223u; return rng >> 8; }

static std::string jsonEscape(const std::string& s) {
    std::string o;
    for (unsigned char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20 || c >= 0x7f) { char b[8]; snprintf(b, sizeof b, "\\u%04x", c); o += b; }
                else o += (char)c;
        }
    }
    return o;
}

static void emit(int board, const char* kind, const std::string& fields) {
    char head[96];
    snprintf(head, sizeof head, "{\"t\":%.3f,\"board\":%d,\"kind\":\"%s\"", now_us / 1000.0, board, kind);
    trace += head;
    if (!fields.empty()) { trace += ","; trace += fields; }
    trace += "}\n";
    traceCount++;
}
static void warn(int board, const std::string& msg) {
    warnings++;
    if (warnings <= 200) emit(board, "warn", "\"msg\":\"" + jsonEscape(msg) + "\"");
}

// ---------- pins and registers ----------
// Uno mapping: D0-D7 -> PORTD b0-b7, D8-D13 -> PORTB b0-b5, A0-A5 -> PORTC b0-b5
static bool pinToPort(int pin, int& port, int& bit) {   // port: 0=B 1=D 2=C
    if (pin >= 0 && pin <= 7) { port = 1; bit = pin; return true; }
    if (pin >= 8 && pin <= 13) { port = 0; bit = pin - 8; return true; }
    if (pin >= 14 && pin <= 19) { port = 2; bit = pin - 14; return true; }
    return false;
}
static int portToPin(int port, int bit) {
    if (port == 1) return bit;
    if (port == 0) return bit <= 5 ? 8 + bit : -1;
    return bit <= 5 ? 14 + bit : -1;
}

static int inputLevel(Board& b, int pin) {
    if (b.ext[pin] != -1) return b.ext[pin];
    if (b.mode[pin] == INPUT_PULLUP) return 1;
    // floating: noise
    return (nextRand() & 1);
}
static int pinLevel(Board& b, int pin) {
    if (b.mode[pin] == OUTPUT) return b.out[pin];
    return inputLevel(b, pin);
}

static void tracePinIfChanged(Board& b, int pin) {
    if (b.mode[pin] != OUTPUT) return;
    int lvl = b.out[pin];
    if (lvl != b.lastLevel[pin]) {
        b.lastLevel[pin] = lvl;
        emit(b.id, "pin", "\"pin\":" + std::to_string(pin) + ",\"value\":" + std::to_string(lvl));
    }
}

// Sketch may have written DDRx/PORTx directly: pull those into mode/out.
static void regsToPins(Board& b) {
    for (int port = 0; port < 3; port++) {
        uint8_t ddr = b.regs[port * 3 + 0], prt = b.regs[port * 3 + 1];
        if (ddr == b.regsSeen[port * 3 + 0] && prt == b.regsSeen[port * 3 + 1]) continue;
        for (int bit = 0; bit < 8; bit++) {
            int pin = portToPin(port, bit);
            if (pin < 0) continue;
            uint8_t m = 1 << bit;
            bool ddrChanged = (ddr & m) != (b.regsSeen[port * 3] & m);
            bool prtChanged = (prt & m) != (b.regsSeen[port * 3 + 1] & m);
            if (ddrChanged) {
                b.mode[pin] = (ddr & m) ? OUTPUT : ((prt & m) ? INPUT_PULLUP : INPUT);
                emit(b.id, "pinMode", "\"pin\":" + std::to_string(pin) + ",\"mode\":\"" +
                     (b.mode[pin] == OUTPUT ? "OUTPUT" : b.mode[pin] == INPUT_PULLUP ? "INPUT_PULLUP" : "INPUT") + "\",\"via\":\"register\"");
            }
            if (prtChanged || ddrChanged) {
                if (b.mode[pin] == OUTPUT) { b.out[pin] = (prt & m) ? 1 : 0; tracePinIfChanged(b, pin); }
                else b.mode[pin] = (prt & m) ? INPUT_PULLUP : INPUT;
            }
        }
        b.regsSeen[port * 3 + 0] = ddr; b.regsSeen[port * 3 + 1] = prt;
    }
}
// Refresh PINx from the pin levels so register-level reads work.
static void pinsToRegs(Board& b) {
    for (int port = 0; port < 3; port++) {
        uint8_t v = 0;
        for (int bit = 0; bit < 8; bit++) {
            int pin = portToPin(port, bit);
            if (pin < 0) continue;
            if (b.mode[pin] == OUTPUT) { if (b.out[pin]) v |= 1 << bit; }
            else if (b.ext[pin] != -1) { if (b.ext[pin]) v |= 1 << bit; }
            else if (b.mode[pin] == INPUT_PULLUP) v |= 1 << bit;
            else if (nextRand() & 1) v |= 1 << bit;
        }
        b.regs[port * 3 + 2] = v;
    }
}
static void syncRegs(Board& b) { regsToPins(b); pinsToRegs(b); }

// ---------- scenario events ----------
static void applyEvent(const Event& e) {
    if (e.board < 0 || e.board >= (int)boards.size()) return;
    Board& b = *boards[e.board];
    if (e.kind == "pin") {
        b.ext[e.pin] = e.value;
        emit(e.board, "input", "\"pin\":" + std::to_string(e.pin) + ",\"value\":" + std::to_string(e.value));
    } else if (e.kind == "release") {
        b.ext[e.pin] = -1;
        emit(e.board, "input", "\"pin\":" + std::to_string(e.pin) + ",\"value\":\"released\"");
    } else if (e.kind == "analog") {
        b.analog[e.pin] = e.value;
        emit(e.board, "input", "\"pin\":" + std::to_string(e.pin) + ",\"analog\":" + std::to_string(e.value));
    } else if (e.kind == "serial") {
        for (unsigned char c : e.text) { if (b.rx.size() < 64) b.rx.push_back(c); else warn(e.board, "serial RX buffer overflow (64 bytes)"); }
        emit(e.board, "rx", "\"text\":\"" + jsonEscape(e.text) + "\"");
    }
    pinsToRegs(b);
}

// ---------- scheduler ----------
static int argminWake() {
    int r = 0;
    for (size_t i = 1; i < boards.size(); i++) if (boards[i]->wake_at < boards[r]->wake_at) r = (int)i;
    return r;
}
static int currentBoardId() {
    // the running board (only one thread executes sketch code at a time)
    return running;
}

// Called with the lock held by the running board: move its wake time forward,
// apply due events, hand over to whichever board is due next.
static void advanceLocked(std::unique_lock<std::mutex>& lk, uint64_t us) {
    Board& me = *boards[running];
    regsToPins(me);
    me.wake_at = now_us + us;
    for (;;) {
        int r = argminWake();
        uint64_t next = boards[r]->wake_at;
        while (nextEvent < events.size() && events[nextEvent].t_us <= next) {
            now_us = std::max(now_us, events[nextEvent].t_us);
            applyEvent(events[nextEvent]);
            nextEvent++;
        }
        now_us = std::max(now_us, next);
        if (now_us >= duration_us) { finished = true; CV.notify_all(); CV.wait(lk, [] { return false; }); }
        if (r == running) { pinsToRegs(me); return; }
        running = r;
        CV.notify_all();
        CV.wait(lk, [&] { return running == me.id; });
        pinsToRegs(me);
        return;
    }
}
static void advance(uint64_t us) {
    std::unique_lock<std::mutex> lk(M);
    advanceLocked(lk, us);
}

static void boardThread(Board* b) {
    std::unique_lock<std::mutex> lk(M);
    CV.wait(lk, [&] { return running == b->id; });
    lk.unlock();
    b->setupFn();
    for (;;) {
        b->loopFn();
        advance(tick_us);
    }
}

// ---------- public API ----------
void registerBoard(int id, void (*setupFn)(), void (*loopFn)()) {
    while ((int)boards.size() <= id) { Board* nb = new Board(); nb->id = (int)boards.size(); boards.push_back(nb); }
    if (setupFn) boards[id]->setupFn = setupFn;
    if (loopFn) boards[id]->loopFn = loopFn;
}

static std::string unescape(const std::string& s) {
    std::string o;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char c = s[++i];
            if (c == 'n') o += '\n'; else if (c == 'r') o += '\r'; else if (c == 't') o += '\t';
            else if (c == '0') o += '\0'; else if (c == 'x' && i + 2 < s.size()) { o += (char)strtol(s.substr(i + 1, 2).c_str(), nullptr, 16); i += 2; }
            else o += c;
        } else o += s[i];
    }
    return o;
}

static void loadScenario(const char* path) {
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream is(line);
        std::string w; is >> w;
        if (w == "duration") { double ms; is >> ms; duration_us = (uint64_t)(ms * 1000); }
        else if (w == "tick") { is >> tick_us; }
        else if (w == "seed") { is >> rng; }
        else if (w == "link") { int a, b; is >> a >> b; registerBoard(a, nullptr, nullptr); registerBoard(b, nullptr, nullptr); boards[a]->linkTo = b; boards[b]->linkTo = a; }
        else if (w == "at") {
            double ms; is >> ms; Event e; e.t_us = (uint64_t)(ms * 1000); e.pin = 0; e.value = 0;
            is >> e.kind >> e.board;
            if (e.kind == "pin") {
                is >> e.pin >> e.value;
                std::string opt; int bounce = 0; if (is >> opt && opt == "bounce") is >> bounce;
                if (bounce > 0) {
                    // a burst of random transitions over `bounce` ms, then the final level
                    uint64_t t = e.t_us; int lvl = !e.value; int n = 3 + (nextRand() % 6);
                    for (int k = 0; k < n; k++) {
                        Event s = e; s.t_us = t; s.value = lvl; events.push_back(s);
                        lvl = !lvl; t += 100 + nextRand() % ((uint64_t)bounce * 1000 / n);
                    }
                    e.t_us = e.t_us + (uint64_t)bounce * 1000;
                }
                events.push_back(e);
            } else if (e.kind == "release") { is >> e.pin; events.push_back(e); }
            else if (e.kind == "analog") { is >> e.pin >> e.value; events.push_back(e); }
            else if (e.kind == "serial") { std::string rest; std::getline(is, rest); size_t p = rest.find_first_not_of(' '); e.text = unescape(p == std::string::npos ? "" : rest.substr(p)); events.push_back(e); }
        }
    }
    std::stable_sort(events.begin(), events.end(), [](const Event& a, const Event& b) { return a.t_us < b.t_us; });
}

int run(int argc, char** argv) {
    if (argc > 1) loadScenario(argv[1]);
    for (Board* b : boards) if (!b->setupFn) { fprintf(stderr, "board %d has no sketch\n", b->id); return 2; }
    for (Board* b : boards) b->th = std::thread(boardThread, b);
    auto start = std::chrono::steady_clock::now();
    {
        std::unique_lock<std::mutex> lk(M);
        running = 0;
        CV.notify_all();
        while (!finished) {
            if (CV.wait_for(lk, std::chrono::milliseconds(200)) == std::cv_status::timeout) {
                double wall = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
                if (wall > 30.0) {
                    emit(running, "warn", "\"msg\":\"simulation stuck: the sketch is busy-waiting without calling any Arduino function (wall-clock limit 30 s)\"");
                    break;
                }
            }
        }
        for (Board* b : boards) for (int p = 0; p < 20; p++) if (b->mode[p] == OUTPUT) tracePinIfChanged(*b, p);
        printf("{\"duration_ms\":%.3f,\"end_ms\":%.3f,\"boards\":%zu,\"warnings\":%d,\"events\":[\n", duration_us / 1000.0, now_us / 1000.0, boards.size(), warnings);
        // trace lines are newline separated; join with commas
        std::string out; out.reserve(trace.size() + traceCount);
        size_t pos = 0; bool first = true;
        while (pos < trace.size()) {
            size_t nl = trace.find('\n', pos);
            if (!first) out += ",\n";
            out += trace.substr(pos, nl - pos); first = false; pos = nl + 1;
        }
        fputs(out.c_str(), stdout);
        printf("\n]}\n");
        fflush(stdout);
    }
    std::_Exit(0);
}

// ----- API used by Arduino.h -----
static Board& B(int id) { return *boards[id]; }
static const char* modeName(int m) { return m == OUTPUT ? "OUTPUT" : m == INPUT_PULLUP ? "INPUT_PULLUP" : "INPUT"; }

void pinMode(int board, int pin, int mode) {
    std::unique_lock<std::mutex> lk(M);
    if (pin < 0 || pin > 19) { warn(board, "pinMode on invalid pin " + std::to_string(pin)); return; }
    Board& b = B(board); regsToPins(b);
    b.mode[pin] = mode;
    int port, bit; pinToPort(pin, port, bit);
    if (mode == OUTPUT) b.regs[port * 3] |= 1 << bit; else b.regs[port * 3] &= ~(1 << bit);
    if (mode == INPUT_PULLUP) b.regs[port * 3 + 1] |= 1 << bit; else if (mode == INPUT) b.regs[port * 3 + 1] &= ~(1 << bit);
    b.regsSeen[port * 3] = b.regs[port * 3]; b.regsSeen[port * 3 + 1] = b.regs[port * 3 + 1];
    emit(board, "pinMode", "\"pin\":" + std::to_string(pin) + ",\"mode\":\"" + modeName(mode) + "\"");
    if (mode == OUTPUT) tracePinIfChanged(b, pin);
    if (b.serialOn && (pin == 0 || pin == 1)) warn(board, "pin " + std::to_string(pin) + " is used by Serial (RX/TX)");
    advanceLocked(lk, 4);
}
void digitalWrite(int board, int pin, int value) {
    std::unique_lock<std::mutex> lk(M);
    if (pin < 0 || pin > 19) { warn(board, "digitalWrite on invalid pin " + std::to_string(pin)); return; }
    Board& b = B(board); regsToPins(b);
    value = value ? 1 : 0;
    int port, bit; pinToPort(pin, port, bit);
    if (b.mode[pin] == OUTPUT) {
        b.out[pin] = value;
        if (value) b.regs[port * 3 + 1] |= 1 << bit; else b.regs[port * 3 + 1] &= ~(1 << bit);
        b.regsSeen[port * 3 + 1] = b.regs[port * 3 + 1];
        b.pwm[pin] = -1;
        tracePinIfChanged(b, pin);
    } else {
        // on a real AVR this toggles the pull-up; also a classic bug
        if (b.mode[pin] == -1) warn(board, "digitalWrite(" + std::to_string(pin) + ") before pinMode: pin is still an INPUT, the LED will not light");
        b.mode[pin] = value ? INPUT_PULLUP : INPUT;
    }
    advanceLocked(lk, 4);
}
int digitalRead(int board, int pin) {
    std::unique_lock<std::mutex> lk(M);
    if (pin < 0 || pin > 19) { warn(board, "digitalRead on invalid pin " + std::to_string(pin)); return 0; }
    Board& b = B(board); regsToPins(b);
    int v;
    if (b.mode[pin] == OUTPUT) v = b.out[pin];
    else {
        if (b.mode[pin] == -1) { b.mode[pin] = INPUT; warn(board, "digitalRead(" + std::to_string(pin) + ") without pinMode (defaults to INPUT, floating)"); }
        if (b.ext[pin] == -1 && b.mode[pin] == INPUT) {
            static int floatWarns = 0;
            if (floatWarns++ < 3) warn(board, "digitalRead(" + std::to_string(pin) + ") on a FLOATING input: value is random noise");
        }
        v = inputLevel(b, pin);
    }
    advanceLocked(lk, 4);
    return v;
}
void analogWrite(int board, int pin, int value) {
    std::unique_lock<std::mutex> lk(M);
    Board& b = B(board);
    static const int pwmPins[] = {3, 5, 6, 9, 10, 11};
    bool ok = std::find(std::begin(pwmPins), std::end(pwmPins), pin) != std::end(pwmPins);
    if (!ok) warn(board, "analogWrite on pin " + std::to_string(pin) + " which has no PWM (~): behaves as digitalWrite(value<128?LOW:HIGH)");
    value = value < 0 ? 0 : value > 255 ? 255 : value;
    if (b.mode[pin] != OUTPUT) b.mode[pin] = OUTPUT;
    if (!ok) { b.out[pin] = value < 128 ? 0 : 1; tracePinIfChanged(b, pin); }
    else if (b.pwm[pin] != value) {
        b.pwm[pin] = value;
        emit(board, "pwm", "\"pin\":" + std::to_string(pin) + ",\"value\":" + std::to_string(value));
        b.out[pin] = value == 255 ? 1 : 0; b.lastLevel[pin] = value == 255 ? 1 : (value == 0 ? 0 : -1);
    }
    advanceLocked(lk, 8);
}
int analogRead(int board, int pin) {
    std::unique_lock<std::mutex> lk(M);
    Board& b = B(board);
    if (pin >= 0 && pin <= 5) pin += 14;
    int v = (pin >= 14 && pin <= 19) ? b.analog[pin] : 0;
    advanceLocked(lk, 100);
    return v;
}
unsigned long millis() { std::unique_lock<std::mutex> lk(M); unsigned long v = (unsigned long)(now_us / 1000); advanceLocked(lk, 1); return v; }
unsigned long micros() { std::unique_lock<std::mutex> lk(M); unsigned long v = (unsigned long)now_us; advanceLocked(lk, 1); return v; }
void delay(unsigned long ms) {
    std::unique_lock<std::mutex> lk(M);
    if (ms >= 100) emit(running, "delay", "\"ms\":" + std::to_string(ms));
    advanceLocked(lk, (uint64_t)ms * 1000);
}
void delayMicroseconds(unsigned int us) { advance(us); }
void yieldTick() { advance(tick_us); }
void tone(int board, int pin, unsigned int freq, unsigned long dur) {
    std::unique_lock<std::mutex> lk(M);
    Board& b = B(board);
    if (b.toneFreq[pin] != freq) { b.toneFreq[pin] = freq; emit(board, "tone", "\"pin\":" + std::to_string(pin) + ",\"freq\":" + std::to_string(freq) + ",\"dur\":" + std::to_string(dur)); }
    advanceLocked(lk, 4);
}
void noTone(int board, int pin) {
    std::unique_lock<std::mutex> lk(M);
    Board& b = B(board);
    if (b.toneFreq[pin] != 0) { b.toneFreq[pin] = 0; emit(board, "tone", "\"pin\":" + std::to_string(pin) + ",\"freq\":0,\"dur\":0"); }
    advanceLocked(lk, 4);
}
void serialBegin(int board, unsigned long baud) {
    std::unique_lock<std::mutex> lk(M);
    B(board).serialOn = true; B(board).baud = baud;
    emit(board, "serialBegin", "\"baud\":" + std::to_string(baud));
    advanceLocked(lk, 10);
}
static void serialOut(int board, const std::string& bytes) {
    Board& b = B(board);
    if (!b.serialOn) { static int w = 0; if (w++ < 2) warn(board, "Serial used before Serial.begin()"); }
    std::string disp; bool allText = true;
    for (unsigned char c : bytes) if (!(c == '\r' || c == '\n' || c == '\t' || (c >= 0x20 && c < 0x7f))) allText = false;
    std::string arr;
    for (size_t i = 0; i < bytes.size(); i++) { if (i) arr += ","; arr += std::to_string((unsigned char)bytes[i]); }
    emit(board, "tx", "\"text\":\"" + jsonEscape(bytes) + "\",\"bytes\":[" + arr + "],\"printable\":" + (allText ? "true" : "false"));
    if (b.linkTo >= 0) {
        Board& o = B(b.linkTo);
        for (unsigned char c : bytes) { if (o.rx.size() < 64) o.rx.push_back(c); else warn(b.linkTo, "serial RX buffer overflow (64 bytes)"); }
    }
    // time on the wire: 10 bits per byte at `baud`
    uint64_t us = b.baud ? (uint64_t)bytes.size() * 10 * 1000000 / b.baud : 0;
    (void)us;
}
void serialWriteByte(int board, uint8_t c) {
    std::unique_lock<std::mutex> lk(M);
    serialOut(board, std::string(1, (char)c));
    uint64_t us = B(board).baud ? 10ull * 1000000 / B(board).baud : 1;
    advanceLocked(lk, us);
}
void serialWriteStr(int board, const char* s) {
    std::unique_lock<std::mutex> lk(M);
    std::string str(s);
    serialOut(board, str);
    uint64_t us = B(board).baud ? (uint64_t)str.size() * 10ull * 1000000 / B(board).baud : 1;
    advanceLocked(lk, us);
}
int serialAvailable(int board) { std::unique_lock<std::mutex> lk(M); int n = (int)B(board).rx.size(); advanceLocked(lk, 2); return n; }
int serialRead(int board) {
    std::unique_lock<std::mutex> lk(M);
    Board& b = B(board); int v = -1;
    if (!b.rx.empty()) { v = b.rx.front(); b.rx.pop_front(); }
    advanceLocked(lk, 2);
    return v;
}
int serialPeek(int board) { std::unique_lock<std::mutex> lk(M); Board& b = B(board); int v = b.rx.empty() ? -1 : b.rx.front(); advanceLocked(lk, 2); return v; }
uint8_t* regAddr(int board, const char* name) {
    registerBoard(board, nullptr, nullptr);
    static const char* names[9] = {"DDRB", "PORTB", "PINB", "DDRD", "PORTD", "PIND", "DDRC", "PORTC", "PINC"};
    for (int i = 0; i < 9; i++) if (!strcmp(names[i], name)) return &B(board).regs[i];
    return &B(board).regs[0];
}

} // namespace sim
