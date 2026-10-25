// Midterm 2 (2026), Exercice 9 : alarme de magasin (blancs completes)
const int PIR = 2, LED_VERTE = 11, LED_ROUGE = 12, BUZZER = 13;
const unsigned long T_ALERTE = 3000, T_COOLDOWN = 2000, T_BIP = 300;
enum class Etat { STANDBY, ALERT, COOLDOWN };
Etat etat = Etat::STANDBY;
unsigned long tEntree = 0;

void changerEtat(Etat e, const char *nom) {
  etat = e;
  tEntree = millis();                                   // (1)
  Serial.print("State: "); Serial.println(nom);
}
void setup() {
  Serial.begin(9600);
  pinMode(PIR, INPUT);                                  // (2) le capteur impose son niveau
  pinMode(LED_VERTE, OUTPUT); pinMode(LED_ROUGE, OUTPUT); pinMode(BUZZER, OUTPUT);
  changerEtat(Etat::STANDBY, "STANDBY");
}
void loop() {
  unsigned long ecoule = millis() - tEntree;
  switch (etat) {
    case Etat::STANDBY:
      digitalWrite(LED_VERTE, HIGH); digitalWrite(LED_ROUGE, LOW); digitalWrite(BUZZER, LOW);
      if (digitalRead(PIR) == HIGH)                      // (3)
        changerEtat(Etat::ALERT, "ALERT");
      break;
    case Etat::ALERT:
      digitalWrite(LED_VERTE, LOW); digitalWrite(LED_ROUGE, HIGH);
      digitalWrite(BUZZER, (ecoule / T_BIP) % 2 == 0);   // (4)
      if (ecoule >= T_ALERTE)                            // (5)
        changerEtat(Etat::COOLDOWN, "COOLDOWN");
      break;
    case Etat::COOLDOWN:
      digitalWrite(LED_ROUGE, LOW);
      digitalWrite(BUZZER, LOW);                         // (6)
      if (ecoule >= T_COOLDOWN)
        changerEtat(Etat::STANDBY, "STANDBY");           // (7)
      break;
  }
}
