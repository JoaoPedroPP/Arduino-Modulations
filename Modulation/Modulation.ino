#include "ask.hpp"
#include "fsk.hpp"

FSKModulation fsk;
ASKModulation ask;

int led = 13;
int ledPin = 9;

void setup() {
  ask.begin();
//  fsk.begin();
}

void loop() {
  ask.transmite('U');
//    fsk.transmite('U',1);
//  fsk.transmite("joao pedro poloni ponce escreveu esse software", 46);
}
