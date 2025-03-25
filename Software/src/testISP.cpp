#include <Arduino.h>

// ATtiny85 hat 6 nutzbare Pins: 0 bis 5
const int pins[] = {0, 1, 2, 3, 4, 5};
const int numPins = sizeof(pins) / sizeof(pins[0]);

void setup() {
    for (int i = 0; i < numPins; i++) {
        pinMode(pins[i], OUTPUT);  // Setzt alle Pins als Ausgang
    }
}

void loop() {
    for (int i = 0; i < numPins; i++) {
        digitalWrite(pins[i], HIGH);
        delay(100);
        digitalWrite(pins[i], LOW);
        delay(100);
    }
}
