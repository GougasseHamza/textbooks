// Midterm 2 (2026), Exercice 4c : compteur de pieces, anti-rebond logiciel 30 ms sur le fin de course
const int BP1 = 2, FC = 3, LED_VERTE = 8, LED_ROUGE = 9;
const unsigned long T_DEBOUNCE = 30;
int fcStable = LOW, fcLecturePrec = LOW;   // pull-down externe : repos LOW, actionne HIGH
unsigned long fcChange = 0;
unsigned long pieces = 0;

bool fcActive() {                          // true une seule fois par activation
  int l = digitalRead(FC);
  if (l != fcLecturePrec) { fcChange = millis(); fcLecturePrec = l; }
  if (millis() - fcChange >= T_DEBOUNCE && l != fcStable) {
    fcStable = l;
    return l == HIGH;                      // front montant stable = une piece
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  pinMode(BP1, INPUT_PULLUP); pinMode(FC, INPUT);
  pinMode(LED_VERTE, OUTPUT); pinMode(LED_ROUGE, OUTPUT);
}
void loop() {
  if (fcActive()) { pieces++; Serial.print("Pieces: "); Serial.println(pieces); }
}
