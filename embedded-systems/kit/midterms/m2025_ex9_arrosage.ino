// Midterm 2024-2025, Exercice 9 : arrosage automatique de la serre (sketch complet, non bloquant)
const int CAPT_SOL = 2, FLOTTEUR = 3, BTN_MANUEL = 4, POMPE = 7, LED_ALARME = 9;
const unsigned long T_ARROSAGE_MAX = 30000, T_INFILTRATION = 60000UL, T_CLIGNO = 250;   // 2 Hz = 250 ms

enum class Etat { VEILLE, ARROSAGE, INFILTRATION, MANUEL, ALARME };
Etat etat = Etat::VEILLE;
unsigned long tEntree = 0, tCligno = 0;
bool led = false;

bool solSec()       { return digitalRead(CAPT_SOL) == HIGH; }     // a)
bool cuveVide()     { return digitalRead(FLOTTEUR) == HIGH; }
bool boutonManuel() { return digitalRead(BTN_MANUEL) == LOW; }
void changer(Etat e) { etat = e; tEntree = millis(); }

void setup() {
  pinMode(CAPT_SOL, INPUT); pinMode(FLOTTEUR, INPUT_PULLUP); pinMode(BTN_MANUEL, INPUT_PULLUP);
  pinMode(POMPE, OUTPUT); pinMode(LED_ALARME, OUTPUT);
}

void loop() {
  unsigned long now = millis(), ecoule = now - tEntree;
  switch (etat) {
    case Etat::VEILLE:
      digitalWrite(POMPE, LOW); digitalWrite(LED_ALARME, LOW);
      if (boutonManuel()) changer(Etat::MANUEL);
      else if (solSec() && !cuveVide()) changer(Etat::ARROSAGE);
      break;
    case Etat::ARROSAGE:
      digitalWrite(POMPE, HIGH);
      if (cuveVide()) changer(Etat::ALARME);                              // marche a sec interdite
      else if (!solSec() || ecoule >= T_ARROSAGE_MAX) changer(Etat::INFILTRATION);
      break;
    case Etat::INFILTRATION:
      digitalWrite(POMPE, LOW);
      if (ecoule >= T_INFILTRATION) changer(Etat::VEILLE);
      break;
    case Etat::MANUEL:
      digitalWrite(POMPE, HIGH);
      if (cuveVide()) changer(Etat::ALARME);
      else if (!boutonManuel()) changer(Etat::VEILLE);
      break;
    case Etat::ALARME:
      digitalWrite(POMPE, LOW);
      if (now - tCligno >= T_CLIGNO) { tCligno = now; led = !led; digitalWrite(LED_ALARME, led); }
      if (!cuveVide()) changer(Etat::VEILLE);
      break;
  }
}
