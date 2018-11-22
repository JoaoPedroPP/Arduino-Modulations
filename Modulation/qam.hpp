#ifndef qam_h
#define qam_h

#include <Arduino.h>

#define BAUD_RATE_QAM   (1225) // Taxa de trasmissao de simbolos
#define FREQUENCY_QAM (4900)

class QAMModulation {
    private:
        volatile uint8_t *_txPortReg;
	    uint8_t _txPortMask;
        unsigned long _TempoUltimaTransmissao;
        void modulate(uint8_t data);

    public:
        QAMModulation();
        ~QAMModulation();
        void begin();
        virtual size_t transmite(const uint8_t *buffer, size_t size);
        virtual size_t transmite(uint8_t data);
};

#endif