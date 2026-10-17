# ex26 Blink through the AVR registers (what pinMode/digitalWrite do underneath)

Pin 13 is bit 5 of port B (PB5). Three 8-bit registers control a port: `DDRx` (data direction, 1 = output), `PORTx` (output value, or pull-up enable when input),
`PINx` (read the physical level). Blink pin 13 at 1 Hz (500/500) using only `DDRB`, `PORTB` and `delay()`: no `pinMode`, no `digitalWrite`.
Use bit operations: set a bit with `|= (1 << 5)`, clear with `&= ~(1 << 5)`, toggle with `^= (1 << 5)`. `_BV(5)` is `(1 << 5)`.

Checked: pin 13 becomes an OUTPUT (the simulator notes "via register"), 500/500 ms blinking; `pinMode`/`digitalWrite` calls fail the test.
