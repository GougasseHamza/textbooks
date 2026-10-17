# ex22 Table-driven traffic light, fully non-blocking

LEDs: red 6, yellow 5, green 4. Cycle: RED 5 s -> GREEN 5 s -> YELLOW blinking (100 ms ON / 100 ms OFF) for 3 s -> RED ...
Implement the lab's design: `enum class State`, `enum class Event { TIMEOUT, BLINK }`, a `struct Transition { from, event, to, action }` table,
and a `processEvent()` engine that scans the table. Replace the lab's `delay(5000)` by a `stateEnteredAt` timestamp: `loop()` emits TIMEOUT when
`millis() - stateEnteredAt >= duration` of the current state. Start in RED (red LED on from t = 0).

Checked: red HIGH for 5 s, then green HIGH for 5 s (red off), then yellow toggles every 100 ms for 3 s (red and green off), then red again (tolerance 10 ms); no `delay()`.
