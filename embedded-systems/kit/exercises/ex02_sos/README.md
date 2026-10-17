# ex02 Morse SOS on the built-in LED

Repeat forever: S (3 dots), O (3 dashes), S (3 dots), then a 1 s pause.
A dot is 200 ms ON, a dash is 600 ms ON. Between symbols of the same letter the LED stays OFF for 200 ms;
between letters it stays OFF for 600 ms; after the whole word it stays OFF for 1000 ms (this replaces the letter gap).

Checked: the sequence of ON durations is 200,200,200,600,600,600,200,200,200 then an OFF gap of at least 1000 ms, twice in a row.

Hint: write `void dot()` and `void dash()` helpers and a `void letterGap()`.
