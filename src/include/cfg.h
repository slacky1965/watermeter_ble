#ifndef SRC_INCLUDE_CFG_H_
#define SRC_INCLUDE_CFG_H_

#include "stdint.h"

#define ID_CONFIG       0x0FED1410
#define ID_COUNTERS     0x0FED1965
#define TOP_MASK        0xFFFFFFFF

typedef struct __attribute__((packed)) _counters_t {
    uint32_t id;                        /* ID - ID_COUNTERS                     */
    uint32_t hot_water_count;           /* Last number of liters hot water      */
    uint32_t cold_water_count;          /* Last number of litres cold water     */
} counters_t;

/* must be no more than FLASH_PAGE_SIZE (256) bytes */
typedef struct __attribute__((packed)) _watermeter_config_t {
    uint32_t   id;                      /* 1st place only! ID - ID_CONFIG       */
    uint32_t   top;                     /* 2st place only! 0x0 .. 0xFFFFFFFF    */
    uint32_t   flash_addr;              /* 3st place only! flash page address   */
    counters_t counters;                /* 4st place only! water counters       */
    uint8_t    size;                    /* 5st place only! sizeof config        */
    uint8_t    liters_per_pulse;        /* liters per pulse                     */
    uint8_t    ble_name[BLE_NAME_SIZE]; /* Module name with MAC                 */
    uint8_t    encrypted;               /* non-encrypted = 0, encrypted = 1     */
    uint8_t    bindkey[16];             /* secret key                           */
} watermeter_config_t;

extern watermeter_config_t watermeter_config;

void init_config();
void write_config();

#endif /* SRC_INCLUDE_CFG_H_ */
