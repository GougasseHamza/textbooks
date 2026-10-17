# ex09 The LED follows a button (internal pull-up)

A push button is wired between pin 3 and GND, nothing else. Configure pin 3 with the internal pull-up, so the pin reads HIGH when released and LOW when pressed.
The LED on pin 12 must be ON exactly while the button is pressed.

Checked: pin 3 is INPUT_PULLUP (a plain INPUT would float: the simulator returns random noise for a floating input, and the test presses are then unreliable);
pin 12 is HIGH during each press and LOW between presses.
