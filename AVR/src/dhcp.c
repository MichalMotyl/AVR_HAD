/*! \file dhcp.c \brief DHCP Protocol Library. */
//*****************************************************************************
//
// File Name	: 'dhcp.c'
// Title		: DHCP Protocol Library
// Author		: Pascal Stang
// Created		: 9/17/2005
// Revised		: 9/17/2005
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************

#include "global.h"
#include "net.h"
#include "nic.h"
#include "ip.h"
#include "netstack.h"
#include <stdio.h>
#include "dhcp.h"
#include "ip.h"
#include "enc28j60.h"
#include "debug.h"
#include "config.h"

// global variables
uint32_t DhcpServerIP;		///< IP address of the DHCP server that offered lease
uint32_t DhcpTransactID;	///< Unique transaction ID that identifies DHCP request/replies
uint32_t DhcpLeaseTime;		///< Number of seconds left in DHCP lease
uint8_t dhcp_ip_addr_status = 0;
uint8_t dhcp_request_retry_timer = 0;
uint8_t dhcp_address_refresh = 0;
uint16_t dhcp_address_refresh_cnt = 0;

//static char host_name[DHCP_MAX_HOST_NAME_LEN] = "AVR_LanRelay";

void dhcpInit(void)
{
    uint8_t	macaddr[6];

    enc28j60EnableBroadcast();
	// get interface mac address
    nicGetMacAddress(macaddr);
	// set transaction ID based on mac address
	DhcpTransactID = *((uint32_t*)&macaddr);
	// reset lease time
	DhcpLeaseTime = 0;
    dhcp_ip_addr_status = 0;
    dhcp_address_refresh = 0;
    dhcp_request_retry_timer = 0;
}

/**
 * @brief handleDHCP
 */
void handleDHCP()
{
    if(!enc28j60linkup())
    {
        DHCP_DEBUG("link is down...\n\r");
        return;
    }

    if(dhcp_ip_addr_status)
    {
        dhcpTimer();

        if(!DhcpLeaseTime)
        {
            if(!dhcp_address_refresh)
            {
                dhcp_address_refresh = 1;
                //send DHCPREQUEST to extend lease time
                dhcpRequest();
            }
            else
            {
                if(!dhcp_address_refresh_cnt)
                {//if time is up and we dont recieve ack from server - we need request new address
                    dhcp_ip_addr_status = 0;
                    dhcp_address_refresh = 0;
                    dhcpInit();
                }
                //wait for ack
            }
        }
    }
    else
    {
        if(!dhcp_request_retry_timer)
        {
            dhcp_request_retry_timer=DHCP_RETRY_COUNTER;

            dhcpRequest();
        }
    }
}

void dhcpIn(unsigned int len, struct netDhcpHeader* packet)
{
	uint8_t msgtype;
	uint32_t sid;
	uint8_t* optptr;
    uint32_t val;
	uint32_t netmask;
	uint32_t gateway;

    // check that this is a reply, and for me
    if((packet->bootp.op != BOOTP_OP_BOOTREPLY) || (packet->bootp.xid != DhcpTransactID)) return;
	// process incoming packet
	// check reply type
	dhcpGetOption(packet->options, DHCP_OPT_DHCPMSGTYPE, 1, &msgtype);

    //DHCP_DEBUG("DHCP: Received msgtype = %d\r\n", msgtype);
	
	if(msgtype == DHCP_MSG_DHCPOFFER)
	{
        //dhcp_ip_addr_status = 1;

        //disable broadcast reception when we go offer message from server
        enc28j60DisableBroadcast();

		// get DHCP server ID
		dhcpGetOption(packet->options, DHCP_OPT_SERVERID, 4, &sid);

		// build DHCP request (on top of this reply)
		packet->bootp.op = BOOTP_OP_BOOTREQUEST;		// request type

        packet->bootp.flags = 0;
		// set operation
        val = DHCP_MSG_DHCPREQUEST;
        optptr = dhcpSetOption(packet->options, DHCP_OPT_DHCPMSGTYPE, 1, (uint8_t*)&val);
		// set the server ID
        optptr = dhcpSetOption(optptr, DHCP_OPT_SERVERID, 4, (uint8_t*)&sid);
		// request the IP previously offered
        optptr = dhcpSetOption(optptr, DHCP_OPT_REQUESTEDIP, 4, (uint8_t*)&packet->bootp.yiaddr);
        //send host name
        optptr = dhcpSetOption(optptr, DHCP_OPT_HOSTNAME, DHCP_MAX_HOST_NAME_LEN,getConfigData()->host_name);

		// request additional information
		((uint8_t*)&val)[0] = DHCP_OPT_NETMASK;
		((uint8_t*)&val)[1] = DHCP_OPT_ROUTERS;
		((uint8_t*)&val)[2] = DHCP_OPT_DNSSERVERS;
		((uint8_t*)&val)[3] = DHCP_OPT_DOMAINNAME;

        optptr = dhcpSetOption(optptr, DHCP_OPT_PARAMREQLIST, 4, (uint8_t*)&val);

		// send DHCP request
		DhcpServerIP = htonl(sid);

        //SET SYSTEM IP ADDR AND GTW
        ipSetConfig(ntohl(packet->bootp.yiaddr), NETMASK, ntohl(packet->bootp.giaddr));

        //struct ipConfig *cfg = ipGetConfig();
        //netPrintIPAddr(cfg->ip);

        udpSend(DhcpServerIP, DHCP_UDP_SERVER_PORT,DHCP_UDP_CLIENT_PORT, DHCP_HEADER_LEN+3+6+6+6+1+(DHCP_MAX_HOST_NAME_LEN+2), (uint8_t*)packet);
	}
	else if(msgtype == DHCP_MSG_DHCPACK)
	{
		// get netmask
        dhcp_ip_addr_status = 1;
		dhcpGetOption(packet->options, DHCP_OPT_NETMASK, 4, &val);
		netmask = htonl(val);
		// get gateway
		dhcpGetOption(packet->options, DHCP_OPT_ROUTERS, 4, &val);
		gateway = htonl(val);
		// get gateway
		dhcpGetOption(packet->options, DHCP_OPT_LEASETIME, 4, &val);
		DhcpLeaseTime = htonl(val);
        dhcp_address_refresh_cnt = (uint16_t)(0.87*DhcpLeaseTime);
		// assign new network info
		ipSetConfig(htonl(packet->bootp.yiaddr), netmask, gateway);

        if(dhcp_address_refresh)
        {
            dhcp_address_refresh = 0;
            DHCP_DEBUG("DHCP: Got request refresh ACK!!!\r\n");
        }
        //DHCP_DEBUG("DHCP: Got request ACK, bind complete\r\n");
        DHCP_DEBUG("DHCP: Received lease time %lu dhcp_address_refresh_cnt:%d\r\n", DhcpLeaseTime,dhcp_address_refresh_cnt);
	}
}

void dhcpRequest(void)
{
	struct netDhcpHeader* packet;
	uint32_t val;
    uint8_t *b = netstackGetBuffer();

    packet = (struct netDhcpHeader*)&b[ETH_HEADER_LEN+IP_HEADER_LEN+UDP_HEADER_LEN];

	// build BOOTP/DHCP header
	packet->bootp.op = BOOTP_OP_BOOTREQUEST;		// request type
	packet->bootp.htype = BOOTP_HTYPE_ETHERNET;
	packet->bootp.hlen = BOOTP_HLEN_ETHERNET;
	packet->bootp.ciaddr = htonl(ipGetConfig()->ip);
	packet->bootp.yiaddr = HTONL(0l);
	packet->bootp.siaddr = HTONL(0l);
	packet->bootp.giaddr = HTONL(0l);
	nicGetMacAddress(&packet->bootp.chaddr[0]);	// fill client hardware address
	packet->bootp.xid = DhcpTransactID;

    if(!dhcp_address_refresh)packet->bootp.flags = HTONS(0x8000);//
    else
        packet->bootp.flags = HTONS(0);//
	
	// build DHCP request
	// begin with magic cookie
	packet->cookie = 0x63538263;
	// set operation
	val = DHCP_MSG_DHCPDISCOVER;
    dhcpSetOption(packet->options, DHCP_OPT_DHCPMSGTYPE, 1, (uint8_t*)&val);

	// send request
    udpSend(0xFFFFFFFF, DHCP_UDP_SERVER_PORT,DHCP_UDP_CLIENT_PORT, DHCP_HEADER_LEN+3+1, (uint8_t*)packet);
}

void dhcpRelease(void)
{
	struct netDhcpHeader* packet;
	uint32_t val;
	uint8_t* optptr;
	
	packet = (struct netDhcpHeader*)&netstackGetBuffer()[ETH_HEADER_LEN+IP_HEADER_LEN+UDP_HEADER_LEN];

	// build BOOTP/DHCP header
	packet->bootp.op = BOOTP_OP_BOOTREQUEST;		// request type
	packet->bootp.htype = BOOTP_HTYPE_ETHERNET;
	packet->bootp.hlen = BOOTP_HLEN_ETHERNET;
	packet->bootp.ciaddr = htonl(ipGetConfig()->ip);
	packet->bootp.yiaddr = HTONL(0l);
	packet->bootp.siaddr = HTONL(0l);
	packet->bootp.giaddr = HTONL(0l);
	nicGetMacAddress(&packet->bootp.chaddr[0]);	// fill client hardware address
	packet->bootp.xid = DhcpTransactID;			// set trans ID (use part of MAC address)
	packet->bootp.flags = HTONS(1);
	
	// build DHCP request
	// begin with magic cookie
	packet->cookie = 0x63538263;
	// set operation
	val = DHCP_MSG_DHCPRELEASE;
    optptr = dhcpSetOption(packet->options, DHCP_OPT_DHCPMSGTYPE, 1, (uint8_t*)&val);
	// set the server ID
	val = htonl(DhcpServerIP);
    optptr = dhcpSetOption(optptr, DHCP_OPT_SERVERID, 4, (uint8_t*)&val);
	// request the IP previously offered
    optptr = dhcpSetOption(optptr, DHCP_OPT_REQUESTEDIP, 4, (uint8_t*)&packet->bootp.ciaddr);

	// send release
    udpSend(DhcpServerIP, DHCP_UDP_SERVER_PORT,DHCP_UDP_CLIENT_PORT, DHCP_HEADER_LEN+3+6+6+1, (uint8_t*)packet);
	
	// deconfigure ip addressing
	ipSetConfig(0,0,0);
	DhcpLeaseTime = 0;
}

void dhcpTimer(void)
{
	// this function to be called once per second
	// decrement lease time
	if(DhcpLeaseTime)
		DhcpLeaseTime--;
}

uint8_t dhcpGetOption(uint8_t* options, uint8_t optcode, uint8_t optlen, void* optvalptr)
{
	uint8_t i;

	// parse for desired option
	for (;;)
	{
		// skip pad characters
		if(*options == DHCP_OPT_PAD)
			options++;
		// break if end reached
		else if(*options == DHCP_OPT_END)
			break;
		// check for desired option
		else if(*options == optcode)
		{
			// found desired option
			// limit size to actual option length
			optlen = MIN(optlen, *(options+1));
			//if(*(options+1) < optlen)
			//	optlen = *(options+1);
			
			// copy contents of option
			for(i=0; i<optlen; i++)
				*(((uint8_t*)optvalptr)+i) = *(options+i+2);
			// return length of option
			return *(options+1);
		}
		else
		{
			// skip to next option
			options++;
			options+=*options;
			options++;
		}
	}
	// failed to find desired option
	return 0;
}


uint8_t* dhcpSetOption(uint8_t* options, uint8_t optcode, uint8_t optlen, uint8_t* optvalptr)
{
	// use current options address as write point

	// set optcode
	*options++ = optcode;
	// set optlen
	*options++ = optlen;
	// copy in argument/data
	while(optlen--)
	{
        *options++ = *(uint8_t*)optvalptr++;
	}
	// write end marker
	*options = DHCP_OPT_END;

	// return address of end marker, to be used as a future write point
	return options;
}
