# ex16 Multi-digit numbers with parseInt()

The operator types a number in the Serial Monitor (e.g. `125` followed by newline). `Serial.read()` would give the character '1' (49).
Use `Serial.parseInt()` to get the whole integer and reply `got 125`. Ignore an empty result (parseInt returns 0 after its 1 s timeout when nothing valid arrives;
to be safe, only call parseInt when `Serial.available() > 0`, and discard the trailing newline).

Checked: inputs `25\n`, `125\n`, `7\n` give `got 25`, `got 125`, `got 7` and nothing else.
