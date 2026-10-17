# ex12 Push-button timer (one-shot)

Button on pin 3 (INPUT_PULLUP), LED on pin 12. A valid press starts the machine: the LED turns ON for 5 s and then OFF automatically.
Presses during the 5 s are ignored (they do not extend the time). Print `Machine started` and `Machine stopped`. No `delay()`.

Checked: press at 1 s -> LED ON from 1 s to 6 s (tolerance 10 ms), a press at 3 s changes nothing, LED OFF at 6 s; a press at 8 s starts a new 5 s run; messages in order.
