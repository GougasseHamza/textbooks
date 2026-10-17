// ex25 -- everything from the course in one sketch: debounce, several millis() timers, a 3-state FSM
const int BUTTON = 3;
const int GREEN = 12;
const int RED = 11;
const unsigned long BLINK = 500, PRODUCT_INTERVAL = 2000, RED_TIME = 5000, DEBOUNCE_DELAY = 50;
const int MAX_PRODUCTS = 10;

enum class State { STOPPED, RUNNING, FINISHING };
State state = State::STOPPED;

unsigned long lastBlink = 0, lastProduct = 0, stateEnteredAt = 0;
int products = 0;
bool greenOn = false;

int buttonState = HIGH, lastReading = HIGH;
unsigned long lastDebounceTime = 0;
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
  pinMode(GREEN, OUTPUT); pinMode(RED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  bool pressed = buttonPressed();     // called every loop so the debouncer keeps tracking
  unsigned long now = millis();

  switch (state) {
    case State::STOPPED:
      if (pressed) {
        state = State::RUNNING;
        products = 0;
        lastBlink = lastProduct = now;
        greenOn = true; digitalWrite(GREEN, HIGH);
      }
      break;

    case State::RUNNING:
      if (now - lastBlink >= BLINK) {                 // task: green blinking
        lastBlink = now; greenOn = !greenOn; digitalWrite(GREEN, greenOn);
      }
      if (now - lastProduct >= PRODUCT_INTERVAL) {    // task: production
        lastProduct = now;
        products++;
        Serial.print("Product produced : "); Serial.println(products);
        if (products >= MAX_PRODUCTS) {               // automatic stop
          state = State::FINISHING;
          stateEnteredAt = now;
          greenOn = false; digitalWrite(GREEN, LOW);
          digitalWrite(RED, HIGH);
        }
      }
      break;

    case State::FINISHING:
      if (now - stateEnteredAt >= RED_TIME) {
        digitalWrite(RED, LOW);
        state = State::STOPPED;
      }
      break;
  }
}
