#include <avr/eeprom.h>
#include <stdio.h>
#include <netstack.h>
#include <avr/pgmspace.h>

#include "config.h"


struct sys_config ee_config EEMEM = {
    STATIC_IP,{192,168,1,22},{192,168,1,1},{255,255,255,0},{"AVRLanRelay"},{0x10,0x00,0x00,0xf9,0xf3,0x17},MAX_FEATURES,
};

struct dev_features dev_features_ee[MAX_FEATURES] EEMEM = {{REALY_OUTPUT,0,"FUNCTION_0"},{REALY_OUTPUT,1,"FUNCTION_1"},{REALY_OUTPUT,2,"FUNCTION_2"},{REALY_OUTPUT,3,"FUNCTION_3"}};

struct sys_config config;
struct dev_features features[MAX_FEATURES];


void LoadConfig()
{
    eeprom_read_block(&config,&ee_config,CONFIG_LEN);
    eeprom_read_block(&features,&dev_features_ee,FEATURES_LEN);

    //printf("fff %d %d %s\n\r",getFeatureData()[0].feature_id,getFeatureData()[0].feature_type,getFeatureData()[0].feature_name);
}

void saveConfig()
{
    //config.dhcp = 1;
    //eeprom_write_block (&config,(uint8_t*)(E2END-CONFIG_LEN),CONFIG_LEN);
    //eeprom_write_block (&config,(uint8_t*)&ee_config,CONFIG_LEN);
}

struct sys_config * getConfigData()
{
    return &config;
}

struct dev_features * getFeatureData()
{
    return features;
}
