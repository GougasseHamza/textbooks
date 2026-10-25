// Midterm 2023-2024, Exercice 6 : passage a niveau, version corrigee (5 erreurs)
const int CAPTEUR = 2;
const int FEU = 8, SONNERIE = 9;
const unsigned long T_ALARME = 8000, T_CLIGNO = 500;
unsigned long tDebut = 0, tCligno = 0;      // (A) unsigned long, pas int
bool alarme = false, feu = false;

void setup() {
  pinMode(CAPTEUR, INPUT_PULLUP);           // (B) contact vers GND : pull-up interne, sinon entree flottante
  pinMode(FEU, OUTPUT);
  pinMode(SONNERIE, OUTPUT);                // (C) sortie manquante
}

void loop() {
  if (!alarme && digitalRead(CAPTEUR) == LOW) {   // (D) == et non =
    alarme = true; tDebut = millis(); tCligno = millis(); feu = true;
  }
  if (alarme) {                             // (E) pas de ';' apres le if
    if (millis() - tCligno >= T_CLIGNO) { tCligno = millis(); feu = !feu; }
    digitalWrite(FEU, feu);                 // feu rouge clignotant
    digitalWrite(SONNERIE, HIGH);
    if (millis() - tDebut >= T_ALARME) {    // (F) millis() au lieu de delay(8000)
      alarme = false;
      digitalWrite(FEU, LOW); digitalWrite(SONNERIE, LOW);
    }
  }
}
