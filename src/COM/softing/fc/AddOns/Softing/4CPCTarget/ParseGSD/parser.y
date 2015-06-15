
%{
#include "stdafx.h"
#include "gsd_scan.h"
#include "PrmTextDB.h"
#include "ExtParamDB.h"
#include "gsdmessage.h"
%}
/*===========================================================================*/
/*===       YACC Definitions                                                 */
/*===========================================================================*/

/* %prefix gsd_ */

%union
{
	LONG            ulVal;
    BOOL            bVal;
    int             iToken;
    TCHAR          *pszText;
    CString        *pString;
    CByteArray     *pOctets;
    CPrmTextItem   *pTextItem;
    CPrmTextDef    *pTextDef;
    CExtParamDefinition *pParamDef;
    GSDDATATYPE		dType;
    listEnumValues *pEnumValues;
}


/*---------------------------------------------------------------------------*/
/*--------   Tokens                                                          */
%token <iToken>     _KEYWORD, _PROFIBUS_DP
%token <ulVal>      _INTEGER_LITERAL

%token  <iToken>    CRLF                _COMMENT
%token  <pszText>   _VISIBLE_STRING,    _NAME_CHAR,     _OTHER_CHAR,  _SUB_FAMILY  

/* token of keywords: */
%token <iToken>     _GSD_REVISION,  _VENDOR_NAME,   _MODEL_NAME,    _REVISION
%token <iToken>     _REVISION_NUMBER, _IDENT_NUMBER,  _PROTOCOL_IDENT
%token <iToken>     _STATION_TYPE,  _FMS_SUPP,      _HW_RELEASE,    _SW_RELEASE
%token <iToken>     _PHYSICAL_LAYER
                        
%token <iToken>     _9_6_SUPP,      _19_2_SUPP,     _93_75_SUPP,    _187_5_SUPP
%token <iToken>     _500_SUPP,      _1_5M_SUPP,     _3M_SUPP,       _6M_SUPP
%token <iToken>     _12M_SUPP,      _31_25_SUPP,    _45_45_SUPP
                        
%token <iToken>     _MAXTSDR_9_6,   _MAXTSDR_19_2,  _MAXTSDR_93_75, _MAXTSDR_187_5
%token <iToken>     _MAXTSDR_500,   _MAXTSDR_1_5M,  _MAXTSDR_3M,    _MAXTSDR_6M
%token <iToken>     _MAXTSDR_12M,   _MAXTSDR_31_25, _MAXTSDR_45_45
                        
%token <iToken>     _REDUNDANCY,    _REPEATER_CTRL_SIG, _24V_PINS
                        
%token <iToken>     _IMPL_TYPE,     _BMP_DEVICE,    _BMP_DIAG,      _BMP_SF
                        
%token <iToken>     _DOWNLOAD_SUPP, _UPLOAD_SUPP,   _ACT_PARA_BRCT_SUPP,    _ACT_PARAM_SUPP
%token <iToken>     _MAX_MPS_LENGTH,_MAX_LSDU_MS,   _MAX_LSDU_MM,   _MIN_POLL_TIMEOUT
%token <iToken>     _DPV1_MASTER
                        
%token <iToken>     _TRDY_9_6,      _TRDY_19_2,     _TRDY_93_75,    _TRDY_187_5
%token <iToken>     _TRDY_500,      _TRDY_1_5M,     _TRDY_3M,       _TRDY_6M
%token <iToken>     _TRDY_12M,      _TRDY_31_25,    _TRDY_45_45
                        
%token <iToken>     _TQUI_9_6,      _TQUI_19_2,     _TQUI_93_75,    _TQUI_187_5
%token <iToken>     _TQUI_500,      _TQUI_1_5M,     _TQUI_3M,       _TQUI_6M
%token <iToken>     _TQUI_12M,      _TQUI_31_25,    _TQUI_45_45
                        
%token <iToken>     _TSET_9_6,      _TSET_19_2,     _TSET_93_75,    _TSET_187_5
%token <iToken>     _TSET_500,      _TSET_1_5M,     _TSET_3M,       _TSET_6M
%token <iToken>     _TSET_12M,      _TSET_31_25,    _TSET_45_45
                        
%token <iToken>     _LAS_LEN
%token <iToken>     _TSDI_9_6,      _TSDI_19_2,     _TSDI_93_75,    _TSDI_187_5
%token <iToken>     _TSDI_500,      _TSDI_1_5M,     _TSDI_3M,       _TSDI_6M
%token <iToken>     _TSDI_12M,      _TSDI_31_25,    _TSDI_45_45
                        
%token <iToken>     _MAX_SLAVES_SUPP,       _MAX_MASTER_INP_LEN
%token <iToken>     _MAX_MASTER_OUT_LEN,    _MAX_MASTER_DATA_LEN
                        
%token <iToken>     _FREEZE_MODE_SUPP,      _SYNC_MODE_SUPP
%token <iToken>     _AUTO_BAUD_SUPP,        _SET_SLAVE_ADD_SUPP
%token <iToken>     _USER_PRM_DATA_LEN,     _USER_PRM_DATA,         _USER_PRM_DATA_DEF
%token <iToken>     _MIN_SLAVE_INTERVALL,   _MODULAR_STATION,       _MAX_MODULE
%token <iToken>     _MAX_INPUT_LEN,         _MAX_OUTPUT_LEN,        _MAX_DATA_LEN
%token <iToken>     _UNIT_DIAG_BIT,         _UNIT_DIAG_AREA,        _UNIT_DIAG_AREA_END
%token <iToken>     _CHANNEL_DIAG,          _VALUE
%token <iToken>     _MODULE,                _END_MODULE,			_MODUL_OFFSET
%token <iToken>     _FAIL_SAFE,				_MAX_DIAG_DATA_LEN,		_SLAVE_FAMILY
                        
%token <iToken>     _MAX_USER_PRM_DATA_LEN, _EXT_USR_PRM_DATA_REF,  _EXT_USR_PRM_DATA_CONST
%token <iToken>     _EXT_MOD_PRM_DATA_LEN
%token <iToken>     _EXT_USR_PRM_DATA,      _END_EXT_USR_PRM_DATA,  _PRM_TEXT_REF
%token <iToken>     _PRM_TEXT,              _END_PRM_TEXT,          _TEXT
                        
%token <iToken>     _TYPE_UNSIGNED8,        _TYPE_UNSIGNED16,       _TYPE_UNSIGNED32
%token <iToken>     _TYPE_SIGNED8,          _TYPE_SIGNED16,         _TYPE_SIGNED32
%token <iToken>     _TYPE_BIT,              _TYPE_BIT_AREA
                        
%token <iToken>     _DPV1_SLAVE,            _C1_RW_SUPP,			_C2_RW_SUPP
%token <iToken>		_C1_MAX_DATA_LEN,		_C2_MAX_DATA_LEN,		_C1_RESPONSE_TO
%token <iToken>		_C2_RESPONSE_TO,		_C1_RW_REQ,				_C2_RW_REQ
%token <iToken>		_C2_MAX_CNT_CHANNELS,	_MAX_INIT_PDU_LEN,		_DPV1_DATA_TYPES
%token <iToken>		_WD_BASE_1MS_SUPP,		_CHECK_CFG_MODE
%token <iToken>     _ALARM_SUPP,			_DIAGNOSTIC_ALARM_SUPP,	_PROCESS_ALARM_SUPP
%token <iToken>     _PULL_PLUG_ALARM_SUPP,	_STATUS_ALARM_SUPP,     _UPDATE_ALARM_SUPP
%token <iToken>     _MAN_SPEC_ALARM_SUPP,   _EXTRA_ALARM_SAP_SUPP,	_ALARM_SEQ_MODE_CNT
%token <iToken>		_ALARM_TYPE_MODE_SUPP,	_FAIL_SAVE_REQUIRED
%token <iToken>     _DIAGNOSTIC_ALARM_RQ,	_PROCESS_ALARM_RQ,		_PULL_PLUG_ALARM_RQ
%token <iToken>     _STATUS_ALARM_RQ,		_UPDATE_ALARM_RQ,		_MAN_SPEC_ALARM_RQ

%token <iToken>		_MASTER_SYNC_MODE_SUPP,	_MASTER_FAIL_SAFE_SUPP,	_DPV1_CONFORMANCE_CLASS
%token <iToken>		_C1_MASTER_RW_SUPP,		_M_DPV1_ALARM_SUPP,		_MASTER_FREEZE_MODE_SUPP
%token <iToken>     _M_DIAGNOSTIC_ALARM_SUPP,_M_PROCESS_ALARM_SUPP,	_M_PULL_PLUG_ALARM_SUPP	
%token <iToken>     _M_STATUS_ALARM_SUPP,	_M_UPDATE_ALARM_SUPP,	_M_MAN_SPEC_ALARM_SUPP
%token <iToken>     _M_EXTRA_ALARM_SAP_SUPP,_M_ALARM_SEQ_MODE,		_M_ALARM_TYPE_MODE_SUPP

%token <iToken>		_PHYSICAL_IF,			_END_PHYSICAL_IF
%token <iToken>		_TX_DEL_9_6,	_TX_DEL_19_2,	_TX_DEL_31_25,	_TX_DEL_45_45
%token <iToken>		_TX_DEL_93_75,	_TX_DEL_187_5,	_TX_DEL_500,	_TX_DEL_1_5M
%token <iToken>		_TX_DEL_3M,		_TX_DEL_6M,		_TX_DEL_12M
%token <iToken>		_RC_DEL_9_6,	_RC_DEL_19_2,	_RC_DEL_31_25,	_RC_DEL_45_45
%token <iToken>		_RC_DEL_93_75,	_RC_DEL_187_5,	_RC_DEL_500,	_RC_DEL_1_5M
%token <iToken>		_RC_DEL_3M,		_RC_DEL_6M,		_RC_DEL_12M
%token <iToken>		_DIAG_UPDATE_DELAY,		_INFO_TEXT
%token <iToken>		_SLOT_DEF,		_END_SLOT_DEF,	_SLOT
%token <iToken>     _USER_KEYWORD

%type <pString>     visible_text, name_string, bmp_file_name, module_name, ext_user_prm_data_name
%type <pString>		sub_family, info_text, diag_text
%type <pOctets>     octet_string, const_prm_data, config
%type <ulVal>       unsigned8, unsigned16, unsigned32, prm_data_value, octet, numeric, reference_number, prm_text_ref
%type <ulVal>       reference_offset, reference_number, default_value, min_value, max_value, bit_number, const_offset
%type <ulVal>       module_reference, ext_module_prm_data_len, ext_mod_prm_len, tx_delay, rc_delay, value, error_type
%type <dType>       data_type_name, bit_area
%type <bVal>        Boolean
%type <pTextItem>   text_item
%type <pTextDef>    text_list, ext_user_prm_data_def_opt
%type <pParamDef>   ext_user_prm_data_def
%type <pEnumValues> data_type_array, allowed_values

%{

#include "gsd_Parser.h"
#include "DP_Module.h"

#define GUARD(ptr)              if ((ptr) == NULL) YYRETURN(-1);
#define SAFE_DELETE(pObj)       if ((pObj) != NULL) delete (pObj);

#define THIS_PTR                ((gsd_Parser*) this)
#define STATION                 (THIS_PTR->m_pStation)
#define PRM_TEXT_TABLE          (THIS_PTR->m_mapTextDefDB)

#define USR_PRM_DEF_DB          (THIS_PTR->m_extParamDB)
#define CURRENT_USR_PRM_DEF     (THIS_PTR->m_pCurrUserParamDef)
#define CURRENT_MODULE          (THIS_PTR->m_pCurrModule)
#define CURRENT_PI				(THIS_PTR->m_pPI)
#define CURRENT_DIAG			(THIS_PTR->m_pUnitDiag)
    
// some defines to have shorter actions
#define SET_INT_VALUE(ptr, member, value)   \
                                GUARD(ptr); (ptr)->member = value;

#define SET_STRING_VALUE(ptr, member, string)   \
                                GUARD(ptr); GUARD(string); \
                                (ptr)->member = *string;   \
                                SAFE_DELETE(string);

#define SET_OCTET_VALUE(ptr, member, octets)   \
                                GUARD(ptr); (ptr)->member.Copy(*octets); \
                                SAFE_DELETE(octets);

#define SET_MODULE_STRING(member,string) \
								if (CURRENT_MODULE != NULL) \
								{	GUARD(string);\
									CURRENT_MODULE->member = *string; \
									SAFE_DELETE(string); \
								}

#pragma warning(disable: 4102)      // 'xxx' : unreferenced label
#pragma warning(disable: 4309)      // 'initializing' : truncation of constant value
#pragma warning(disable: 4305)      // 'initializing' : truncation from 'const unsigned int' to 'unsigned char'

%}

%%
/*===========================================================================*/
/*===       YACC Grammar for GSD                                             */
/*===========================================================================*/

gsd :
        /* empty */
    |   gsd_header
        gsd_body
    ;

gsd_header :
        _PROFIBUS_DP line_end
    ;

gsd_body :
        standard_gsd
    ;

standard_gsd :
        unit_definition_list
    ;

extended_gsd_item :
        prm_text_def
    |   ext_user_prm_data_def
    |   ext_user_prm_data_const
    |   ext_user_prm_data_ref
    ;

module_def_list :
        module_def_item
    |   module_def_list module_def_item
    ;

module_def_item :
		info_text line_end			{ SET_MODULE_STRING(m_strInfoText,$1)}
    |   channel_diag_def
    |   ext_user_prm_data_const
    |   ext_user_prm_data_ref
	|	alarm_requirement line_end	/* TODO empty */
    |   ext_module_prm_data_len
                                    {
                                        if (CURRENT_MODULE != NULL)
                                        {
                                            CURRENT_MODULE->m_lExtModParamLen = $1;
                                        }
                                    }

    |   user_definition
    |   error CRLF
    ;

module_definition :
        _MODULE '='  module_def_config  line_end
            opt_module_def_ext
        _END_MODULE line_end
                                    {
										FC_RELEASE_PTR(CURRENT_MODULE);
                                    }
    ;

module_def_config :
        module_name config          {
										HRESULT hr = THIS_PTR->CreateModule(*$1,$2);
										SAFE_DELETE($1);
										SAFE_DELETE($2);

                                        if (FAILED(hr)) {
                                            YYRETURN(-1);
                                        }
                                    }
    ;

config :
        octet
                                    { 
                                        $$ = new CByteArray; GUARD($$);
                                        $$->SetSize(0, 10);
                                        $$->Add((BYTE) $1);
                                    }
    |   config ',' octet
                                    {
                                        GUARD($1);
                                        $$->Add((BYTE) $3);
                                    }
    ;

opt_module_def_ext :
        /* empty */
	|	mod_ref_line
	|	module_def_list
	|	mod_ref_line module_def_list
    ;

mod_ref_line :
		module_reference line_end
        {
            if (CURRENT_MODULE != NULL)
            {
                CURRENT_MODULE->m_lModuleReference = $1;
            }
        }
	;

unit_definition_list :
        unit_def_item
    |   unit_definition_list  unit_def_item
    ;

/* Definition of value texts in ExtUserParams */
prm_text_def :
        _PRM_TEXT '=' reference_number line_end
        text_list
        _END_PRM_TEXT line_end
                                    {
                                        if ($5 != NULL) 
                                        {
                                            $5->PutRefNumber($3);
                                            CPrmTextDef* pDef = NULL;
                                            if (PRM_TEXT_TABLE.Lookup($3, pDef))
                                            {
                                                *pDef = *$5;
                                                delete $5;
                                                pDef->PutRefNumber($3);
                                            }
                                            else
                                            {
                                                PRM_TEXT_TABLE.SetAt($3, $5);
                                            }
                                        }
                                        // Empty Text-Lists are ignored!
                                    }
    ;

text_list :
        text_item
                                    {
                                        $$ = new CPrmTextDef;
                                        GUARD($$);
                                        $$->AddItem($1);
                                    }
    |   text_list text_item
                                    {
                                        GUARD($1);
                                        $$->AddItem($2);
                                    }
    |   error CRLF                  { $$ = NULL; }
    ;

text_item :
        _TEXT '(' prm_data_value ')' '=' visible_text line_end
                                    { 
                                        $$ = new CPrmTextItem($3, *$6);
                                        GUARD($$);
                                        SAFE_DELETE($6);
                                    }
    ;

/* Definition of ExtUser data */
ext_user_prm_data_def :
        _EXT_USR_PRM_DATA '=' reference_number  ext_user_prm_data_name line_end
            data_type_name default_value
            ext_user_prm_data_def_values_opt
            ext_user_prm_data_def_opt
        _END_EXT_USR_PRM_DATA line_end
                                    {
                                        if (CURRENT_USR_PRM_DEF == NULL) {
                                            CURRENT_USR_PRM_DEF = new CExtParamDefinition;
                                        }
                                        GUARD(CURRENT_USR_PRM_DEF);
                                        CURRENT_USR_PRM_DEF->m_dataType = $6;        // $6=data_type_name                                      
                                        CURRENT_USR_PRM_DEF->m_strName = *$4;        // $4=ext_user_prm_data_name
                                        SAFE_DELETE($4);
                                        CURRENT_USR_PRM_DEF->m_lReference = $3;
                                        CURRENT_USR_PRM_DEF->m_lDefaultValue = $7;   // $7=default_value
                                        if ($9 != NULL)
                                        {
                                            // have also text definitions!
                                            CURRENT_USR_PRM_DEF->m_textDef = *$9;    // $9=ext_user_prm_data_def_opt
                                        }
                                        USR_PRM_DEF_DB.AddDefinition(CURRENT_USR_PRM_DEF);
                                        CURRENT_USR_PRM_DEF = NULL;
                                    }
    ;

ext_user_prm_data_def_values_opt :
        /* empty */
    |   min_value '-' max_value line_end
                                    {
                                        if (CURRENT_USR_PRM_DEF == NULL) {
                                            CURRENT_USR_PRM_DEF = new CExtParamDefinition;
                                        }
                                        GUARD(CURRENT_USR_PRM_DEF);
                                        CURRENT_USR_PRM_DEF->m_lMinValue = $1;
                                        CURRENT_USR_PRM_DEF->m_lMaxValue = $3;
                                        CURRENT_USR_PRM_DEF->m_paramType = CExtParamDefinition::variableRange;
                                    }
    |   allowed_values
                                    {
                                        if (CURRENT_USR_PRM_DEF == NULL) {
                                            CURRENT_USR_PRM_DEF = new CExtParamDefinition;
                                        }
                                        GUARD(CURRENT_USR_PRM_DEF);
                                        CURRENT_USR_PRM_DEF->m_paramType = CExtParamDefinition::variableEnum;
                                        POSITION pos;
                                        GUARD($1);
                                        for (pos = $1->GetHeadPosition(); pos != NULL; )
                                        {
                                            CURRENT_USR_PRM_DEF->m_listEnumValues.AddTail($1->GetNext(pos));
                                        }
                                        SAFE_DELETE($1);
                                    }
    ;

ext_user_prm_data_def_opt :
        /* empty */                 { $$ = NULL; }
    |   prm_text_ref
                                    {
                                        CPrmTextDef*    pDef = NULL;
                                        if (PRM_TEXT_TABLE.Lookup($1, pDef)) 
                                        {
                                            // text definition exists!
                                            $$ = pDef;
                                        }
                                        else 
                                        {
                                            // Create Forward Declaration of entry that is still to come in GSD
                                            $$ = new CPrmTextDef; 
                                        }
                                        GUARD($$)
                                    }
    ;

prm_text_ref :
        _PRM_TEXT_REF '=' reference_number line_end
                                    {
                                        $$ = $3;
                                    }
    ;

ext_module_prm_data_len :
        _EXT_MOD_PRM_DATA_LEN '=' ext_mod_prm_len line_end
                                    {
                                        $$ = $3;
                                    }
    ;


/* actual "instances" of ExtUserParam (const or variable) */
ext_user_prm_data_const :
        _EXT_USR_PRM_DATA_CONST '(' const_offset ')' '=' const_prm_data line_end
                                    {
                                        CDPExtUserParamObject *pParam = NULL;
                                        HRESULT hr = CComObject<CDP_ExtUserParam>::CreateInstance(&pParam);
                                        if (FAILED(hr)) {
                                            YYRETURN(-1);
                                        }
                                        pParam->AddRef();
                                        /* don't have to lookup in definitions DB */
                                        pParam->m_lOffset = $3;
                                        pParam->m_paramType = CExtParamDefinition::constantValue;
                                        SET_OCTET_VALUE(pParam, m_usrParamBytes, $6);
                                        /* decide what scope we're currently in: */
                                        if (CURRENT_MODULE != NULL) {
                                            // ext params belong to module
                                            CURRENT_MODULE->m_listExtUserParamObject.Add(pParam);
                                        }
                                        else
                                        {
                                            // ext params are device level
                                            STATION->m_listExtUserParamObject.Add(pParam);
                                        }

                                    }
    ;

ext_user_prm_data_ref :
        _EXT_USR_PRM_DATA_REF '(' reference_offset ')' '=' reference_number line_end
                                    {
                                        CDPExtUserParamObject *pParam = NULL;
                                        HRESULT hr = CComObject<CDP_ExtUserParam>::CreateInstance(&pParam);
                                        if (FAILED(hr)) {
                                            YYRETURN(-1);
                                        }
                                        pParam->AddRef();
                                        /* lookup in definitions DB */
                                        pParam->m_lRefNum = $6;
                                        pParam->m_lOffset = $3;
                                        pParam->m_paramType = CExtParamDefinition::referenceValue;

                                        /* decide what scope we're currently in: */
                                        if (CURRENT_MODULE != NULL) {
                                            // ext params belong to module
                                            CURRENT_MODULE->m_listExtUserParamObject.Add(pParam);
                                        }
                                        else
                                        {
                                            // ext params are device level
                                            STATION->m_listExtUserParamObject.Add(pParam);
                                        }
                                    }
    ;

/* slave diagnostics */
diag_def :
        unit_diag_area_def /* for Unit_Diag_Area and Unit_Diag_Bit */
    |   channel_diag_def
    |   user_definition 
    ;

unit_def_item :
        diag_def
    |   extended_gsd_item
    |   module_definition
	|	slot_def
	|	ph_interface_def
    |   _GSD_REVISION '=' unsigned8 line_end    {   
													if(THIS_PTR->CheckRevision($3) == false) {
														YYRETURN(-2);
													}
                                                }
    |   _VENDOR_NAME '=' name_string line_end   {
                                                    SET_STRING_VALUE(STATION, m_strVendorName, $3);
                                                }
    |   _MODEL_NAME '=' name_string line_end    {   
                                                    SET_STRING_VALUE(STATION, m_strModelName, $3);
                                                }
    |   _REVISION '=' name_string line_end      {
                                                    SET_STRING_VALUE(STATION, m_strRevision, $3);
                                                }
    |   _REVISION_NUMBER '=' unsigned8 line_end {
                                                    SET_INT_VALUE(STATION, m_lRevisionNumber, $3);
                                                }
    |   _IDENT_NUMBER '=' unsigned16 line_end   {
                                                    SET_INT_VALUE(STATION, m_lIdentNumber, $3);
                                                }
    |   _PROTOCOL_IDENT '=' unsigned8 line_end  {
                                                    SET_INT_VALUE(STATION, m_lProtocolIdent, $3);
                                                }
    |   _STATION_TYPE '=' unsigned8 line_end    {
                                                    SET_INT_VALUE(STATION, m_lStationType, $3);
                                                    if (0 == $3) {
                                                        STATION->m_stationCategory = DP_Slave;
                                                    }
                                                    else {
                                                        STATION->m_stationCategory = DP_Master;
                                                    }
                                                }
    |   _FMS_SUPP '=' Boolean line_end          {   
                                                    SET_INT_VALUE(STATION, m_bFMS_Supported, $3);
                                                }
    |   _HW_RELEASE '=' name_string line_end    {
                                                    SET_STRING_VALUE(STATION, m_strHW_Release, $3);
                                                }
    |   _SW_RELEASE '=' name_string line_end    {
                                                    SET_STRING_VALUE(STATION, m_strSW_Release, $3);
                                                }
    |   _PHYSICAL_LAYER '=' unsigned8 line_end  {   SET_INT_VALUE(STATION, m_lPhysLayer, $3); }
	|	info_text line_end						{	SET_STRING_VALUE(STATION, m_strInfoText, $1); }
    |   _9_6_SUPP '=' Boolean line_end          {   if ($3) STATION->AddSupportedBaudrate(KBAUD_9_6); }
    |   _19_2_SUPP '=' Boolean line_end         {   if ($3) STATION->AddSupportedBaudrate(KBAUD_19_2); }
    |   _93_75_SUPP '=' Boolean line_end        {   if ($3) STATION->AddSupportedBaudrate(KBAUD_93_75); }
    |   _187_5_SUPP '=' Boolean line_end        {   if ($3) STATION->AddSupportedBaudrate(KBAUD_187_5); }
    |   _500_SUPP '=' Boolean line_end          {   if ($3) STATION->AddSupportedBaudrate(KBAUD_500); }
    |   _1_5M_SUPP '=' Boolean line_end         {   if ($3) STATION->AddSupportedBaudrate(MBAUD_1_5); }
    |   _3M_SUPP '=' Boolean line_end           {   if ($3) STATION->AddSupportedBaudrate(MBAUD_3); }
    |   _6M_SUPP '=' Boolean line_end           {   if ($3) STATION->AddSupportedBaudrate(MBAUD_6); }
    |   _12M_SUPP '=' Boolean line_end          {   if ($3) STATION->AddSupportedBaudrate(MBAUD_12); }
    |   _31_25_SUPP '=' Boolean line_end        {   if ($3) STATION->AddSupportedBaudrate(KBAUD_31_25); }
    |   _45_45_SUPP '=' Boolean line_end        {   if ($3) STATION->AddSupportedBaudrate(KBAUD_45_45); }
    |   _MAXTSDR_9_6 '=' unsigned16 line_end    {   STATION->m_mapMaxTsdr.SetAt(KBAUD_9_6, $3); }
    |   _MAXTSDR_19_2 '=' unsigned16 line_end   {   STATION->m_mapMaxTsdr.SetAt(KBAUD_19_2, $3); }
    |   _MAXTSDR_93_75 '=' unsigned16 line_end  {   STATION->m_mapMaxTsdr.SetAt(KBAUD_93_75, $3); }
    |   _MAXTSDR_187_5 '=' unsigned16 line_end  {   STATION->m_mapMaxTsdr.SetAt(KBAUD_187_5, $3); }
    |   _MAXTSDR_500 '=' unsigned16 line_end    {   STATION->m_mapMaxTsdr.SetAt(KBAUD_500, $3); }
    |   _MAXTSDR_1_5M '=' unsigned16 line_end   {   STATION->m_mapMaxTsdr.SetAt(MBAUD_1_5, $3); }
    |   _MAXTSDR_3M '=' unsigned16 line_end     {   STATION->m_mapMaxTsdr.SetAt(MBAUD_3, $3); }
    |   _MAXTSDR_6M '=' unsigned16 line_end     {   STATION->m_mapMaxTsdr.SetAt(MBAUD_6, $3); }
    |   _MAXTSDR_12M '=' unsigned16 line_end    {   STATION->m_mapMaxTsdr.SetAt(MBAUD_12, $3); }
    |   _MAXTSDR_31_25 '=' unsigned16 line_end  {   STATION->m_mapMaxTsdr.SetAt(KBAUD_31_25, $3); }
    |   _MAXTSDR_45_45 '=' unsigned16 line_end  {   STATION->m_mapMaxTsdr.SetAt(KBAUD_45_45, $3); }
    |   _REDUNDANCY '=' Boolean line_end        {   
                                                    SET_INT_VALUE(STATION, m_bRedundancy, $3);
                                                }
    |   _REPEATER_CTRL_SIG '=' unsigned8 line_end   {   
                                                    SET_INT_VALUE(STATION, m_lRepeaterCtrlSig, $3);
                                                }
    |   _24V_PINS '=' unsigned8 line_end        {   
                                                    SET_INT_VALUE(STATION, m_lPins_24V, $3);
                                                }
    |   _IMPL_TYPE '=' name_string line_end     {
                                                    SET_STRING_VALUE(STATION, m_strImplementationType, $3);
                                                }
    |   _BMP_DEVICE '=' bmp_file_name line_end  {
                                                    SET_STRING_VALUE(STATION, m_strBitmapDevice, $3);
                                                }
    |   _BMP_DIAG '=' bmp_file_name line_end    {
                                                    SET_STRING_VALUE(STATION, m_strBitmapDiag, $3);
                                                }
    |   _BMP_SF '=' bmp_file_name line_end      {
                                                    SET_STRING_VALUE(STATION, m_strBitmapSF, $3);
                                                }
/* master specific */
    |   _DOWNLOAD_SUPP '=' Boolean line_end         {   SET_INT_VALUE(STATION, m_bDownloadSupp, $3); }
    |   _UPLOAD_SUPP '=' Boolean line_end           {   SET_INT_VALUE(STATION, m_bUploadSupp, $3); }
    |   _ACT_PARA_BRCT_SUPP '=' Boolean line_end    {   SET_INT_VALUE(STATION, m_bActParaBrctSupp, $3); }
    |   _ACT_PARAM_SUPP '=' Boolean line_end        {   SET_INT_VALUE(STATION, m_bActParamSupp, $3); }
    |   _MAX_MPS_LENGTH '=' unsigned32 line_end     {   SET_INT_VALUE(STATION, m_lMaxMPSLen, $3); }
    |   _MAX_LSDU_MS '=' unsigned8 line_end         {   SET_INT_VALUE(STATION, m_lMaxLsduMS, $3); }
    |   _MAX_LSDU_MM '=' unsigned8 line_end         {   SET_INT_VALUE(STATION, m_lMaxLsduMM, $3); }
    |   _MIN_POLL_TIMEOUT '=' unsigned16 line_end   {   SET_INT_VALUE(STATION, m_lMinPollTimeout, $3); }
    |   _DPV1_MASTER '=' Boolean line_end           {   SET_INT_VALUE(STATION, m_bDPV1Master, $3); }
	|	_DPV1_CONFORMANCE_CLASS '=' unsigned8 line_end	
													{	SET_INT_VALUE(STATION,m_nDPV1ConformanceClass,(short)$3); }
	|	_C1_MASTER_RW_SUPP '=' Boolean line_end		{	SET_INT_VALUE(STATION,m_bMasterC1RWSupp,$3); }
	|	_M_DPV1_ALARM_SUPP '=' Boolean line_end		{	SET_INT_VALUE(STATION,m_bDPV1AlarmSupp,$3); }
	|	master_alarm_support	line_end			/* TODO empty */
	|	_M_ALARM_SEQ_MODE '=' unsigned8 line_end	/* TODO empty */
	|	_M_ALARM_TYPE_MODE_SUPP '=' Boolean line_end/* TODO empty */
    |   _TRDY_9_6 '=' unsigned8 line_end            {   STATION->m_mapTrdy.SetAt(KBAUD_9_6, $3); }
    |   _TRDY_19_2 '=' unsigned8 line_end           {   STATION->m_mapTrdy.SetAt(KBAUD_19_2, $3); }
    |   _TRDY_93_75 '=' unsigned8 line_end          {   STATION->m_mapTrdy.SetAt(KBAUD_93_75, $3); }
    |   _TRDY_187_5 '=' unsigned8 line_end          {   STATION->m_mapTrdy.SetAt(KBAUD_187_5, $3); }
    |   _TRDY_500 '=' unsigned8 line_end            {   STATION->m_mapTrdy.SetAt(KBAUD_500, $3); }
    |   _TRDY_1_5M '=' unsigned8 line_end           {   STATION->m_mapTrdy.SetAt(MBAUD_1_5, $3); }
    |   _TRDY_3M '=' unsigned8 line_end             {   STATION->m_mapTrdy.SetAt(MBAUD_3, $3); }
    |   _TRDY_6M '=' unsigned8 line_end             {   STATION->m_mapTrdy.SetAt(MBAUD_6, $3); }
    |   _TRDY_12M '=' unsigned8 line_end            {   STATION->m_mapTrdy.SetAt(MBAUD_12, $3); }
    |   _TRDY_31_25 '=' unsigned8 line_end          {   STATION->m_mapTrdy.SetAt(KBAUD_31_25, $3); }
    |   _TRDY_45_45 '=' unsigned8 line_end          {   STATION->m_mapTrdy.SetAt(KBAUD_45_45, $3); }

    |   _TQUI_9_6 '=' unsigned8 line_end            {   STATION->m_mapTqui.SetAt(KBAUD_9_6, $3); }
    |   _TQUI_19_2 '=' unsigned8 line_end           {   STATION->m_mapTqui.SetAt(KBAUD_19_2, $3); }
    |   _TQUI_93_75 '=' unsigned8 line_end          {   STATION->m_mapTqui.SetAt(KBAUD_93_75, $3); }
    |   _TQUI_187_5 '=' unsigned8 line_end          {   STATION->m_mapTqui.SetAt(KBAUD_187_5, $3); }
    |   _TQUI_500 '=' unsigned8 line_end            {   STATION->m_mapTqui.SetAt(KBAUD_500, $3); }
    |   _TQUI_1_5M '=' unsigned8 line_end           {   STATION->m_mapTqui.SetAt(MBAUD_1_5, $3); }
    |   _TQUI_3M '=' unsigned8 line_end             {   STATION->m_mapTqui.SetAt(MBAUD_3, $3); }
    |   _TQUI_6M '=' unsigned8 line_end             {   STATION->m_mapTqui.SetAt(MBAUD_6, $3); }
    |   _TQUI_12M '=' unsigned8 line_end            {   STATION->m_mapTqui.SetAt(MBAUD_12, $3); }
    |   _TQUI_31_25 '=' unsigned8 line_end          {   STATION->m_mapTqui.SetAt(KBAUD_31_25, $3); }
    |   _TQUI_45_45 '=' unsigned8 line_end          {   STATION->m_mapTqui.SetAt(KBAUD_45_45, $3); }

    |   _TSET_9_6 '=' unsigned8 line_end            {   STATION->m_mapTset.SetAt(KBAUD_9_6, $3); }
    |   _TSET_19_2 '=' unsigned8 line_end           {   STATION->m_mapTset.SetAt(KBAUD_19_2, $3); }
    |   _TSET_93_75 '=' unsigned8 line_end          {   STATION->m_mapTset.SetAt(KBAUD_93_75, $3); }
    |   _TSET_187_5 '=' unsigned8 line_end          {   STATION->m_mapTset.SetAt(KBAUD_187_5, $3); }
    |   _TSET_500 '=' unsigned8 line_end            {   STATION->m_mapTset.SetAt(KBAUD_500, $3); }
    |   _TSET_1_5M '=' unsigned8 line_end           {   STATION->m_mapTset.SetAt(MBAUD_1_5, $3); }
    |   _TSET_3M '=' unsigned8 line_end             {   STATION->m_mapTset.SetAt(MBAUD_3, $3); }
    |   _TSET_6M '=' unsigned8 line_end             {   STATION->m_mapTset.SetAt(MBAUD_6, $3); }
    |   _TSET_12M '=' unsigned8 line_end            {   STATION->m_mapTset.SetAt(MBAUD_12, $3); }
    |   _TSET_31_25 '=' unsigned8 line_end          {   STATION->m_mapTset.SetAt(KBAUD_31_25, $3); }
    |   _TSET_45_45 '=' unsigned8 line_end          {   STATION->m_mapTset.SetAt(KBAUD_45_45, $3); }

    |   _TSDI_9_6 '=' unsigned16 line_end           {   STATION->m_mapTsdi.SetAt(KBAUD_9_6, $3); }
    |   _TSDI_19_2 '=' unsigned16 line_end          {   STATION->m_mapTsdi.SetAt(KBAUD_19_2, $3); }
    |   _TSDI_93_75 '=' unsigned16 line_end         {   STATION->m_mapTsdi.SetAt(KBAUD_93_75, $3); }
    |   _TSDI_187_5 '=' unsigned16 line_end         {   STATION->m_mapTsdi.SetAt(KBAUD_187_5, $3); }
    |   _TSDI_500 '=' unsigned16 line_end           {   STATION->m_mapTsdi.SetAt(KBAUD_500, $3); }
    |   _TSDI_1_5M '=' unsigned16 line_end          {   STATION->m_mapTsdi.SetAt(MBAUD_1_5, $3); }
    |   _TSDI_3M '=' unsigned16 line_end            {   STATION->m_mapTsdi.SetAt(MBAUD_3, $3); }
    |   _TSDI_6M '=' unsigned16 line_end            {   STATION->m_mapTsdi.SetAt(MBAUD_6, $3); }
    |   _TSDI_12M '=' unsigned16 line_end           {   STATION->m_mapTsdi.SetAt(MBAUD_12, $3); }
    |   _TSDI_31_25 '=' unsigned16 line_end         {   STATION->m_mapTsdi.SetAt(KBAUD_31_25, $3); }
    |   _TSDI_45_45 '=' unsigned16 line_end         {   STATION->m_mapTsdi.SetAt(KBAUD_45_45, $3); }

    |   _LAS_LEN '=' unsigned8 line_end                 {   SET_INT_VALUE(STATION, m_lLasLen, $3); }
    |   _MAX_SLAVES_SUPP '=' unsigned8 line_end         {   SET_INT_VALUE(STATION, m_lMaxSlavesSupp, $3); }
    |   _MAX_MASTER_INP_LEN '=' unsigned8 line_end      {   SET_INT_VALUE(STATION, m_lMaxMasterInputLen, $3); }
    |   _MAX_MASTER_OUT_LEN '=' unsigned8 line_end      {   SET_INT_VALUE(STATION, m_lMaxMasterOutputLen, $3); }
    |   _MAX_MASTER_DATA_LEN '=' unsigned16 line_end    {   SET_INT_VALUE(STATION, m_lMaxMasterDataLen, $3); }
	|	_MASTER_FREEZE_MODE_SUPP '=' Boolean line_end	{	SET_INT_VALUE(STATION, m_bMasterFreezeModeSupp, $3); }
	|	_MASTER_SYNC_MODE_SUPP '=' Boolean line_end		{	SET_INT_VALUE(STATION, m_bMasterSyncModeSupp, $3); }
	|	_MASTER_FAIL_SAFE_SUPP '=' Boolean line_end		{	SET_INT_VALUE(STATION, m_bFailSafeSupp, $3); }

/* slave specific */
    |   _FREEZE_MODE_SUPP '=' Boolean line_end  {   
                                                    SET_INT_VALUE(STATION, m_bFreezeModeSupp, $3);
                                                }
    |   _SYNC_MODE_SUPP '=' Boolean line_end    {   
                                                    SET_INT_VALUE(STATION, m_bSyncModeSupp, $3);
                                                }
    |   _AUTO_BAUD_SUPP '=' Boolean line_end    {   
                                                    SET_INT_VALUE(STATION, m_bAutoBaudSupp, $3);
                                                }
    |   _SET_SLAVE_ADD_SUPP '=' Boolean line_end    {   
                                                    SET_INT_VALUE(STATION, m_bSetSlaveAddSupp, $3);
                                                }
    |   _USER_PRM_DATA_LEN '=' unsigned8 line_end   {   
                                                    SET_INT_VALUE(STATION, m_lenUserPrmData, $3);
                                                }
    |   _USER_PRM_DATA '=' octet_string line_end    {
                                                    SET_OCTET_VALUE(STATION, m_userPrmData, $3);
                                                }
    |   _USER_PRM_DATA_DEF '=' octet_string line_end    { SAFE_DELETE($3); }
    |   _MIN_SLAVE_INTERVALL '=' unsigned16 line_end    {   
                                                    SET_INT_VALUE(STATION, m_lMinSlaveIntervall, $3);
                                                }
    |   _MODULAR_STATION '=' Boolean line_end   {   
                                                    GUARD(STATION);
                                                    STATION->m_bModularStation = ($3 != 0);
                                                    if (STATION->m_bModularStation) {
                                                        STATION->m_lMaxModule    = (STATION->m_lMaxModule != NOT_SPECIFIED) ? STATION->m_lMaxModule : MISSING;
                                                        STATION->m_lMaxInputLen  = (STATION->m_lMaxInputLen != NOT_SPECIFIED) ? STATION->m_lMaxInputLen : MISSING;
                                                        STATION->m_lMaxOutputLen = (STATION->m_lMaxOutputLen != NOT_SPECIFIED) ? STATION->m_lMaxOutputLen : MISSING;
                                                        STATION->m_lMaxDataLen   = (STATION->m_lMaxDataLen != NOT_SPECIFIED) ? STATION->m_lMaxDataLen : MISSING;
                                                    }
                                                    else {
                                                        STATION->m_lMaxModule    = NOT_SPECIFIED;
                                                        STATION->m_lMaxInputLen  = NOT_SPECIFIED;
                                                        STATION->m_lMaxOutputLen = NOT_SPECIFIED;
                                                        STATION->m_lMaxDataLen   = NOT_SPECIFIED;
                                                    }
                                                }
    |   _MAX_MODULE '=' unsigned8 line_end      {   
                                                    SET_INT_VALUE(STATION, m_lMaxModule, $3);
                                                }
    |   _MAX_INPUT_LEN '=' unsigned8 line_end   {   
                                                    SET_INT_VALUE(STATION, m_lMaxInputLen, $3);
                                                }
    |   _MAX_OUTPUT_LEN '=' unsigned8 line_end  {   
                                                    SET_INT_VALUE(STATION, m_lMaxOutputLen, $3);
                                                }
    |   _MAX_DATA_LEN '=' unsigned16 line_end   {   
                                                    SET_INT_VALUE(STATION, m_lMaxDataLen, $3);
                                                }
    |   _FAIL_SAFE '=' Boolean line_end         {   
                                                    SET_INT_VALUE(STATION, m_bFailSafe, $3);
                                                }
    |   _FAIL_SAVE_REQUIRED		'=' Boolean line_end	{ SET_INT_VALUE(STATION, m_bFailSafeRq, $3);}
	|	_DIAG_UPDATE_DELAY		'=' unsigned8 line_end	{ SET_INT_VALUE(STATION, m_nDiagUpdateDelay,(short)$3);}
    |   _MAX_DIAG_DATA_LEN		'=' unsigned8 line_end	{ SET_INT_VALUE(STATION, m_lMaxDiagDataLen, $3);}
    |   _MODUL_OFFSET			'=' unsigned8 line_end
    |   slave_family line_end
    |   _DPV1_SLAVE				'=' Boolean line_end	{ SET_INT_VALUE(STATION, m_bDPV1Slave, $3);}
	|	_C1_RW_SUPP				'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_CommRel[C1-1].m_bReadWriteSupp,$3);} 
	|	_C2_RW_SUPP				'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_CommRel[C2-1].m_bReadWriteSupp,$3);} 
	|	_C1_MAX_DATA_LEN		'=' unsigned8 line_end	{ SET_INT_VALUE(STATION,m_CommRel[C1-1].m_nMaxDataLen,(short)$3);} 
	|	_C2_MAX_DATA_LEN		'=' unsigned8 line_end	{ SET_INT_VALUE(STATION,m_CommRel[C2-1].m_nMaxDataLen,(short)$3);} 
	|	_C1_RESPONSE_TO			'=' unsigned16 line_end	{ SET_INT_VALUE(STATION,m_CommRel[C1-1].m_lResponseTimeOut,$3);} 
	|	_C2_RESPONSE_TO			'=' unsigned16 line_end	{ SET_INT_VALUE(STATION,m_CommRel[C2-1].m_lResponseTimeOut,$3);} 
	|	_C1_RW_REQ				'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_CommRel[C1-1].m_bReadWriteRequired,$3);} 
	|	_C2_RW_REQ				'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_CommRel[C2-1].m_bReadWriteRequired,$3);} 
	|	_C2_MAX_CNT_CHANNELS	'='	unsigned8 line_end	{ SET_INT_VALUE(STATION,m_CommRel[C2-1].m_nMaxCountChannels,(short)$3);} 
	|	_MAX_INIT_PDU_LEN		'=' unsigned8 line_end	{ SET_INT_VALUE(STATION,m_nMaxInitPDULen,(short)$3);}
	|	_ALARM_SUPP				'=' Boolean line_end	/* TODO empty */
	|	alarm_support	line_end	/* TODO empty */
	|	_ALARM_SEQ_MODE_CNT		'=' unsigned8 line_end	/* TODO empty */
	|	_ALARM_TYPE_MODE_SUPP	'='	Boolean line_end	/* TODO empty */
	|	alarm_requirement line_end	/* TODO empty */
	|	_DPV1_DATA_TYPES		'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_bDPV1DataTypes,$3); }
	|	_WD_BASE_1MS_SUPP		'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_bWDBase1msSupp,$3); }
	|	_CHECK_CFG_MODE			'='	Boolean line_end	{ SET_INT_VALUE(STATION,m_bCheckCfgMode, $3); }
    |   _MAX_USER_PRM_DATA_LEN	'=' unsigned8 line_end	{ SET_INT_VALUE(STATION, m_lMaxUserPrmDataLen, $3);}
    |   error CRLF
    ;

master_alarm_support:
	 	_M_DIAGNOSTIC_ALARM_SUPP	'=' Boolean	/* TODO empty */
    |   _M_PROCESS_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _M_PULL_PLUG_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _M_STATUS_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _M_UPDATE_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _M_MAN_SPEC_ALARM_SUPP		'=' Boolean	/* TODO empty */
	|	_M_EXTRA_ALARM_SAP_SUPP		'=' Boolean /* TODO empty */
	;

alarm_support :
	 	_DIAGNOSTIC_ALARM_SUPP	'=' Boolean	/* TODO empty */
    |   _PROCESS_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _PULL_PLUG_ALARM_SUPP	'=' Boolean	/* TODO empty */
    |   _STATUS_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _UPDATE_ALARM_SUPP		'=' Boolean	/* TODO empty */
    |   _MAN_SPEC_ALARM_SUPP	'=' Boolean	/* TODO empty */
	|	_EXTRA_ALARM_SAP_SUPP	'=' Boolean /* TODO empty */
	;
	
alarm_requirement:
	 	_DIAGNOSTIC_ALARM_RQ	'=' Boolean	/* TODO empty */
    |   _PROCESS_ALARM_RQ		'=' Boolean	/* TODO empty */
    |   _PULL_PLUG_ALARM_RQ		'=' Boolean	/* TODO empty */
    |   _STATUS_ALARM_RQ		'=' Boolean	/* TODO empty */
    |   _UPDATE_ALARM_RQ		'=' Boolean	/* TODO empty */
    |   _MAN_SPEC_ALARM_RQ		'=' Boolean	/* TODO empty */
	;

channel_diag_def :
        _CHANNEL_DIAG '(' error_type ')' '=' diag_text line_end
		{ 
			HRESULT hr = THIS_PTR->CreateChannelDiag($3,*$6);
			SAFE_DELETE($6);
			if (FAILED(hr)) {
				YYRETURN(-1);
			}
		}
    ;

info_text:
		_INFO_TEXT '=' name_string { $$ = $3; }
	;

module_refs:
		module_reference '-' module_reference	{ THIS_PTR->AddModRef($1,$3); }
	|	mod_ref_string
	;

opt_module_refs:
		/*empty*/
	|	module_refs
	;

slot_item:
		_SLOT '(' unsigned8 ')' '=' name_string module_reference
		{ THIS_PTR->ClearModRefList(); }
		opt_module_refs line_end
		{	
			GUARD($6);
			HRESULT hr = THIS_PTR->CreateSlot($3,$6,$7); 
			//clean up first
			SAFE_DELETE($6);
			THIS_PTR->ClearModRefList();
			if (FAILED(hr)) {
				YYRETURN(-1);
			}
		}
	;

slot_list:
		slot_item
	|	slot_list slot_item
	;

slot_def:
		_SLOT_DEF line_end
		slot_list
		_END_SLOT_DEF line_end
	;

tx_delay:
		_TX_DEL_9_6			{ $$ = KBAUD_9_6; }
	|	_TX_DEL_19_2		{ $$ = KBAUD_19_2; }
	|	_TX_DEL_31_25		{ $$ = KBAUD_31_25; }
	|	_TX_DEL_45_45		{ $$ = KBAUD_45_45; }
	|	_TX_DEL_93_75		{ $$ = KBAUD_93_75; }
	|	_TX_DEL_187_5		{ $$ = KBAUD_187_5; }
	|	_TX_DEL_500			{ $$ = KBAUD_500; }
	|	_TX_DEL_1_5M		{ $$ = MBAUD_1_5; }
	|	_TX_DEL_3M			{ $$ = MBAUD_3; }
	|	_TX_DEL_6M			{ $$ = MBAUD_6; }
	|	_TX_DEL_12M			{ $$ = MBAUD_12; }
	;

rc_delay:
		_RC_DEL_9_6			{ $$ = KBAUD_9_6; }
	|	_RC_DEL_19_2		{ $$ = KBAUD_19_2 ; }
	|	_RC_DEL_31_25		{ $$ = KBAUD_31_25; }
	|	_RC_DEL_45_45		{ $$ = KBAUD_45_45; }
	|	_RC_DEL_93_75		{ $$ = KBAUD_93_75; }
	|	_RC_DEL_187_5		{ $$ = KBAUD_187_5; }
	|	_RC_DEL_500			{ $$ = KBAUD_500; }
	|	_RC_DEL_1_5M		{ $$ = MBAUD_1_5; }
	|	_RC_DEL_3M			{ $$ = MBAUD_3; }
	|	_RC_DEL_6M			{ $$ = MBAUD_6; }
	|	_RC_DEL_12M			{ $$ = MBAUD_12; }
	;

ph_delay_item :
	tx_delay '=' unsigned16 line_end
		{	if(CURRENT_PI) { CURRENT_PI->AddTXDelay($1,$3); } }
	| rc_delay '=' unsigned16 line_end
		{	if(CURRENT_PI) { CURRENT_PI->AddRCDelay($1,$3); } }
	;

ph_delay_list :
	ph_delay_item
	|	ph_delay_list ph_delay_item
	;

ph_interface_def:
		_PHYSICAL_IF '=' unsigned8 line_end		
		{ 
			HRESULT hr = THIS_PTR->CreatePIObject($3); 
			if (FAILED(hr)) {
				YYRETURN(-1);
			}
		}
		ph_delay_list _END_PHYSICAL_IF line_end	/* do we need a opt_ph_delay_list ? */
	;

error_type:	
		unsigned8
	;

unit_diag_area_def :
		_UNIT_DIAG_AREA '=' bit_number '-' bit_number line_end
		{
			HRESULT hr = THIS_PTR->CreateUnitDiag(DiagArea,$3,$5); 
			//clean up first
			if (FAILED(hr)) {
				YYRETURN(-1);
			}
		}
		value_list
        _UNIT_DIAG_AREA_END line_end
		| _UNIT_DIAG_BIT '(' bit_number ')' '=' diag_text line_end
		{
			HRESULT hr = THIS_PTR->CreateUnitDiag(DiagBit,$3,NOT_SPECIFIED);
			//clean up first
			if (FAILED(hr)) {
				SAFE_DELETE($6);
				YYRETURN(-1);
			}

			if(CURRENT_DIAG != NULL) { 
				CURRENT_DIAG->AddValue(1,*$6);
			}

			SAFE_DELETE($6);
		}
    ;

value_list :
        value_item
    |   value_list  value_item
    ;

value_item :
        _VALUE '(' value ')' '=' diag_text line_end
		{
			if(CURRENT_DIAG != NULL) { 
				CURRENT_DIAG->AddValue($3,*$6);
			}

			SAFE_DELETE($6);
		}
    ;

visible_text :
        _VISIBLE_STRING             { $$ = new CString($1);  GUARD($$); }
    ;

prm_data_value :
        numeric
    ;

allowed_values :
        data_type_array line_end    { $$ = $1; }
    ;

max_value :
        numeric
    ;

min_value :
        numeric
    ;

default_value :
        numeric
    ;

line_end :
        CRLF
    |   _COMMENT
    |   line_end _COMMENT
    |   line_end CRLF
    ;


data_type_array :
        numeric
                                    {
                                        $$ = new listEnumValues;
                                        GUARD($$);
                                        $$->AddTail($1);
//                                        if (CURRENT_USR_PRM_DEF == NULL) {
//                                            CURRENT_USR_PRM_DEF = new CExtParamDefinition;
//                                        }
//                                        GUARD(CURRENT_USR_PRM_DEF);
//                                        CURRENT_USR_PRM_DEF->m_paramType = CExtParamDefinition::variableEnum;
                                    }
    |   data_type_array ',' numeric
                                    {
                                        GUARD($1);
                                        $$->AddTail($3);
                                    }
    ;

data_type_name :
        _TYPE_UNSIGNED8             { $$=uSignedInt8;  }
    |   _TYPE_UNSIGNED16            { $$=uSignedInt16; }
    |   _TYPE_UNSIGNED32            { $$=uSignedInt32; }
    |   _TYPE_SIGNED8               { $$=signedInt8;   }
    |   _TYPE_SIGNED16              { $$=signedInt16;  }
    |   _TYPE_SIGNED32              { $$=signedInt32;  }
    |   _TYPE_BIT '(' bit_number ')'
                                    {
                                        if (CURRENT_USR_PRM_DEF == NULL) {
                                            CURRENT_USR_PRM_DEF = new CExtParamDefinition;
                                        }
                                        GUARD(CURRENT_USR_PRM_DEF);
                                        CURRENT_USR_PRM_DEF->m_lAreaStart = $3;
                                        CURRENT_USR_PRM_DEF->m_lAreaLength = 1;                                      
                                        $$=bitArea;
                                    }
    |   bit_area
    ;

bit_area :        
        _TYPE_BIT_AREA '(' bit_number '-' bit_number ')'
                                    {
                                        if (CURRENT_USR_PRM_DEF == NULL) {
                                            CURRENT_USR_PRM_DEF = new CExtParamDefinition;
                                        }
                                        GUARD(CURRENT_USR_PRM_DEF);
                                        CURRENT_USR_PRM_DEF->m_lAreaStart = $3;
                                        CURRENT_USR_PRM_DEF->m_lAreaLength = ($5 - $3) + 1;
                                        $$=bitArea;
                                    }
    ;

module_reference :
        unsigned16
    ;

mod_ref_string :
		module_reference					{ THIS_PTR->AddModRef($1); }
	|	mod_ref_string ',' module_reference	{ THIS_PTR->AddModRef($3); }
	;

const_prm_data :
        octet_string
    ;

const_offset :
        unsigned8
    ;

reference_offset :
        unsigned8
    ;

reference_number :
        unsigned16
    ;

ext_mod_prm_len :
        unsigned8
    ;

bmp_file_name :
        _VISIBLE_STRING             { $$ = new CString($1);  GUARD($$); }
    ;

sub_family :
		_SUB_FAMILY					{ $$ = new CString($1); GUARD($$); }
    |   sub_family _SUB_FAMILY		{ GUARD($1); *($1) += $2; $$ = $1;}
    ;

slave_family :
	_SLAVE_FAMILY '=' unsigned8		{ THIS_PTR->SetSlaveFamily($3,NULL); }
	| _SLAVE_FAMILY '=' unsigned8 sub_family 
									{ THIS_PTR->SetSlaveFamily($3,$4); SAFE_DELETE($4);}
	;

value :
        unsigned16
    ;

module_name :
        _VISIBLE_STRING             { $$ = new CString($1);  GUARD($$); }
    ;

name_string :
        _VISIBLE_STRING             { $$ = new CString($1);  GUARD($$); }
    ;

ext_user_prm_data_name :
        _VISIBLE_STRING             { $$ = new CString($1);  GUARD($$); }
    ;

diag_text :
        _VISIBLE_STRING				{ $$ = new CString($1);  GUARD($$); }
    ;

user_definition :
        _USER_KEYWORD line_end      /* empty */
  |   _USER_KEYWORD                 /* empty */
    ;

octet_string :
    octet                           { 
                                        $$ = new CByteArray; GUARD($$);
                                        $$->SetSize(0, 10);
                                        $$->Add((BYTE) $1);
                                    }
    |   octet_string ',' octet      
                                    {
                                        GUARD($1);
                                        $$->Add((BYTE) $3);
                                    }
    ;

octet :
        numeric                     { 
                                        if ($1 > 0xff) {    // must be a byte value!
                                            YYRETURN(-1);
                                        }
                                        $$ = $1;
                                    }
    ;

unsigned8 :
        numeric
    ;

unsigned16 :
        numeric
    ;

unsigned32 :
        numeric
    ;

/*
signed8 :
        numeric
    ;

signed16 :
        numeric
    ;

signed32 :
        numeric
    ;
*/

Boolean :
        numeric                     { $$ = (BOOL) ($1 != 0); }
    ;

bit_number :
        numeric
    ;

numeric :
        _INTEGER_LITERAL
    | '-' _INTEGER_LITERAL          { $$ = -$2; }
    ;


%%

//------------------------------------------------------------------*
/**
 * Default ctor of GSD Parser object. Creates station object of 
 * type CDP_Station.
 * @see	~gsd_Parser(), CDP_Station
*/
gsd_Parser::gsd_Parser(int sz)
  : yy_parse(sz),
    m_pCurrUserParamDef(NULL),
    m_pCurrModule(NULL),
	m_pPI(NULL),
	m_ModRefList(),
	m_pUnitDiag(NULL)
{
    HRESULT hr = CComObject<CDP_Station>::CreateInstance(&m_pStation);
    _ASSERTE(SUCCEEDED(hr));
    _ASSERTE(m_pStation != NULL);
    if (m_pStation != NULL) {
        m_pStation->AddRef();
    }
}

//------------------------------------------------------------------*
/**
 * Dtor of GSD Parser object. Releases the hold reference of the 
 * station object CDP_Station and cleans up members.
 * @see	gsd_Parser(), CDP_Station
*/
gsd_Parser::~gsd_Parser()
{
    // now clean up our own stuff
	FC_RELEASE_PTR(m_pStation);
	FC_RELEASE_PTR(m_pPI);
	FC_RELEASE_PTR(m_pUnitDiag);
	FC_RELEASE_PTR(m_pCurrModule)
	ClearModRefList();

    POSITION pos;
    for (pos = m_mapTextDefDB.GetStartPosition(); pos != NULL; )
    {
        LONG    lRef;
        CPrmTextDef*    pDef = NULL;
        m_mapTextDefDB.GetNextAssoc(pos, lRef, pDef);
        ASSERT(pDef != NULL);
        if (pDef != NULL) {
            delete pDef;
        }
    }
    m_mapTextDefDB.RemoveAll();
}

//------------------------------------------------------------------*
HRESULT gsd_Parser::GetDP_Station(IUnknown **pUnk)
{
    HRESULT hr = S_OK;
    if (pUnk == NULL) {
        return E_POINTER;
    }

    if (m_pStation != NULL) {
        *pUnk = m_pStation->GetUnknown();
        (*pUnk)->AddRef();
    }
    else {
        *pUnk = NULL;
        hr = E_NOINTERFACE;
    }
    return hr;
}

//------------------------------------------------------------------*

void gsd_Parser::processObjectList(listExtUserParamObject* exList)
{
	int iCount = exList->GetCount();
    if (!iCount) {	// ready, as there are no used references
        return;
    }

    for (int i=0;i < iCount;i++) {
		//[] operator makes implicit addref()
        CDPExtUserParamObject* pExt = (*exList)[i];
		if(!pExt) {
			_ASSERTE(pExt);
			continue;
		}

        CExtParamDefinition *pDefinition = NULL;
        if (pExt->m_paramType != CExtParamDefinition::constantValue)
        {
            if (USR_PRM_DEF_DB.GetDefinition(pExt->m_lRefNum, pDefinition))
            {
                /* copy contents of definition */
                pExt->CopyDefinitionValues(pDefinition);
            }
            else
            {
				CString str;
				GSD_FormatMessage(str, E_GSD_TEXT_REF,(int)pExt->m_lRefNum);
				Message(E_GSD_TEXT_REF,str);
            }
        }

		FC_RELEASE_PTR(pExt);
    }
}

void gsd_Parser::PostProcess()
{
    int iCount;
	CDPModuleObject* pModule= NULL;

	// process list of errors and messages.
	processMessages();
    processObjectList(&(STATION->m_listExtUserParamObject));

    // iterate through all MODULES
	iCount = STATION->m_listModules.GetCount();

	for(int i= 0; i< iCount; i++) {
		pModule = STATION->m_listModules[i];
		if(pModule) {
			processObjectList(&(pModule->m_listExtUserParamObject));
			pModule->Release();
		}
	}

	STATION->Check();
	STATION->SlotAssignment();
}

//------------------------------------------------------------------*
/**
 * Checks revision of GSD agains supported GSD. GSD parser upports up 
 * to GSD revision 2.0. Issues a warning if GSD revision is higher 
 * than the supported revision, but continues parsing.
 *
 * @param			lRevision - GSD revision number scanned from GSD.
 * @see				gsd_parser, define SUPPORTED_GSD_REVISION.
*/
bool gsd_Parser::CheckRevision(long lRevision)
{
	CString str;
	m_pStation->m_lGSD_Revision = lRevision;
	
	if(lRevision > SUPPORTED_GSD_REVISION) {
		GSD_FormatMessage(str, E_GSD_REVISION,(int)lRevision,SUPPORTED_GSD_REVISION);
		Message(E_GSD_REVISION,str);
		return false;
	}

	return true;
}

void gsd_Parser::SetSlaveFamily(long lMainFamily, CString *pOptSubFamily)
{
	if(!m_pStation) {
		return;
	}

	m_pStation->m_lMainFamily = lMainFamily;
	m_pStation->m_strSlaveFamily.Format(_T("%d"),lMainFamily);
	if(pOptSubFamily) {
		m_pStation->m_strSlaveFamily += *pOptSubFamily;
	}
}

HRESULT gsd_Parser::CreatePIObject(long lInterfaceId)
{
    CDP_PIObject *pPI = NULL;
	CDP_PIObject *pTemp = NULL;

    HRESULT hr = CComObject<CDP_PhysicalInterface>::CreateInstance(&pPI);
    if (FAILED(hr)) {
        return hr;
    }

	pPI->m_lInterfaceId = lInterfaceId;
	pPI->AddRef(); //this reference is for the list.
    m_pStation->m_listPI.Add(pPI);

	pTemp = m_pPI;
	pPI->AddRef(); //for the current physical inteface reference.
	m_pPI = pPI; //assign new one

	FC_RELEASE_PTR(pTemp); //release the old one
	return S_OK;
}

HRESULT gsd_Parser::CreateSlot(long lSlotNo, CString* pstrName, long lDefault)
{
	CDP_SlotObject* pSlot = NULL;

    HRESULT hr = CComObject<CDP_Slot>::CreateInstance(&pSlot);
    if (FAILED(hr)) {
        return hr;
    }

	pSlot->m_lSlotNo = lSlotNo;
	if(pstrName) {
		pSlot->m_strName = *pstrName;
	}
	pSlot->m_lDefRef = lDefault;
	pSlot->AddModuleReferences(&m_ModRefList);
	pSlot->AddRef(); //this reference is for the list.
    m_pStation->m_listSlot.Add(pSlot);
	return S_OK;
}

void gsd_Parser::ClearModRefList()
{
	m_ModRefList.RemoveAll();	
}

void gsd_Parser::AddModRef(long lStart, long lEnd)
{
	if(lEnd == 0L) {
		m_ModRefList.AddTail(lStart);
	}
	else {
		long l;
		if(lEnd < lStart) {
			l = lStart;
			lStart = lEnd;
			lEnd = l;
		}

		for(l = lStart; l<= lEnd; l++) {
			m_ModRefList.AddTail(l);
		}
	}
}

HRESULT gsd_Parser::CreateModule(LPCTSTR pszName,CByteArray *pOctets)
{
	CDPModuleObject *pModule = NULL;
	CDPModuleObject *pTemp = NULL;

	FC_PCHECK_HR(pszName,E_POINTER);
	FC_PCHECK_HR(pOctets,E_POINTER);

	HRESULT hr = CComObject<CDP_Module>::CreateInstance(&pModule);
	if (FAILED(hr)) {
		return hr;
	}
	
	pModule->m_strName = pszName;
	pModule->m_configBytes.Copy(*pOctets);

	pModule->AddRef(); //this reference is for the list.
	m_pStation->m_listModules.Add(pModule);

	pTemp = m_pCurrModule;
	pModule->AddRef(); //for the current module.
	m_pCurrModule = pModule; //assign new one

	FC_RELEASE_PTR(pTemp); //release the old one
	return S_OK;
}

void gsd_Parser::processMessages()
{
	CString strFile;

	CGsdScan *pScan = dynamic_cast<CGsdScan*>(scan);
	if(!pScan) {
		return;
	}

	pScan->SetMessages(m_pStation);
	strFile = pScan->GetGsdFile();
	m_pStation->SetGsdFile(strFile);
}

void gsd_Parser::Message(HRESULT hr,LPCTSTR pszMessage)
{
    CGsdScan *pScan = dynamic_cast<CGsdScan*>(scan);
    if(!pScan) {
        return;
    }
	pScan->Message(hr,pszMessage);
}

HRESULT gsd_Parser::CreateUnitDiag(DIAG_TYPE type, long lBitStart,long lBitEnd)
{
	CDP_UnitDiagObject* p = NULL;
	CDP_UnitDiagObject* pTemp = NULL;

    HRESULT hr = CComObject<CDP_UnitDiag>::CreateInstance(&p);
    if (FAILED(hr)) {
        return hr;
    }

	p->m_Type = type;
	p->m_lBitAreaStart = lBitStart;
	p->m_lBitAreaEnd = lBitEnd;
	p->AddRef(); //this reference is for the list.
    //add to list of unit diagnostic definitions 
	m_pStation->m_listUnitDiag.Add(p);

	pTemp = m_pUnitDiag;
	p->AddRef(); //for the current unit diagnostic definition.
	m_pUnitDiag = p; //assign new one

	FC_RELEASE_PTR(pTemp); //release the old one
	return S_OK;
}

HRESULT gsd_Parser::CreateChannelDiag(long lErrorType, LPCTSTR pszDiagText)
{
	CDP_UnitDiagObject* p = NULL;
	CDP_UnitDiagObject* pTemp = NULL;

    HRESULT hr = CComObject<CDP_UnitDiag>::CreateInstance(&p);
    if (FAILED(hr)) {
        return hr;
    }

	p->m_Type = DiagChannel;
	p->m_lBitAreaStart = -1L;
	p->m_lBitAreaEnd = -1L;
	//this reference is for the list.
	p->AddValue(lErrorType, pszDiagText);
	p->AddRef(); 

	if(m_pCurrModule != NULL) { //module specific
		m_pCurrModule->m_listDiag.Add(p);
	}
	else { //station specific
	    //add to list of unit diagnostic definitions 
		m_pStation->m_listUnitDiag.Add(p);
	}

	return S_OK;
}




