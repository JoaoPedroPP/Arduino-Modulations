#ifndef psk_h
#define psk_h

#include <Arduino.h>

#define BAUD_RATE_PSK   (1225) // Taxa de trasmissao de simbolos
#define FREQUENCY_PSK (4900)

class PSKModulation {
    private:
        volatile uint8_t *_txPortReg;
	    uint8_t _txPortMask;
        unsigned long _TempoUltimaTransmissao;
        void modulate(uint8_t data);

    public:
        PSKModulation();
        ~PSKModulation();
        void begin();
        virtual size_t transmite(const uint8_t *buffer, size_t size);
        virtual size_t transmite(uint8_t data);
};

#endif