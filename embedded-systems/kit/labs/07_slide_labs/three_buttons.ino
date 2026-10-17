// Lecture 2, Lab 3 "Three-button challenge": each button uses a different pull configuration, each drives its own LED.
//  button 1: pin 2, EXTERNAL PULL-DOWN (10 k to GND, button to 5 V)    -> pressed reads HIGH
//  button 2: pin 3, EXTERNAL PULL-UP   (10 k to 5 V, button to GND)    -> pressed reads LOW
//  button 3: pin 4, INTERNAL PULL-UP   (INPUT_PULLUP, button to GND)   -> pressed reads LOW
const int BTN_PULLDOWN = 2, BTN_EXT_PULLUP = 3, BTN_INT_PULLUP = 4;
const int LED1 = 10, LED2 = 11, LED3 = 12;

void setup() {
  pinMode(BTN_PULLDOWN, INPUT);          // level defined by the external resistor
  pinMode(BTN_EXT_PULLUP, INPUT);        // level defined by the external resistor
  pinMode(BTN_INT_PULLUP, INPUT_PULLUP); // level defined by the chip's internal resistor
  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT); pinMode(LED3, OUTPUT);
}

void loop() {
  bool p1 = (digitalRead(BTN_PULLDOWN) == HIGH);    // active-high
  bool p2 = (digitalRead(BTN_EXT_PULLUP) == LOW);   // active-low
  bool p3 = (digitalRead(BTN_INT_PULLUP) == LOW);   // active-low
  digitalWrite(LED1, p1);
  digitalWrite(LED2, p2);
  digitalWrite(LED3, p3);
}
