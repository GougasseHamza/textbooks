# Embedded Systems (M233) offline kit

Everything runs on this Mac with no internet: `python3` (3.9+), `clang++` (Xcode command-line tools) and,
optionally, `avr-g++` (Homebrew `osx-cross/avr/avr-gcc@14`, installed at `/opt/homebrew/opt/avr-gcc@14/bin`).

```
python3 check.py list                       # the 27 exercises
python3 check.py ex06                       # grade exercises/ex06_two_timers/sketch.ino
python3 check.py ex06 my.ino                # grade any file
python3 check.py all --solution             # re-verify the reference solutions (27/27)
python3 labs/verify_labs.py                 # re-verify the lab solutions (23/23)
python3 arduino_sim.py run x.ino --wave --press 1000:3 --serial "2000:ON\n" --duration 8000
python3 arduino_sim.py run a.ino --board2 b.ino      # two boards, Serial crossed
python3 arduino_sim.py avr x.ino            # type-check for the real ATmega328P (int = 16 bits)
```

* `sim/` the Arduino Uno emulation (`Arduino.h` + `sim.cpp`): virtual clock, pins, PWM, tone, Serial, registers, two boards.
* `exercises/` statements (`README.md`) and starters; write your code in `sketch.ino`.
* `tests/` the automatic checks; `solutions/` reference solutions; `labs/` solutions to the course labs.
* `../Embedded_Systems_Complete_Guide.pdf` the guide (parts A to J).

Simulator notes: `loop()` costs 100 us per iteration, `digitalRead` 4 us, a Serial byte 1.04 ms at 9600 baud.
Floating inputs return random noise (with a warning), like real hardware. Button presses from `--press`
or the tests bounce for 5 ms. `delay()` is logged so tests can forbid it.
