#include <PSM.h>

const byte valuePin = A3; // potentiometer input

const byte controlPin = 13;  // triac output
const byte interruptPin = 3; // phase sensing

const unsigned int range = 127;

PSM pump(interruptPin, controlPin, range);

const unsigned int valueFactor = 4; // precalculated constant 1024 [10 bit ADC resolution] / 128 [range + 1] / 2
volatile unsigned int value = 0;

void setup() {
}

void updateValue() {
  int readValue = analogRead(valuePin);
  int oldValue = value * valueFactor * 2;

  if (readValue >= (oldValue + valueFactor * 3) || (readValue + valueFactor) < oldValue) { // add some hysteresis to filter out noise
    value = readValue / valueFactor / 2;
    pump.set(value);
  }
}

void loop() {
  updateValue();
  delay(500);
}
