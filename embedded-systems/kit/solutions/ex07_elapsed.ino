// ex07 -- periodic printing with millis(); integer arithmetic to split seconds into m:ss
const unsigned long INTERVAL = 2000;
unsigned long previous = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();
  if (now - previous >= INTERVAL) {
    previous += INTERVAL;                 // += keeps the schedule exact (no drift from loop latency)
    unsigned long totalSeconds = now / 1000;
    unsigned long minutes = totalSeconds / 60;
    unsigned long seconds = totalSeconds % 60;
    Serial.print(minutes);
    Serial.print(':');
    if (seconds < 10) Serial.print('0');  // zero padding
    Serial.println(seconds);
  }
}
