# ex06 Two independent timers with millis()

Green LED on pin 12 toggles every 500 ms, red LED on pin 11 toggles every 1500 ms. No `delay()` anywhere.

Checked: pin 12 toggles every 500 ms and pin 11 every 1500 ms (tolerance 5 ms) over 10 s; any `delay()` >= 100 ms fails the test.

Why two `previous` variables? Each LED has its own "last toggle time"; one shared variable would reset the other LED's countdown.
