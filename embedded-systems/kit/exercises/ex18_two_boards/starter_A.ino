// Arduino A sends a counter to Arduino B (two boards) -- board A
// Board A: int production = 5; send every 2 s with Serial.write()
// Board B: LED_BUILTIN ON when received value >= 10

void setup() {
  Serial.begin(9600);
}

void loop() {
  // TODO
}
