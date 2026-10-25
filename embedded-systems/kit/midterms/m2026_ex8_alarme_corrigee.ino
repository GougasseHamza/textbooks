// Midterm 2 (2026), Exercice 8d : alarme anti-intrusion corrigee (delai de sortie + arret sirene 3 min)
const int CLE = 2, PIR = 3, LED_V = 8, LED_R = 9, SIRENE = 10, BUZZER = 11;
const unsigned long T_SORTIE = 30000UL, T_ENTREE = 15000UL, T_SIRENE = 180000UL, T_CLIGNO = 500, T_DEBOUNCE = 50;
enum class Etat { DESARME, SORTIE, ARME, ENTREE, ALARME, MEMO };
Etat etat = Etat::DESARME;
unsigned long tEntree = 0, tCligno = 0, tChange = 0;
int lecturePrec = HIGH, etatStable = HIGH; bool led = false;
bool cle() {                       // bouton a cle vers GND : un front = une action
  int l = digitalRead(CLE);
  if (l != lecturePrec) { tChange = millis(); lecturePrec = l; }
  if (millis() - tChange > T_DEBOUNCE && l != etatStable) { etatStable = l; return l == LOW; }
  return false;
}
bool pir() { return digitalRead(PIR) == HIGH; }
void changer(Etat e) { etat = e; tEntree = millis(); }
void setup() {
  pinMode(CLE, INPUT_PULLUP); pinMode(PIR, INPUT);
  pinMode(LED_V, OUTPUT); pinMode(LED_R, OUTPUT); pinMode(SIRENE, OUTPUT); pinMode(BUZZER, OUTPUT);
}
void loop() {
  bool k = cle(); unsigned long now = millis(), ecoule = now - tEntree;
  if (now - tCligno >= T_CLIGNO) { tCligno = now; led = !led; }
  switch (etat) {
    case Etat::DESARME:
      digitalWrite(LED_V, HIGH); digitalWrite(LED_R, LOW); digitalWrite(SIRENE, LOW); digitalWrite(BUZZER, LOW);
      if (k) changer(Etat::SORTIE);
      break;
    case Etat::SORTIE:                                 // delai de sortie : PIR ignore 30 s
      digitalWrite(LED_V, LOW); digitalWrite(LED_R, led); digitalWrite(BUZZER, LOW);
      if (k) changer(Etat::DESARME);
      else if (ecoule >= T_SORTIE) changer(Etat::ARME);
      break;
    case Etat::ARME:
      digitalWrite(LED_R, HIGH);
      if (k) changer(Etat::DESARME);
      else if (pir()) changer(Etat::ENTREE);
      break;
    case Etat::ENTREE:                                 // delai d'entree : 15 s pour taper la cle
      digitalWrite(LED_R, led); digitalWrite(BUZZER, led);
      if (k) changer(Etat::DESARME);
      else if (ecoule >= T_ENTREE) changer(Etat::ALARME);
      break;
    case Etat::ALARME:
      digitalWrite(SIRENE, HIGH); digitalWrite(LED_R, led); digitalWrite(BUZZER, LOW);
      if (k) changer(Etat::DESARME);
      else if (ecoule >= T_SIRENE) changer(Etat::MEMO); // reglementation : sirene coupee apres 3 min
      break;
    case Etat::MEMO:                                   // sirene coupee, intrusion memorisee, toujours arme
      digitalWrite(SIRENE, LOW); digitalWrite(LED_R, led);
      if (k) changer(Etat::DESARME);
      else if (pir()) changer(Etat::ALARME);
      break;
  }
}
