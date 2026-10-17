// ex13 -- Arduino -> PC. println() sends the decimal digits as ASCII text plus "\r\n".
int nb_pieces = 5;

void setup() {
  Serial.begin(9600);        // 9600 bits/s: about 1 ms per character
}

void loop() {
  Serial.println(nb_pieces);
  nb_pieces++;
  delay(3000);
}
