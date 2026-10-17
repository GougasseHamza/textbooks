// Serial lab, Exercise 7, Arduino B: remote machine, LED shows the state
void setup() { Serial.begin(9600); pinMode(LED_BUILTIN, OUTPUT); }
void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    if (cmd == 1) digitalWrite(LED_BUILTIN, HIGH);
    else if (cmd == 0) digitalWrite(LED_BUILTIN, LOW);
  }
}
