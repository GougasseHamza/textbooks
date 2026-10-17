// Floating-pin lab, Solution B: external 10 kohm pull-down to GND, button between 5 V and D3. Pressed = HIGH (active-high).
const int Pin_Input = 3;
const int LED = 13;

void setup() {
  pinMode(Pin_Input, INPUT);          // plain INPUT: the external resistor defines the idle level (LOW)
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(Pin_Input) == HIGH) {  // button to 5 V closes: HIGH
    Serial.println("Button Pressed");
    digitalWrite(LED, HIGH);
  } else {
    Serial.println("Button Released");
    digitalWrite(LED, LOW);
  }
  delay(50);
}
