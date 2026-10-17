# ex24 Store alarm: STANDBY / ALERT / COOLDOWN

PIR sensor output on pin 2 (plain INPUT: the sensor drives the pin, HIGH while motion is detected), buzzer on pin 11, green LED 13, red LED 12.
STANDBY: green on, red off, buzzer silent. Motion (PIR HIGH) -> ALERT: red on, green off, buzzer beeps 300 ms ON / 300 ms OFF for 3 s, then -> COOLDOWN:
buzzer silent, red off, green off, ignore the PIR for 2 s, then -> STANDBY. Print `State: STANDBY`, `State: ALERT`, `State: COOLDOWN` on every entry (also at start).
No `delay()`.

Checked: PIR HIGH at 1 s: buzzer (pin 11) toggles every 300 ms from 1 s to 4 s; silent afterwards; PIR HIGH again at 5 s (during cooldown) is ignored; PIR at 8 s starts a new alert; the State lines.
