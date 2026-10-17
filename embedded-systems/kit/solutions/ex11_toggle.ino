// ex11 -- edge detection: act on the press event, not on the level
const int BUTTON = 3;
const int LED = 12;
const unsigned long DEBOUNCE_DELAY = 50;

int buttonState = HIGH, lastReading = HIGH;
unsigned long lastDebounceTime = 0;
bool ledOn = false;

bool buttonPressed() {
  int reading = digitalRead(BUTTON);
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
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (buttonPressed()) {          // true only on the falling edge, once per press
    ledOn = !ledOn;
    digitalWrite(LED, ledOn);
  }
}
