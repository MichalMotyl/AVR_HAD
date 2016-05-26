/*! \file arp.c \brief ARP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'arp.c'
// Title		: ARP Protocol Library
// Author		: Pascal Stang
// Created		: 9/7/2004
// Revised		: 7/3/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************
#include <stdio.h>
#include "global.h"
#include "net.h"
#include "nic.h"
#include "arp.h"


/// Single ARP table entry/record
struct ArpEntry
{
	uint32_t ipaddr;			///< remote-note IP address
	struct netEthAddr ethaddr;	///< remote-node ethernet (hardware/mac) address
	uint8_t time;				///< time to live (in ARP table); this is decremented by arpTimer()
};

struct ArpEntry ArpMyAddr;		///< my local interface information (IP and MAC address)
struct ArpEntry ArpTable[ARP_TABLE_SIZE];	///< ARP table of matched IP<->MAC associations


void arpInit(void)
{
    uint8_t i;
	// initialize all ArpTable elements to unused
	for(i=0; i<ARP_TABLE_SIZE; i++)
	{
		ArpTable[i].ipaddr = 0;
		ArpTable[i].time = 0;
	}
}

void arpSetAddress(struct netEthAddr* myeth, uint32_t myip)
{
	// set local address record
	ArpMyAddr.ethaddr = *myeth;
	ArpMyAddr.ipaddr = myip;
}

void arpArpIn(unsigned int len, struct netEthArpHeader* packet)
{
	#ifdef ARP_DEBUG
	rprintfProgStrM("Received ARP Request\r\n");
	arpPrintHeader( &packet->arp );
	#endif

	// for now, we just reply to requests
	// need to add ARP cache
	if(	(packet->arp.dipaddr == HTONL(ArpMyAddr.ipaddr)) &&
		(packet->arp.opcode == htons(ARP_OPCODE_REQUEST)) )
	{

		// in ARP header
		// copy sender's address info to dest. fields
        packet->arp.dhwaddr = packet->arp.shwaddr;
		packet->arp.dipaddr = packet->arp.sipaddr;
		// fill in our information
        packet->arp.shwaddr = ArpMyAddr.ethaddr;
        packet->arp.sipaddr = HTONL(ArpMyAddr.ipaddr);
		// change op to reply
		packet->arp.opcode = htons(ARP_OPCODE_REPLY);
		
//        printf("arp reply to:%x.%x.%x.%x\n\r",
//                ((unsigned char*)&packet->arp.dhwaddr)[3],
//                ((unsigned char*)&packet->arp.dhwaddr)[2],
//                ((unsigned char*)&packet->arp.dhwaddr)[1],
//                ((unsigned char*)&packet->arp.dhwaddr)[0]);

//        printf("arp reply to:%d.%d.%d.%d\n\r",
//                ((unsigned char*)&packet->arp.dipaddr)[3],
//                ((unsigned char*)&packet->arp.dipaddr)[2],
//                ((unsigned char*)&packet->arp.dipaddr)[1],
//                ((unsigned char*)&packet->arp.dipaddr)[0]);

		// in ethernet header
        packet->eth.dest = packet->eth.src;
        packet->eth.src  = ArpMyAddr.ethaddr;

		// send reply!
        nicSend(len, (uint8_t*)packet);
	}
}

void arpIpIn(struct netEthIpHeader* packet)
{
	int8_t index;

	// check if sender is already present in arp table
	index = arpMatchIp(HTONL(packet->ip.srcipaddr));
	if(index != -1)
	{
		// sender's IP address found, update ARP entry
		ArpTable[index].ethaddr = packet->eth.src;
		// and we're done
		return;
	}

	// sender was not present in table,
	// must add in empty/expired slot
	for(index=0; index<ARP_TABLE_SIZE; index++)
	{
		if(!ArpTable[index].time)
		{
            //("AddSender...\n\r");
			// write entry
			ArpTable[index].ethaddr = packet->eth.src;
			ArpTable[index].ipaddr = HTONL(packet->ip.srcipaddr);
			ArpTable[index].time = ARP_CACHE_TIME_TO_LIVE;
			// and we're done
			return;
		}
	}
	// no space in table, we give up
}

void arpIpOut(struct netEthIpHeader* packet, uint32_t phyDstIp)
{
    int index = -1;
	// check if destination is already present in arp table
	// use the physical dstIp if it's provided, otherwise the dstIp in packet
	if(phyDstIp)
		index = arpMatchIp(phyDstIp);
	else
		index = arpMatchIp(HTONL(packet->ip.destipaddr));
	// fill in ethernet info
	if(index != -1)
	{
		// ARP entry present, fill eth address(es)
		packet->eth.src  = ArpMyAddr.ethaddr;
		packet->eth.dest = ArpTable[index].ethaddr;
		packet->eth.type = HTONS(ETHTYPE_IP);
	}
	else
	{
		// not in table, must send ARP request
		packet->eth.src = ArpMyAddr.ethaddr;
		// MUST CHANGE, but for now, send this one broadcast
		packet->eth.dest.addr[0] = 0xFF;
		packet->eth.dest.addr[1] = 0xFF;
		packet->eth.dest.addr[2] = 0xFF;
		packet->eth.dest.addr[3] = 0xFF;
		packet->eth.dest.addr[4] = 0xFF;
		packet->eth.dest.addr[5] = 0xFF;
		packet->eth.type = HTONS(ETHTYPE_IP);
	}
}

void arpTimer(void)
{
	int index;
	// this function meant to be called on a regular time interval

	// decrement time-to-live for all entries
	for(index=0; index<ARP_TABLE_SIZE; index++)
	{
		if(ArpTable[index].time)
			ArpTable[index].time--;
	}
}

int arpMatchIp(uint32_t ipaddr)
{
	uint8_t i;

	// check if IP address is present in arp table
	for(i=0; i<ARP_TABLE_SIZE; i++)
	{
		if(ArpTable[i].ipaddr == ipaddr)
		{
			// IP address found
			return i;
		}
	}
	// no match
	return -1;
}

