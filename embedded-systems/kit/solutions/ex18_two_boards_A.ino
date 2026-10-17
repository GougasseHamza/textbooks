// ex18 board A -- production station: sends the raw byte value every 2 s
int production = 5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.write(production);   // ONE byte with the numeric value (not its decimal text)
  production++;
  delay(2000);
}
