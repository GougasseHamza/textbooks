void compter() {
  int a = 0;
  static int b = 0;
  a++; b++;
  Serial.print(a); Serial.print(" "); Serial.println(b);
}
void setup() { Serial.begin(9600); for (int i = 0; i < 3; i++) compter(); }
void loop() {}
