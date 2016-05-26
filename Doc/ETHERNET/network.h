#define ICMP_PROTOCOL	1
#define IGMP_PROTOCOL	2
#define TCP_PROTOCOL	6
#define UDP_PROTOCOL	17
#define ENCAP_PROTOCOL	41
#define OSPF_PROTOCOL	89
#define SCTP_PROTOCOL	132

https://ask.wireshark.org/questions/7523/ubuntu-machine-no-interfaces-listed


typedef struct UDP_PACKET{
  uint8_t src_port;
  uint8_t dst_port;
  uint8_t len;
  uint8_t *data;
};

typedef struct ip_option{
  uint8_t 	copied:1;
  uint8_t 	option_class:2;
  uint8_t 	option_number:5;
  uint8_t 	option_len;
  uint8_t 	*option_data;
};

typedef union {
  struct {
	uint8_t 	version:4;
	uint8_t 	ihl:4;
	uint8_t 	dscp:6;
	uint8_t 	ecn:2;
	uint16_t 	total_len;
	uint16_t 	identification;
	uint8_t 	flags:3;
	uint8_t 	fragment_offset:13;
	uint8_t 	time_to_live;
	uint8_t 	protocol;
	uint16_t 	header_chksum;
	uint8_t[4] 	src_ip_addr;
	uint8_t[4] 	dst_ip_addr;
  };
  uint8_t ip_tab[20];
  
  ip_option 	option;
  uint8_t 	*data;
}ip_datagram;

typedef struct eth_frame{
  uint8_t 	preamble;
  uint8_t 	dst_mac[6];
  uint8_t 	src_mac[6];
  uint16_t	type_len;
  uint8_t 	*data;
  uint8_t 	fcs[4];
};