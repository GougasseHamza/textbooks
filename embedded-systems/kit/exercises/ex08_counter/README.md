# ex08 Production counter and an independent LED

One product is produced every 3 s. Print `Product produced : N` (N = 1, 2, 3, ...). An LED on pin 12 toggles every 500 ms, independently.
After 20 products the counter stops (no more messages) but the LED keeps blinking. No `delay()`.

Checked: 20 messages at 3, 6, ..., 60 s with the exact text; none after 60 s; pin 12 toggles every 500 ms throughout 70 s.
