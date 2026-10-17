// ex22 -- table-driven FSM, non-blocking version of the lab's Listing 1
enum class State { GREEN, YELLOW, RED };
enum class Event { TIMEOUT, BLINK };

struct Transition {
  State from;
  Event event;
  State to;
  void (*action)();          // function pointer executed during the transition
};

const int ledRED = 6, ledYELLOW = 5, ledGREEN = 4;
const unsigned long RED_TIME = 5000, GREEN_TIME = 5000, YELLOW_TIME = 3000, BLINK_HALF = 100;

State currentState = State::RED;
unsigned long stateEnteredAt = 0;   // replaces delay(): when did the current state begin?
unsigned long blinkTime = 0;        // last yellow toggle

// ---- actions (entry of each state) ----
void turnOnRed() {
  digitalWrite(ledRED, HIGH); digitalWrite(ledGREEN, LOW); digitalWrite(ledYELLOW, LOW);
  stateEnteredAt = millis();
}
void turnOnGreen() {
  digitalWrite(ledRED, LOW); digitalWrite(ledGREEN, HIGH); digitalWrite(ledYELLOW, LOW);
  stateEnteredAt = millis();
}
void turnOnYellow() {
  digitalWrite(ledRED, LOW); digitalWrite(ledGREEN, LOW); digitalWrite(ledYELLOW, HIGH);
  stateEnteredAt = millis();
  blinkTime = stateEnteredAt;
}
void blinkYellow() {
  if (millis() - blinkTime >= BLINK_HALF) {
    blinkTime = millis();
    digitalWrite(ledYELLOW, !digitalRead(ledYELLOW));
  }
}

Transition transitions[] = {
  { State::RED,    Event::TIMEOUT, State::GREEN,  &turnOnGreen  },
  { State::GREEN,  Event::TIMEOUT, State::YELLOW, &turnOnYellow },
  { State::YELLOW, Event::BLINK,   State::YELLOW, &blinkYellow  },   // self-loop
  { State::YELLOW, Event::TIMEOUT, State::RED,    &turnOnRed    },
};
const int NB_TRANSITIONS = sizeof(transitions) / sizeof(transitions[0]);

// ---- engine ----
void processEvent(Event event = Event::TIMEOUT) {
  for (int i = 0; i < NB_TRANSITIONS; i++) {
    if (transitions[i].from == currentState && transitions[i].event == event) {
      transitions[i].action();
      currentState = transitions[i].to;
      break;
    }
  }
}

unsigned long durationOf(State s) {
  switch (s) {
    case State::RED:    return RED_TIME;
    case State::GREEN:  return GREEN_TIME;
    default:            return YELLOW_TIME;
  }
}

void setup() {
  pinMode(ledRED, OUTPUT); pinMode(ledYELLOW, OUTPUT); pinMode(ledGREEN, OUTPUT);
  turnOnRed();                                  // entry action of the initial state
}

void loop() {
  unsigned long now = millis();
  if (now - stateEnteredAt >= durationOf(currentState)) {
    processEvent(Event::TIMEOUT);               // the only place a timeout is generated
  } else if (currentState == State::YELLOW) {
    processEvent(Event::BLINK);                 // stay in YELLOW, toggle the LED
  }
}
