// Serial lab, Exercises 5-6, Arduino B (control station)
int nb_pieces_received = 0;
void setup() { Serial.begin(9600); }
void loop() {
  if (Serial.available() > 0) {
    nb_pieces_received = Serial.read();   // 5, 6, 7 ... exactly the byte A wrote
  }
}
