#ifndef fsk_h
#define fsk_h

#include <Arduino.h>

#define BAUD_RATE   (1024) // Taxa de trasmissao de simbolos
#define HIGH_FREQUENCY (2000)
#define LOW_FREQUENCY (1000)

class FSKModulation {
    private:
        volatile uint8_t *_txPortReg;
	    uint8_t _txPortMask;
        unsigned long _TempoUltimaTransmissao;
        void modulate(uint8_t data);
    
    public:
        FSKModulation();
        ~FSKModulation();
        char helloworld();
        void begin();
	    // virtual size_t transmite(uint8_t data);
        virtual size_t transmite(const uint8_t *buffer, size_t size);
};

#endif