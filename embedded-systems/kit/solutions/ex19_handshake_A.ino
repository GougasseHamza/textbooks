// ex19 board A -- sends a request value, lights its LED according to B's answer
const int LED = LED_BUILTIN;
int value = 15;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 1000) {     // one request per second, non-blocking
    lastSend = millis();
    Serial.write(value);
  }
  if (Serial.available() > 0) {          // B's answer: raw byte 1 or 0
    int reply = Serial.read();
    digitalWrite(LED, reply == 1 ? HIGH : LOW);
  }
}
