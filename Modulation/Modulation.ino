#include "fsk.hpp"
FSKModulation fsk;

int led = 13;
int ledPin = 9;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  if (fsk.helloworld() == 'a'){
    digitalWrite(13, HIGH);
    delayMicroseconds(100); // Approximately 10% duty cycle @ 1KHz
    digitalWrite(13, LOW);
    delayMicroseconds(100);
//    int i = 0;
//    for (i = 0; i < 256; i++){
//        analogWrite(ledPin, i);
//        delay(1000);
//      }
//    while(i > 0){
//        i--;
//        analogWrite(ledPin, i);
//        delay(1000);
//      }
//      digitalWrite(led, HIGH);
//      delay(300);
//      digitalWrite(led, LOW);
//      delay(300);
    }

}
