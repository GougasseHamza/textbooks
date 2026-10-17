// ex02 -- SOS in Morse code with helper functions
const int LED = LED_BUILTIN;
const int DOT = 200, DASH = 600, GAP = 200, LETTER_GAP = 600, WORD_PAUSE = 1000;

void flash(int ms) {              // one symbol: ON for ms, then the intra-letter gap
  digitalWrite(LED, HIGH);
  delay(ms);
  digitalWrite(LED, LOW);
  delay(GAP);
}
void dot()  { flash(DOT); }
void dash() { flash(DASH); }
void letterGap() { delay(LETTER_GAP - GAP); }   // flash() already waited GAP

void setup() { pinMode(LED, OUTPUT); }

void loop() {
  dot(); dot(); dot();  letterGap();     // S
  dash(); dash(); dash(); letterGap();   // O
  dot(); dot(); dot();                   // S
  delay(WORD_PAUSE - GAP);               // 1 s pause after the word
}
