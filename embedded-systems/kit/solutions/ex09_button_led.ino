// ex09 -- INPUT_PULLUP: released = HIGH, pressed = LOW. Invert the reading to get "pressed".
const int BUTTON = 3;
const int LED = 12;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);   // enables the ~20-50 kohm internal resistor to 5 V
  pinMode(LED, OUTPUT);
}

void loop() {
  bool pressed = (digitalRead(BUTTON) == LOW);
  digitalWrite(LED, pressed ? HIGH : LOW);
}
