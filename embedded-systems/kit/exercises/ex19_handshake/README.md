# ex19 Request / acknowledgement between two boards

Board A sends the value 15 with `Serial.write()` once per second. Board B reads each value and replies `1` (raw byte) if the value is >= 10, else `0`.
Board A reads the reply: LED_BUILTIN ON on 1, OFF on 0. (This is the Serial lab's exercises 9, 10 and 11 combined.)

Checked: A sends the byte 15; B answers the byte 1; A's LED is ON afterwards. At 5.5 s the test injects a byte 3 into B's receive buffer
(as if A had sent 3): B must answer 0 and A's LED must go OFF until the next 15 is acknowledged.
