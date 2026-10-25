// Midterm 2023-2024, Exercice 9 : feu tricolore avec bouton pieton et mode nuit (sketch complet)
const int LED_V = 8, LED_O = 9, LED_R = 10, LED_PIETON = 11;
const int BTN_PIETON = 2, SW_NUIT = 4;             // vers GND, INPUT_PULLUP, LOW = actif
const unsigned long T_VERT_MIN = 10000, T_ORANGE = 3000, T_ROUGE = 8000, T_CLIGNO = 500, T_DEBOUNCE = 50;

enum class Etat { VERT, ORANGE, ROUGE, NUIT };
Etat etat = Etat::VERT;
unsigned long tEntree = 0, tCligno = 0, tChange = 0;
bool demandePieton = false, orange = false;        // b) l'appui est memorise ici
int lecturePrec = HIGH, etatStable = HIGH;

bool nuit() { return digitalRead(SW_NUIT) == LOW; }
bool appuiPieton() {                                // vrai une fois par appui (anti-rebond)
  int l = digitalRead(BTN_PIETON);
  if (l != lecturePrec) { tChange = millis(); lecturePrec = l; }
  if (millis() - tChange > T_DEBOUNCE && l != etatStable) { etatStable = l; return l == LOW; }
  return false;
}
void feux(bool v, bool o, bool r, bool p) {
  digitalWrite(LED_V, v); digitalWrite(LED_O, o); digitalWrite(LED_R, r); digitalWrite(LED_PIETON, p);
}
void changer(Etat e) { etat = e; tEntree = millis(); }

void setup() {
  pinMode(LED_V, OUTPUT); pinMode(LED_O, OUTPUT); pinMode(LED_R, OUTPUT); pinMode(LED_PIETON, OUTPUT);
  pinMode(BTN_PIETON, INPUT_PULLUP); pinMode(SW_NUIT, INPUT_PULLUP);
  changer(Etat::VERT);
}

void loop() {
  if (appuiPieton()) demandePieton = true;         // memorise meme pendant les 10 s
  unsigned long now = millis(), ecoule = now - tEntree;
  if (etat != Etat::NUIT && nuit()) changer(Etat::NUIT);   // depuis n'importe quel etat
  switch (etat) {
    case Etat::VERT:
      feux(1, 0, 0, 0);
      if (ecoule >= T_VERT_MIN && demandePieton) changer(Etat::ORANGE);
      break;
    case Etat::ORANGE:
      feux(0, 1, 0, 0);
      if (ecoule >= T_ORANGE) changer(Etat::ROUGE);
      break;
    case Etat::ROUGE:
      feux(0, 0, 1, 1);
      if (ecoule >= T_ROUGE) { demandePieton = false; changer(Etat::VERT); }   // demande effacee
      break;
    case Etat::NUIT:
      if (now - tCligno >= T_CLIGNO) { tCligno = now; orange = !orange; }
      feux(0, orange, 0, 0);
      if (!nuit()) { demandePieton = false; changer(Etat::ROUGE); }            // d) on repart de ROUGE
      break;
  }
}
