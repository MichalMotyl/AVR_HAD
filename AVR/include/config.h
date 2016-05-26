#ifndef CONFIG_H
#define CONFIG_H

#include <avr/pgmspace.h>
#include <avr/io.h>
#include "dhcp.h"
#include "netstack.h"

#define SPI_DDR  DDRB
#define SPI_PORT PORTB
#define SPI_CS   2
#define SPI_MOSI 3
#define SPI_MISO 4
#define SPI_SCK  5



#define CONFIG_LEN sizeof(config)
#define FEATURES_LEN sizeof(features)

#define OUTPUT_MAX_CNT 4
#define MAX_FEATURES 4
#define STATIC_IP 0
#define USE_DHCP 1
#define REALY_OUTPUT 0
#define TEMPERATURE 1

//struct output {
//    uint8_t output_name[32];
//}GNUC_PACKED;

//struct device_features{
//    uint8_t output_cnt;
//    struct output out[OUTPUT_MAX_CNT];

//}GNUC_PACKED;

struct dev_features
{
    uint8_t feature_type;//0 - relay output,1 -temperature input
    uint8_t feature_id;
    uint8_t feature_name[32];

}GNUC_PACKED;

struct sys_config
{
        uint8_t dhcp;           //dhcp flag - if 1 ,use dhcp,else static
        uint8_t static_ip[4];   //static ip
        uint8_t gateway[4];     //static ip
        uint8_t netmask[4];     //static ip
        uint8_t host_name[DHCP_MAX_HOST_NAME_LEN]; //host name when dhcp used
        uint8_t mac[6];         //mac addr
        uint8_t fetures_cnt;

} GNUC_PACKED;

extern struct sys_config config;
extern struct dev_features features[4];

void LoadConfig();
void saveConfig();
struct sys_config * getConfigData();
struct dev_features * getFeatureData();
#endif
