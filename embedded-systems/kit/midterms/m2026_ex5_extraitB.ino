struct Zone { int pin; int active; };
Zone zones[] = { {2, 1}, {3, 1}, {4, 0} };
void setup() {
  Serial.begin(9600);
  Serial.println(sizeof(zones));
  for (int i = 0; i < sizeof(zones) / sizeof(zones[0]); i++)
    if (zones[i].active) Serial.println(zones[i].pin);
}
void loop() {}
