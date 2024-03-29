#include <stdint.h>
#include "tl_common.h"
#include "stack/ble/ble.h"

#include "log.h"
#include "ble.h"
#include "app.h"
#include "app_att.h"
#include "battery.h"
#include "cfg.h"

typedef struct
{
  /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMin;
  /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMax;
  /** Number of LL latency connection events (0x0000 - 0x03e8) */
  u16 latency;
  /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
  u16 timeout;
} gap_periConnectParams_t;

static const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;

//static const u16 extReportRefUUID = GATT_UUID_EXT_REPORT_REF;

//static const u16 reportRefUUID = GATT_UUID_REPORT_REF;

//static const u16 characterPresentFormatUUID = GATT_UUID_CHAR_PRESENT_FORMAT;

static const u16 userdesc_UUID	= GATT_UUID_CHAR_USER_DESC;

static const u16 serviceChangeUUID = GATT_UUID_SERVICE_CHANGE;

static const u16 my_primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;

static const u16 my_characterUUID = GATT_UUID_CHARACTER;

static const u16 my_devServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;

//static const u16 my_PnPUUID = CHARACTERISTIC_UUID_PNP_ID;

static const u16 my_devNameUUID = GATT_UUID_DEVICE_NAME;

static const u16 my_gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;

static const u16 my_appearanceUIID = GATT_UUID_APPEARANCE;

static const u16 my_periConnParamUUID = GATT_UUID_PERI_CONN_PARAM;

static const u16 my_appearance = GAP_APPEARE_UNKNOWN;

static const u16 my_gattServiceUUID = SERVICE_UUID_GENERIC_ATTRIBUTE;

static const gap_periConnectParams_t my_periConnParameters = {20, 40, 0, 1000};

static u16 serviceChangeVal[2] = {0};

static u8 serviceChangeCCC[2] = {0,0};

static const u8 my_devName[] = WRAPPING_BRACES(DEV_NAME_CHR);

//static const u8 my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};

//////////////////////// Battery /////////////////////////////////////////////////
static const u16 my_batServiceUUID        = SERVICE_UUID_BATTERY;
static const u16 my_batCharUUID       	  = CHARACTERISTIC_UUID_BATTERY_LEVEL;
_attribute_data_retention_ uint16_t batteryValueInCCC;

#define SERVICE_UUID_BINARY_SENSOR      0x183b
#define CHARACTERISTIC_UUID_COUNT1      0x2aea
#define CHARACTERISTIC_UUID_COUNT2      0x2aeb

//////////////////////// Counters /////////////////////////////////////////////////
static const u16 my_countServiceUUID      = SERVICE_UUID_BINARY_SENSOR; // Binary Sensor
static const u16 my_hotCharUUID           = CHARACTERISTIC_UUID_COUNT1;
static const u16 my_coldCharUUID          = CHARACTERISTIC_UUID_COUNT2;
_attribute_data_retention_ uint16_t hotValueInCCC;
_attribute_data_retention_ uint16_t coldValueInCCC;


// RxTx Char
static const  u16 my_RxTxUUID              = 0x1f1f;
static const  u16 my_RxTx_ServiceUUID      = 0x1f10;
_attribute_data_retention_ u8 my_RxTx_Data[16];
_attribute_data_retention_ u16 RxTxValueInCCC;


///////////////////////////////////////////////////////////
static const  u8 my_OtaUUID[16]            = WRAPPING_BRACES(TELINK_SPP_DATA_OTA);
static const  u8 my_OtaServiceUUID[16]     = WRAPPING_BRACES(TELINK_OTA_UUID_SERVICE);
static u8 my_OtaData                       = 0x00;

static const u8  my_OtaName[] = {'O', 'T', 'A'};


//// GAP attribute values
static const u8 my_devNameCharVal[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(GenericAccess_DeviceName_DP_H), U16_HI(GenericAccess_DeviceName_DP_H),
	U16_LO(GATT_UUID_DEVICE_NAME), U16_HI(GATT_UUID_DEVICE_NAME)
};
static const u8 my_appearanceCharVal[5] = {
	CHAR_PROP_READ,
	U16_LO(GenericAccess_Appearance_DP_H), U16_HI(GenericAccess_Appearance_DP_H),
	U16_LO(GATT_UUID_APPEARANCE), U16_HI(GATT_UUID_APPEARANCE)
};
static const u8 my_periConnParamCharVal[5] = {
	CHAR_PROP_READ,
	U16_LO(CONN_PARAM_DP_H), U16_HI(CONN_PARAM_DP_H),
	U16_LO(GATT_UUID_PERI_CONN_PARAM), U16_HI(GATT_UUID_PERI_CONN_PARAM)
};


//// GATT attribute values
static const u8 my_serviceChangeCharVal[5] = {
	CHAR_PROP_INDICATE,
	U16_LO(GenericAttribute_ServiceChanged_DP_H), U16_HI(GenericAttribute_ServiceChanged_DP_H),
	U16_LO(GATT_UUID_SERVICE_CHANGE), U16_HI(GATT_UUID_SERVICE_CHANGE)
};

#define CHARACTERISTIC_UUID_MODEL_NUMBER        0x2A24 // Model Number String: Watermeter_BLE
#define CHARACTERISTIC_UUID_SERIAL_NUMBER       0x2A25 // Serial Number String:
#define CHARACTERISTIC_UUID_FIRMWARE_REV        0x2A26 // Firmware Revision String: 1.0.0_0100
#define CHARACTERISTIC_UUID_HARDWARE_REV        0x2A27 // Hardware Revision String: TB-04
#define CHARACTERISTIC_UUID_SOFTWARE_REV        0x2A28 // Software Revision String: 0x100
#define CHARACTERISTIC_UUID_MANUFACTURER_NAME   0x2A29 // Manufacturer Name String: ai-thinker.com

//// device Information  attribute values
//static const u16 my_UUID_SYSTEM_ID            = CHARACTERISTIC_UUID_SYSTEM_ID;
static const u16 my_UUID_MODEL_NUMBER       = CHARACTERISTIC_UUID_MODEL_NUMBER;
static const u16 my_UUID_SERIAL_NUMBER      = CHARACTERISTIC_UUID_SERIAL_NUMBER;
static const u16 my_UUID_FIRMWARE_REV       = CHARACTERISTIC_UUID_FIRMWARE_REV;
static const u16 my_UUID_HARDWARE_REV       = CHARACTERISTIC_UUID_HARDWARE_REV;
static const u16 my_UUID_SOFTWARE_REV       = CHARACTERISTIC_UUID_SOFTWARE_REV;
static const u16 my_UUID_MANUFACTURER_NAME  = CHARACTERISTIC_UUID_MANUFACTURER_NAME;

static const u8 my_ModCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_HardRev_DP_H), U16_HI(DeviceInformation_HardRev_DP_H),
    U16_LO(CHARACTERISTIC_UUID_HARDWARE_REV), U16_HI(CHARACTERISTIC_UUID_HARDWARE_REV)
};
static const u8 my_SerialCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_FirmRev_DP_H), U16_HI(DeviceInformation_FirmRev_DP_H),
    U16_LO(CHARACTERISTIC_UUID_SERIAL_NUMBER), U16_HI(CHARACTERISTIC_UUID_SERIAL_NUMBER)
};
static const u8 my_FirmCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_FirmRev_DP_H), U16_HI(DeviceInformation_FirmRev_DP_H),
    U16_LO(CHARACTERISTIC_UUID_FIRMWARE_REV), U16_HI(CHARACTERISTIC_UUID_FIRMWARE_REV)
};
static const u8 my_HardCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_HardRev_DP_H), U16_HI(DeviceInformation_HardRev_DP_H),
    U16_LO(CHARACTERISTIC_UUID_HARDWARE_REV), U16_HI(CHARACTERISTIC_UUID_HARDWARE_REV)
};
static const u8 my_SoftCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_SoftRev_DP_H), U16_HI(DeviceInformation_SoftRev_DP_H),
    U16_LO(CHARACTERISTIC_UUID_SOFTWARE_REV), U16_HI(CHARACTERISTIC_UUID_SOFTWARE_REV)
};
static const u8 my_ManCharVal[5] = {
    CHAR_PROP_READ,
    U16_LO(DeviceInformation_ManName_DP_H), U16_HI(DeviceInformation_ManName_DP_H),
    U16_LO(CHARACTERISTIC_UUID_MANUFACTURER_NAME), U16_HI(CHARACTERISTIC_UUID_MANUFACTURER_NAME)
};

static const u8 my_ModelStr[]               = {DEV_NAME_STR};
static const u8 my_SerialStr[]              = {"EMYZE-FL1O9"};
static const u8 my_FirmStr[]                = {"github.com/slacky1965"};
static const u8 my_HardStr[]                = {"TB-04"};
static const u8 my_SoftStr[]                = {'V','0'+(VERSION>>4),'.','0'+(VERSION&0x0f)}; // "0100"
static const u8 my_ManStr[]                 = {"ai-thinker.com"};

//// Battery attribute values
static const u8 my_batCharVal[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(BATT_LEVEL_INPUT_DP_H), U16_HI(BATT_LEVEL_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_BATTERY_LEVEL), U16_HI(CHARACTERISTIC_UUID_BATTERY_LEVEL)
};

//// Count attribute values
static const u8 my_hotCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    U16_LO(HOT_LEVEL_INPUT_DP_H), U16_HI(HOT_LEVEL_INPUT_DP_H),
    U16_LO(CHARACTERISTIC_UUID_COUNT1), U16_HI(CHARACTERISTIC_UUID_COUNT1)
};

static const u8 my_coldCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    U16_LO(COLD_LEVEL_INPUT_DP_H), U16_HI(COLD_LEVEL_INPUT_DP_H),
    U16_LO(CHARACTERISTIC_UUID_COUNT2), U16_HI(CHARACTERISTIC_UUID_COUNT2)
};

//// OTA attribute values
static const u8 my_OtaCharVal[19] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY,
	U16_LO(OTA_CMD_OUT_DP_H), U16_HI(OTA_CMD_OUT_DP_H),
	TELINK_SPP_DATA_OTA,
};

//// RxTx attribute values
static const u8 my_RxTxCharVal[5] = {
    CHAR_PROP_NOTIFY | CHAR_PROP_WRITE_WITHOUT_RSP,
    U16_LO(RxTx_CMD_OUT_DP_H), U16_HI(RxTx_CMD_OUT_DP_H),
    U16_LO(0x1f1f), U16_HI(0x1f1f)
};



// TM : to modify
_attribute_data_retention_ attribute_t my_Attributes[] = {

	{ATT_END_H - 1, 0,0,0,0,0},	// total num of attribute


	// 0001 - 0007  gap
	{7,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_gapServiceUUID), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_devNameCharVal),(u8*)(&my_characterUUID), (u8*)(my_devNameCharVal), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_devName), (u8*)(&my_devNameUUID), (u8*)&ble_name[2], 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_appearanceCharVal),(u8*)(&my_characterUUID), (u8*)(my_appearanceCharVal), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof (my_appearance), (u8*)(&my_appearanceUIID), 	(u8*)(&my_appearance), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_periConnParamCharVal),(u8*)(&my_characterUUID), (u8*)(my_periConnParamCharVal), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof (my_periConnParameters),(u8*)(&my_periConnParamUUID), 	(u8*)(&my_periConnParameters), 0},


	// 0008 - 000b gatt
	{4,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_gattServiceUUID), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_serviceChangeCharVal),(u8*)(&my_characterUUID), 		(u8*)(my_serviceChangeCharVal), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof (serviceChangeVal), (u8*)(&serviceChangeUUID), 	(u8*)(&serviceChangeVal), 0},
	{0,ATT_PERMISSIONS_RDWR,2,sizeof (serviceChangeCCC),(u8*)(&clientCharacterCfgUUID), (u8*)(serviceChangeCCC), 0},


    // 000c - 0018 Device Information Service
    {13,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),(u8*)(&my_devServiceUUID), 0},

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_ModCharVal),(u8*)(&my_characterUUID),(u8*)(my_ModCharVal), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_ModelStr),(u8*)(&my_UUID_MODEL_NUMBER),(u8*)(my_ModelStr), 0},

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_SerialCharVal),(u8*)(&my_characterUUID),(u8*)(my_SerialCharVal), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_SerialStr),(u8*)(&my_UUID_SERIAL_NUMBER),(u8*)(my_SerialStr), 0},

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_FirmCharVal),(u8*)(&my_characterUUID),(u8*)(my_FirmCharVal), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_FirmStr),(u8*)(&my_UUID_FIRMWARE_REV),(u8*)(my_FirmStr), 0},

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_HardCharVal),(u8*)(&my_characterUUID),(u8*)(my_HardCharVal), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_HardStr),(u8*)(&my_UUID_HARDWARE_REV),(u8*)(my_HardStr), 0},

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_SoftCharVal),(u8*)(&my_characterUUID),(u8*)(my_SoftCharVal), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_SoftStr),(u8*)(&my_UUID_SOFTWARE_REV),(u8*)(my_SoftStr), 0},

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_ManCharVal),(u8*)(&my_characterUUID),(u8*)(my_ManCharVal), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_ManStr),(u8*)(&my_UUID_MANUFACTURER_NAME),(u8*)(my_ManStr), 0},

	////////////////////////////////////// Battery Service /////////////////////////////////////////////////////
	// 0018 - 002d
	{4,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID), (u8*)(&my_batServiceUUID), 0},
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_batCharVal),(u8*)(&my_characterUUID), (u8*)(my_batCharVal), 0},				//prop
	{0,ATT_PERMISSIONS_READ,2,sizeof(battery_level),(u8*)(&my_batCharUUID), (u8*)(&battery_level), 0},	//value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(batteryValueInCCC),(u8*)(&clientCharacterCfgUUID),(u8*)(&batteryValueInCCC), 0},   //value

    ////////////////////////////////////// Count Service /////////////////////////////////////////////////////
    // 002e - 0031
    {7,ATT_PERMISSIONS_READ,2,2,(u8*)(&my_primaryServiceUUID),(u8*)(&my_countServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ,2,sizeof(my_hotCharVal),(u8*)(&my_characterUUID),(u8*)(my_hotCharVal), 0},                //prop
    {0,ATT_PERMISSIONS_READ,2,sizeof(watermeter_config.counters.hot_water_count),(u8*)(&my_hotCharUUID),(u8*)(&watermeter_config.counters.hot_water_count), 0},    //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(hotValueInCCC),(u8*)(&clientCharacterCfgUUID),(u8*)(&hotValueInCCC), 0}, //value

    {0,ATT_PERMISSIONS_READ,2,sizeof(my_coldCharVal),(u8*)(&my_characterUUID),(u8*)(my_coldCharVal), 0},              //prop
    {0,ATT_PERMISSIONS_READ,2,sizeof(watermeter_config.counters.cold_water_count),(u8*)(&my_coldCharUUID),(u8*)(&watermeter_config.counters.cold_water_count), 0},   //value
    {0,ATT_PERMISSIONS_RDWR,2,sizeof(coldValueInCCC),(u8*)(&clientCharacterCfgUUID),(u8*)(&coldValueInCCC), 0},   //value

	////////////////////////////////////// OTA /////////////////////////////////////////////////////
	// 0032 - 0036
	{4,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_OtaServiceUUID), 0},
	{0,ATT_PERMISSIONS_READ, 2, sizeof(my_OtaCharVal),(u8*)(&my_characterUUID), (u8*)(my_OtaCharVal), 0},	 //prop
	{0,ATT_PERMISSIONS_RDWR,16,sizeof(my_OtaData),(u8*)(&my_OtaUUID),	(&my_OtaData), &otaWrite, NULL}, //value
	{0,ATT_PERMISSIONS_READ, 2,sizeof (my_OtaName),(u8*)(&userdesc_UUID), (u8*)(my_OtaName), 0},

    ////////////////////////////////////// RxTx ////////////////////////////////////////////////////
    // RxTx Communication
    {4,ATT_PERMISSIONS_RDWR, 2,2,(u8*)(&my_primaryServiceUUID),     (u8*)(&my_RxTx_ServiceUUID), 0},
    {0,ATT_PERMISSIONS_READ, 2, sizeof(my_RxTxCharVal), (u8*)(&my_characterUUID), (u8*)(my_RxTxCharVal), 0},             //prop
    {0,ATT_PERMISSIONS_RDWR, 2,sizeof(my_RxTx_Data),(u8*)(&my_RxTxUUID), (u8*)&my_RxTx_Data, &RxTxWrite, 0},
    {0,ATT_PERMISSIONS_RDWR, 2,sizeof(RxTxValueInCCC),(u8*)(&clientCharacterCfgUUID),   (u8*)(&RxTxValueInCCC), 0}, //value

};


/**
 * @brief      Initialize the attribute table
 * @param[in]  none
 * @return     none
 */
void	my_att_init (void)
{
	bls_att_setAttributeTable ((u8 *)my_Attributes);
    my_Attributes[RxTx_CMD_OUT_DP_H].perm = ATT_PERMISSIONS_RDWR;

}

uint32_t size_my_devName() {
    return sizeof(my_devName);
}
