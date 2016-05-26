/*! \file ip.c \brief IP (Internet Protocol) Library. */
//*****************************************************************************
//
// File Name	: 'ip.c'
// Title		: IP (Internet Protocol) Library
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

#include "global.h"
#include "net.h"
#include "nic.h"
#include "arp.h"
#include "ip.h"
#include "netstack.h"

struct ipConfig IpMyConfig;	///< Local IP address/config structure


void ipSetConfig(uint32_t myIp, uint32_t netmask, uint32_t gatewayIp)
{
	struct netEthAddr ethaddr;

#if 0
    printf("myIp:%d.%d.%d.%d\n\r",
            ((unsigned char*)&myIp)[3],
            ((unsigned char*)&myIp)[2],
            ((unsigned char*)&myIp)[1],
            ((unsigned char*)&myIp)[0]);

    printf("netmask:%d.%d.%d.%d\n\r",
            ((unsigned char*)&netmask)[3],
            ((unsigned char*)&netmask)[2],
            ((unsigned char*)&netmask)[1],
            ((unsigned char*)&netmask)[0]);

    printf("gatewayIp:%d.%d.%d.%d\n\r",
            ((unsigned char*)&gatewayIp)[3],
            ((unsigned char*)&gatewayIp)[2],
            ((unsigned char*)&gatewayIp)[1],
            ((unsigned char*)&gatewayIp)[0]);
#endif

	// set local addressing
	IpMyConfig.ip = myIp;
	IpMyConfig.netmask = netmask;
	IpMyConfig.gateway = gatewayIp;

	// set ARP association
    nicGetMacAddress(ethaddr.addr);
	arpSetAddress(&ethaddr, myIp);
}

struct ipConfig* ipGetConfig(void)
{
	return &IpMyConfig;
}

void ipSend(uint32_t dstIp, uint8_t protocol, uint16_t len,uint8_t* data)
{
	// make pointer to ethernet/IP header
	struct netEthIpHeader* ethIpHeader;

	// move data pointer to make room for headers
	data -= ETH_HEADER_LEN+IP_HEADER_LEN;
	ethIpHeader = (struct netEthIpHeader*)data;

	// adjust length to add IP header
	len += IP_HEADER_LEN;

    ethIpHeader->eth.dest = ethIpHeader->eth.src;

	// fill IP header
	ethIpHeader->ip.destipaddr = HTONL(dstIp);
	ethIpHeader->ip.srcipaddr = HTONL(IpMyConfig.ip);
	ethIpHeader->ip.proto = protocol;
	ethIpHeader->ip.len = htons(len);
	ethIpHeader->ip.vhl = 0x45;
	ethIpHeader->ip.tos = 0;
    ethIpHeader->ip.ipid = 0;
    ethIpHeader->ip.ipoffset = 0;
	ethIpHeader->ip.ttl = IP_TIME_TO_LIVE;
	ethIpHeader->ip.ipchksum = 0;

	// calculate and apply IP checksum
	// DO THIS ONLY AFTER ALL CHANGES HAVE BEEN MADE TO IP HEADER
	ethIpHeader->ip.ipchksum = netChecksum(&ethIpHeader->ip, IP_HEADER_LEN);

	// add ethernet routing
	// check if we need to send to gateway
    if( (dstIp & IpMyConfig.netmask) == (IpMyConfig.ip & IpMyConfig.netmask) )
    {
        //local send
        arpIpOut(ethIpHeader,0);
    }
    else
    {
        // gateway send
        arpIpOut(ethIpHeader,IpMyConfig.gateway);
    }

	// adjust length to add ethernet header
	len += ETH_HEADER_LEN;

	nicSend(len, data);
}

//void ipSendUdp(struct netEthIpHeader* ethIpHeader, struct netUdpHeader *udp_packet, uint16_t len, uint8_t* udp_data)
//{
//    // make pointer to ethernet/IP header
//    //data should point to data to send
//    uint16_t i;
//    struct netEthIpHeader* tmp_ethIpHeader;
//    uint8_t *tx_buff = netstackGetBuffer();

//    // move data pointer to make room for headers
//    //data += ETH_HEADER_LEN+IP_HEADER_LEN;
//    tmp_ethIpHeader = (struct netEthIpHeader*)tx_buff;

//    // adjust length to add IP header
//    len += IP_HEADER_LEN;

//    //nicGetMacAddress(ethIpHeader->eth.src.addr);

//    // fill IP header
//    tmp_ethIpHeader->ip.destipaddr = HTONL(dstIp);
//    tmp_ethIpHeader->ip.srcipaddr = HTONL(IpMyConfig.ip);
//    tmp_ethIpHeader->ip.proto = protocol;
//    tmp_ethIpHeader->ip.len = htons(len);
//    tmp_ethIpHeader->ip.vhl = 0x45;
//    tmp_ethIpHeader->ip.tos = 0;
//    tmp_ethIpHeader->ip.ipid = 0;
//    tmp_ethIpHeader->ip.ipoffset = 0;
//    tmp_ethIpHeader->ip.ttl = IP_TIME_TO_LIVE;
//    tmp_ethIpHeader->ip.ipchksum = 0;

//    //set pointer to udp header
//    tx_buff = &tx_buff[ETH_HEADER_LEN+IP_HEADER_LEN];
//    *tx_buff = *udp_packet;
//    //fill buffer with data
//    //add buffer size check
//    tx_buff+=UDP_HEADER_LEN;
//    len+=UDP_HEADER_LEN;

//    for(i = 0;i<len;i++)
//    {
//       *tx_buff = *udp_data;
//    }

//    // calculate and apply IP checksum
//    // DO THIS ONLY AFTER ALL CHANGES HAVE BEEN MADE TO IP HEADER
//    ethIpHeader->ip.ipchksum = netChecksum(&ethIpHeader->ip, IP_HEADER_LEN);

//    // add ethernet routing
//    // check if we need to send to gateway
//    if( (dstIp & IpMyConfig.netmask) == (IpMyConfig.ip & IpMyConfig.netmask) )
//    {
//        arpIpOut(ethIpHeader,0);					// local send
//      //  IP_IP_DEBUG("Sending IP packet on local net\r\n");
//    }
//    else
//    {
//        arpIpOut(ethIpHeader,IpMyConfig.gateway);	// gateway send
//      //  IP_DEBUG("Sending IP packet to gateway\r\n");
//    }

//    // adjust length to add ethernet header
//    len += ETH_HEADER_LEN;

//    // send it
//    nicSend(len, data);
//}

void udpSend(uint32_t dstIp, uint16_t dstPort, uint16_t srcPort, uint16_t len, uint8_t* data)
{
	// make pointer to UDP header
    struct netUdpHeader* udpHeader;
	// move data pointer to make room for UDP header
	data -= UDP_HEADER_LEN;
	udpHeader = (struct netUdpHeader*)data;
	// adjust length to add UDP header
	len += UDP_HEADER_LEN;
	// fill UDP header
	udpHeader->destport = htons(dstPort);
	udpHeader->srcport  = htons(srcPort);
	udpHeader->udplen = htons(len);
	udpHeader->udpchksum = 0;

	ipSend(dstIp, IP_PROTO_UDP, len, (uint8_t*)udpHeader);
}


void ipPrintConfig(struct ipConfig* config)
{
    //IP_DEBUG("IP Addr : 0x%x ",config->ip);
    //IP_DEBUG("Netmask : 0x%x",config->netmask);
    //IP_DEBUG("Gateway : 0x%x",config->gateway);
}
