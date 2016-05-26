#include "enc28j60.h"
#include "netstack.h"
#include "config.h"

//static uint8_t mymac[6];// = {'m','o','t','y','l','1'};

void nicInit(void)
{
    //*mymac = *getConfigData()->mac;
    //TODO check if mac addres is valid

    enc28j60Init(getConfigData()->mac);
}

void nicSend(uint16_t len, uint8_t* packet)
{
    //enc28j60PacketSend(len, packet);
    //packetSend(len,packet);
    enc28j60PacketSend(len,packet);
}

uint16_t nicPoll(uint8_t* packet)
{
    return enc28j60PacketReceive(NETSTACK_BUFFERSIZE,packet);
}

uint8_t getLinkStatus()
{
    return enc28j60linkup();
}

void nicGetMacAddress(uint8_t* macaddr)
{
    // read MAC address registers
    // NOTE: MAC address in ENC28J60 is byte-backward
 //   getMacAddr(macaddr);
    enc28j60GetMacAddr(macaddr);
}

