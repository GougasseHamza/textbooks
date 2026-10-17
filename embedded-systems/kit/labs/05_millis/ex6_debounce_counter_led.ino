// millis lab, Exercise 6: count valid presses (50 ms debounce) and toggle an LED on each one
const int BUTTON = 3;
const int LED = 12;
int count = 0;
bool ledOn = false;

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

void setup() { pinMode(BUTTON, INPUT_PULLUP); pinMode(LED, OUTPUT); Serial.begin(9600); }
void loop() {
  if (buttonPressed(BUTTON)) {
    count++;
    Serial.print("Presses: "); Serial.println(count);
    ledOn = !ledOn; digitalWrite(LED, ledOn);
  }
}
// Q4: during the 5-20 ms of bouncing a naive "if (reading == LOW && last == HIGH)" sees several falling edges
// and counts one press as 2-5 presses. The debounce waits until the reading has been stable for 50 ms.
