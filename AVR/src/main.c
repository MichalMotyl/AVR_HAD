#include <avr/io.h>
#include <stdio.h>
#include <system.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "output.h"
#include "debug.h"
#include "config.h"
#include "dhcp.h"
#include "netstack.h"
#include "config.h"


//FUSES =
//    {
//        .low = LFUSE_DEFAULT,
//        .high = (FUSE_BOOTSZ0 & FUSE_BOOTSZ1 & FUSE_EESAVE & FUSE_SPIEN),
//        .extended = EFUSE_DEFAULT,
//    };

//========= FUSE SETTINGS =========
//  SUT1 = 1
//  SUT0 = 1
//  CKSEL3 = 1
//  CKSEL0 = 1
//  SPIEN = 1
//  f_osc = 16MHz

int main(void) {

    uint8_t cnt1 = 100;

    debugInit();

    TimerSetup();

    LoadConfig();

    initInputDevice();

    netstackInit(ntohb4(getConfigData()->static_ip),ntohb4(getConfigData()->netmask),ntohb4(getConfigData()->netmask));

    if(getConfigData()->dhcp) dhcpInit();

        for(;;)
        {
            netstackService();

            if(getTimerOverflowFlag())//10ms
            {
                clearOverflowFlag();
                TCNT0 = TMR0_VALUE;

                readButtons();

                if(getButtonsStatus())
                {//read buttons status and set outputs according to current output status

                }

                if(!cnt1--)//1s
                {
                    cnt1 = 100;

                    arpTimer();

                    if(getConfigData()->dhcp) handleDHCP();
                }
            }
        }
    return 0;
}



