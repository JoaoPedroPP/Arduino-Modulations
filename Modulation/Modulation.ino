#include "ask.hpp"
#include "fsk.hpp"
// #include "psk.hpp"

FSKModulation fsk;
ASKModulation ask;
// PSKModulation psk;

void setup() {
//   ask.begin();
//  fsk.begin();
}

void loop() {
  ask.transmite(0xc1);
//    fsk.transmite(0xaa);
//  fsk.transmite("joao pedro poloni ponce escreveu esse software", 46);
}
