// ex26 -- register-level I/O. pinMode(13, OUTPUT) is DDRB |= (1<<5); digitalWrite(13, HIGH) is PORTB |= (1<<5).
// One register write sets/clears up to 8 pins at once and takes 1 clock cycle instead of ~50 for digitalWrite().
void setup() {
  DDRB |= (1 << PB5);          // PB5 (pin 13) as output
}

void loop() {
  PORTB |= (1 << PB5);         // HIGH
  delay(500);
  PORTB &= ~(1 << PB5);        // LOW   (alternative: PORTB ^= (1 << PB5) toggles)
  delay(500);
}
