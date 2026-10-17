# ex25 Mini-project: industrial machine controller (Lab millis, exercise 8)

Button 3 (INPUT_PULLUP, debounced), green LED 12, red LED 11.
STOPPED: both LEDs off; a press starts the machine. RUNNING: green toggles every 500 ms; one product every 2 s, printed as `Product produced : N`;
after 10 products the machine stops automatically: green off and red on for 5 s (FINISHING), then red off and back to STOPPED, waiting for a press.
Presses while RUNNING or FINISHING are ignored. No `delay()`. The product counter restarts at 1 for each run.

Checked: press at 1 s; products at 3, 5, ..., 21 s; green toggling 500 ms during 1-21 s then off; red on 21-26 s; a press at 23 s ignored; press at 28 s restarts (product 1 at 30 s).
