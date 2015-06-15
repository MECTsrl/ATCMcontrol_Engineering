
#ifndef __DP_STATION_H_
#define __DP_STATION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*
#include "defs.h"
#include "DP_Module.h"
#include "dp_physicalinterface.h"
#include "DP_ExtParam.h"
#include "dp_slot.h"
#include "dp_unitdiag.h"

//----  Forward Class Definitions:   -----------------------------------------*
class   yy_parse;
//----  Defines:    ----------------------------------------------------------*

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CCommRelProperties
 *  Properties of communication relationship (C1, C2)
 *  @see    ....
*/
class CCommRelProperties
{
public:
    CCommRelProperties()
    { Reset(); }

    void    Reset();
    
    BOOL    m_bReadWriteSupp;
    BOOL    m_bReadWriteRequired;
    short   m_nMaxDataLen;
    long    m_lResponseTimeOut;
    short   m_nMaxCountChannels;
};

/////////////////////////////////////////////////////////////////////////////
// CDP_Station

class ATL_NO_VTABLE CDP_Station : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDP_Station, &CLSID_DP_Station>,
	public ISupportErrorInfo,
    public CStationCallback,
	public IDispatchImpl<IDP_Station, &IID_IDP_Station, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IDP_Master, &IID_IDP_Master, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IDP_Slave, &IID_IDP_Slave, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IDP_Master2, &IID_IDP_Master2, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IDP_Slave2, &IID_IDP_Slave2, &LIBID_PARSEGSDLib,2,0>,
	public IDispatchImpl<IDP_Station2, &IID_IDP_Station2, &LIBID_PARSEGSDLib>
{
    friend  yy_parse;       // needed to comfortably set private members from parser!
    friend class gsd_Parser;
//--------------------------
// =Constructor
public:
	                        CDP_Station();
	virtual                 ~CDP_Station();
    HRESULT                 FinalConstruct();
    void                    FinalRelease();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CDP_Station)
	COM_INTERFACE_ENTRY(IDP_Station)
    COM_INTERFACE_ENTRY(IDP_Master)
    COM_INTERFACE_ENTRY(IDP_Slave)
    COM_INTERFACE_ENTRY2(IDispatch, IDP_Station)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IDP_Master2)
	COM_INTERFACE_ENTRY(IDP_Slave2)
	COM_INTERFACE_ENTRY(IDP_Station2)
END_COM_MAP()

//--------------------------
// =Interface
// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

//--------------------------
//= Internal CStationCallBack interface.
    IModule * LookupModule(long lModuleReference);
    void AddMessage(IParseMessage *);
    void SetErrorCount(long);
    void Message(HRESULT, LPCTSTR);

//--------------------------
// =Interface
// IDP_Station
public:
    STDMETHOD(get_ErrorCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_StationCategory)(/*[out, retval]*/ DP_STATION_CAT *pVal);
	STDMETHOD(get_ErrorStrings)(/*[out, retval]*/ LPUNKNOWN *pVal);
	STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FileDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_GSD_Revision)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Vendor_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Model_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Revision)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Revision_Number)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Ident_Number)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Protocol_Ident)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Station_Type)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_HW_Release)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SW_Release)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FMS_supported)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_Redundancy)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_Repeater_Ctrl_Sig)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Pins_24V)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Physical_Layer)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Implementation_Type)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Bitmap_Device)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Bitmap_Diag)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_Bitmap_SF)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_SupportedBaudrates)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_MaxTsdr)(enum tagBAUDRATE baudrate, /*[out, retval]*/ long *pVal);

//--------------------------
// =Interface
// IDP_Station2
	STDMETHOD(get_SupportedGSDRevision)(LONG * pVal);
    STDMETHOD(SetMessageFilter)(MESSAGEFILTER);
	STDMETHOD(get_ParseMessageCollection)(IParseMessageCollection * * pp);
	STDMETHOD(get_PhysicalInterfaceCollection)(IPhysicalInterfaceCollection * * pp);

//--------------------------
// =Interface
// IDP_Master
public:
	STDMETHOD(get_IsDPV1Master)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_DownloadSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_UploadSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_ActParaBrctSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_ActParamSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_MaxMPSLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxLsduMS)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxLsduMM)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MinPollTimeout)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_LasLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxSlavesSupp)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxMasterInputLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxMasterOutputLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxMasterDataLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Trdy)(enum tagBAUDRATE baudrate, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_Tqui)(enum tagBAUDRATE baudrate, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_Tset)(enum tagBAUDRATE baudrate, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_Tsdi)(enum tagBAUDRATE baudrate, /*[out, retval]*/ long *pVal);
//--------------------------
// =Interface
//IDP_Master2
	STDMETHOD(get_MasterFreezeModeSupp)(VARIANT_BOOL * pVal);
	STDMETHOD(get_MasterSyncModeSupp)(VARIANT_BOOL * pVal);
	STDMETHOD(get_FailSafeSupp)(VARIANT_BOOL * pVal);
	STDMETHOD(get_DPV1ConformanceClass)(SHORT * pVal);
	STDMETHOD(get_C1ReadWriteSupp)(VARIANT_BOOL * pVal);
	STDMETHOD(get_DPV1AlarmSupp)(VARIANT_BOOL * pVal);

//--------------------------
// =Interface
// IDP_Slave
public:
	STDMETHOD(get_LenUserParams)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_FreezeModeSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_SyncModeSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_AutoBaudSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_SetSlaveAddSupp)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_UserPrmData)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_MinSlaveIntervall)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ModularStation)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_MaxModule)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxInputLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxOutputLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxDataLen)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_FailSafe)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(get_MaxDiagDataLen)(/*[out, retval]*/ long *pVal);
    STDMETHOD(get_SlaveFamily)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_HasExtUserParam)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(CreateModuleEnumerator)(/*[out, retval]*/ LPUNKNOWN *pVal);
    STDMETHOD(CreateExtUserParamEnumerator)(/*[out, retval]*/ LPUNKNOWN *pVal);
//--------------------------
// =Interface
// IDP_Slave2
	STDMETHOD(get_IsDPV1Slave)(VARIANT_BOOL * pVal);
	STDMETHOD(get_DiagUpdateDelay)(SHORT * pVal);
	STDMETHOD(get_InfoText)(BSTR * psVal);
	STDMETHOD(get_ReadWriteSupp)(tagCommunicationRelationship commrel, VARIANT_BOOL * pVal);
	STDMETHOD(get_MaxDataLen)(tagCommunicationRelationship commrel, SHORT * pVal);
	STDMETHOD(get_ResponseTimeOut)(tagCommunicationRelationship commrel, LONG * pVal);
	STDMETHOD(get_ReadWriteRequired)(tagCommunicationRelationship commrel, VARIANT_BOOL * pVal);
	STDMETHOD(get_MaxCountChannels)(tagCommunicationRelationship commrel, SHORT * pVal);
	STDMETHOD(get_MaxInitatePDULength)(SHORT * pVal);
	STDMETHOD(get_DPV1DataTypes)(VARIANT_BOOL * pVal);
	STDMETHOD(get_WDBase1msSupp)(VARIANT_BOOL * pVal);
	STDMETHOD(get_CheckCfgMode)(VARIANT_BOOL * pVal);
	STDMETHOD(get_FailSafeRequired)(VARIANT_BOOL * pVal);
	STDMETHOD(get_ModuleCollection)(IModuleCollection ** pp);
	STDMETHOD(get_SlotCollection)(ISlotCollection * * pp);
    STDMETHOD(get_DiagnosticCollection)(IDiagnosticInfoCollection** pp);
    STDMETHOD(get_NoDiagosticBytes)(short *pVal);
    STDMETHOD(get_ExtUserParamCollection)(IExtUserParamCollection** pp);
 
// =Operations
    void  AddSupportedBaudrate(BAUDRATE baudrate)
    {
        long lSupported = TRUE; // to keep the template compiler happy
        m_wSupportedBaudrates.Add((WORD) baudrate);
        m_mapSupportedBaudrates.SetAt(baudrate, lSupported);
    }

    BOOL IsBaudrateSupported(BAUDRATE baudrate)
    {
        long    lVal;
        return m_mapSupportedBaudrates.Lookup(baudrate, lVal);
    }

    void SetGsdFile(CString &);
    void Check();
private:
// = Operations
    HRESULT AllocSysString(BSTR*,CString &);
    void SlotAssignment();
// =Attributes
    long                    m_lErrorCount;
    MESSAGEFILTER           m_filter;
    ParseMessageList        m_messages;
    DP_STATION_CAT          m_stationCategory;
    CString                 m_strFileName;
    DATE                    m_fileDate;

    // the DP GSD features:
    //----  IDP_Station
    long                    m_lGSD_Revision;
    CString                 m_strVendorName;
    CString                 m_strModelName;
    CString                 m_strRevision;
    long                    m_lRevisionNumber;
    long                    m_lIdentNumber;
    long                    m_lProtocolIdent;
    long                    m_lStationType;
    CString                 m_strHW_Release;
    CString                 m_strSW_Release;
    BOOL                    m_bFMS_Supported;
    BOOL                    m_bRedundancy;
    long                    m_lRepeaterCtrlSig;
    long                    m_lPins_24V;
    long                    m_lPhysLayer;
    CString                 m_strImplementationType;
    CString                 m_strBitmapDevice;
    CString                 m_strBitmapDiag;
    CString                 m_strBitmapSF;
    CWordArray              m_wSupportedBaudrates;
    mapBaudrateToValue      m_mapSupportedBaudrates;
    mapBaudrateToValue      m_mapMaxTsdr;

    //----  IDP_Master
    BOOL                    m_bDPV1Master;
    BOOL                    m_bDownloadSupp;
    BOOL                    m_bUploadSupp;
    BOOL                    m_bActParaBrctSupp;
    BOOL                    m_bActParamSupp;
    long                    m_lMaxMPSLen;
    long                    m_lMaxLsduMS;
    long                    m_lMaxLsduMM;
    long                    m_lMinPollTimeout;
    long                    m_lLasLen;
    long                    m_lMaxSlavesSupp;
    long                    m_lMaxMasterInputLen;
    long                    m_lMaxMasterOutputLen;
    long                    m_lMaxMasterDataLen;
    mapBaudrateToValue      m_mapTrdy;
    mapBaudrateToValue      m_mapTqui;
    mapBaudrateToValue      m_mapTset;
    mapBaudrateToValue      m_mapTsdi;
    BAUDRATE                m_queryBaudrate;
    //----  IDP_Master2
    BOOL                    m_bMasterFreezeModeSupp;
    BOOL                    m_bMasterSyncModeSupp;
    BOOL                    m_bFailSafeSupp;
    BOOL                    m_bMasterC1RWSupp;
    BOOL                    m_bDPV1AlarmSupp;
    short                   m_nDPV1ConformanceClass;

    //----  IDP_Slave, IDP_Slave2
    BOOL                    m_bDPV1Slave;
	BOOL                    m_bFreezeModeSupp;
	BOOL                    m_bSyncModeSupp;
	BOOL                    m_bAutoBaudSupp;
	BOOL                    m_bSetSlaveAddSupp;
    long                    m_lenUserPrmData;
	CByteArray              m_userPrmData;
	long                    m_lMinSlaveIntervall;
	BOOL                    m_bModularStation;
	long                    m_lMaxModule;
	long                    m_lMaxInputLen;
	long                    m_lMaxOutputLen;
	long                    m_lMaxDataLen;
	BOOL                    m_bFailSafe;
    BOOL                    m_bFailSafeRq;
	long                    m_lMaxDiagDataLen;
    long                    m_lMaxUserPrmDataLen;
    long                    m_lMainFamily;
    CString                 m_strSlaveFamily;
    short                   m_nDiagUpdateDelay;
    CString                 m_strInfoText;
    CCommRelProperties      m_CommRel[C2];
    short                   m_nMaxInitPDULen;
    BOOL                    m_bDPV1DataTypes;
    BOOL                    m_bWDBase1msSupp;
    BOOL                    m_bCheckCfgMode;
    /** list of modules in enumerator */
    listModuleObject        m_listModules;
    /** list of ext user parameters objects of device */
    listExtUserParamObject  m_listExtUserParamObject;
    /** list of physical interface objects */
    listPI                  m_listPI;
    /** list of slot objects */
    listSlot                m_listSlot;
    /** list of unit diagnostic definitions */
    listUnitDiag            m_listUnitDiag;
};


//------------------------------------------------------------------*
/**
 *  class CEnumErrors
 *      enumerate parser errors by string
 *
 *  @see    ....
*/
class CEnumErrors :
        public CComObjectRoot,
	    public IDispatchImpl<IEnumString, &IID_IExtUserParam, &LIBID_PARSEGSDLib,2,0>
//        public IEnumString,
{
//--------------------------
// =Constructor
public:
							CEnumErrors();
							~CEnumErrors();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CEnumErrors)
    COM_INTERFACE_ENTRY(IEnumString)
END_COM_MAP()

//--------------------------
// =Operations
public:
	void					Initialize(CStringArray& errors, int iEnum = 0);

//--------------------------
// =Interface
// IEnumString
    STDMETHODIMP            Next(ULONG		celt,
								 LPOLESTR*	ppStrings,
								 ULONG*		pceltFetched);
    STDMETHODIMP            Skip(ULONG celt);
    STDMETHODIMP            Reset(void);
    STDMETHODIMP            Clone(IEnumString** ppEnumString);

//--------------------------
// =Attributes
private:
	int                     m_iEnum;
								// iterator for any of the groups
	CStringArray            m_errors;
                                // array of errors
};

typedef CComObject<CEnumErrors> CEnumErrorsObject;


/////////////////////////////////////////////////////////////////////
#endif // __DP_STATION_H_

