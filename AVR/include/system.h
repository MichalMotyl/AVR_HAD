//timer clk = 16000000/1024 = 15625Hz
//timer val = 0.01s/((1/15625))

#define TMR0_VALUE  (255-156)

#define getTimerOverflowFlag()  (TIFR0 & 0x01)
#define clearOverflowFlag()     (TIFR0 |= 0x01)

void TimerSetup();

