#include <avr/io.h>
#include <stdio.h>

void initSerialPort()
{
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0 = 0;//16MHz 1M
}

int printCHAR(unsigned char character)
{
    while ((UCSR0A & (1 << UDRE0)) == 0);

    UDR0 = character;

    return 0;
}

