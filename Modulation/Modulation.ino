#include "ask.hpp"
#include "fsk.hpp"
#include "psk.hpp"

FSKModulation fsk;
ASKModulation ask;
PSKModulation psk;

int led = 13;
int ledPin = 9;

void setup() {
  // ask.begin();
//  fsk.begin();
    psk.begin();
}

void loop() {
  psk.transmite('U');
  // ask.transmite('U');
//    fsk.transmite('U',1);
//  fsk.transmite("joao pedro poloni ponce escreveu esse software", 46);
}
