#include "fsk.hpp"
FSKModulation fsk;

int led = 13;
int ledPin = 9;

void setup() {
  fsk.begin();
}

void loop() {
  fsk.transmite("joao pedro poloni ponce escreveu esse software", 46);
}
