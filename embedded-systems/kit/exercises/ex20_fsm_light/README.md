# ex20 Smart-home lighting FSM (OFF -> ON -> BLINK -> OFF)

LED on pin 13, button on pin 12 (INPUT_PULLUP). Each debounced press moves to the next state:
OFF (LED off) -> ON (steady) -> BLINK (toggle every 500 ms) -> OFF -> ...
Use an `enum class State`, a `switch` on the state in `loop()`, and no `delay()`.

Checked: presses at 1, 3, 6, 8 s: LED ON during 1-3 s, toggling every 500 ms during 3-6 s, OFF during 6-8 s, ON after 8 s.
