// millis lab, Exercise 2: LED on pin 12, toggled every 500 ms with millis(), message on each change
const int LED = 12;
const unsigned long INTERVAL = 500;      // Q2-3 used 1000 with delay(), then millis(); Q4 asks 500
unsigned long previous = 0;
bool ledOn = false;
void setup() { pinMode(LED, OUTPUT); Serial.begin(9600); }
void loop() {
  unsigned long now = millis();
  if (now - previous >= INTERVAL) {
    previous = now;
    ledOn = !ledOn;
    digitalWrite(LED, ledOn);
    Serial.println(ledOn ? "LED ON" : "LED OFF");   // Q5
  }
}
