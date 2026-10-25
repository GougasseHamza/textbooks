// Midterm 2 (2026), Exercice 5, extrait C corrige : convoyeur commande par le moniteur serie
const int MOTEUR = 8;
void setup() { Serial.begin(9600); pinMode(MOTEUR, OUTPUT); }
void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r' || c == ' ') return;      // le '\n' de "Nouvelle ligne" n'est pas une commande
    if (c == 'S')      digitalWrite(MOTEUR, LOW);        // == et non =
    else if (c == 'M') digitalWrite(MOTEUR, HIGH);       // commande explicite de marche
  }
}
