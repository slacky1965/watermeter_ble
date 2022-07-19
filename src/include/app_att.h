#ifndef SRC_INCLUDE_APP_ATT_H_
#define SRC_INCLUDE_APP_ATT_H_

///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
typedef enum
{
    ATT_H_START = 0,


    //// Gap ////
    /**********************************************************************************************/
    GenericAccess_PS_H,                     //UUID: 2800,   VALUE: uuid 1800
    GenericAccess_DeviceName_CD_H,          //UUID: 2803,   VALUE:              Prop: Read | Notify
    GenericAccess_DeviceName_DP_H,          //UUID: 2A00,   VALUE: device name
    GenericAccess_Appearance_CD_H,          //UUID: 2803,   VALUE:              Prop: Read
    GenericAccess_Appearance_DP_H,          //UUID: 2A01,   VALUE: appearance
    CONN_PARAM_CD_H,                        //UUID: 2803,   VALUE:              Prop: Read
    CONN_PARAM_DP_H,                        //UUID: 2A04,   VALUE: connParameter


    //// gatt ////
    /**********************************************************************************************/
    GenericAttribute_PS_H,                  //UUID: 2800,   VALUE: uuid 1801
    GenericAttribute_ServiceChanged_CD_H,   //UUID: 2803,   VALUE:              Prop: Indicate
    GenericAttribute_ServiceChanged_DP_H,   //UUID: 2A05,   VALUE: service change
    GenericAttribute_ServiceChanged_CCB_H,  //UUID: 2902,   VALUE: serviceChangeCCC


    //// device information ////
    /**********************************************************************************************/
    DeviceInformation_PS_H,                 //UUID: 2800,   VALUE: uuid 180A
    DeviceInformation_pnpID_CD_H,           //UUID: 2803,   VALUE:              Prop: Read
    DeviceInformation_pnpID_DP_H,           //UUID: 2A50,   VALUE: PnPtrs


    //// battery service ////
    /**********************************************************************************************/
    BATT_PS_H,                              //UUID: 2800,   VALUE: uuid 180f
    BATT_LEVEL_INPUT_CD_H,                  //UUID: 2803,   VALUE:              Prop: Read | Notify
    BATT_LEVEL_INPUT_DP_H,                  //UUID: 2A19    VALUE: batVal
    BATT_LEVEL_INPUT_CCB_H,                 //UUID: 2902,   VALUE: batValCCC

    //// Count service ////
    /**********************************************************************************************/
    HOT_PS_H,                              //UUID: 2800,    VALUE: uuid 181A
    HOT_LEVEL_INPUT_CD_H,                  //UUID: 2803,    VALUE:              Prop: Read | Notify
    HOT_LEVEL_INPUT_DP_H,                  //UUID: 2AEA,    VALUE: hot count
    HOT_LEVEL_INPUT_CCB_H,                 //UUID: 2902,    VALUE: hotValCCC

    COLD_PS_H,                              //UUID: 2800,   VALUE: uuid 181A
    COLD_LEVEL_INPUT_CD_H,                  //UUID: 2803,   VALUE:              Prop: Read | Notify
    COLD_LEVEL_INPUT_DP_H,                  //UUID: 2AEB,   VALUE: hot count
    COLD_LEVEL_INPUT_CCB_H,                 //UUID: 2902,   VALUE: hotValCCC

    //// Ota ////
    /**********************************************************************************************/
    OTA_PS_H,                               //UUID: 2800,   VALUE: telink ota service uuid
    OTA_CMD_OUT_CD_H,                       //UUID: 2803,   VALUE:              Prop: read | write_without_rsp | Notify
    OTA_CMD_OUT_DP_H,                       //UUID: telink ota uuid,  VALUE: otaData
    OTA_CMD_INPUT_CCB_H,                    //UUID: 2902,   VALUE: otaDataCCC
    OTA_CMD_OUT_DESC_H,                     //UUID: 2901,   VALUE: otaName


    ATT_END_H,

}ATT_HANDLE;


void my_att_init(void);



#endif /* SRC_INCLUDE_APP_ATT_H_ */
