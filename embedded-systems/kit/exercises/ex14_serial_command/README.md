# ex14 Commands from the PC: '1' starts, '0' stops

The built-in LED represents a machine. Read characters from Serial without blocking (`Serial.available()` then `Serial.read()`):
character `'1'` turns the LED ON, `'0'` turns it OFF, anything else (including the newline the monitor may send) is ignored.

Checked: the monitor sends `1\n` at 1 s, `0\n` at 2 s, `x\n` at 3 s (ignored), `1` at 4 s: LED ON 1-2 s, OFF 2-4 s, ON after 4 s.
Note the difference between the character '1' (ASCII 49) and the number 1.
