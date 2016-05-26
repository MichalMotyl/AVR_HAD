#include <avr/io.h>

void initOutputDevice();

uint8_t getOutputStatus();

void setOutputStatus(uint8_t val);

#define SYSTEM_LED_PORT         PORTB
#define SYSTEM_LED_PIN          PB5

#define SysLedOn()              (SYSTEM_LED_PORT |=(1<<SYSTEM_LED_PIN))
#define SysLedOff()             (SYSTEM_LED_PORT &=~(1<<SYSTEM_LED_PIN))
#define SysLedToggle()          (PINB |=(1<<PINB5))

//#define Led1On()              (SYSTEM_LED_PORT |=(1<<SYSTEM_LED_PIN))
//#define Led1Off()             (SYSTEM_LED_PORT &=~(1<<SYSTEM_LED_PIN))
//#define SysLedToggle()          (PINB |=(1<<PINB5))
