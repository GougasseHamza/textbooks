// ex12 -- one-shot timer: remember when it started, stop when RUN_TIME has elapsed
const int BUTTON = 3;
const int LED = 12;
const unsigned long RUN_TIME = 5000;
const unsigned long DEBOUNCE_DELAY = 50;

int buttonState = HIGH, lastReading = HIGH;
unsigned long lastDebounceTime = 0;
bool running = false;
unsigned long startTime = 0;

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
  Serial.begin(9600);
}

void loop() {
  bool pressed = buttonPressed();          // always call it, so its state keeps tracking the button
  unsigned long now = millis();
  if (!running) {
    if (pressed) {                         // a press is only honoured while stopped
      running = true;
      startTime = now;
      digitalWrite(LED, HIGH);
      Serial.println("Machine started");
    }
  } else if (now - startTime >= RUN_TIME) {
    running = false;
    digitalWrite(LED, LOW);
    Serial.println("Machine stopped");
  }
}
