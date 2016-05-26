void initInputDevice();
uint8_t getButtonsStatus();
void readButtons();

#define BUTTON_S1           0x01
#define BUTTON_S1_HOLD      0x10

#define BUTTON_S2           0x02
#define BUTTON_S2_HOLD      0x20

#define BUTTON_S3           0x04
#define BUTTON_S3_HOLD      0x40

#define BUTTON_S4           0x08
#define BUTTON_S4_HOLD      0x80

#define DEBOUNCE_TIME       10//



#define BUTTON_S1_STATUS   (PINB & (1<<PINB2))
//i = (DS18B20_PIN & (1<<DS18B20_DQ));
#define BUTTON_S2_STATUS   (PINB & (1<<PINB0))
#define BUTTON_S3_STATUS   (PIND & (1<<PIND6))
#define BUTTON_S4_STATUS   ((PIND & PIND5)>>PIND5)

