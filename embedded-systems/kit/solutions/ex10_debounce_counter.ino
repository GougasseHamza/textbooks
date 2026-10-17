// ex10 -- the lecture's debounce algorithm: a change is accepted only after it has been stable for DEBOUNCE_DELAY
const int BUTTON = 3;
const unsigned long DEBOUNCE_DELAY = 50;

int buttonState = HIGH;        // last *stable* state (HIGH = released)
int lastReading = HIGH;        // last *raw* reading
unsigned long lastDebounceTime = 0;
int count = 0;

// Returns true exactly once per valid press (falling edge HIGH -> LOW)
bool buttonPressed() {
  int reading = digitalRead(BUTTON);
  bool pressed = false;
  if (reading != lastReading) {          // any change (real or bounce) restarts the timer
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {   // stable long enough
    if (reading != buttonState) {        // and different from the accepted state
      buttonState = reading;
      if (buttonState == LOW) pressed = true;
    }
  }
  lastReading = reading;
  return pressed;
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (buttonPressed()) {
    count++;
    Serial.print("count=");
    Serial.println(count);
  }
}
