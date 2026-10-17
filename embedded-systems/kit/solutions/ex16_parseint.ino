// ex16 -- parseInt() skips non-digits, accumulates digits, stops at the first non-digit (the '\n')
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    int c = Serial.peek();                 // look without removing
    if (c >= '0' && c <= '9') {
      long value = Serial.parseInt();      // 25, 125, 7 ... (as a number, not characters)
      Serial.print("got ");
      Serial.println(value);
    } else {
      Serial.read();                       // discard '\n' or anything that is not a digit
    }
  }
}
