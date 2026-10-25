// Midterm 2022-2023, Exercice 5 : lampe a trois modes (blancs completes)
const int BOUTON = 12, LED = 13;
const unsigned long T_BLINK = 500, T_DEBOUNCE = 50;
enum class Mode { OFF, ON, BLINK };
Mode mode = Mode::OFF;
bool led = LOW;
unsigned long tBlink = 0;
int lecturePrec = HIGH, etatStable = HIGH;
unsigned long tChange = 0;

bool appui() {                       // vrai une seule fois par appui
  int l = digitalRead(BOUTON);
  if (l != lecturePrec) { tChange = millis(); lecturePrec = l; }
  if (millis() - tChange > T_DEBOUNCE && l != etatStable) {   // (1) T_DEBOUNCE
    etatStable = l;
    return etatStable == LOW;                                 // (2) LOW : front descendant = appui
  }
  return false;
}

void setup() {
  pinMode(BOUTON, INPUT_PULLUP);                              // (3) INPUT_PULLUP (bouton vers GND)
  pinMode(LED, OUTPUT);
}

void loop() {
  if (appui()) {
    switch (mode) {
      case Mode::OFF:   mode = Mode::ON; break;
      case Mode::ON:    mode = Mode::BLINK;                   // (4) Mode::BLINK
                        led = HIGH; tBlink = millis(); break;
      case Mode::BLINK: mode = Mode::OFF; break;
    }
  }
  switch (mode) {
    case Mode::OFF: digitalWrite(LED, LOW); break;
    case Mode::ON:  digitalWrite(LED, HIGH); break;           // (5) HIGH
    case Mode::BLINK:
      if (millis() - tBlink >= T_BLINK) { tBlink = millis(); led = !led; }   // (6) !led
      digitalWrite(LED, led);
      break;
  }
}
