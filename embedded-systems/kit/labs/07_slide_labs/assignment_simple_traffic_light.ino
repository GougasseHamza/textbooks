// Canvas assignment "Simple Traffic Light V2": red 6, yellow 5, green 4; red 5 s, green 5 s, yellow 2 s, forever (delay-based, as taught in Lecture 1)
const int RED = 6, YELLOW = 5, GREEN = 4;

void setLights(int r, int y, int g) {       // one helper instead of 3 writes everywhere
  digitalWrite(RED, r); digitalWrite(YELLOW, y); digitalWrite(GREEN, g);
}
void setup() { pinMode(RED, OUTPUT); pinMode(YELLOW, OUTPUT); pinMode(GREEN, OUTPUT); }
void loop() {
  setLights(HIGH, LOW, LOW);  delay(5000);   // RED
  setLights(LOW, LOW, HIGH);  delay(5000);   // GREEN
  setLights(LOW, HIGH, LOW);  delay(2000);   // YELLOW
}
