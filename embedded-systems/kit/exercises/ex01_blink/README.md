# ex01 Blink the built-in LED at 1 Hz

Make the built-in LED (pin 13, `LED_BUILTIN`) blink with a period of 1 s: 500 ms ON, 500 ms OFF, forever.

Checked: pin 13 is configured as OUTPUT; every ON and OFF phase lasts 500 ms (tolerance 5 ms); at least 3 full cycles in 5 s.

Hint: `digitalWrite()` then `delay()`. Notice in the timeline that `delay()` is reported as "CPU blocked".
