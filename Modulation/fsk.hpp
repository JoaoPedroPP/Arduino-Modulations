#ifndef fsk_h
#define fsk_h

#include <Arduino.h>

class FSKModulation {
    private:
        int x;
    
    public:
        FSKModulation();
        ~FSKModulation();
        char helloworld();
};

#endif