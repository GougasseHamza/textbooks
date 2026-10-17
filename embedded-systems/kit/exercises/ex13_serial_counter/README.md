# ex13 Production counter to the PC

`nb_pieces` starts at 5. Every 3 s send its value to the PC with `Serial.println()` and then increment it. Baud 9600.

Checked: lines `5`, `6`, `7`, ... at t = 0, 3, 6, ... s (tolerance 20 ms). (Blocking `delay(3000)` is accepted here; from ex06 on you know the better way.)
