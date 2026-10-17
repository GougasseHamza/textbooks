// Serial lab, Exercise 4: what read(), parseInt() and peek() return. Type a line in the monitor and look.
void setup() {
  Serial.begin(9600);
  Serial.println("Type something (e.g. 5, then 25, then A)");
}

void loop() {
  if (Serial.available() > 0) {
    int p1 = Serial.peek();            // looks at the next byte, does not remove it
    int p2 = Serial.peek();            // same byte again
    Serial.print("peek twice: "); Serial.print(p1); Serial.print(' '); Serial.println(p2);
    int c = Serial.read();             // removes ONE byte, returns its ASCII code
    Serial.print("read(): "); Serial.print(c); Serial.print(" = character '"); Serial.write(c); Serial.println("'");
    if (c >= '0' && c <= '9') {
      // put the digit back conceptually: the rest of the number is still in the buffer
      long rest = Serial.parseInt();   // reads the remaining digits (e.g. "5" of "25") -> combine for a demo
      Serial.print("parseInt() of what follows: "); Serial.println(rest);
    }
    while (Serial.available() > 0 && (Serial.peek() == '\n' || Serial.peek() == '\r')) Serial.read();   // discard line ending
    Serial.print("available() now: "); Serial.println(Serial.available());
  }
}
// Part A: sending 5  -> read() returns 53 (the ASCII code of '5'), not 5.  Sending 25 -> read() returns 50 ('2') only.
// Part B: parseInt() returns 25 and 125 as numbers: the right tool for multi-digit numbers.
// Part C: peek() returns 65 ('A') both times and available() stays 1; after read(), available() is 0.
