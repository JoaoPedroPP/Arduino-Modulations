#include "ask.hpp"

#define TX_PIN_ASK      (5) // Pino de trasmissao dos dados

#define TIMER_CLOCK_SELECT       (4) //timer 4 (controls pin 8, 7, 6)
#define MICROS_PER_TIMER_COUNT_ASK   (clockCyclesToMicroseconds(64)) // Numero de microsegundos para o Timer 4 dar overflow

#define BIT_PERIOD_ASK             (1000000/BAUD_RATE_ASK) //E 1.000.000 porque esta em microsegundos. 976us
#define FREQ_MICROS_ASK            (1000000/FREQUENCY_ASK) // Tamanho do pulso (PWM) na frequencia alta. 100us

#define FREQ_CNT_ASK          (BIT_PERIOD_ASK/FREQ_MICROS_ASK) // 9,76

#define MAX_CARRIR_BITS_ASK        (40000/BIT_PERIOD_ASK) // 40ms

#define TCNT_BIT_PERIOD_ASK        (BIT_PERIOD_ASK/MICROS_PER_TIMER_COUNT_ASK) // 15,25
#define TCNT_FREQ_ASK              (FREQ_MICROS_ASK/MICROS_PER_TIMER_COUNT_ASK) // 1,5625

#define TCNT_TH_L_ASK         (TCNT_FREQ_ASK * 0.80)
#define TCNT_TH_H_ASK         (TCNT_FREQ_ASK * 1.15)

ASKModulation::ASKModulation(){
}

ASKModulation::~ASKModulation(){
}

void ASKModulation::begin(){
    pinMode(TX_PIN_ASK, OUTPUT);
    digitalWrite(TX_PIN_ASK, LOW);

    _txPortReg = portOutputRegister(digitalPinToPort(TX_PIN_ASK));
	_txPortMask = digitalPinToBitMask(TX_PIN_ASK);

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
        cnt = (uint8_t)(FREQ_CNT_ASK);
        tcnt2 = (uint8_t)(TCNT_FREQ_ASK / 2);
        tcnt = (uint8_t)(TCNT_FREQ_ASK) - tcnt2;
    }
    else {
        cnt = (uint8_t)(FREQ_CNT_ASK);
        tcnt2 = (uint8_t)(TCNT_FREQ_ASK / 2);
        tcnt = (uint8_t)(TCNT_FREQ_ASK) - tcnt2;
    }
    do {
        cnt--;
        {
            OCR2B += tcnt; // Output Compare Register, quando o timer for igual ao OC2R a saida sera modificada conforme a COM2B
            TIFR2 |= _BV(OCF2B); //Timer Interrupt Flag, atualiza TIFR2 com a operacao ou com o OCF2B andadando um bit para a esquerda
            while(!(TIFR2 & _BV(OCF2B))); //transmite parte alta
        }
        if (data) *_txPortReg ^= _txPortMask;
        {
            OCR2B += tcnt2;
            TIFR2 |= _BV(OCF2B);
            while(!(TIFR2 & _BV(OCF2B))); //transmite parte baixa
        }
        if (data) *_txPortReg ^= _txPortMask;
    } while(cnt);
}

size_t ASKModulation::transmite(const uint8_t *buffer, size_t length){
    uint8_t cnt = ((micros()- _TempoUltimaTransmissao)/BIT_PERIOD_ASK);
    if (cnt > MAX_CARRIR_BITS_ASK){
        cnt = MAX_CARRIR_BITS_ASK;
    }

    size_t n = length;
    while(length--){ // Aqui e onde a magia acontece
        uint8_t data = *buffer++;
        // uint8_t data = 0x253;
        for(uint8_t mask = 1; mask; mask <<= 1){
            if (data & mask){
                modulate(HIGH);
            }
            else {
                modulate(LOW);
            }
        }

        // modulate(HIGH);
    }
    _TempoUltimaTransmissao = micros();
    return n;
}

size_t ASKModulation::transmite(uint8_t data){
    return transmite(&data, sizeof(data));
}