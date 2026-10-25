// Midterm 2 (2026), Exercice 6 : trois boutons, trois LEDs (blancs completes)
const int BP1 = 2, BP2 = 4, BP3 = 9;
const int LED1 = 3, LED2 = 8, LED3 = 10;
void setup() {
  pinMode(BP1, INPUT);          // (1) pull-down externe 10 k vers GND, bouton vers 5 V
  pinMode(BP2, INPUT);          // (2) pull-up externe 10 k vers 5 V, bouton vers GND
  pinMode(BP3, INPUT_PULLUP);   // (3) pull-up interne, bouton vers GND
  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT); pinMode(LED3, OUTPUT);
}
void loop() {
  digitalWrite(LED1, digitalRead(BP1) == HIGH ? HIGH : LOW);   // (4) appuye = HIGH
  digitalWrite(LED2, digitalRead(BP2) == LOW  ? HIGH : LOW);   // (5) appuye = LOW
  digitalWrite(LED3, digitalRead(BP3) == LOW  ? HIGH : LOW);   // (6) appuye = LOW
}
