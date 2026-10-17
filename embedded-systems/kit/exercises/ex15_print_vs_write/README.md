# ex15 print() versus write()

For every byte received, send back on one line: the byte as a decimal number (`Serial.print(b)`), a space, then the same byte raw (`Serial.write(b)`), then a newline.
Sending `A` must produce `65 A`. Sending the digit `0` produces `48 0`.

Checked: inputs A, 0, 1, 2, B produce `65 A`, `48 0`, `49 1`, `50 2`, `66 B`.
What the test really verifies: `print(65)` sends the two characters '6','5' (bytes 54 53); `write(65)` sends one byte 65 which the monitor shows as 'A'.
