#include <avr/io.h>
#include <stdio.h>

#include "uart.h"

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

void debugInit(void)
{
     initSerialPort();
     stdout = &uart_str;
}



