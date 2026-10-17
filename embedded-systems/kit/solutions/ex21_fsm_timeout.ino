// ex21 -- a timed state: record when it was entered, generate the TIMEOUT event from millis()
const int LED = 13;
const int PB = 12;
const unsigned long TIMEOUT = 2000;
const unsigned long BLINK_PERIOD = 200;   // 100 ms ON, 100 ms OFF
const unsigned long DEBOUNCE_DELAY = 50;

enum class State { IDLE, ACTIVE, BLINKING };
State currentState = State::IDLE;
unsigned long stateEnteredAt = 0;   // when BLINKING began
unsigned long lastBlink = 0;

int buttonState = HIGH, lastReading = HIGH;
unsigned long lastDebounceTime = 0;
bool buttonPressed() {
  int reading = digitalRead(PB);
  bool pressed = false;
  if (reading != lastReading) lastDebounceTime = millis();
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) pressed = true;
  }
  lastReading = reading;
  return pressed;
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(PB, INPUT_PULLUP);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop() {
  bool pressed = buttonPressed();
  unsigned long now = millis();

  switch (currentState) {
    case State::IDLE:
      if (pressed) {
        digitalWrite(LED, HIGH);
        currentState = State::ACTIVE;
        Serial.println("IDLE -> ACTIVE");
      }
      break;

    case State::ACTIVE:
      if (pressed) {
        currentState = State::BLINKING;
        stateEnteredAt = now;            // start the 2 s timeout
        lastBlink = now;
        digitalWrite(LED, LOW);
        Serial.println("ACTIVE -> BLINKING");
      }
      break;

    case State::BLINKING:
      if (pressed || now - stateEnteredAt >= TIMEOUT) {   // two events lead to the same transition
        digitalWrite(LED, LOW);
        currentState = State::IDLE;
        Serial.println("BLINKING -> IDLE");
      } else if (now - lastBlink >= BLINK_PERIOD / 2) {  // stay action: blink
        lastBlink = now;
        digitalWrite(LED, !digitalRead(LED));
      }
      break;
  }
}
