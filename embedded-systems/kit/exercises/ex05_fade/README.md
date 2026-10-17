# ex05 PWM breathing LED on pin 5

Pin 5 is PWM-capable (~). Fade the LED in from duty 0 to 255 in steps of 5 with 30 ms per step, then fade out the same way, forever.

Checked: `analogWrite` values go 0,5,10,...,255 then 250,...,0 and each value lasts 30 ms (tolerance 2 ms).
`analogWrite` on a non-PWM pin produces a simulator warning.
