//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DP_Station.h"
#include "resource.h"
#include "gsdmessage.h"
#include "parsemessage.h"
#include "gsd_keyw.h"
#include "gsd_scan.h"
#include "Parser.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CCommRelProperties::Reset()
{
    m_bReadWriteSupp = DEFAULT;
    m_bReadWriteRequired = DEFAULT;
    m_nMaxDataLen = DEFAULT;
    m_lResponseTimeOut = DEFAULT;
    m_nMaxCountChannels = DEFAULT;
}

/////////////////////////////////////////////////////////////////////////////
// CDP_Station


//------------------------------------------------------------------*
CDP_Station::CDP_Station()
  : m_lErrorCount(0),
    m_fileDate(0.0),
    m_stationCategory(noDP),
    m_lGSD_Revision(NOT_SPECIFIED),
    m_lRevisionNumber(NOT_SPECIFIED),
    m_lIdentNumber(MISSING),
    m_lProtocolIdent(MISSING),
    m_lStationType(MISSING),
    m_bFMS_Supported(DEFAULT),
    m_bRedundancy(DEFAULT),
    m_lRepeaterCtrlSig(DEFAULT),
    m_lPins_24V(DEFAULT),
    m_lPhysLayer(NOT_SPECIFIED),
    m_bFreezeModeSupp(DEFAULT),
    m_bSyncModeSupp(DEFAULT),
    m_bAutoBaudSupp(DEFAULT),
    m_bSetSlaveAddSupp(DEFAULT),
    m_lenUserPrmData(DEFAULT),
    m_lMinSlaveIntervall(MISSING),
    m_bModularStation(DEFAULT),
    m_lMaxModule(NOT_SPECIFIED),
    m_lMaxInputLen(NOT_SPECIFIED),
    m_lMaxOutputLen(NOT_SPECIFIED),
    m_lMaxDataLen(NOT_SPECIFIED),
    m_bFailSafe(DEFAULT),
    m_lMaxDiagDataLen(MISSING),
    m_lMaxUserPrmDataLen(NOT_SPECIFIED),
    m_bDPV1Master(FALSE),
    m_bDownloadSupp(DEFAULT),
    m_bUploadSupp(DEFAULT),
    m_bActParaBrctSupp(DEFAULT),
    m_bActParamSupp(DEFAULT),
    m_lMaxMPSLen(MISSING),
    m_lMaxLsduMS(MISSING),
    m_lMaxLsduMM(MISSING),
    m_lMinPollTimeout(MISSING),
    m_lLasLen(NOT_SPECIFIED),
    m_lMaxSlavesSupp(MISSING),
    m_lMaxMasterInputLen(NOT_SPECIFIED),
    m_lMaxMasterOutputLen(NOT_SPECIFIED),
    m_lMaxMasterDataLen(NOT_SPECIFIED),
    m_queryBaudrate(KBAUD_9_6),
    m_strInfoText(),
    m_listPI(),
    m_listSlot(),
    m_messages(),
    m_listUnitDiag()
{
    //load missing string and preset other string members.
    m_strVendorName.LoadString(IDS_MISSING_STRING);
    m_strModelName = m_strVendorName;
    m_strRevision = m_strVendorName;
    m_strHW_Release = m_strVendorName;
    m_strSW_Release = m_strVendorName;
    m_strSlaveFamily = m_strVendorName;
    
    m_bMasterFreezeModeSupp = DEFAULT;
    m_bMasterSyncModeSupp = DEFAULT;
    m_bFailSafeSupp = DEFAULT;
    m_bMasterC1RWSupp = DEFAULT;
    m_bDPV1AlarmSupp = DEFAULT;
    m_nDPV1ConformanceClass = NOT_SPECIFIED;
    m_nDiagUpdateDelay = DEFAULT;
    m_bFailSafeRq = DEFAULT;
    m_bDPV1Slave = DEFAULT;
    m_nMaxInitPDULen = DEFAULT;
    m_bDPV1DataTypes = DEFAULT;
    m_bWDBase1msSupp = DEFAULT;
    m_bCheckCfgMode = DEFAULT;
    m_lMainFamily = DEFAULT;
    m_filter = NoFilter;

    for(int i= 0; i< FC_ARRAY_LEN(m_CommRel); i++) {
        m_CommRel[i].Reset();
    }

}

//------------------------------------------------------------------*
CDP_Station::~CDP_Station()
{}

//------------------------------------------------------------------*
HRESULT CDP_Station::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void CDP_Station::FinalRelease()
{
	CDPModuleObject* pModule = NULL;
    
    m_listPI.RemoveAll();
    m_listSlot.RemoveAll();
    m_listModules.RemoveAll();
    m_messages.RemoveAll();
    m_listExtUserParamObject.RemoveAll();
    m_listUnitDiag.RemoveAll();
}

HRESULT CDP_Station::AllocSysString(BSTR* ps,CString & str)
{
    return ::AllocSysString(ps,str);
}

void CDP_Station::SlotAssignment()
{
    int iCount = m_listSlot.GetCount();
    for(int i= 0;i<iCount;i++) {
        CDP_SlotObject* pSlot = m_listSlot[i]; //already addref'd
        if(pSlot) {
            pSlot->SlotAssignment(this);
            pSlot->Release();
        }
    }

}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDP_Station,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_ErrorCount(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lErrorCount;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_ErrorStrings(LPUNKNOWN *ppUnk)
{
    //this method is only for legacy support.
    IParseMessage * pmsg = NULL;
    int iCount = 0;
    BSTR s = NULL;
    HRESULT hr = S_OK;
    VARIANT_BOOL bError;
    
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(ppUnk,E_POINTER);
   
    *ppUnk = NULL;
    
    try {
        CStringArray errors;
        CString str;

        iCount = m_messages.GetCount();
        for(int i= 0; i< iCount; i++) {
            pmsg = m_messages[i];
            if(!pmsg) {
                continue;
            }
            
            pmsg->IsError(&bError);
            if(bError == VARIANT_TRUE) {
                pmsg->get_Message(&s);
                str = s;
                FC_FREE_BSTR(s);
                errors.Add(str);
            }

            FC_RELEASE_PTR(pmsg);
        }

	    CEnumErrorsObject *pEnum = new CEnumErrorsObject;
        if (pEnum == NULL) {
		    return E_OUTOFMEMORY;
        }
	
        pEnum->Initialize(errors);
	    hr = pEnum->QueryInterface(IID_IEnumString, (void**)ppUnk);
    }
    catch(CMemoryException * e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }

    FC_FREE_BSTR(s);
    FC_RELEASE_PTR(pmsg);
    return hr;
}


//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_FileName(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strFileName);
}
//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_FileDate(DATE *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_fileDate;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_StationCategory(DP_STATION_CAT * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_stationCategory;
	return S_OK;
}

//=============================================================================
//      IDP_Station
//=============================================================================

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_GSD_Revision(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lGSD_Revision;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Vendor_Name(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strVendorName);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Model_Name(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strModelName);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Revision(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strRevision);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Revision_Number(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lRevisionNumber;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Ident_Number(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lIdentNumber;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Protocol_Ident(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lProtocolIdent;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Station_Type(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lStationType;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_HW_Release(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strHW_Release);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_SW_Release(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strSW_Release);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_FMS_supported(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_bFMS_Supported;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Redundancy(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bRedundancy;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Repeater_Ctrl_Sig(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lRepeaterCtrlSig;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Pins_24V(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lPins_24V;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Physical_Layer(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lPhysLayer;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Implementation_Type(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strImplementationType);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Bitmap_Device(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strBitmapDevice);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Bitmap_Diag(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_strBitmapDiag.AllocSysString();
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Bitmap_SF(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_strBitmapSF.AllocSysString();
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_SupportedBaudrates(VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    pVal->vt = VT_I2 | VT_ARRAY;   // array of shorts
    SAFEARRAYBOUND boundData;
    boundData.lLbound = 0;
    boundData.cElements = m_wSupportedBaudrates.GetSize();

    SAFEARRAY   *psaData = ::SafeArrayCreate(VT_I2, 1, &boundData);
    if (psaData == NULL) {
        return E_OUTOFMEMORY;
    }
    
    // copy baudrates to SAFEARRAY
    HRESULT hr;
    if (boundData.cElements > 0)
    {
        const WORD *pBaudrates = m_wSupportedBaudrates.GetData();
        _ASSERTE(pBaudrates != NULL);
        BYTE *pRawData;
        hr = ::SafeArrayAccessData(psaData, (void HUGEP* FAR*)&pRawData);
        ::memcpy(pRawData, pBaudrates, m_wSupportedBaudrates.GetSize() * sizeof(WORD));
        ::SafeArrayUnaccessData(psaData);
    }
    pVal->parray = psaData;

	return S_OK;
}


//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxTsdr(enum tagBAUDRATE baudrate, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (!IsBaudrateSupported(baudrate)) {
        *pVal = NOT_SPECIFIED;
        return S_FALSE;
    }
    *pVal = MISSING;
    long lVal = 0;
    if (!m_mapMaxTsdr.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;

	return S_OK;
}

//=============================================================================
//      IDP_Station2
//=============================================================================

STDMETHODIMP CDP_Station::get_SupportedGSDRevision(LONG * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = SUPPORTED_GSD_REVISION;		
	return S_OK;
}

STDMETHODIMP CDP_Station::SetMessageFilter(MESSAGEFILTER filter)
{
    m_filter = filter;
    return S_OK;
}

STDMETHODIMP CDP_Station::get_ParseMessageCollection(IParseMessageCollection ** pp)
{
    int iCount,i;
    CComObject<ParseMessageCollection> *pc = NULL;
    IParseMessage *pmsg = NULL;
    VARIANT_BOOL bError;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<ParseMessageCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = 0;
        if(m_filter == NoFilter) {
            iCount = m_messages.GetCount();
        }
        else {
            for(i= 0; i< m_messages.GetCount(); i++) {
                pmsg = m_messages[i]; //makes an addref.
                if(!pmsg) {
                    _ASSERTE(pmsg);
                    continue;
                }

                pmsg->IsError(&bError);

                switch(m_filter) {
                case Errors:
                    if(bError == VARIANT_TRUE) {
                        iCount++;
                    }
                    break;
                case Warnings:
                    if(bError == VARIANT_FALSE) {
                        iCount++;
                    }
                    break;
                default:
                    break;
                }

                FC_RELEASE_PTR(pmsg);
            }
        }

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        bool bInsert;
        for(i= 0; i< iCount; i++) {
            pmsg = m_messages[i]; //makes an addref.
            if(!pmsg) {
                _ASSERTE(pmsg);
                continue;
            }

            pmsg->IsError(&bError);
            bInsert = false;

            switch(m_filter) {
            case NoFilter:
                bInsert = true;
                break;
            case Errors:
                if(bError == VARIANT_TRUE) {
                    bInsert = true;
                }
                break;
            case Warnings:
                if(bError == VARIANT_FALSE) {
                    bInsert = true;
                }
                break;
            default:
                break;
            }

            if(bInsert) {
                hrLoop = pc->Insert(i,pmsg);
                if(FAILED(hrLoop)) {
                    hr = hrLoop;
                }
            }

            FC_RELEASE_PTR(pmsg);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

STDMETHODIMP CDP_Station::get_PhysicalInterfaceCollection(IPhysicalInterfaceCollection ** pp)
{
    int iCount,i;
    CComObject<PhysicalInterfaceCollection> *pc = NULL;
    IPhysicalInterface *pPI = NULL;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<PhysicalInterfaceCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listPI.GetCount();
        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDP_PIObject * p = m_listPI[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            pPI = dynamic_cast<IPhysicalInterface*>(p);
            _ASSERTE(pPI);

            hrLoop = pc->Insert(i,pPI);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pPI);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}


//=============================================================================
//      IDP_Master
//=============================================================================


//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_IsDPV1Master(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bDPV1Master;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_DownloadSupp(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bDownloadSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_UploadSupp(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bUploadSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_ActParaBrctSupp(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bActParaBrctSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_ActParamSupp(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bActParamSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxMPSLen(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxMPSLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxLsduMS(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxLsduMS;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxLsduMM(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxLsduMM;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MinPollTimeout(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMinPollTimeout;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_LasLen(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lLasLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxSlavesSupp(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxSlavesSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxMasterInputLen(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxMasterInputLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxMasterOutputLen(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxMasterOutputLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxMasterDataLen(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxMasterDataLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Trdy(enum tagBAUDRATE baudrate, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (!IsBaudrateSupported(baudrate)) {
        *pVal = NOT_SPECIFIED;
        return S_FALSE;
    }
    *pVal = MISSING;
    long lVal = 0;
    if (!m_mapTrdy.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Tqui(enum tagBAUDRATE baudrate, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (!IsBaudrateSupported(baudrate)) {
        *pVal = NOT_SPECIFIED;
        return S_FALSE;
    }
    *pVal = MISSING;
    long lVal = 0;
    if (!m_mapTqui.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Tset(enum tagBAUDRATE baudrate, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (!IsBaudrateSupported(baudrate)) {
        *pVal = NOT_SPECIFIED;
        return S_FALSE;
    }
    *pVal = MISSING;
    long lVal = 0;
    if (!m_mapTset.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_Tsdi(enum tagBAUDRATE baudrate, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);

    if (!IsBaudrateSupported(baudrate)) {
        *pVal = NOT_SPECIFIED;
        return S_FALSE;
    }
    *pVal = MISSING;
    long lVal = 0;
    if (!m_mapTsdi.Lookup(baudrate, lVal)) {
        *pVal = MISSING;
        return S_FALSE;
    }
    *pVal = lVal;
	return S_OK;
}

STDMETHODIMP CDP_Station::get_MasterFreezeModeSupp(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bMasterFreezeModeSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_MasterSyncModeSupp(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bMasterSyncModeSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_FailSafeSupp(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bFailSafeSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_DPV1ConformanceClass(SHORT * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_nDPV1ConformanceClass;		
	return S_OK;
}

STDMETHODIMP CDP_Station::get_C1ReadWriteSupp(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bMasterC1RWSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_DPV1AlarmSupp(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bDPV1AlarmSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

//=============================================================================
//      IDP_Slave
//=============================================================================

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_FreezeModeSupp(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_bFreezeModeSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_SyncModeSupp(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bSyncModeSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_AutoBaudSupp(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bAutoBaudSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_SetSlaveAddSupp(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bSetSlaveAddSupp;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_UserPrmData(VARIANT * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    pVal->vt = VT_UI1 | VT_ARRAY;   // array of bytes
    SAFEARRAYBOUND boundData;
    boundData.lLbound = 0;
    boundData.cElements = m_userPrmData.GetSize();

    SAFEARRAY   *psaData = ::SafeArrayCreate(VT_UI1, 1, &boundData);
    if (psaData == NULL) {
        return E_OUTOFMEMORY;
    }
    
    // copy UsrParam bytes to SAFEARRAY
    HRESULT hr;
    const BYTE *pParamBytes = m_userPrmData.GetData();
    if (pParamBytes != NULL)
    {
        BYTE *pRawData;
        hr = ::SafeArrayAccessData(psaData, (void HUGEP* FAR*)&pRawData);
        ::memcpy(pRawData, pParamBytes, m_userPrmData.GetSize());
        ::SafeArrayUnaccessData(psaData);
    }
    pVal->parray = psaData;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MinSlaveIntervall(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMinSlaveIntervall;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_ModularStation(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_bModularStation;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxModule(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxModule;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxInputLen(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxInputLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxOutputLen(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxOutputLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_LenUserParams(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pVal == NULL)
		return E_POINTER;

	*pVal = m_lenUserPrmData;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxDataLen(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lMaxDataLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_FailSafe(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_bFailSafe;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_MaxDiagDataLen(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lMaxDiagDataLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_SlaveFamily(BSTR * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return AllocSysString(pVal,m_strSlaveFamily);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::get_HasExtUserParam(BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    BOOL bModule = FALSE;
    *pVal = (m_listExtUserParamObject.GetCount() > 0);

    int iCount = m_listModules.GetCount();
    for(int i = 0;i<iCount; i++) {
        //[] operator makes implicit addref.
        CDPModuleObject* pModule = m_listModules[i];
        if(!pModule) {
            _ASSERTE(pModule);
            continue;
        }

        pModule->get_HasExtUserParam(&bModule);
        *pVal = *pVal || bModule;
        pModule->Release();
    }
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::CreateModuleEnumerator(LPUNKNOWN *ppUnk)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(ppUnk,E_POINTER);
    *ppUnk = NULL;

	CEnumModuleObject *pEnumModules = new CEnumModuleObject;
    if (!pEnumModules) {
		return E_OUTOFMEMORY;
    }
	pEnumModules->Initialize(m_listModules);
	return pEnumModules->QueryInterface(IID_IEnumUnknown, (void**)ppUnk);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Station::CreateExtUserParamEnumerator(LPUNKNOWN *ppUnk)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(ppUnk,E_POINTER);
    *ppUnk = NULL;

	CEnumExtUserParamObject *pEnumUserParams = new CEnumExtUserParamObject;
    if (pEnumUserParams == NULL) {
		return E_OUTOFMEMORY;
    }
	pEnumUserParams->Initialize(m_listExtUserParamObject);
	return pEnumUserParams->QueryInterface(IID_IEnumUnknown, (void**)ppUnk);
}

//=============================================================================
//      IDP_Slave2
//=============================================================================

STDMETHODIMP CDP_Station::get_IsDPV1Slave(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bDPV1Slave) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_DiagUpdateDelay(SHORT * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_nDiagUpdateDelay;		
	return S_OK;
}

STDMETHODIMP CDP_Station::get_InfoText(BSTR * psVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(psVal,E_POINTER);
    return AllocSysString(psVal,m_strInfoText);
}

STDMETHODIMP CDP_Station::get_ReadWriteSupp(tagCommunicationRelationship commrel, VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    
    CCommRelProperties & ref = m_CommRel[commrel-1];
    *pVal = (ref.m_bReadWriteSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_MaxDataLen(tagCommunicationRelationship commrel, SHORT * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    
    CCommRelProperties & ref = m_CommRel[commrel-1];
    *pVal = ref.m_nMaxDataLen;
    return S_OK;
}

STDMETHODIMP CDP_Station::get_ResponseTimeOut(tagCommunicationRelationship commrel, LONG * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    
    CCommRelProperties & ref = m_CommRel[commrel-1];
    *pVal = ref.m_lResponseTimeOut;
    return S_OK;
}

STDMETHODIMP CDP_Station::get_ReadWriteRequired(tagCommunicationRelationship commrel, VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    
    CCommRelProperties & ref = m_CommRel[commrel-1];
    *pVal = (ref.m_bReadWriteRequired) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_MaxCountChannels(tagCommunicationRelationship commrel, SHORT * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    //commrel must be C2
    if(commrel != C2) {
        return E_INVALIDARG;
    }
    
    CCommRelProperties & ref = m_CommRel[commrel-1];
    *pVal = ref.m_nMaxCountChannels;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_MaxInitatePDULength(SHORT * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_nMaxInitPDULen;
	return S_OK;
}

STDMETHODIMP CDP_Station::get_DPV1DataTypes(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bDPV1DataTypes) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_WDBase1msSupp(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bWDBase1msSupp) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_CheckCfgMode(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bCheckCfgMode) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_FailSafeRequired(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_bFailSafeRq) ? VARIANT_TRUE : VARIANT_FALSE;
   	return S_OK;
}

STDMETHODIMP CDP_Station::get_ModuleCollection(IModuleCollection * * pp)
{
    int iCount,i;
    CComObject<ModuleCollection> *pc = NULL;
    IModule *pModule = NULL;
    HRESULT hr = S_OK,hrLoop;
    
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<ModuleCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listModules.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDPModuleObject * p = m_listModules[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }

            pModule = dynamic_cast<IModule*>(p);
            _ASSERTE(pModule);

            hrLoop = pc->Insert(i,pModule);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pModule);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

STDMETHODIMP CDP_Station::get_SlotCollection(ISlotCollection * * pp)
{
    int iCount,i;
    CComObject<SlotCollection> *pc = NULL;
    ISlot *pSlot;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<SlotCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listSlot.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDP_SlotObject * p = m_listSlot[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            pSlot = dynamic_cast<ISlot*>(p);
            _ASSERTE(pSlot);

            hrLoop = pc->Insert(i,pSlot);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pSlot);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

STDMETHODIMP CDP_Station::get_DiagnosticCollection(IDiagnosticInfoCollection** pp)
{
    int iCount,i;
    CComObject<DiagCollection> *pc = NULL;
    IDiagnosticInfo *pInfo;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<DiagCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listUnitDiag.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDP_UnitDiagObject * p = m_listUnitDiag[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            pInfo = dynamic_cast<IDiagnosticInfo*>(p);
            _ASSERTE(pInfo);

            hrLoop = pc->Insert(i,pInfo);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pInfo);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

STDMETHODIMP CDP_Station::get_NoDiagosticBytes(short *pVal)
{
    int iCount,i;
    long l,lMax;
    DIAG_TYPE type;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);

    iCount = m_listUnitDiag.GetCount();
    lMax = 0L;

    for(i= 0; i< iCount; i++) {
        CDP_UnitDiagObject * p = m_listUnitDiag[i]; //makes an addref.
        if(!p) {
            _ASSERTE(p);
            continue;
        }

        p->get_DiagnosticType(&type);

        l = 0L;
        switch(type) {
        case DiagBit:
            p->get_BitAreaStart(&l);
            break;
        case DiagArea:
            p->get_BitAreaEnd(&l);
            break;
        case DiagChannel:
            FC_RELEASE_PTR(p);
            continue;
        default: //l stays 0L
            break;
        }

        lMax = max(lMax,l);

        FC_RELEASE_PTR(p);
    }

    *pVal = (lMax / 8) + ((lMax % 8) > 0 ? 1 : 0 ); //assumes bitsize of byte == 8
    return S_OK;
}

STDMETHODIMP CDP_Station::get_ExtUserParamCollection(IExtUserParamCollection** pp)
{
    int iCount,i;
    CComObject<ExtUserParamCollection> *pc = NULL;
    IExtUserParam *pParam;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<ExtUserParamCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listExtUserParamObject.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDPExtUserParamObject * p = m_listExtUserParamObject[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            pParam = dynamic_cast<IExtUserParam*>(p);
            _ASSERTE(pParam);

            hrLoop = pc->Insert(i,pParam);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pParam);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

IModule * CDP_Station::LookupModule(long lModuleReference)
{
    IModule * pRet = NULL;
    POSITION pos = NULL;
    long l;
    int iCount;

    iCount = m_listModules.GetCount();
    for(int i = 0;i<iCount; i++) {
        //[] operator makes implicit addref.
        CDPModuleObject* pModule = m_listModules[i];
        if(!pModule) {
            _ASSERTE(pModule);
            continue;
        }

        pModule->get_ModuleReference(&l);
        if(l == lModuleReference) {
            pModule->QueryInterface(&pRet); //makes the addref
            pModule->Release(); //addref of [] operator
            return pRet;
        }

        pModule->Release();
    }

    return pRet;
}

void CDP_Station::AddMessage(IParseMessage *pMessage)
{
    if(pMessage) { //already addref'd
        m_messages.Add(pMessage);
    }
}

void CDP_Station::SetErrorCount(long lCount)
{
    m_lErrorCount += lCount;
}

void CDP_Station::Message(HRESULT hrMessage, LPCTSTR pszMessage)
{
	try {
		VARIANT_BOOL bError;
		ParseMessage * pmsg = NULL;

        pmsg = ::CreateParseMessage(hrMessage,pszMessage);
        if(!pmsg) {
            return;
        }

		m_messages.Add(pmsg);

		pmsg->IsError(&bError);
		if(bError == VARIANT_TRUE) {
		    m_lErrorCount++;	
		}
	}
	catch(CMemoryException* e) {
		e->Delete();
	}
}

void CDP_Station::SetGsdFile(CString & str)
{
	m_strFileName = str;
    // try to determine file date:
    m_fileDate = 0.0;
    HANDLE hFile = ::CreateFile(m_strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        FILETIME    ft;
        if (::GetFileTime(hFile, NULL, NULL, &ft))
        {
            SYSTEMTIME  tmSys;
            FILETIME    ftLocal;
            ::FileTimeToLocalFileTime(&ft, &ftLocal);
            ::FileTimeToSystemTime(&ftLocal, &tmSys);
            ::SystemTimeToVariantTime(&tmSys, &m_fileDate);
        }
        ::CloseHandle(hFile);
    }
}


//------------------------------------------------------------------*
/**
 * Performs semantic checks after parse process.
 * @see	GSD Specification for PROFIBUS-DP Version 4.1		
*/
void CDP_Station::Check()
{
    int iCount,i;
    CDPModuleObject * pModule = NULL;

    try {
        CGsdKeyword keywords;
        CString str,strMsg;
        //Check DPV1_Conformance_Class
        if( m_lGSD_Revision >= 3 && m_bDPV1Master == TRUE &&
            m_nDPV1ConformanceClass == NOT_SPECIFIED) {
            //issue warning - Conformance class of the DP-Master (Class1) 
            //required as of GSD Revision 3
            GSD_FormatMessage(strMsg, W_GSD_CONFORMANCE_CLASS);
            Message(W_GSD_CONFORMANCE_CLASS,strMsg);
        }

        //Check PrmText values agains boundaries of ExtUserPrmData
        iCount = m_listExtUserParamObject.GetCount();
        for(i= 0; i< iCount; i++) {
            CDPExtUserParamObject * p = m_listExtUserParamObject[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            
            p->Check(this);
            FC_RELEASE_PTR(p);
        }

        //Check modules for module reference etc.
        iCount = m_listModules.GetCount();
        for(i= 0; i< iCount; i++) {
            pModule = m_listModules[i]; //makes an addref.
            if(!pModule) {
                _ASSERTE(pModule);
                continue;
            }

            pModule->Check(m_lGSD_Revision,this);
            FC_RELEASE_PTR(pModule);
        }
        
        // checks according to section 2.3.9 Slave related keywords for DP extensions
        // of GSD Specification for PROFIBUS-DP Version 4.1
        if(m_bDPV1Slave == FALSE) {
            if(m_CommRel[C1-1].m_bReadWriteSupp) {
                str = keywords.KeywordFromToken(_C1_RW_SUPP);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK01,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK01,strMsg);
            }

            if(m_bDPV1DataTypes) {
                str = keywords.KeywordFromToken(_DPV1_DATA_TYPES);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK01,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK01,strMsg);
            }

            if(m_bCheckCfgMode) {
                str = keywords.KeywordFromToken(_CHECK_CFG_MODE);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK01,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK01,strMsg);
            }
        }
        else { //DPV1-Slave specified
            //C1 communication relationship checks
            if(!m_CommRel[C1-1].m_bReadWriteSupp) { // No C1 support
                if(m_CommRel[C1-1].m_nMaxDataLen > 0) {
                    str = keywords.KeywordFromToken(_C1_MAX_DATA_LEN);
                    GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK02,(LPCTSTR)str);
                    Message(W_GSD_DPV1_CHECK02,strMsg);
                }

                if(m_CommRel[C1-1].m_lResponseTimeOut > 0) {
                    str = keywords.KeywordFromToken(_C1_RESPONSE_TO);
                    GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK02,(LPCTSTR)str);
                    Message(W_GSD_DPV1_CHECK02,strMsg);
                }

                if(m_CommRel[C1-1].m_bReadWriteRequired) {
                    str = keywords.KeywordFromToken(_C1_RW_REQ);
                    GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK02,(LPCTSTR)str);
                    Message(W_GSD_DPV1_CHECK02,strMsg);
                }

            }
            //else TODO Checks with alarm config
        }

        //C2 communication relationship checks
        if(!m_CommRel[C2-1].m_bReadWriteSupp) { // No C2 support
            if(m_CommRel[C2-1].m_nMaxDataLen > 0) {
                str = keywords.KeywordFromToken(_C2_MAX_DATA_LEN);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK03,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK03,strMsg);
            }

            if(m_CommRel[C2-1].m_lResponseTimeOut > 0) {
                str = keywords.KeywordFromToken(_C2_RESPONSE_TO);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK03,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK03,strMsg);
            }

            if(m_CommRel[C2-1].m_bReadWriteRequired) {
                str = keywords.KeywordFromToken(_C2_RW_REQ);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK03,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK03,strMsg);
            }

            if(m_CommRel[C2-1].m_nMaxCountChannels > 0) {
                str = keywords.KeywordFromToken(_C2_MAX_CNT_CHANNELS);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK03,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK03,strMsg);
            }

            if(m_nMaxInitPDULen > 0) {
                str = keywords.KeywordFromToken(_MAX_INIT_PDU_LEN);
                GSD_FormatMessage(strMsg,W_GSD_DPV1_CHECK03,(LPCTSTR)str);
                Message(W_GSD_DPV1_CHECK03,strMsg);
            }
        }
    }
    catch(CMemoryException* e) {
        e->Delete();
    }

    FC_RELEASE_PTR(pModule);
}


/////////////////////////////////////////////////////////////////////
// CEnumErrors

//------------------------------------------------------------------*
CEnumErrors::CEnumErrors()
{
	m_iEnum = 0;
}

//------------------------------------------------------------------*
CEnumErrors::~CEnumErrors()
{}

//------------------------------------------------------------------*
void CEnumErrors::Initialize(CStringArray& errors, int iEnum /* = 0 */)
{
    m_errors.RemoveAll();
    m_errors.Copy(errors);
	Reset();
    m_iEnum = iEnum;
}

//------------------------------------------------------------------*
//Implementation for IEnumUnknown
STDMETHODIMP CEnumErrors::Next(ULONG celt, LPOLESTR* ppStrings, ULONG* pceltFetched)
{
    ULONG   cntAdded = 0;
    USES_CONVERSION;

	CString strCurrent;
    for (ULONG i = 0; (i < celt) && (m_iEnum < m_errors.GetSize()); i++)
	{
        strCurrent = m_errors.GetAt(m_iEnum++);
		ppStrings[cntAdded] = (LPWSTR) ::CoTaskMemAlloc((strCurrent.GetLength() + 1) * sizeof(WCHAR));
		ASSERT(ppStrings[cntAdded] != NULL);
		::wcscpy(ppStrings[cntAdded], T2W((LPTSTR)(LPCTSTR)strCurrent));
		cntAdded++;
	}
	if (pceltFetched != NULL) {
		*pceltFetched = cntAdded;
	}

	return (celt == cntAdded) ? S_OK : S_FALSE;
	
}

//------------------------------------------------------------------*
STDMETHODIMP CEnumErrors::Skip(ULONG celt)
{
    ULONG i;
	for (i = 0; (i < celt) && (m_iEnum < m_errors.GetSize()); i++)
	{
    	m_iEnum++;
	}

	return (celt == i) ? S_OK : S_FALSE;
}

//------------------------------------------------------------------*
STDMETHODIMP CEnumErrors::Reset(void)
{
	m_iEnum = 0;
	return S_OK;	
}

//------------------------------------------------------------------*
// Implementation for IEnumUnknown
STDMETHODIMP CEnumErrors::Clone(IEnumString **ppEnumString)
{
    if (ppEnumString == NULL) {
        return E_POINTER;
    }
    *ppEnumString = NULL;

    CComObject<CEnumErrors>   *pEnum = NULL;
    HRESULT hr = CComObject<CEnumErrors>::CreateInstance(&pEnum);
    if (FAILED(hr)) {
        return hr;
    }
	pEnum->Initialize(m_errors, m_iEnum);
    // this QI also increments the missing AddRef from CreateInstance !!
	return pEnum->QueryInterface(IID_IEnumString, (LPVOID*)ppEnumString);
} 


