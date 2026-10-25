// Midterm 2023-2024, Exercice 8 : passage a niveau, FSM corrigee (second train, panne du capteur de sortie)
const int APPROCHE = 2, SORTIE = 3;                 // contacts vers GND, INPUT_PULLUP, LOW = actif
const int FEUX = 8, SONNERIE = 9, BARRIERE = 10, DEFAUT = 11;   // BARRIERE HIGH = baissee
const unsigned long T_ANNONCE = 8000, T_REOUV = 6000, T_MAX_FERME = 120000UL;
enum class Etat { OUVERT, ANNONCE, FERME, REOUVERTURE, DEFAUT_FERME };
Etat etat = Etat::OUVERT;
unsigned long tEntree = 0;
bool approche() { return digitalRead(APPROCHE) == LOW; }
bool sortie()   { return digitalRead(SORTIE) == LOW; }
void changer(Etat e) { etat = e; tEntree = millis(); }
void setup() {
  pinMode(APPROCHE, INPUT_PULLUP); pinMode(SORTIE, INPUT_PULLUP);
  pinMode(FEUX, OUTPUT); pinMode(SONNERIE, OUTPUT); pinMode(BARRIERE, OUTPUT); pinMode(DEFAUT, OUTPUT);
}
void loop() {
  unsigned long ecoule = millis() - tEntree;
  switch (etat) {
    case Etat::OUVERT:
      digitalWrite(FEUX, LOW); digitalWrite(SONNERIE, LOW); digitalWrite(BARRIERE, LOW);
      if (approche()) changer(Etat::ANNONCE);
      break;
    case Etat::ANNONCE:
      digitalWrite(FEUX, HIGH); digitalWrite(SONNERIE, HIGH);
      if (ecoule >= T_ANNONCE) changer(Etat::FERME);
      break;
    case Etat::FERME:
      digitalWrite(BARRIERE, HIGH); digitalWrite(SONNERIE, LOW);
      if (sortie()) changer(Etat::REOUVERTURE);
      else if (ecoule >= T_MAX_FERME) changer(Etat::DEFAUT_FERME);   // capteur de sortie en panne : on reste ferme, on signale
      break;
    case Etat::REOUVERTURE:
      if (approche()) changer(Etat::FERME);        // second train : on referme AVANT d'avoir fini d'ouvrir (etat sur)
      else if (ecoule >= T_REOUV) { digitalWrite(BARRIERE, LOW); changer(Etat::OUVERT); }
      break;
    case Etat::DEFAUT_FERME:                       // barrieres fermees, feux allumes, voyant defaut : intervention humaine
      digitalWrite(DEFAUT, HIGH); digitalWrite(FEUX, HIGH);
      break;
  }
}
