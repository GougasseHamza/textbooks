// ex19 board B -- analyses the received value and acknowledges with 1 or 0
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    int received = Serial.read();
    if (received >= 10) Serial.write(1);
    else                Serial.write(0);
  }
}
