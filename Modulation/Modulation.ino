#include "fsk.hpp"
FSKModulation fsk;

int led = 13;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  if (fsk.helloworld() == 'b'){
      digitalWrite(led, HIGH);
      delay(300);
      digitalWrite(led, LOW);
      delay(300);
    }

}
