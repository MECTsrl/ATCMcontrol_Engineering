//----  Includes:   -------------------------------------------*

#include "stdafx.h"
#include "Gsd_Keyw.h"
#include "gsd_scan.h"
#include "PrmTextDB.h"
#include "ExtParamDB.h"
#include "Parser.h" 
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// type of GSD this keyword belongs to;
enum GSD_type { general, master, slave };

// table of keywords:
static const struct _keyword
{
    /** token for keyword (see parser.y) */
    int                 iToken;
    /** keyword */
    const char*         pszKeyword;
    /** type of GSD item - master relevant, slave relevant, general */
    enum GSD_type       typeGSD;
    /** TRUE: parameter is mandatory */
    BOOL                bMandatory;
    /**
        number of GSD revision this keyword was introduced
        Revision 0 -> standard DP
        Revision 1 -> enhancements to DP (13-09-96)
        Revision 2 -> DP/V1 (spring 97)
        Revision 3 -> enhancements to DP/V1, Slot def. Physical interface etc () 
    */
    int                 iRevision;
}s_Keywords[] = {
    // General keywords
    // iToken:              pszKeyword:                 typeGSD, mand.  revision
    _GSD_REVISION,          "GSD_Revision",             general, TRUE,  1,
    _VENDOR_NAME,           "Vendor_Name",              general, TRUE,  1,
    _MODEL_NAME,            "Model_Name",               general, TRUE,  0,
    _REVISION,              "Revision",                 general, TRUE,  0,
    _REVISION_NUMBER,       "Revision",                 general, FALSE, 1,
    _IDENT_NUMBER,          "Ident_Number",             general, TRUE,  0,
    _PROTOCOL_IDENT,        "Protocol_Ident",           general, TRUE,  0,
    _STATION_TYPE,          "Station_Type",             general, TRUE,  0,
    _FMS_SUPP,              "FMS_supp",                 general, FALSE, 0,
    _HW_RELEASE,            "Hardware_Release",         general, TRUE,  0,
    _SW_RELEASE,            "Software_Release",         general, TRUE,  0,
    // DP V1
    _PHYSICAL_LAYER,        "Physical_Layer",           general, FALSE, 2,

    _9_6_SUPP,              "9.6_supp",                 general, FALSE, 0,
    _19_2_SUPP,             "19.2_supp",                general, FALSE, 0,
    _93_75_SUPP,            "93.75_supp",               general, FALSE, 0,
    _187_5_SUPP,            "187.5_supp",               general, FALSE, 0,
    _500_SUPP,              "500_supp",                 general, FALSE, 0,
    _1_5M_SUPP,             "1.5M_supp",                general, FALSE, 0,
    _3M_SUPP,               "3M_supp",                  general, FALSE, 0,
    _6M_SUPP,               "6M_supp",                  general, FALSE, 0,
    _12M_SUPP,              "12M_supp",                 general, FALSE, 0,
    // DP V1
    _31_25_SUPP,            "31.25_supp",               general, FALSE, 2,
    _45_45_SUPP,            "45.45_supp",               general, FALSE, 2,

    _MAXTSDR_9_6,           "MaxTsdr_9.6",              general, FALSE, 0,
    _MAXTSDR_19_2,          "MaxTsdr_19.2",             general, FALSE, 0,
    _MAXTSDR_93_75,         "MaxTsdr_93.75",            general, FALSE, 0,
    _MAXTSDR_187_5,         "MaxTsdr_187.5",            general, FALSE, 0,
    _MAXTSDR_500,           "MaxTsdr_500",              general, FALSE, 0,
    _MAXTSDR_1_5M,          "MaxTsdr_1.5M",             general, FALSE, 0,
    _MAXTSDR_3M,            "MaxTsdr_3M",               general, FALSE, 0,
    _MAXTSDR_6M,            "MaxTsdr_6M",               general, FALSE, 0,
    _MAXTSDR_12M,           "MaxTsdr_12M",              general, FALSE, 0,
    // DP V1
    _MAXTSDR_31_25,         "MaxTsdr_31.25",            general, FALSE, 2,
    _MAXTSDR_45_45,         "MaxTsdr_45.45",            general, FALSE, 2,

    _REDUNDANCY,            "Redundancy",               general, FALSE, 0,
    _REPEATER_CTRL_SIG,     "Repeater_Ctrl_Sig",        general, FALSE, 0,
    _24V_PINS,              "24V_Pins",                 general, FALSE, 0,

    _IMPL_TYPE,             "Implementation_Type",      general, FALSE, 1,
    _BMP_DEVICE,            "Bitmap_Device",            general, FALSE, 1,
    _BMP_DIAG,              "Bitmap_Diag",              general, FALSE, 1,
    _BMP_SF,                "Bitmap_SF",                general, FALSE, 1,

    // DP-master (class 1) related keywords   
    // iToken:              pszKeyword:                 typeGSD, mand.  revision
    _MASTER_FREEZE_MODE_SUPP,"Master_Freeze_Mode_supp", master,  FALSE,  3,
    _MASTER_SYNC_MODE_SUPP, "Master_Sync_Mode_supp",    master,  FALSE,  3,
    _MASTER_FAIL_SAFE_SUPP, "Master_Fail_Safe_supp",    master,  FALSE,  3,
    _DOWNLOAD_SUPP,         "Download_supp",            master,  FALSE, 0,
    _UPLOAD_SUPP,           "Upload_supp",              master,  FALSE, 0,
    _ACT_PARA_BRCT_SUPP,    "Act_Para_Brct_supp",       master,  FALSE, 0,
    _ACT_PARAM_SUPP,        "Act_Param_supp",           master,  FALSE, 0,
    _MAX_MPS_LENGTH,        "Max_MPS_Length",           master,  TRUE,  0,
    _MAX_LSDU_MS,           "Max_Lsdu_MS",              master,  TRUE,  0,
    _MAX_LSDU_MM,           "Max_Lsdu_MM",              master,  TRUE,  0,
    _MIN_POLL_TIMEOUT,      "Min_Poll_Timeout",         master,  TRUE,  0,
    _DPV1_MASTER,           "DPV1_Master",              master,  FALSE, 2,
    _DPV1_CONFORMANCE_CLASS,"DPV1_Conformance_Class",   master,  FALSE, 3,
    _C1_MASTER_RW_SUPP,     "C1_Master_Read_Write_supp",master,  FALSE, 3,
    _M_DPV1_ALARM_SUPP,     "Master_DPV1_Alarm_supp",   master,  FALSE, 3,
    _M_DIAGNOSTIC_ALARM_SUPP,"Master_Diagnostic_Alarm_supp",    master,   FALSE, 3,
    _M_PROCESS_ALARM_SUPP,  "Master_Process_Alarm_supp",        master,   FALSE, 3,
    _M_PULL_PLUG_ALARM_SUPP,"Master_Pull_Plug_Alarm_supp",      master,   FALSE, 3,
    _M_STATUS_ALARM_SUPP,   "Master_Status_Alarm_supp",         master,   FALSE, 3,
    _M_UPDATE_ALARM_SUPP,   "Master_Update_Alarm_supp",         master,   FALSE, 3,
    _M_MAN_SPEC_ALARM_SUPP, "Master_Manufacturer_Specific_Alarm_supp", master,   FALSE, 3,
    _M_EXTRA_ALARM_SAP_SUPP,"Master_Extra_Alarm_SAP_supp",      master,   FALSE, 3,
    _M_ALARM_SEQ_MODE,      "Master_Alarm_Sequence_Mode",       master,   FALSE, 3,
    _M_ALARM_TYPE_MODE_SUPP,"Master_Alarm_Type_Mode_supp",      master,   FALSE, 3,
    _TRDY_9_6,              "Trdy_9.6",                 master,  FALSE, 0,
    _TRDY_19_2,             "Trdy_19.2",                master,  FALSE, 0,
    _TRDY_93_75,            "Trdy_93.75",               master,  FALSE, 0,
    _TRDY_187_5,            "Trdy_187.5",               master,  FALSE, 0,
    _TRDY_500,              "Trdy_500",                 master,  FALSE, 0,
    _TRDY_1_5M,             "Trdy_1.5M",                master,  FALSE, 0,
    _TRDY_3M,               "Trdy_3M",                  master,  FALSE, 0,
    _TRDY_6M,               "Trdy_6M",                  master,  FALSE, 0,
    _TRDY_12M,              "Trdy_12M",                 master,  FALSE, 0,
    // DP V1   
    _TRDY_31_25,            "Trdy_31.25",               master,  FALSE, 2,
    _TRDY_45_45,            "Trdy_45.45",               master,  FALSE, 2,

    _TQUI_9_6,              "Tqui_9.6",                 master,  FALSE, 0,
    _TQUI_19_2,             "Tqui_19.2",                master,  FALSE, 0,
    _TQUI_93_75,            "Tqui_93.75",               master,  FALSE, 0,
    _TQUI_187_5,            "Tqui_187.5",               master,  FALSE, 0,
    _TQUI_500,              "Tqui_500",                 master,  FALSE, 0,
    _TQUI_1_5M,             "Tqui_1.5M",                master,  FALSE, 0,
    _TQUI_3M,               "Tqui_3M",                  master,  FALSE, 0,
    _TQUI_6M,               "Tqui_6M",                  master,  FALSE, 0,
    _TQUI_12M,              "Tqui_12M",                 master,  FALSE, 0,
    // DP V1   
    _TQUI_31_25,            "Tqui_31.25",               master,  FALSE, 2,
    _TQUI_45_45,            "Tqui_45.45",               master,  FALSE, 2,

    _TSET_9_6,              "Tset_9.6",                 master,  FALSE, 0,
    _TSET_19_2,             "Tset_19.2",                master,  FALSE, 0,
    _TSET_93_75,            "Tset_93.75",               master,  FALSE, 0,
    _TSET_187_5,            "Tset_187.5",               master,  FALSE, 0,
    _TSET_500,              "Tset_500",                 master,  FALSE, 0,
    _TSET_1_5M,             "Tset_1.5M",                master,  FALSE, 0,
    _TSET_3M,               "Tset_3M",                  master,  FALSE, 0,
    _TSET_6M,               "Tset_6M",                  master,  FALSE, 0,
    _TSET_12M,              "Tset_12M",                 master,  FALSE, 0,
    // DP V1   
    _TSET_31_25,            "Tset_31.25",               master,  FALSE, 2,
    _TSET_45_45,            "Tset_45.45",               master,  FALSE, 2,

    _LAS_LEN,               "LAS_Len",                  master,  TRUE,  0,
    _TSDI_9_6,              "Tsdi_9.6",                 master,  FALSE, 0,
    _TSDI_19_2,             "Tsdi_19.2",                master,  FALSE, 0,
    _TSDI_93_75,            "Tsdi_93.75",               master,  FALSE, 0,
    _TSDI_187_5,            "Tsdi_187.5",               master,  FALSE, 0,
    _TSDI_500,              "Tsdi_500",                 master,  FALSE, 0,
    _TSDI_1_5M,             "Tsdi_1.5M",                master,  FALSE, 0,
    _TSDI_3M,               "Tsdi_3M",                  master,  FALSE, 0,
    _TSDI_6M,               "Tsdi_6M",                  master,  FALSE, 0,
    _TSDI_12M,              "Tsdi_12M",                 master,  FALSE, 0,
    // DP V1
    _TSDI_31_25,            "Tsdi_31.25",               master,  FALSE, 2,
    _TSDI_45_45,            "Tsdi_45.45",               master,  FALSE, 2,

    _MAX_SLAVES_SUPP,       "Max_Slaves_supp",          master,  TRUE,  0,
    _MAX_MASTER_INP_LEN,    "Max_Master_Input_Len",     master,  FALSE, 1,
    _MAX_MASTER_OUT_LEN,    "Max_Master_Output_Len",    master,  FALSE, 1,
    _MAX_MASTER_DATA_LEN,   "Max_Master_Data_Len",      master,  FALSE, 1,

    // slave related keywords
    // iToken:              pszKeyword:                 typeGSD, mand.  revision
    _FREEZE_MODE_SUPP,      "Freeze_Mode_supp",         slave,   FALSE, 0,
    _SYNC_MODE_SUPP,        "Sync_Mode_supp",           slave,   FALSE, 0,
    _AUTO_BAUD_SUPP,        "Auto_Baud_supp",           slave,   FALSE, 0,
    _SET_SLAVE_ADD_SUPP,    "Set_Slave_Add_supp",       slave,   FALSE, 0,
    _USER_PRM_DATA_LEN,     "User_Prm_Data_Len",        slave,   FALSE, 0,
    _USER_PRM_DATA,         "User_Prm_Data",            slave,   FALSE, 0,
    _USER_PRM_DATA_DEF,     "User_Prm_Data_Def",        slave,   FALSE, 1,
    _MIN_SLAVE_INTERVALL,   "Min_Slave_Intervall",      slave,   TRUE,  0,
    _MODULAR_STATION,       "Modular_Station",          slave,   FALSE, 0,
    _MAX_MODULE,            "Max_Module",               slave,   FALSE, 0,
    _MAX_INPUT_LEN,         "Max_Input_Len",            slave,   FALSE, 0,
    _MAX_OUTPUT_LEN,        "Max_Output_Len",           slave,   FALSE, 0,
    _MAX_DATA_LEN,          "Max_Data_Len",             slave,   FALSE, 0,
    _UNIT_DIAG_BIT,         "Unit_Diag_Bit",            slave,   FALSE, 0,
    _UNIT_DIAG_AREA,        "Unit_Diag_Area",           slave,   FALSE, 0,
    _UNIT_DIAG_AREA_END,    "Unit_Diag_Area_End",       slave,   FALSE, 0,
    _CHANNEL_DIAG,          "Channel_Diag",             slave,   FALSE, 0,
    _VALUE,                 "Value",                    slave,   FALSE, 0,
    _MODULE,                "Module",                   slave,   TRUE,  0,
    _END_MODULE,            "EndModule",                slave,   TRUE,  0,
    _FAIL_SAFE,             "Fail_Safe",                slave,   FALSE, 1,
    _FAIL_SAVE_REQUIRED,    "Fail_Safe_required",       slave,   FALSE, 3,
    _DIAG_UPDATE_DELAY,     "Diag_Update_Delay",        slave,   FALSE, 3, 
    _MAX_DIAG_DATA_LEN,     "Max_Diag_Data_Len",        slave,   TRUE,  1,
    _MODUL_OFFSET,          "Modul_Offset",             slave,   FALSE, 1,
    _SLAVE_FAMILY,          "Slave_Family",             slave,   TRUE,  1,

    _MAX_USER_PRM_DATA_LEN, "Max_User_Prm_Data_Len",    slave,   FALSE, 1,
    _EXT_MOD_PRM_DATA_LEN,  "Ext_Module_Prm_Data_Len",  slave,   FALSE, 1,
    _EXT_USR_PRM_DATA_REF,  "Ext_User_Prm_Data_Ref",    slave,   FALSE, 1,
    _EXT_USR_PRM_DATA_CONST,"Ext_User_Prm_Data_Const",  slave,   FALSE, 1,
    _EXT_USR_PRM_DATA,      "ExtUserPrmData",           slave,   FALSE, 1,
    _END_EXT_USR_PRM_DATA,  "EndExtUserPrmData",        slave,   FALSE, 1,
    _PRM_TEXT_REF,          "Prm_Text_Ref",             slave,   FALSE, 1,
    _PRM_TEXT_REF,          "PrmTextRef",               slave,   FALSE, 1,
    _PRM_TEXT,              "PrmText",                  slave,   FALSE, 1,
    _END_PRM_TEXT,          "EndPrmText",               slave,   FALSE, 1,
    _TEXT,                  "Text",                     slave,   FALSE, 1,

    _TYPE_UNSIGNED8,        "Unsigned8",                slave,   FALSE, 1,
    _TYPE_UNSIGNED16,       "Unsigned16",               slave,   FALSE, 1,
    _TYPE_UNSIGNED32,       "Unsigned32",               slave,   FALSE, 1,
    _TYPE_SIGNED8,          "Signed8",                  slave,   FALSE, 1,
    _TYPE_SIGNED16,         "Signed16",                 slave,   FALSE, 1,
    _TYPE_SIGNED32,         "Signed32",                 slave,   FALSE, 1,
    _TYPE_BIT,              "Bit",                      slave,   FALSE, 1,
    _TYPE_BIT_AREA,         "BitArea",                  slave,   FALSE, 1,
    
    _DPV1_SLAVE,            "DPV1_Slave",               slave,   FALSE, 2,
    _C1_RW_SUPP,            "C1_Read_Write_supp",       slave,   FALSE, 3,
    _C2_RW_SUPP,            "C2_Read_Write_supp",       slave,   FALSE, 3,
    _C1_MAX_DATA_LEN,       "C1_Max_Data_Len",          slave,   FALSE, 3,
    _C2_MAX_DATA_LEN,       "C2_Max_Data_Len",          slave,   FALSE, 3,
    _C1_RESPONSE_TO,        "C1_Response_Timeout",      slave,   FALSE, 3,
    _C2_RESPONSE_TO,        "C2_Response_Timeout",      slave,   FALSE, 3,
    _C1_RW_REQ,             "C1_Read_Write_required",   slave,   FALSE, 3,
    _C2_RW_REQ,             "C2_Read_Write_required",   slave,   FALSE, 3,
    _C2_MAX_CNT_CHANNELS,   "C2_Max_Count_Channels",    slave,   FALSE, 3,
    _MAX_INIT_PDU_LEN,      "Max_Initiate_PDU_Length",  slave,   FALSE, 3,
    _ALARM_SUPP,            "Alarm_supported",          slave,   FALSE, 2,
    _DIAGNOSTIC_ALARM_SUPP, "Diagnostic_Alarm_supp",    slave,   FALSE, 3,
    _PROCESS_ALARM_SUPP,    "Process_Alarm_supp",       slave,   FALSE, 3,
    _PULL_PLUG_ALARM_SUPP,  "Pull_Plug_Alarm_supp",     slave,   FALSE, 3,
    _STATUS_ALARM_SUPP,     "Status_Alarm_supp",        slave,   FALSE, 3,
    _UPDATE_ALARM_SUPP,     "Update_Alarm_supp",        slave,   FALSE, 3,
    _MAN_SPEC_ALARM_SUPP,   "Manufacturer_Specific_Alarm_supp", slave,   FALSE, 3,
    _EXTRA_ALARM_SAP_SUPP,  "Extra_Alarm_SAP_supp",     slave,   FALSE, 3,
    _ALARM_SEQ_MODE_CNT,    "Alarm_Sequence_Mode_Count",slave,   FALSE, 3,
    _ALARM_TYPE_MODE_SUPP,  "Alarm_Type_Mode_supp",     slave,   FALSE, 3,
    _DIAGNOSTIC_ALARM_RQ,   "Diagnostic_Alarm_required",slave,   FALSE, 3,
    _PROCESS_ALARM_RQ,      "Process_Alarm_required",   slave,   FALSE, 3,
    _PULL_PLUG_ALARM_RQ,    "Pull_Plug_Alarm_required", slave,   FALSE, 3,
    _STATUS_ALARM_RQ,       "Status_Alarm_required",    slave,   FALSE, 3,
    _UPDATE_ALARM_RQ,       "Update_Alarm_required",    slave,   FALSE, 3,
    _MAN_SPEC_ALARM_RQ,     "Manufacturer_Specific_Alarm_required", slave,   FALSE, 3,
    _DPV1_DATA_TYPES,       "DPV1_Data_Types",          slave,   FALSE, 3,
    _WD_BASE_1MS_SUPP,      "WD_Base_1ms_supp",         slave,   FALSE, 3,
    _CHECK_CFG_MODE,        "Check_Cfg_Mode",           slave,   FALSE, 3,
    _SLOT_DEF,              "SlotDefinition",           slave,   FALSE, 3,
    _END_SLOT_DEF,          "EndSlotDefinition",        slave,   FALSE, 3,
    _SLOT,                  "Slot",                     slave,   FALSE, 3,

    _PHYSICAL_IF,           "Physical_Interface",       general, FALSE,  3,
    _END_PHYSICAL_IF,       "End_Physical_Interface",   general, FALSE,  3,
    
    _TX_DEL_9_6,            "Transmission_Delay_9.6",   general, FALSE,  3,
    _TX_DEL_19_2,           "Transmission_Delay_19.2",  general, FALSE,  3,
    _TX_DEL_31_25,          "Transmission_Delay_31.25", general, FALSE,  3,
    _TX_DEL_45_45,          "Transmission_Delay_45.45", general, FALSE,  3,
    _TX_DEL_93_75,          "Transmission_Delay_93.75", general, FALSE,  3,
    _TX_DEL_187_5,          "Transmission_Delay_187.5", general, FALSE,  3,
    _TX_DEL_500,            "Transmission_Delay_500",   general, FALSE,  3,
    _TX_DEL_1_5M,           "Transmission_Delay_1.5M",  general, FALSE,  3,
    _TX_DEL_3M,             "Transmission_Delay_3M",    general, FALSE,  3,
    _TX_DEL_6M,             "Transmission_Delay_6M",    general, FALSE,  3,
    _TX_DEL_12M,            "Transmission_Delay_12M",   general, FALSE,  3,

    _RC_DEL_9_6,            "Reaction_Delay_9.6",       general, FALSE,  3,
    _RC_DEL_19_2,           "Reaction_Delay_19.2",      general, FALSE,  3,
    _RC_DEL_31_25,          "Reaction_Delay_31.25",     general, FALSE,  3,
    _RC_DEL_45_45,          "Reaction_Delay_45.45",     general, FALSE,  3,
    _RC_DEL_93_75,          "Reaction_Delay_93.75",     general, FALSE,  3,
    _RC_DEL_187_5,          "Reaction_Delay_187.5",     general, FALSE,  3,
    _RC_DEL_500,            "Reaction_Delay_500",       general, FALSE,  3,
    _RC_DEL_1_5M,           "Reaction_Delay_1.5M",      general, FALSE,  3,
    _RC_DEL_3M,             "Reaction_Delay_3M",        general, FALSE,  3,
    _RC_DEL_6M,             "Reaction_Delay_6M",        general, FALSE,  3,
    _RC_DEL_12M,            "Reaction_Delay_12M",       general, FALSE,  3,
    _INFO_TEXT,             "Info_Text",                general, FALSE,  3,
    // end of table marker !!!
    0,                      NULL,                       general, FALSE, 0
};

///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CGsdKeyword
///////////////////////////////////////////////////////////////////////////////

//----------------------------- FUNCTION -------------------------------------*
CGsdKeyword::CGsdKeyword()
/*>>>> 
ctor

Result
  void
<<<<*/
{
    InitializeKeywords();
}


//----------------------------- FUNCTION -------------------------------------*
CGsdKeyword::CGsdKeyword(CGsdKeyword const & other)
/*>>>> 
copy ctor

Result
  void
<<<<*/
{
    *this = other;  // simply call assignment operator
}


//----------------------------- FUNCTION -------------------------------------*
CGsdKeyword const &
    CGsdKeyword::operator = (CGsdKeyword const & other)
/*>>>> 
assign <other> to <this>

I   other:  source to copy from

Result
  reference to <this>
<<<<*/
{
    if (this == &other) {
        return *this;       // copying myself, I'm done
    }

    // copy object data:
    ASSERT(FALSE);      // just in case, since actual copy is not implemented!!

    return *this;
}


//----------------------------- FUNCTION -------------------------------------*
CGsdKeyword::~CGsdKeyword()
/*>>>> 
dtor

Result
  void
<<<<*/
{
}

//----------------------------- FUNCTION -------------------------------------*
int
    CGsdKeyword::LookupKeyword(LPCSTR pszKeyword, int& iRevision)
/*>>>> 
lookup keyword

I   pszKeyword:     keword to look for
O   iRevision:      GSD revision this keyword belongs to

Result
  token index of keyword if found; <0 if not found/unknown keyword
<<<<*/
{
    // plain stupid: linear search list thru list!!!!!!!!!!!
    for (int i = 0; s_Keywords[i].pszKeyword != NULL; i++)
    {
        if (::stricmp(s_Keywords[i].pszKeyword, pszKeyword) == 0) {
            iRevision = s_Keywords[i].iRevision;
            return s_Keywords[i].iToken;
        }
    }

    iRevision = 0;
    return -1;
}

//----------------------------- FUNCTION -------------------------------------*
LPCSTR
    CGsdKeyword::KeywordFromToken(int iToken)
/*>>>> 
get keyword from token

I   iToken:         token index of keyword

Result
  ptr to keyword if found; NULL if token not in keyword list
<<<<*/
{
    // plain stupid: linear search list thru list!!!!!!!!!!!
    for (int i = 0; s_Keywords[i].pszKeyword != NULL; i++)
    {
        if (s_Keywords[i].iToken == iToken) {
            return s_Keywords[i].pszKeyword;
        }
    }

    return NULL;
}


//----------------------------- FUNCTION -------------------------------------*
void
    CGsdKeyword::InitializeKeywords(void)
/*>>>> 
initialize keyword table for later lookups

Result
  void
<<<<*/
{
}
