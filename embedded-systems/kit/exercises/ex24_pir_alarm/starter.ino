// Store alarm FSM: STANDBY / ALERT / COOLDOWN with a PIR sensor and buzzer
const int PIR = 2;       // HIGH while motion is detected
const int BUZZER = 11;
const int GREEN = 13;    // ON in STANDBY
const int RED = 12;      // ON in ALERT
const unsigned long ALERT_TIME = 3000, COOLDOWN_TIME = 2000, BEEP_HALF = 300;

void setup() {
  // TODO
}

void loop() {
  // TODO
}
