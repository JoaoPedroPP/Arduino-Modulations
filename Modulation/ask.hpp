#ifndef ask_h
#define ask_h

#include <Arduino.h>

#define BAUD_RATE_ASK   (1225) // Taxa de trasmissao de simbolos
#define FREQUENCY_ASK (4900)

class ASKModulation {
    private:
        volatile uint8_t *_txPortReg;
	    uint8_t _txPortMask;
        unsigned long _TempoUltimaTransmissao;
        void modulate(uint8_t data);

    public:
        ASKModulation();
        ~ASKModulation();
        void begin();
        virtual size_t transmite(const uint8_t *buffer, size_t size);
};

#endif