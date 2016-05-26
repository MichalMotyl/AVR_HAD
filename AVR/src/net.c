/*! \file net.c \brief Network support library. */
//*****************************************************************************
//
// File Name	: 'net.c'
// Title		: Network support library
// Author		: Pascal Stang
// Created		: 8/30/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************
#include <stdio.h>
#include <inttypes.h>

#include "debug.h"
#include "global.h"
#include "net.h"

uint16_t htons(uint16_t host)
{
    uint16_t result = 0;
    htonb2(host, (uint8_t*)&result);
    return result;
}

uint32_t htonl(uint32_t host)
{
    uint32_t result = 0;
    htonb4(host, (uint8_t*)&result);
    return result;
}

uint16_t ntohs(uint16_t network)
{
    return ntohb2((uint8_t*)&network);
}

uint32_t ntohl(uint32_t network)
{
    return ntohb4((uint8_t*)&network);
}

void htonb2(uint16_t host, uint8_t* network2)
{
    network2[0] = (host & 0xff00) >> 8;
    network2[1] = host & 0x00ff;
}

uint16_t ntohb2(const uint8_t* network2)
{
    return
        (uint16_t)network2[0] << 8
        | network2[1];
}

void htonb3(uint32_t host, uint8_t* network3)
{
    network3[0] = (host & 0x00ff0000) >> 16;
    network3[1] = (host & 0x0000ff00) >> 8;
    network3[2] = host & 0x000000ff;
}

uint32_t ntohb3(const uint8_t* network3)
{
    return
        (uint32_t)network3[0] << 16
        | (uint32_t)network3[1] << 8
        | network3[2];
}

void htonb4(uint32_t host, uint8_t* network4)
{
    network4[0] = (host & 0xff000000) >> 24;
    network4[1] = (host & 0x00ff0000) >> 16;
    network4[2] = (host & 0x0000ff00) >> 8;
    network4[3] = host & 0x000000ff;
}

uint32_t ntohb4(const uint8_t* network4)
{
    return
        (uint32_t)network4[0] << 24
        | (uint32_t)network4[1] << 16
        | (uint32_t)network4[2] << 8
        | network4[3];
}

uint16_t netChecksum(void *data, uint16_t len)
{
    register uint32_t sum = 0;

    for (;;) {
        if (len < 2)
            break;
		//sum += *((uint16_t *)data)++;
		sum += *((uint16_t *)data);
		data+=2;
        len -= 2;
    }
    if (len)
        sum += *(uint8_t *) data;

    while ((len = (uint16_t) (sum >> 16)) != 0)
        sum = (uint16_t) sum + len;

    return (uint16_t) sum ^ 0xFFFF;
}


void netPrintIPAddr(uint32_t ipaddr)
{
    DEBUG("%d.%d.%d.%d\n\r",
        ((unsigned char*)&ipaddr)[3],
        ((unsigned char*)&ipaddr)[2],
        ((unsigned char*)&ipaddr)[1],
        ((unsigned char*)&ipaddr)[0]);
}


