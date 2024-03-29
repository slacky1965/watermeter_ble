#pragma once

#define VERSION                 0x21    /* BCD format (0x10 -> '1.0')   */
#define WATERMETER_ID           0xFED5  /* uint16_t                     */
#define DEV_NAME_CHR            'W','a','t','e','r','m','e','t','e','r','_','B','L','E'
#define DEV_NAME_STR            "Watermeter_BLE"
/* 0x16,0x09,'W','a','t','e','r','m','e','t','e','r','_','B','L','E','_',0x00,0x00,0x00,0x00,0x00,0x00 */
#define BLE_NAME_SIZE           23

#define ON                      1
#define OFF                     0

#define UART_PRINT_DEBUG_ENABLE OFF         /* if 1 use printf() over uart  */


/************************ Advertising_Interval *************************************/

#define ADV_INTERVAL_MIN        4000        /* 4000 * 0.625 = 2500 ms or 2.5 sec */
#define ADV_INTERVAL_MAX        4010        /* 4010 * 0.625 = 2506 ms or 2.5 sec */
//#define ADV_INTERVAL_MIN        ADV_INTERVAL_250MS
//#define ADV_INTERVAL_MAX        ADV_INTERVAL_300MS

#define MY_RF_POWER_INDEX       RF_POWER_P0p04dBm

/**************************** Configure UART ***************************************/
#if UART_PRINT_DEBUG_ENABLE
#define PRINT_BAUD_RATE         115200
#define DEBUG_INFO_TX_PIN       UART_TX_PB1
#define PB1_DATA_OUT            ON
#define PB1_OUTPUT_ENABLE       ON
#define PULL_WAKEUP_SRC_PB1     PM_PIN_PULLUP_1M
#define PB1_FUNC                AS_GPIO
#endif /* UART_PRINT_DEBUG_ENABLE */

/************************* Configure counters ***************************************/
#define LITERS_PER_PULSE        10              /* How many liters per one pulse */
#define COUNTERS_OVERFLOW       100000000       /* counters overflow             */

/************************* Configure HOT GPIO ***************************************/
#define HOT_GPIO                GPIO_PB7
#define PB7_INPUT_ENABLE        ON
#define PB7_DATA_OUT            OFF
#define PB7_OUTPUT_ENABLE       OFF
#define PB7_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PB7     PM_PIN_PULLUP_1M

/************************* Configure COLD GPIO **************************************/
#define COLD_GPIO               GPIO_PB4
#define PB4_INPUT_ENABLE        ON
#define PB4_DATA_OUT            OFF
#define PB4_OUTPUT_ENABLE       OFF
#define PB4_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PB4     PM_PIN_PULLUP_1M

/************************* Configure RWL GPIO **************************************/
#define RWL_GPIO                GPIO_PA7
#define PA7_INPUT_ENABLE        ON
#define PA7_DATA_OUT            OFF
#define PA7_OUTPUT_ENABLE       OFF
#define PA7_FUNC                AS_GPIO
//#define PULL_WAKEUP_SRC_PA7     PM_PIN_UP_DOWN_FLOAT //PM_PIN_PULLUP_1M

/************************* Configure VBAT GPIO ***************************************/
#define GPIO_VBAT               GPIO_PC4
#define PC4_INPUT_ENABLE        OFF
#define PC4_OUTPUT_ENABLE       ON
#define PC4_DATA_OUT            ON
#define PC4_FUNC                AS_GPIO
#define SHL_ADC_VBAT            9 //C4P

/************************* For 512K Flash only ***************************************/
/* Flash map:
  0x00000 Old Firmware bin
  0x20000 OTA New bin storage Area
  0x40000 User Data Area
  0x74000 Pair & Security info
  0x76000 MAC address
  0x77000 Customize freq_offset adjust cap value
  0x78000 User Data Area (conflict in master mode)
  0x80000 End Flash
 */
#define BEGIN_USER_DATA         0x40000
#define END_USER_DATA           0x74000
#define FLASH_SECTOR_SIZE       0x1000
#define FLASH_PAGE_SIZE         PAGE_SIZE // 0x100
#define FLASH_SECTOR_MAC        0x76000

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE      0
#define WATCHDOG_INIT_TIMEOUT       500  //ms

///////////////////////// System Clock  Configuration /////////////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  								24000000// 16000000

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	#define SYS_CLK_TYPE  									SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	#define SYS_CLK_TYPE  									SYS_CLK_24M_Crystal
#elif(CLOCK_SYS_CLOCK_HZ == 32000000)
	#define SYS_CLK_TYPE  									SYS_CLK_32M_Crystal
#elif(CLOCK_SYS_CLOCK_HZ == 48000000)
	#define SYS_CLK_TYPE  									SYS_CLK_48M_Crystal
#else
	#error "unsupported system clock !"
#endif

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};

#include "vendor/common/default_config.h"

