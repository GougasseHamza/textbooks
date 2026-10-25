// Midterm 2024-2025, Exercice 8 : remplissage de bouteilles (enum + loop complet)
const int BOUTEILLE = 2, NIVEAU = 3, ACQUIT = 4;   // INPUT_PULLUP, LOW = actif
const int VANNE = 8, BOUCHEUSE = 9, CONVOYEUR = 10, LED_DEFAUT = 11;
const unsigned long T_MAX_REMPL = 5000, T_BOUCHAGE = 1500, T_EVAC = 2000;

enum class Etat { ATTENTE, REMPLISSAGE, BOUCHAGE, EVACUATION, DEFAUT };
Etat etat = Etat::ATTENTE;
unsigned long tEntree = 0;
bool actif(int pin) { return digitalRead(pin) == LOW; }
void changer(Etat e) { etat = e; tEntree = millis(); }

void setup() {
  pinMode(BOUTEILLE, INPUT_PULLUP); pinMode(NIVEAU, INPUT_PULLUP); pinMode(ACQUIT, INPUT_PULLUP);
  pinMode(VANNE, OUTPUT); pinMode(BOUCHEUSE, OUTPUT); pinMode(CONVOYEUR, OUTPUT); pinMode(LED_DEFAUT, OUTPUT);
}

void loop() {
  unsigned long ecoule = millis() - tEntree;
  switch (etat) {
    case Etat::ATTENTE:
      if (actif(BOUTEILLE)) { digitalWrite(VANNE, HIGH); changer(Etat::REMPLISSAGE); }
      break;
    case Etat::REMPLISSAGE:
      if (actif(NIVEAU)) { digitalWrite(VANNE, LOW); digitalWrite(BOUCHEUSE, HIGH); changer(Etat::BOUCHAGE); }
      else if (ecoule >= T_MAX_REMPL) { digitalWrite(VANNE, LOW); digitalWrite(LED_DEFAUT, HIGH); changer(Etat::DEFAUT); }
      break;
    case Etat::BOUCHAGE:
      if (ecoule >= T_BOUCHAGE) { digitalWrite(BOUCHEUSE, LOW); digitalWrite(CONVOYEUR, HIGH); changer(Etat::EVACUATION); }
      break;
    case Etat::EVACUATION:
      if (ecoule >= T_EVAC) { digitalWrite(CONVOYEUR, LOW); changer(Etat::ATTENTE); }
      break;
    case Etat::DEFAUT:
      if (actif(ACQUIT)) { digitalWrite(LED_DEFAUT, LOW); changer(Etat::ATTENTE); }
      break;
  }
}
