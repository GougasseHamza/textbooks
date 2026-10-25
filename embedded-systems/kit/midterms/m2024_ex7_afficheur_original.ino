// Midterm 2023-2024, Exercice 7 : programme tel quel (pour la trace)
void setup() { Serial.begin(9600); }
void loop() {
  if (Serial.available() > 0) {
    char type = Serial.read();
    int valeur = Serial.parseInt();
    switch (type) {
      case 'Q': Serial.print("Quai "); Serial.println(valeur); break;
      case 'R': Serial.print("Retard : "); Serial.print(valeur); Serial.println(" min");
      case 'A': Serial.println("Annule"); break;
      default: Serial.println("Commande inconnue");
    }
  }
}
