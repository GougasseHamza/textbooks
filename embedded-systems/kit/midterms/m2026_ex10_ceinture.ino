// Midterm 2 (2026), Exercice 10 : rappel de ceinture (sketch complet)
const int SIEGE = 2, CEINTURE = 3, MOTEUR = 4, VOYANT = 8, BUZZER = 9;   // entrees vers GND, INPUT_PULLUP
const unsigned long T_VISUEL = 10000, T_SONORE = 90000UL, T_CLIGNO = 500;
enum class Etat { REPOS, VISUEL, SONORE, MUET };
Etat etat = Etat::REPOS;
unsigned long tEntree = 0;

bool siegeOccupe()     { return digitalRead(SIEGE) == LOW; }      // a) actif = LOW
bool ceintureBouclee() { return digitalRead(CEINTURE) == LOW; }
bool moteurAllume()    { return digitalRead(MOTEUR) == LOW; }
bool alerte()          { return moteurAllume() && siegeOccupe() && !ceintureBouclee(); }
void changer(Etat e) { etat = e; tEntree = millis(); }

void setup() {
  pinMode(SIEGE, INPUT_PULLUP); pinMode(CEINTURE, INPUT_PULLUP); pinMode(MOTEUR, INPUT_PULLUP);
  pinMode(VOYANT, OUTPUT); pinMode(BUZZER, OUTPUT);
}
void loop() {
  unsigned long ecoule = millis() - tEntree;
  bool phase = (ecoule / T_CLIGNO) % 2 == 0;          // 1 Hz : 500 ms ON / 500 ms OFF, sans delay
  if (etat != Etat::REPOS && !alerte()) changer(Etat::REPOS);   // retour immediat depuis tout etat d'alerte
  switch (etat) {
    case Etat::REPOS:
      digitalWrite(VOYANT, LOW); digitalWrite(BUZZER, LOW);
      if (alerte()) changer(Etat::VISUEL);
      break;
    case Etat::VISUEL:
      digitalWrite(VOYANT, phase); digitalWrite(BUZZER, LOW);
      if (ecoule >= T_VISUEL) changer(Etat::SONORE);
      break;
    case Etat::SONORE:
      digitalWrite(VOYANT, phase); digitalWrite(BUZZER, phase);
      if (ecoule >= T_SONORE) changer(Etat::MUET);
      break;
    case Etat::MUET:
      digitalWrite(VOYANT, HIGH); digitalWrite(BUZZER, LOW);
      break;
  }
}
