// Midterm 2023-2024, Exercice 7 : afficheur de quai, version corrigee
void setup() { Serial.begin(9600); }
void loop() {
  if (Serial.available() > 0) {
    char type = Serial.read();
    if (type == '\n' || type == '\r' || type == ' ') return;   // defaut 2 : ignorer le '\n' restant
    int valeur = Serial.parseInt();
    switch (type) {
      case 'Q': Serial.print("Quai "); Serial.println(valeur); break;
      case 'R': Serial.print("Retard : "); Serial.print(valeur); Serial.println(" min"); break;  // defaut 1 : break manquant
      case 'A': Serial.println("Annule"); break;
      default:  Serial.println("Commande inconnue");
    }
  }
}
