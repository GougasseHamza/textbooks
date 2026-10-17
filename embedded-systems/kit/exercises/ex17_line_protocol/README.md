# ex17 Line-based command protocol with a small FSM

Commands arrive as text lines terminated by `\n`: `ON`, `OFF`, `BLINK`. They drive the built-in LED:
ON = steady on, OFF = off, BLINK = toggle every 250 ms (non-blocking). Unknown lines print `unknown: <text>`.
Read a full line with `Serial.readStringUntil('\n')` (only when data is available), `trim()` it, then switch on it.

Checked: ON at 1 s -> LED HIGH; BLINK at 2 s -> toggles every 250 ms; OFF at 4 s -> LOW and stays; `foo` -> `unknown: foo`.
