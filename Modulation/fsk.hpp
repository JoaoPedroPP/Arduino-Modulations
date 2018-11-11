#ifndef fsk_h
#define fsk_h

#include <Arduino.h>

#define BAUD_RATE   (1024)
#define HIGH_FREQUENCY (10000)
#define LOW_FREQUENCY (5000)

class FSKModulation {
    private:
        volatile uint8_t *_txPortReg;
	    uint8_t _txPortMask;
        void modulate(uint8_t data);
    
    public:
        FSKModulation();
        ~FSKModulation();
        char helloworld();
        void write();
};

#endif