#include <avr/io.h>
#include <stdio.h>

static uint8_t relayStatus = 0;

void initOutputDevice()
{
    DDRB |= (1<<DDB5);

    relayStatus = 0;
    //init relay outputs
}

uint8_t getOutputStatus()
{
    return relayStatus;
}

void setOutputStatus(uint8_t val)
{
    relayStatus = val;
}
