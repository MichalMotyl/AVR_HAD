/*! \file icmp.c \brief ICMP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'icmp.c'
// Title		: ICMP (Internet Control Message Protocol) Protocol Library
// Author		: Pascal Stang
// Created		: 9/10/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include "global.h"
#include "net.h"
#include "nic.h"
#include "arp.h"
#include "icmp.h"
#include "debug.h"


void icmpInit(void)
{
}

void icmpIpIn(icmpip_hdr* packet)
{
	// check ICMP type
	switch(packet->icmp.type)
	{
	case ICMP_TYPE_ECHOREQUEST:
		// echo request
		icmpEchoRequest(packet);
		break;
	default:
		break;
	}
}

void icmpEchoRequest(icmpip_hdr* packet)
{
	uint32_t tempIp;
	
	// change type to reply
	packet->icmp.type = ICMP_TYPE_ECHOREPLY;
	// recalculate checksum
	packet->icmp.icmpchksum = 0;
    packet->icmp.icmpchksum = netChecksum((uint8_t*)&packet->icmp, htons(packet->ip.len)-IP_HEADER_LEN);
	// return to sender
	tempIp = packet->ip.destipaddr;
	packet->ip.destipaddr = packet->ip.srcipaddr;
	packet->ip.srcipaddr = tempIp;
	// add ethernet routing
    arpIpOut((struct netEthIpHeader*)(((uint8_t*)packet)-ETH_HEADER_LEN), 0);
	
	// debugging
	#if NET_DEBUG >= 2
		icmpPrintHeader(packet);
        //debugPrintHexTable(htons(packet->ip.len), (uint8_t*)packet);
	#endif
	
	// send it (packet->ip.len+ETH_HEADER_LEN
    nicSend(htons(packet->ip.len)+ETH_HEADER_LEN, (((uint8_t*)packet)-ETH_HEADER_LEN));
}

#ifdef ICMP_DEBUG_PRINT
void icmpPrintHeader(icmpip_hdr* packet)
{
    printfProgStrM("ICMP Packet:\r\n");
	// print source IP address
    printfProgStrM("SrcIpAddr: ");	netPrintIPAddr(htonl(packet->ip.srcipaddr));	printfCRLF();
	// print dest IP address
    printfProgStrM("DstIpAddr: ");	netPrintIPAddr(htonl(packet->ip.destipaddr));	printfCRLF();
	// print type
    printfProgStrM("Type   : ");
	switch(packet->icmp.type)
	{
    case ICMP_TYPE_ECHOREQUEST:		printfProgStrM("ECHO REQUEST"); break;
    case ICMP_TYPE_ECHOREPLY:		printfProgStrM("ECHO REPLY"); break;
    default:						printfProgStrM("UNKNOWN"); break;
	}
    printfCRLF();
	// print code
    printfProgStrM("Code   : 0x");	printfuint8_t(packet->icmp.icode);			printfCRLF();
}
#endif
