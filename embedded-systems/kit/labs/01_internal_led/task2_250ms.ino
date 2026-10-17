// Lab internal LED, Task 2.1: both delays 250 ms -> period 500 ms, frequency 2 Hz (was 1 Hz)
void setup() { pinMode(LED_BUILTIN, OUTPUT); }
void loop() {
  digitalWrite(LED_BUILTIN, HIGH); delay(250);
  digitalWrite(LED_BUILTIN, LOW);  delay(250);
}
