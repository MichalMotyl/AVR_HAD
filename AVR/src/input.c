#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "input.h"

static uint8_t button1_cnt;
static uint8_t button2_cnt;
static uint8_t button3_cnt;
static uint8_t button4_cnt;
static uint8_t buttons_status = 0;

void initInputDevice()
{
    button1_cnt = DEBOUNCE_TIME;
    button2_cnt = DEBOUNCE_TIME;
    button3_cnt = DEBOUNCE_TIME;
    button4_cnt = DEBOUNCE_TIME;

    DDRB &= ~(1<<DDB0);
    DDRB &= ~(1<<DDB2);
    DDRD &= ~(1<<DDD6);
}


/**
 * @brief getButtonsStatus
 * @return Status of HW buttons
 */
uint8_t getButtonsStatus()
{
    return buttons_status;
}

/**
 * @brief readB1
 */
static void readB1()
{
    if(!BUTTON_S1_STATUS && (buttons_status & BUTTON_S1)==0)
    {
        if(!button1_cnt--)
        {
            button1_cnt = DEBOUNCE_TIME;
            buttons_status |= BUTTON_S1;
        }
    }
    else
    {
        if(buttons_status & BUTTON_S1)
        {
            if(BUTTON_S1_STATUS)
            {
                button1_cnt = DEBOUNCE_TIME;
                buttons_status &= ~BUTTON_S1;
            }
        }
        else
        button1_cnt = DEBOUNCE_TIME;
    }
}

/**
 * @brief readB2
 */
static void readB2()
{
    if(!BUTTON_S2_STATUS && (buttons_status & BUTTON_S2)==0)
    {
        if(!button2_cnt--)
        {
            button2_cnt = DEBOUNCE_TIME;
            buttons_status |= BUTTON_S2;
        }
    }
    else
    {
        if(buttons_status & BUTTON_S2)
        {
            if(BUTTON_S2_STATUS)
            {
                button2_cnt = DEBOUNCE_TIME;
                buttons_status &= ~0x02;
            }
        }
        else
        button2_cnt = DEBOUNCE_TIME;
    }
}


static void readB3()
{
    if(!BUTTON_S3_STATUS && (buttons_status & BUTTON_S3)==0)
    {
        if(!button3_cnt--)
        {
            button3_cnt = DEBOUNCE_TIME;
            buttons_status |= BUTTON_S3;
        }
    }
    else
    {
        if(buttons_status & BUTTON_S3)
        {
            if(BUTTON_S3_STATUS)
            {
                button3_cnt = DEBOUNCE_TIME;
                buttons_status &= ~BUTTON_S3;
            }
        }
        else
        button3_cnt = DEBOUNCE_TIME;
    }
}


/**
 * @brief readB4
 */
static void readB4()
{
    if(!BUTTON_S4_STATUS && (buttons_status & BUTTON_S4)==0)
    {
        if(!button4_cnt--)
        {
            button4_cnt = DEBOUNCE_TIME;
            buttons_status |= BUTTON_S4;
        }
    }
    else
    {
        if(buttons_status & BUTTON_S4)
        {
            if(BUTTON_S4_STATUS)
            {
                button4_cnt = DEBOUNCE_TIME;
                buttons_status &= ~BUTTON_S4;
            }
        }
        else
        button4_cnt = DEBOUNCE_TIME;
    }
}



/**
 * @brief readButtons
 */
void readButtons()
{
    readB1();
    readB2();
    readB3();
    readB4();
}
