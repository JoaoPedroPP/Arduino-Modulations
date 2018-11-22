#include "qam.hpp"

#define TX_PIN_QAM      (9) // Pino de trasmissao dos dados
#define PHASE_PIN       (8)

#define TIMER_CLOCK_SELECT       (4) //timer 4 (controls pin 8, 7, 6)
#define MICROS_PER_TIMER_COUNT_QAM   (clockCyclesToMicroseconds(64)) // Numero de microsegundos para o Timer 4 dar overflow

#define BIT_PERIOD_QAM             (1000000/BAUD_RATE_QAM) //E 1.000.000 porque esta em microsegundos. 976us
#define FREQ_MICROS_QAM            (1000000/FREQUENCY_QAM) // Tamanho do pulso (PWM) na frequencia alta. 100us

#define FREQ_CNT_QAM          (BIT_PERIOD_QAM/FREQ_MICROS_QAM) // 9,76

#define TCNT_BIT_PERIOD_QAM        (BIT_PERIOD_QAM/MICROS_PER_TIMER_COUNT_QAM) // 15,25
#define TCNT_FREQ_QAM              (FREQ_MICROS_QAM/MICROS_PER_TIMER_COUNT_QAM) // 1,5625

QAMModulation::QAMModulation(){
}

QAMModulation::~QAMModulation(){
}

void QAMModulation::begin(){
    pinMode(TX_PIN_QAM, OUTPUT);
    pinMode(PHASE_PIN, OUTPUT);

    analogWrite(TX_PIN_QAM, 0);
    digitalWrite(PHASE_PIN, LOW);

    _txPortReg = portOutputRegister(digitalPinToPort(TX_PIN_QAM));
	_txPortMask = digitalPinToBitMask(TX_PIN_QAM);

    TCCR2A = 0;
	TCCR2B = TIMER_CLOCK_SELECT;
	ACSR   = _BV(ACIE) | _BV(ACIS1);
	DIDR1  = _BV(AIN1D) | _BV(AIN0D); // digital port off
}

void QAMModulation::modulate(uint8_t data){
    uint8_t cnt;
    uint8_t tcnt;
    uint8_t tcnt2;

    cnt = (uint8_t)(FREQ_CNT_QAM);
    tcnt2 = (uint8_t)(TCNT_FREQ_QAM / 2);
    tcnt = (uint8_t)(TCNT_FREQ_QAM) - tcnt2;
    
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

size_t QAMModulation::transmite(const uint8_t *buffer, size_t length){
    
    size_t n = length;
    while(length--){ // Aqui e onde a magia acontece
        uint8_t data = *buffer++;
        for(uint8_t mask = 1; mask; mask <<= 1){
            if (data & mask){
                digitalWrite(PHASE_PIN, LOW);
                modulate(HIGH);
            }
            else {
                digitalWrite(PHASE_PIN, HIGH);
                modulate(LOW);
            }
        }
    }
    digitalWrite(PHASE_PIN, LOW);
    _TempoUltimaTransmissao = micros();
    return n;
}

size_t QAMModulation::transmite(uint8_t data){
    return transmite(&data, sizeof(data));
}