// millis lab, Exercise 7: three LEDs (500 / 1000 / 2000 ms), "System active" every 5 s, a button enables/disables the blinking
const int LEDS[] = {12, 11, 10};
const unsigned long PERIODS[] = {500, 1000, 2000};
const int NB = 3;
const int BUTTON = 3;
unsigned long previous[NB] = {0, 0, 0};
bool state[NB] = {false, false, false};
unsigned long prevMsg = 0;
bool blinkingEnabled = true;

// ---- debounced button: true once per valid press ----
const unsigned long DEBOUNCE_DELAY = 50;
int buttonState = HIGH, lastReading = HIGH;
unsigned long lastDebounceTime = 0;
bool buttonPressed(int pin) {
  int reading = digitalRead(pin);
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
  for (int i = 0; i < NB; i++) pinMode(LEDS[i], OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}
void loop() {
  unsigned long now = millis();
  if (buttonPressed(BUTTON)) {                       // Q5: enable / disable
    blinkingEnabled = !blinkingEnabled;
    if (!blinkingEnabled) for (int i = 0; i < NB; i++) { state[i] = false; digitalWrite(LEDS[i], LOW); }
  }
  if (blinkingEnabled) {
    for (int i = 0; i < NB; i++) {                   // one timer per LED, in arrays
      if (now - previous[i] >= PERIODS[i]) {
        previous[i] = now;
        state[i] = !state[i];
        digitalWrite(LEDS[i], state[i]);
      }
    }
  }
  if (now - prevMsg >= 5000) {                       // Q4: never interrupted by the button
    prevMsg = now;
    Serial.println("System active");
  }
}
