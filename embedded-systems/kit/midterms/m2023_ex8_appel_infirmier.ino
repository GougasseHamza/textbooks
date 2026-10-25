// Midterm 2022-2023, Exercice 8 : appel infirmier (sketch complet, non bloquant)
const int BTN_APPEL = 2, BTN_PRESENCE = 3;
const int LED_COULOIR = 8, LED_VERTE = 9, BUZZER = 10;
const unsigned long T_DEBOUNCE = 50, T_URGENCE = 60000UL;
const unsigned long T_BIP = 200, T_PERIODE_BIP = 5000, T_LED_1HZ = 500, T_LED_4HZ = 125;

enum class Etat { REPOS, APPEL, URGENCE, PRESENCE };   // declare avant toute fonction qui l'utilise
Etat etat = Etat::REPOS;
unsigned long tEntree = 0, tLed = 0;
bool led = false;

struct Bouton {                 // a) toutes les donnees d'un bouton : la fonction les modifie -> par reference
  byte pin;
  int etatStable;               // dernier niveau stable (HIGH au repos avec INPUT_PULLUP)
  int lecturePrec;              // derniere lecture brute
  unsigned long tChange;        // instant du dernier changement brut
};
Bouton appel    = {BTN_APPEL,    HIGH, HIGH, 0};
Bouton presence = {BTN_PRESENCE, HIGH, HIGH, 0};

bool appui(Bouton &b) {         // vrai une seule fois par appui (front descendant debounce)
  int l = digitalRead(b.pin);
  if (l != b.lecturePrec) { b.tChange = millis(); b.lecturePrec = l; }
  if (millis() - b.tChange > T_DEBOUNCE && l != b.etatStable) {
    b.etatStable = l;
    return l == LOW;
  }
  return false;
}

void changer(Etat e) { etat = e; tEntree = millis(); tLed = millis(); led = false; }

void setup() {
  pinMode(BTN_APPEL, INPUT_PULLUP); pinMode(BTN_PRESENCE, INPUT_PULLUP);
  pinMode(LED_COULOIR, OUTPUT); pinMode(LED_VERTE, OUTPUT); pinMode(BUZZER, OUTPUT);
}

void loop() {
  bool a = appui(appel), p = appui(presence);   // lire les deux boutons a chaque tour
  unsigned long now = millis(), ecoule = now - tEntree;
  switch (etat) {
    case Etat::REPOS:
      digitalWrite(LED_COULOIR, LOW); digitalWrite(LED_VERTE, LOW); digitalWrite(BUZZER, LOW);
      if (a) changer(Etat::APPEL);
      break;
    case Etat::APPEL:
      if (now - tLed >= T_LED_1HZ) { tLed = now; led = !led; digitalWrite(LED_COULOIR, led); }
      digitalWrite(BUZZER, (ecoule % T_PERIODE_BIP) < T_BIP);      // 200 ms toutes les 5 s
      if (p) changer(Etat::PRESENCE);
      else if (ecoule >= T_URGENCE) changer(Etat::URGENCE);
      break;
    case Etat::URGENCE:
      if (now - tLed >= T_LED_4HZ) { tLed = now; led = !led; digitalWrite(LED_COULOIR, led); }
      digitalWrite(BUZZER, HIGH);
      if (p) changer(Etat::PRESENCE);
      break;
    case Etat::PRESENCE:
      digitalWrite(LED_COULOIR, LOW); digitalWrite(BUZZER, LOW); digitalWrite(LED_VERTE, HIGH);
      if (p) changer(Etat::REPOS);                                  // fin des soins
      break;
  }
}
