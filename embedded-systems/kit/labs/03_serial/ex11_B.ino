// Serial lab, Exercise 11, Station B: when the received count reaches 10, send 1 back
void setup() { Serial.begin(9600); }
void loop() {
  if (Serial.available() > 0) {
    int received = Serial.read();
    if (received >= 10) Serial.write(1);
  }
}
