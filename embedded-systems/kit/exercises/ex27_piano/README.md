# ex27 Three-key piano with tone()

Keys on pins 2, 3, 4 (INPUT_PULLUP), piezo buzzer on pin 8. While a key is held, play its note with `tone(BUZZER, freq)`:
pin 2 = C4 262 Hz, pin 3 = D4 294 Hz, pin 4 = E4 330 Hz. When no key is held, `noTone(BUZZER)`. Lowest pin wins if several are held.
Use the `KEYS[]` / `NOTES[]` arrays and a `for` loop, not three copies of the code.

Checked: hold pin 2 (1-2 s) -> tone 262; pin 3 (3-4 s) -> 294; pin 4 (5-6 s) -> 330; silence (freq 0) between keys.
