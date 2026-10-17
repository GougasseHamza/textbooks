# Exercises (offline, auto-checked)

Workflow for each exercise `exNN_name/`:

1. Read `README.md` (the statement) and copy `starter.ino` to `sketch.ino`.
2. Write your code in `sketch.ino` (plain Arduino: `setup()`, `loop()`).
3. Run it to watch the timeline:
   `python3 arduino_sim.py run exercises/exNN_name/sketch.ino --wave --press 1000:3`
4. Grade it: `python3 check.py exNN`  (prints PASS or the first failures).
5. Compare with `solutions/exNN_name.ino` only after you have a PASS, or when stuck.

Two-board exercises (ex18, ex19) use `sketch_A.ino` and `sketch_B.ino`;
`python3 check.py ex18` links their Serial ports (TX of A to RX of B and back).

`python3 check.py list` lists everything; `python3 check.py all --solution` re-verifies the reference solutions.
If `avr-g++` is installed, `python3 arduino_sim.py avr sketch.ino` also compiles for the real ATmega328P
(catches things the host compiler cannot, like `int` being 16 bits).

| # | Topic | Lab it prepares |
|---|-------|-----------------|
| 01-05 | digitalWrite, delay, timing, duty cycle, PWM | Lab internal LED, Lab external LED |
| 06-08 | millis() patterns: several timers, periodic printing, counters | Lab millis ex 1-4 |
| 09-12 | digital input, pull-ups, debouncing, one-shot timer | Lab floating pin, Lab millis ex 5-6 |
| 13-19 | Serial: print/write/read/parseInt, line protocols, two boards | Lab Serial ex 1-11 |
| 20-25 | FSMs: switch-based, table-driven, timeouts, external events | Lab traffic light, FSM labs, mini-project |
| 26-27 | AVR registers, tone() | Lecture 2 title, Piano lab |
