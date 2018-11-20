#include "ask.hpp"

#define TX_PIN      (5) // Pino de trasmissao dos dados

#define TIMER_CLOCK_SELECT       (4) //timer 4 (controls pin 8, 7, 6)
#define MICROS_PER_TIMER_COUNT   (clockCyclesToMicroseconds(64)) // Numero de microsegundos para o Timer 4 dar overflow

#define BIT_PERIOD             (1000000/BAUD_RATE) //E 1.000.000 porque esta em microsegundos. 976us
#define FREQ_MICROS            (1000000/FREQUENCY) // Tamanho do pulso (PWM) na frequencia alta. 100us

#define FREQ_CNT          (BIT_PERIOD/FREQ_MICROS) // 9,76

#define MAX_CARRIR_BITS        (40000/BIT_PERIOD) // 40ms

#define TCNT_BIT_PERIOD        (BIT_PERIOD/MICROS_PER_TIMER_COUNT) // 15,25
#define TCNT_FREQ              (FREQ_MICROS/MICROS_PER_TIMER_COUNT) // 1,5625

#define TCNT_TH_L         (TCNT_FREQ * 0.80)
#define TCNT_TH_H         (TCNT_FREQ * 1.15)

ASKModulation::ASKModulation(){
}

ASKModulation::~ASKModulation(){
}

void ASKModulation::begin(){
    pinMode(TX_PIN, OUTPUT);
    digitalWrite(TX_PIN, LOW);

    _txPortReg = portOutputRegister(digitalPinToPort(TX_PIN));
	_txPortMask = digitalPinToBitMask(TX_PIN);

    TCCR2A = 0;
	TCCR2B = TIMER_CLOCK_SELECT;
	ACSR   = _BV(ACIE) | _BV(ACIS1);
	DIDR1  = _BV(AIN1D) | _BV(AIN0D); // digital port off
}

void ASKModulation::modulate(uint8_t data){
    uint8_t cnt;
    uint8_t tcnt;
    uint8_t tcnt2;

    if (data){
        cnt = (uint8_t)(FREQ_CNT);
        tcnt2 = (uint8_t)(TCNT_FREQ / 2);
        tcnt = (uint8_t)(TCNT_FREQ) - tcnt2;
    }
    else {
        cnt = (uint8_t)(FREQ_CNT);
        tcnt2 = (uint8_t)0;
        tcnt = (uint8_t)0;
    }
    do {
        cnt--;
        {
            OCR2B += tcnt; // Output Compare Register, quando o timer for igual ao OC2R a saida sera modificada conforme a COM2B
            TIFR2 |= _BV(OCF2B); //Timer Interrupt Flag, atualiza TIFR2 com a operacao ou com o OCF2B andadando um bit para a esquerda
            while(!(TIFR2 & _BV(OCF2B))); //transmite parte alta
        }
        *_txPortReg ^= _txPortMask;
        {
            OCR2B += tcnt2;
            TIFR2 |= _BV(OCF2B);
            while(!(TIFR2 & _BV(OCF2B))); //transmite parte baixa
        }
        *_txPortReg ^= _txPortMask;
    } while(cnt);
}

size_t ASKModulation::transmite(const uint8_t *buffer, size_t length){
    uint8_t cnt = ((micros()- _TempoUltimaTransmissao)/BIT_PERIOD);
    if (cnt > MAX_CARRIR_BITS){
        cnt = MAX_CARRIR_BITS;
    }

    size_t n = length;
    while(length--){ // Aqui e onde a magia acontece
    uint8_t data = *buffer++;
        for(uint8_t mask = 1; mask; mask <<= 1){
            if (data & mask){
                modulate(HIGH);
            }
            else {
                modulate(LOW);
            }
        }
        _TempoUltimaTransmissao = micros();
        return n;
    }
}