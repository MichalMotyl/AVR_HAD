/*! \file netstack.c \brief Network Stack. */
//*****************************************************************************
//
// File Name	: 'netstack.c'
// Title		: Network Stack
// Author		: Pascal Stang
// Created		: 6/28/2005
// Revised		: 9/20/2005
// Version		: 0.3
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "netstack.h"
#include "output.h"
#include "dhcp.h"

extern uint8_t dhcp_ip_addr_status;

#define CMD_SET_OUTPUT          0x01
#define CMD_GET_OUTPUT          0x02
#define CMD_SEND_CONFIG         0x03
#define CMD_SAVE_CONFIG         0x04
#define CMD_READ_CONFIG         0x05
#define CMD_GET_FEATURE         0x06
#define CMD_GET_FEATURE_NUMBER  0x07
#define CMD_HANDSHAKE           0x08


uint8_t NetBuffer[NETSTACK_BUFFERSIZE];

void netstackInit(uint32_t ipaddress, uint32_t netmask, uint32_t gatewayip)
{
    // init network device driver
    nicInit();
    // init ARP
    arpInit();
    // init addressing
    ipSetConfig(ipaddress, netmask, gatewayip);
}

uint8_t* netstackGetBuffer(void)
{
    return NetBuffer;
}

int netstackService(void)
{
    uint16_t len;
    struct netEthHeader* ethPacket;
    uint8_t *p;

    // look for a packet
    len = nicPoll(NetBuffer);

    if(len)
    {
        ethPacket = (struct netEthHeader*)NetBuffer;

        NETSTACK_DEBUG("Received packet len: %d, type:0x%x\n\r", len,ntohs(ethPacket->type));

        if(ethPacket->type == HTONS(ETHTYPE_IP))
        {
            arpIpIn((struct netEthIpHeader*)&NetBuffer[0]);

            p = NetBuffer+ETH_HEADER_LEN;
            netstackIPProcess( len-ETH_HEADER_LEN, (ip_hdr*)p );

        }
        else if(ethPacket->type == HTONS(ETHTYPE_ARP))
        {
            arpArpIn(len, ((struct netEthArpHeader*)NetBuffer) );
        }
    }
    return len;
}

void netstackIPProcess(uint16_t len, ip_hdr* packet)
{
    // check IP addressing, stop processing if not for me and not a broadcast
    if( (htonl(packet->destipaddr) != ipGetConfig()->ip) &&
            (htonl(packet->destipaddr) != (ipGetConfig()->ip|ipGetConfig()->netmask)) &&
            (htonl(packet->destipaddr) != 0xFFFFFFFF) )
        return;

    // handle ICMP packet
    if( packet->proto == IP_PROTO_ICMP )
    {
        NETSTACK_DEBUG("NET Rx: ICMP/IP packet\r\n");

        icmpIpIn((icmpip_hdr*)packet);
    }
    else if( packet->proto == IP_PROTO_UDP )
    {
        NETSTACK_DEBUG("NET Rx: UDP/IP packet\r\n");

        netstackUDPIPProcess(len, ((struct udpip_hdr*)packet) );
    }
    else if( packet->proto == IP_PROTO_TCP )
    {
        NETSTACK_DEBUG("NET Rx: TCP/IP packet\r\n");
        netstackTCPIPProcess(len, ((tcpip_hdr*)packet) );
    }
    else
    {
        NETSTACK_DEBUG("NET Rx: IP packet\r\n");
    }
}

void netstackUDPIPProcess(uint16_t len, struct udpip_hdr* packet)
{
    uint16_t payloadlen=0;
    uint8_t* payloaddata=0;

    payloadlen = ntohs(packet->udp.udplen);
    payloadlen -= (IP_HEADER_LEN+UDP_HEADER_LEN); // subtract header
    // get UDP payload data
    //p = (uint8_t*)packet;
    //payloaddata = p+(IP_HEADER_LEN+UDP_HEADER_LEN);

    if(payloadlen<=0) return;

    payloaddata = (uint8_t*)packet+(IP_HEADER_LEN+UDP_HEADER_LEN);

    //    if(!getConfigData()->dhcp)
    //    {
    if(packet->udp.destport == htons(RX_PORT))
    {
        //printf("Got msg from CONTROL_PORT!!! payloadlen %d payloaddata:0x%x 0x%x\n\r",payloadlen,payloaddata[0],payloaddata[1]);

        if(payloaddata[0]==0x55)
        {
            switch(payloaddata[1])
            {
            case CMD_READ_CONFIG:
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_READ_CONFIG;
                memcpy(&payloaddata[2],getConfigData(),CONFIG_LEN);
                break;

            case CMD_SAVE_CONFIG:
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_SAVE_CONFIG;
                //SAVE CONFIG INTO EE
                saveConfig();
                break;

            case CMD_SEND_CONFIG:
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_SEND_CONFIG;
                memcpy(getConfigData(),&payloaddata[2],CONFIG_LEN);
                break;

            case CMD_SET_OUTPUT:
                setOutputStatus(payloaddata[2]);
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_SET_OUTPUT;
                break;

            case CMD_GET_OUTPUT:
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_GET_OUTPUT;
                payloaddata[2]= getOutputStatus();
                break;

            case CMD_GET_FEATURE:
            {
                uint8_t feature_idx = payloaddata[2];
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_GET_FEATURE;
                memcpy(&payloaddata[2],&features[feature_idx],sizeof(features));
            }
                break;

            case CMD_GET_FEATURE_NUMBER:
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_GET_FEATURE_NUMBER;
                payloaddata[2]=getConfigData()->fetures_cnt;
                break;


            case CMD_HANDSHAKE:
                memset(payloaddata,0,payloadlen);
                payloaddata[0]=0xAA;
                payloaddata[1]=CMD_HANDSHAKE;
                break;


            default:
                return;
                break;
            }

            //                if(payloaddata[1]==CMD_READ_CONFIG)
            //                {
            //                  //  printf("Recieved CMD_READ_CONFIG...\n\r");
            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_READ_CONFIG;
            //                    memcpy(&payloaddata[2],getConfigData(),CONFIG_LEN);
            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }

            //                if(payloaddata[1]==CMD_SAVE_CONFIG)
            //                {
            //                 //   printf("Recieved CMD_SAVE_CONFIG...\n\r");
            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_SAVE_CONFIG;
            //                    //SAVE CONFIG INTO EE
            //                    saveConfig();
            //                    //memcpy(&payloaddata[2],getConfigData(),CONFIG_LEN);
            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }



            //                if(payloaddata[1]==CMD_SEND_CONFIG)
            //                {
            //                    printf("Recieved CMD_SEND_CONFIG...\n\r");
            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_SEND_CONFIG;
            //                    memcpy(getConfigData(),&payloaddata[2],CONFIG_LEN);

            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }


            //                if(payloaddata[1]==CMD_SET_OUTPUT)
            //                {
            //               //     printf("Recieved CMD_SET_OUTPUT...\n\r");

            //                    setOutputStatus(payloaddata[2]);
            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_SET_OUTPUT;

            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }



            //                if(payloaddata[1]==CMD_GET_OUTPUT)
            //                {
            //              //      printf("Recieved CMD_GET_OUTPUT...\n\r");

            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_GET_OUTPUT;
            //                    payloaddata[2]= getOutputStatus();

            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }


            //                if(payloaddata[1]==CMD_GET_FEATURE)
            //                {
            //                    uint8_t feature_idx = payloaddata[2];
            //                    printf("Recieved CMD_GET_FEATURE idx:%d %s\n\r",feature_idx,features[feature_idx].feature_name);

            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_GET_FEATURE;
            //                    memcpy(&payloaddata[2],&features[feature_idx],sizeof(features));
            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }


            //                if(payloaddata[1]==CMD_GET_FEATURE_NUMBER)
            //                {
            //                    printf("Recieved CMD_GET_FEATURE_NUMBER...\n\r");

            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_GET_FEATURE_NUMBER;
            //                    payloaddata[2]=getConfigData()->fetures_cnt;

            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }

            //                if(payloaddata[1]==CMD_HANDSHAKE)
            //                {
            //                    printf("Recieved CMD_HANDSHAKE...\n\r");

            //                    memset(payloaddata,0,payloadlen);
            //                    payloaddata[0]=0xAA;
            //                    payloaddata[1]=CMD_HANDSHAKE;

            //                    udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);
            //                    return;
            //                }

            udpSend(htonl(packet->ip.srcipaddr),TX_PORT,RX_PORT,64,payloaddata);

        }

    }
    else if(packet->udp.destport == htons(LOOPBACK_PORT))
    {
        // loopback - return packet to sender
        //  udpSend(htonl(packet->ip.srcipaddr), LOOPBACK_PORT, payloadlen, payloaddata);
    }
    // }
    // else
    // {
    if(NHTONS(packet->udp.destport) == DHCP_UDP_CLIENT_PORT)
    {
        dhcpIn(payloadlen,(struct netDhcpHeader*)payloaddata);
    }
    //}
}




void netstackTCPIPProcess(unsigned int len, tcpip_hdr* packet)
{
    DEBUG("Received TCP/IP Packet: len=%d\r\n", len);
}

