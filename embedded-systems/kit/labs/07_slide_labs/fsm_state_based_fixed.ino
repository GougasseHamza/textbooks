// Canvas assignment "FSM State Based Reasoning": the given code, corrected. The 8 bugs are marked FIX.
const int LED = 13;
const int PB = 12;
const unsigned long TIMEOUT = 2000;          // FIX 1: TIMEOUT was never defined
enum class State { IDLE, ACTIVE, BLINK };
State currentState = State::IDLE;
unsigned long initialTime = 0;
unsigned long lastBlink = 0;

// ---- debounced button: true once per valid press ----
const unsigned long DEBOUNCE_DELAY = 50;
int buttonState = HIGH, lastReading = HIGH;
unsigned long lastDebounceTime = 0;
int debouncingButton(int pin) {
  int reading = digitalRead(pin);
  int pressed = 0;
  if (reading != lastReading) lastDebounceTime = millis();
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) pressed = 1;
  }
  lastReading = reading;
  return pressed;
}

// FIX 2: debouncingButton() was called but never written (above: the lecture's debounce, returns 1 once per press)

// ============= Actions =========
void turnOff()   { digitalWrite(LED, LOW); }
void turnOn()    { digitalWrite(LED, HIGH); }          // FIX 3: was declared turnON but called turnOn (C++ is case-sensitive)
void updateLED() { digitalWrite(LED, !digitalRead(LED)); }
void blink() {
  unsigned long now = millis();
  if (now - lastBlink >= 500) { updateLED(); lastBlink = now; }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(PB, INPUT_PULLUP);
  turnOff();
}

void loop() {
  int pressed = debouncingButton(PB);
  switch (currentState) {                              // FIX 4: the '{' after switch(...) was missing
    case State::IDLE:
      if (pressed == 1) { turnOn(); currentState = State::ACTIVE; }
      break;
    case State::ACTIVE:
      if (pressed == 1) {
        currentState = State::BLINK;
        initialTime = millis();                        // FIX 5: was 'intialTime' (typo)
        lastBlink = initialTime;                       // FIX 6: start the blink timer when entering BLINK (blink() call here did nothing useful)
      }
      break;
    case State::BLINK: {                               // FIX 7: braces needed: a variable ('now') cannot be declared directly under a case label
      unsigned long now = millis();
      if (now - initialTime < TIMEOUT && pressed == 0) {
        blink();
      } else {
        turnOff();
        currentState = State::IDLE;
      }
      break;
    }
  }                                                    // FIX 8: closing brace of the switch
}
