// ex01 -- Blink at 1 Hz (period 1000 ms, duty cycle 50 %)
const int LED = LED_BUILTIN;   // pin 13 = PB5 on the ATmega328P

void setup() {
  pinMode(LED, OUTPUT);        // push-pull output driver
}

void loop() {
  digitalWrite(LED, HIGH);     // 5 V on the pin: current flows through the 1 kohm resistor and the LED
  delay(500);                  // blocking wait: the CPU does nothing else for 500 ms
  digitalWrite(LED, LOW);
  delay(500);
}
