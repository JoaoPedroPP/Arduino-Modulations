#include "psk.hpp"

#define TX_PIN_PSK      (6) // Pino de trasmissao dos dados
#define PHASE_PIN       (4)

#define TIMER_CLOCK_SELECT       (4) //timer 4 (controls pin 8, 7, 6)
#define MICROS_PER_TIMER_COUNT_PSK   (clockCyclesToMicroseconds(64)) // Numero de microsegundos para o Timer 4 dar overflow

#define BIT_PERIOD_PSK             (1000000/BAUD_RATE_PSK) //E 1.000.000 porque esta em microsegundos. 976us
#define FREQ_MICROS_PSK            (1000000/FREQUENCY_PSK) // Tamanho do pulso (PWM) na frequencia alta. 100us

#define FREQ_CNT_PSK          (BIT_PERIOD_PSK/FREQ_MICROS_PSK) // 9,76

#define TCNT_BIT_PERIOD_PSK        (BIT_PERIOD_PSK/MICROS_PER_TIMER_COUNT_PSK) // 15,25
#define TCNT_FREQ_PSK              (FREQ_MICROS_PSK/MICROS_PER_TIMER_COUNT_PSK) // 1,5625

PSKModulation::PSKModulation(){
}

PSKModulation::~PSKModulation(){
}

void PSKModulation::begin(){
    pinMode(TX_PIN_PSK, OUTPUT);
    pinMode(PHASE_PIN, OUTPUT);
    
    digitalWrite(TX_PIN_PSK, LOW);
    digitalWrite(PHASE_PIN, LOW);

    _txPortReg = portOutputRegister(digitalPinToPort(TX_PIN_PSK));
	_txPortMask = digitalPinToBitMask(TX_PIN_PSK);
    _phPortReg = portOutputRegister(digitalPinToPort(PHASE_PIN));
	_phPortMask = digitalPinToBitMask(PHASE_PIN);

    TCCR2A = 0;
	TCCR2B = TIMER_CLOCK_SELECT;
	ACSR   = _BV(ACIE) | _BV(ACIS1);
	DIDR1  = _BV(AIN1D) | _BV(AIN0D); // digital port off
}

void PSKModulation::modulate(uint8_t data){
    uint8_t cnt;
    uint8_t tcnt;
    uint8_t tcnt2;

    cnt = (uint8_t)(FREQ_CNT_PSK);
    tcnt2 = (uint8_t)(TCNT_FREQ_PSK / 2);
    tcnt = (uint8_t)(TCNT_FREQ_PSK) - tcnt2;

    // if (data) digitalWrite(PHASE_PIN, LOW);
    // else digitalWrite(PHASE_PIN, HIGH);
    
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

size_t PSKModulation::transmite(const uint8_t *buffer, size_t length){
    
    size_t n = length;
    while(length--){ // Aqui e onde a magia acontece
        uint8_t data = *buffer++;
        // uint8_t data = 0x01;
        for(uint8_t mask = 1; mask; mask <<= 1){
            if (data & mask){
                digitalWrite(PHASE_PIN, LOW);
                // delay(500);
                modulate(HIGH);
            }
            else {
                digitalWrite(PHASE_PIN, HIGH);
                // delay(500);
                modulate(LOW);
            }
        }
    }
    // digitalWrite(PHASE_PIN, LOW);
    _TempoUltimaTransmissao = micros();
    return n;
}

size_t PSKModulation::transmite(uint8_t data){
    return transmite(&data, sizeof(data));
}