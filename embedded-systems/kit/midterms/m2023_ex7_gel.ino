// Midterm 2022-2023, Exercice 7 : distributeur de gel (FSM complete, non bloquante)
const int CAPTEUR_MAIN = 2, NIVEAU_BAS = 3, BTN_MAINT = 4;
const int POMPE = 8, LED_VERTE = 9, LED_ROUGE = 10;
const unsigned long T_DOSE = 800, T_ATTENTE = 2000, T_CLIGNO = 500;   // 1 Hz = 500 ms ON / 500 ms OFF

enum class Etat { PRET, DISTRIB, ATTENTE, VIDE };
Etat etat = Etat::PRET;
unsigned long tEntree = 0, tCligno = 0;
bool ledRouge = false;

bool mainDetectee()   { return digitalRead(CAPTEUR_MAIN) == LOW; }
bool reservoirVide()  { return digitalRead(NIVEAU_BAS) == LOW; }
bool maintenance()    { return digitalRead(BTN_MAINT) == LOW; }
void changer(Etat e)  { etat = e; tEntree = millis(); }

void setup() {
  pinMode(CAPTEUR_MAIN, INPUT);          // le capteur IR a sa propre sortie
  pinMode(NIVEAU_BAS, INPUT_PULLUP);
  pinMode(BTN_MAINT, INPUT_PULLUP);
  pinMode(POMPE, OUTPUT); pinMode(LED_VERTE, OUTPUT); pinMode(LED_ROUGE, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  switch (etat) {
    case Etat::PRET:                                   // (demande : code du case PRET)
      digitalWrite(LED_VERTE, HIGH); digitalWrite(LED_ROUGE, LOW); digitalWrite(POMPE, LOW);
      if (reservoirVide())      changer(Etat::VIDE);     // (4)
      else if (mainDetectee())  changer(Etat::DISTRIB);  // (1)
      break;
    case Etat::DISTRIB:                                // (6) : pompe ON, LED verte eteinte
      digitalWrite(LED_VERTE, LOW); digitalWrite(POMPE, HIGH);
      if (reservoirVide())      changer(Etat::VIDE);     // amelioration d) : securite pendant la dose
      else if (now - tEntree >= T_DOSE) changer(Etat::ATTENTE);   // (2)
      break;
    case Etat::ATTENTE:
      digitalWrite(POMPE, LOW);
      if (now - tEntree >= T_ATTENTE) changer(Etat::PRET);        // (3)
      break;
    case Etat::VIDE:                                   // (demande : code du case VIDE)
      digitalWrite(POMPE, LOW); digitalWrite(LED_VERTE, LOW);
      if (now - tCligno >= T_CLIGNO) { tCligno = now; ledRouge = !ledRouge; digitalWrite(LED_ROUGE, ledRouge); }
      if (maintenance() && !reservoirVide()) changer(Etat::PRET);  // (5) : valide seulement si rempli
      break;
  }
}
