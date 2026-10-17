// Lecture 2, slide "Advanced FSM: Transition Table": IDLE / ACTIVE / BLINKING with events PRESSED, KEEPBLINKING, TIMEOUT.
// Non-blocking: the lecture's delay(200) is replaced by the debounce; blinking is 500 ms, timeout 2 s.
const int LED = 13;
const int PB = 12;
const unsigned long TIMEOUT = 2000, BLINK_PERIOD = 500;

enum class Event { PRESSED, KEEPBLINKING, TIMEOUT };
enum class State { IDLE, ACTIVE, BLINKING };
struct Transition { State from; Event event; State to; void (*action)(); };

State currentState = State::IDLE;
unsigned long stateEnteredAt = 0, lastBlink = 0;

void turnOn()        { digitalWrite(LED, HIGH); }
void turnOff()       { digitalWrite(LED, LOW); }
void startBlinking() { stateEnteredAt = millis(); lastBlink = stateEnteredAt; digitalWrite(LED, LOW); }
void blink()         { if (millis() - lastBlink >= BLINK_PERIOD) { lastBlink = millis(); digitalWrite(LED, !digitalRead(LED)); } }

Transition transitions[] = {
  { State::IDLE,     Event::PRESSED,      State::ACTIVE,   &turnOn        },
  { State::ACTIVE,   Event::PRESSED,      State::BLINKING, &startBlinking },
  { State::BLINKING, Event::TIMEOUT,      State::IDLE,     &turnOff       },
  { State::BLINKING, Event::KEEPBLINKING, State::BLINKING, &blink         },
  { State::BLINKING, Event::PRESSED,      State::IDLE,     &turnOff       },
};
const int NB_TRANSITIONS = sizeof(transitions) / sizeof(transitions[0]);

void processEvent(Event event = Event::PRESSED) {
  for (int i = 0; i < NB_TRANSITIONS; i++) {
    if (transitions[i].from == currentState && transitions[i].event == event) {
      transitions[i].action(); currentState = transitions[i].to; return;
    }
  }
  // no row: the event is ignored in this state (e.g. KEEPBLINKING in IDLE)
}

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

void setup() { pinMode(LED, OUTPUT); pinMode(PB, INPUT_PULLUP); turnOff(); }

void loop() {
  // event generation: the sensors and the clock produce events, the table decides what they mean
  if (buttonPressed(PB)) processEvent(Event::PRESSED);
  if (currentState == State::BLINKING) {
    if (millis() - stateEnteredAt >= TIMEOUT) processEvent(Event::TIMEOUT);
    else processEvent(Event::KEEPBLINKING);
  }
}
