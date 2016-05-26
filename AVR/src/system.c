#include <avr/io.h>
#include <stdio.h>

#include "system.h"


void TimerSetup()
{
    TCCR0A = 0x00;
    TCCR0B = 0x05;//prescaler 1024
    TCNT0 = TMR0_VALUE;
    //OCR0A = 157;
}


void IOsetup(char type,char pin,char *port)
{


}
