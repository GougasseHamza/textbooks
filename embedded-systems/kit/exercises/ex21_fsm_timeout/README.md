# ex21 IDLE / ACTIVE / BLINKING with a 2 s timeout (non-blocking)

LED 13, button 12 (INPUT_PULLUP). IDLE: LED off. A press -> ACTIVE: LED on. A press -> BLINKING: LED toggles every 100 ms.
BLINKING ends either on a press or automatically after 2 s; both go back to IDLE. The lecture's version uses `delay(200)` and `delay(100)`;
yours must not block at all (debounce with millis()). Print each transition as `IDLE -> ACTIVE`, `ACTIVE -> BLINKING`, `BLINKING -> IDLE`.

Checked: press at 1 s (ON), press at 2 s (blink 100 ms), auto-return at 4 s (tolerance 60 ms, LED off), press at 5 s (ON), press at 6 s (blink), press at 6.5 s (off); transition lines.
