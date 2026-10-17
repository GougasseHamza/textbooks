# ex18 Arduino A -> Arduino B (two boards)

Wiring: A.TX(1) -> B.RX(0), A.RX(0) <- B.TX(1), GND to GND. Both at 9600 baud.
Board A: `production` starts at 5 and is sent with `Serial.write()` every 2 s, then incremented.
Board B: receives each value with `Serial.read()`; LED_BUILTIN ON when the value is >= 10, OFF otherwise.

Files: `sketch_A.ino` and `sketch_B.ino`. Checked: bytes 5,6,7,... leave A at 0,2,4,... s; B's LED turns on when 10 arrives (t = 10 s) and stays on.
Why `write()` and not `print()`? `print(10)` would send '1','0' (49, 48): B would read 49 and 48, never 10.
