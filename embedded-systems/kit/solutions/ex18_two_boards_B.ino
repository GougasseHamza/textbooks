// ex18 board B -- monitoring station: LED on when production >= 10
const int LED = LED_BUILTIN;
int productionReceived = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    productionReceived = Serial.read();        // the byte A wrote: 5, 6, 7, ...
    digitalWrite(LED, productionReceived >= 10 ? HIGH : LOW);
  }
}
