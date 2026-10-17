# ex10 Count presses with a 50 ms software debounce

Button on pin 3 (INPUT_PULLUP). Each *new* press increments a counter and prints `count=N`.
A mechanical button bounces for a few ms on every press and release; the simulator reproduces this.
Implement the lecture's debounce: remember the last raw reading and the time it last changed; accept a new stable
state only after the reading has been stable for 50 ms. No `delay()`.

Checked: 5 bouncing presses give exactly `count=1` ... `count=5`; holding the button for 2 s counts once; no `delay()`.
