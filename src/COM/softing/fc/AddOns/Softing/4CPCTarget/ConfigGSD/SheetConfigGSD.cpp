
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ConfigGSD.h"
#include "SheetConfigGSD.h"

#include "ParseGSD_i.c"
#include "pgsd_i.c"
#include "defines.h"
#include "ExtParam.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetConfigGSD

IMPLEMENT_DYNAMIC(CSheetConfigGSD, CPropertySheet)

//------------------------ DEFAULT CONSTRUCTOR -------------------------------*
CSheetConfigGSD::CSheetConfigGSD() : CPropertySheet()
{
	InitSheet();
}

//-------------------------- CONSTRUCTOR -------------------------------------*
CSheetConfigGSD::CSheetConfigGSD(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	InitSheet();
}

//-------------------------- CONSTRUCTOR -------------------------------------*
CSheetConfigGSD::CSheetConfigGSD(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	InitSheet();
}

//--------------------------- DESTRUCTOR -------------------------------------*
CSheetConfigGSD::~CSheetConfigGSD()
{
	if (m_pParser)
		m_pParser->Release();
	m_pParser = NULL;
	if (m_pUnknown)
		m_pUnknown->Release();
	m_pUnknown = NULL;

	delete m_pSlaveInfos;
	delete m_pSlaveModule;
	delete m_pModuleSpec;
	delete m_pErrorPage;
	delete m_pCfgUserData;
}


BEGIN_MESSAGE_MAP(CSheetConfigGSD, CPropertySheet)
	//{{AFX_MSG_MAP(CSheetConfigGSD)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetConfigGSD message handlers

//----------------------------- FUNCTION -------------------------------------*
void CSheetConfigGSD::InitSheet()
{
	m_pUnknown = NULL;
	m_pParser = NULL;
	m_pHelpConnection = NULL;
	m_bSpecificView = FALSE;
	m_wSuppBaudRates = 0;

	HRESULT hr;
	hr = ::CoInitialize(NULL);
	_ASSERTE(SUCCEEDED(hr));
	CLSID   clsid = CLSID_ParseGsdFile;
	hr = ::CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IParseGsdFile, (void**) &m_pParser);
	_ASSERTE(SUCCEEDED(hr));
	if (FAILED(hr)) {
		MessageBox(_T("Could not create COM-Object!"), _T("Parser Error"), MB_OK | MB_ICONERROR);
		m_pParser = NULL;
		return;
	}

	m_pSlaveInfos = new PropGSDSlaveInfos;
	m_pSlaveModule = new PropGSDSlaveModule;
	m_pModuleSpec = new PropGSDModule(m_pSlaveModule);
	m_pErrorPage = new PropErrors;
	m_pCfgUserData = new PropCfgUserData;

	AddPage(m_pSlaveInfos);
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::EncodeSupportedBaudRates(VARIANT vtBaudrates)
{
    // is it what we expected in VARIANT?
    ASSERT(vtBaudrates.vt == (VT_I2 | VT_ARRAY));
    if (vtBaudrates.vt == (VT_I2 | VT_ARRAY))
    {
		m_wSuppBaudRates = 0;
        // now show values
        SAFEARRAY   *psaBytes = vtBaudrates.parray;
        HRESULT hr;

        long    lBound = 0, uBound = 0;
        hr = ::SafeArrayGetLBound(psaBytes, 1, &lBound);
        _ASSERTE(SUCCEEDED(hr));
        hr = ::SafeArrayGetUBound(psaBytes, 1, &uBound);
        _ASSERTE(SUCCEEDED(hr));
        // loop thru all objects in this level:
        for (long i = lBound; i <= uBound; i++)
        {
            WORD wBaudrate;
            hr = ::SafeArrayGetElement(psaBytes, &i, &wBaudrate);
            _ASSERTE(SUCCEEDED(hr));
            if (FAILED(hr)) {
                continue;
            }

			switch (wBaudrate)	{
			case KBAUD_9_6:
				m_wSuppBaudRates |= _9_6kBaud;
				break;
			case KBAUD_19_2:
				m_wSuppBaudRates |= _19_2kBaud;
				break;
			case KBAUD_31_25:
				m_wSuppBaudRates |= _31_25kBaud;
				break;
			case KBAUD_45_45:
				m_wSuppBaudRates |= _45_45kBaud;
				break;
			case KBAUD_93_75:
				m_wSuppBaudRates |= _93_75kBaud;		
				break;
			case KBAUD_187_5:
				m_wSuppBaudRates |= _187_5kBaud;		
				break;
			case KBAUD_500:
				m_wSuppBaudRates |= _500kBaud;		
				break;
			case KBAUD_750:
				m_wSuppBaudRates |= _750kBaud;	
				break;
			case MBAUD_1_5:
				m_wSuppBaudRates |= _1_5MBaud;	
				break;
			case MBAUD_3:
				m_wSuppBaudRates |= _3MBaud;	
				break;
			case MBAUD_6:
				m_wSuppBaudRates |= _6MBaud;		
				break;
			case MBAUD_12:
				m_wSuppBaudRates |= _12MBaud;		
				break;
			default:
                ASSERT(FALSE);
				return FALSE;
				break;
			}
		}
	}

	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::InitPropSlaveInfos()
{
	m_pSlaveInfos->SetFileName(m_szFileName);

	HRESULT hr;
	CComQIPtr<IDP_Station, &IID_IDP_Station>    pStation;
    pStation = m_pUnknown;
    if (pStation != NULL)
    {
        long    lValue = 0;
		double	dbValue = 0;
        BSTR    sName;
		BOOL	bValue = FALSE;

		hr = pStation->get_FileDate(&dbValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetFileDate(dbValue);
        }
        hr = pStation->get_Vendor_Name(&sName);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetVendorName(sName);
            ::SysFreeString(sName);
        }
        hr = pStation->get_Model_Name(&sName);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetModelName(sName);
            ::SysFreeString(sName);
        }
        hr = pStation->get_Ident_Number(&lValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetIdentNmb(lValue);
        }
        hr = pStation->get_Station_Type(&lValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetStationType((WORD)lValue);
        }
        hr = pStation->get_HW_Release(&sName);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetHWRelease(sName);
            ::SysFreeString(sName);
        }
        hr = pStation->get_SW_Release(&sName);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetSWRelease(sName);
            ::SysFreeString(sName);
        }
        hr = pStation->get_GSD_Revision(&lValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetGSDRevision(lValue);
        }
        hr = pStation->get_Revision(&sName);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetRevisionString(sName);
            ::SysFreeString(sName);
		}
        hr = pStation->get_FMS_supported(&bValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetFMSSupported(bValue);
        }
		hr = pStation->get_Redundancy(&bValue);
		if (SUCCEEDED(hr))	{
			m_pSlaveInfos->SetRedundancy(bValue);
		}
        hr = pStation->get_Repeater_Ctrl_Sig(&lValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetRepeaterSignal(lValue);
        }
        hr = pStation->get_Pins_24V(&lValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetPin24V(lValue);
        }

        CComVariant vtBaudrates;
        hr = pStation->get_SupportedBaudrates(&vtBaudrates);
        if (SUCCEEDED(hr)) {
            if (EncodeSupportedBaudRates(vtBaudrates))
				m_pSlaveInfos->SetSupportedBaudRates(m_wSuppBaudRates);
        }
	}

    // is it a DP-Slave?
    CComQIPtr<IDP_Slave, &IID_IDP_Slave>  pSlave;
    pSlave = pStation;

	if (pSlave)	{
		BOOL	bValue = FALSE;

		hr = pSlave->get_SyncModeSupp(&bValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetSyncRequest(bValue);
        }
		hr = pSlave->get_FreezeModeSupp(&bValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetFreezeRequest(bValue);
        }
		hr = pSlave->get_AutoBaudSupp(&bValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetAutoBaudSupp(bValue);
        }
		hr = pSlave->get_SetSlaveAddSupp(&bValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetSlaveAddrSetSupported(bValue);
        }
		hr = pSlave->get_FailSafe(&bValue);
        if (SUCCEEDED(hr)) {
			m_pSlaveInfos->SetFailSafe(bValue);
        }
	}
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::InitPropSlaveModules()
{
	HRESULT hr;
	LONG lValue;
//	BOOL bValue;
	LPUNKNOWN pUnknown = NULL;
	// is it a DP-Slave?
	CComQIPtr<IDP_Slave, &IID_IDP_Slave>  pSlave;
	pSlave = m_pUnknown;
	if (pSlave)
	{
		m_pSlaveModule->SetModularType(m_bIsModular);
		if (m_bIsModular)	{
			hr = pSlave->get_MaxModule(&lValue);
			if (SUCCEEDED(hr)) {
				m_pSlaveModule->SetMaxModule(lValue);
			}
			hr = pSlave->get_MaxInputLen(&lValue);
			if (SUCCEEDED(hr)) {
				m_pSlaveModule->SetMaxInputLen(lValue);
			}
			hr = pSlave->get_MaxOutputLen(&lValue);
			if (SUCCEEDED(hr)) {
				m_pSlaveModule->SetMaxOutputLen(lValue);
			}
			hr = pSlave->get_MaxDataLen(&lValue);
			if (SUCCEEDED(hr)) {
				m_pSlaveModule->SetMaxDataLen(lValue);
			}
		}
		else	{
				m_pSlaveModule->SetMaxModule(0x7FFFFFFF);
				m_pSlaveModule->SetMaxInputLen(0);
				m_pSlaveModule->SetMaxOutputLen(0);
				m_pSlaveModule->SetMaxDataLen(0);
		}

		BSTR	sName = NULL;
		CComQIPtr<IEnumUnknown, &IID_IEnumUnknown>    pEnumModules;
        int modid = 0;

		hr = pSlave->CreateModuleEnumerator(&pUnknown);
		if (SUCCEEDED(hr))
		{
			pEnumModules = pUnknown;
			pUnknown->Release();
			if (pEnumModules)
			{
				ULONG   ulFetched = 0;
				// enumerate thru modules!
				while(pEnumModules->Next(1, &pUnknown, &ulFetched) == S_OK)
				{
					CComQIPtr<IModule, &IID_IModule>    pModule;
					pModule = pUnknown;
					pUnknown->Release();
					if (pModule)
					{
						// print module data
						hr = pModule->get_ModName(&sName);
						if (SUCCEEDED(hr)) {
							CDP_Module* pNewModule = new CDP_Module(modid++);
							pNewModule->SetModuleName(sName);
							LONG lVal;
							if (SUCCEEDED(pModule->get_LenUserParams(&lVal)))
								pNewModule->SetUserPrmLen(lVal);
                            GetAndSaveConfigBytes(pModule, pNewModule);
							m_pSlaveModule->AddNewModule(pNewModule);
							::SysFreeString(sName);

							BOOL    bHasExtUserParam = FALSE;
							hr = pModule->get_HasExtUserParam(&bHasExtUserParam);
							if (SUCCEEDED(hr) && bHasExtUserParam) {
								hr = pModule->CreateExtUserParamEnumerator(&pUnknown);
								if (SUCCEEDED(hr))
								{
									CComQIPtr<IEnumUnknown, &IID_IEnumUnknown>    pEnum;
									pEnum = pUnknown;
									pUnknown->Release();
									if (pEnum == NULL) {
										return E_NOINTERFACE;
									}
									ULONG   ulFetched = 0;
									IUnknown* pUnk = NULL;
									HRESULT   hr = S_FALSE;

									// enumerate thru ext user params!
									while (pEnum->Next(1, &pUnk, &ulFetched) == S_OK)
									{
										CComQIPtr<IExtUserParam, &IID_IExtUserParam>    pParam;
										pParam = pUnk;
										pUnk->Release();
										if (pParam)
										{
											ExtParam* pExtParam = new ExtParam();
											GetExtUserParams(pParam, pExtParam);
											pNewModule->AddExtParamDef(pExtParam);
										}
									}
								}
							}
						}
					}
				}
			}
		}
 	}

	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::GetAndSaveConfigBytes(IModule* pIModule, CDP_Module* pModule)
{
    HRESULT     hr;
    CComVariant vtBytes;
    // fetch config bytes
    hr = pIModule->get_ConfigBytes(&vtBytes);
    if (FAILED(hr)) {
        return hr;
    }
    // is it what we expected in VARIANT?
    ASSERT(vtBytes.vt == (VT_UI1 | VT_ARRAY));
    if (vtBytes.vt == (VT_UI1 | VT_ARRAY))
    {
        // now show values
        SAFEARRAY   *psaBytes = vtBytes.parray;
        long    lBound = 0, uBound = 0;
        hr = ::SafeArrayGetLBound(psaBytes, 1, &lBound);
        _ASSERTE(SUCCEEDED(hr));
        hr = ::SafeArrayGetUBound(psaBytes, 1, &uBound);
        _ASSERTE(SUCCEEDED(hr));
        // loop thru all objects in this level:
        for (long i = lBound; i <= uBound; i++)
        {
            BYTE    ucVal;
            hr = ::SafeArrayGetElement(psaBytes, &i, &ucVal);
            _ASSERTE(SUCCEEDED(hr));
            if (FAILED(hr)) {
                continue;
            }
			pModule->AddConfigByte(ucVal);
            // next byte
        }
    }
    return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
HRESULT CSheetConfigGSD::InitPropGSDModule()
{
	HRESULT	hr;
	VARIANT	vtVal;
    long lVal;
    IUnknown * pUnknown = NULL;
	CComQIPtr<IDP_Slave, &IID_IDP_Slave>  pSlave;

    pSlave = m_pUnknown;
	if (!pSlave) {
        return E_NOINTERFACE;
    }

    //get the User_Prm_Data first.
    hr = pSlave->get_LenUserParams(&lVal);
    if(FAILED(hr)) {
        return hr;
    }
    
    m_pModuleSpec->SetUserPrmLen(lVal);

    if(lVal) { // > 0
        ::VariantInit(&vtVal);
		hr = pSlave->get_UserPrmData(&vtVal);
	    if (FAILED(hr)) {
			return hr;
	    }
        //and set the data to the property page
    	m_pModuleSpec->SetUserPrmData(vtVal);
    }

    //if Ext_User_Prm_Data_Const/Ref is available "or" it into the 
    //m_DefaultUserPrmData by calling AddDefaultUserPrmData()

    //be careful: flag has to be set prior to calling this method
    if(m_bHasExtUserPrm) { //ext user params
        hr = pSlave->CreateExtUserParamEnumerator(&pUnknown);
		if (SUCCEEDED(hr))
		{
			CComQIPtr<IEnumUnknown, &IID_IEnumUnknown>    pEnum;
			pEnum = pUnknown;
			pUnknown->Release();
			if (pEnum == NULL) {
				return E_NOINTERFACE;
			}
			ULONG   ulFetched = 0;
			IUnknown    *pUnk = NULL;
			HRESULT hr = S_FALSE;

			// enumerate thru ext user params!
			while (pEnum->Next(1, &pUnk, &ulFetched) == S_OK)
			{
				CComQIPtr<IExtUserParam, &IID_IExtUserParam>    pParam;
				pParam = pUnk;
				pUnk->Release();
				if (pParam)
				{
					ExtParam* pExtParam = new ExtParam();
					GetExtUserParams(pParam, pExtParam);
					m_pModuleSpec->AddExtUserParam(pExtParam);
                    m_pModuleSpec->AddDefaultUserPrmData(pExtParam);
				}
			}
		}
    }

    return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
void CSheetConfigGSD::InitErrorPage()
{
    CComQIPtr<IDP_Station, &IID_IDP_Station>    pStation;
    pStation = m_pUnknown;

	if (pStation)	{
		LONG lErrors;
        HRESULT hr = pStation->get_ErrorCount(&lErrors);
        if (SUCCEEDED(hr) && (lErrors > 0)) {
			AddPage(m_pErrorPage);				// show ErrorPage if there are some errors
			m_pErrorPage->SetErrorCount(lErrors);
			IUnknown* pUnk = NULL;
			hr = pStation->get_ErrorStrings(&pUnk);
			if (SUCCEEDED(hr)) {
				CComQIPtr<IEnumString, &IID_IEnumString>    pEnum;
				pEnum = pUnk;
				pUnk->Release();
				if (pEnum == NULL) {
					return;
				}
				ULONG   ulFetched = 0;
				LPOLESTR  pText = NULL;
				HRESULT hr = S_FALSE;
				USES_CONVERSION;
				// enumerate thru ext user params!
				while (pEnum->Next(1, &pText, &ulFetched) == S_OK)
				{
					CString strError = pText;
					m_pErrorPage->AddErrorString(strError);
					::CoTaskMemFree(pText);
				}
			}// end if
			SetActivePage(m_pErrorPage);
        }// end if
	}// end if
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::InitPropCfgUserData()
{
	HRESULT	hr;
	VARIANT	vtVal;
	CByteArray CfgData;

	CComQIPtr<IDP_Slave, &IID_IDP_Slave>  pSlave;
	pSlave = m_pUnknown;
	if (pSlave)
	{
		hr = pSlave->get_UserPrmData(&vtVal);
	    if (FAILED(hr)) {
			return FALSE;
	    }
		m_pCfgUserData->SetUserPrmData(vtVal);
	
		return TRUE;
	}
	return FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
void CSheetConfigGSD::SetGSDFileName(CString szFileName)
{
	m_szFileName = szFileName;
}

//----------------------------- FUNCTION -------------------------------------*
HRESULT CSheetConfigGSD::ParseAndInitPages()
{
    m_pUnknown = NULL;
	BSTR	sFile = m_szFileName.AllocSysString();
    HRESULT hr = m_pParser->ParseFile(sFile, (LPUNKNOWN*)&m_pUnknown);
	::SysFreeString(sFile);
    if (FAILED(hr)) {
		return hr;
    }

    if (!InitPropSlaveInfos()) {
		return S_FALSE;
    }

	{
		CComQIPtr<IDP_Slave, &IID_IDP_Slave>  pSlave;
		pSlave = m_pUnknown;
		hr = pSlave->get_ModularStation(&m_bIsModular);
		hr = pSlave->get_HasExtUserParam(&m_bHasExtUserPrm);
	}
	
	InitPropSlaveModules();
	InitPropGSDModule();
	AddPage(m_pSlaveModule);

	if ( !m_bHasExtUserPrm ){
		InitPropCfgUserData();

		AddPage(m_pCfgUserData);
	}
	else	{
//		InitPropGSDModule();
		AddPage(m_pModuleSpec);
	}

	InitErrorPage();
	return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::GetCfgByteArray(CByteArray* pCfgBytes)
{
	return m_pSlaveModule->CreateCfgByteArray(pCfgBytes);
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::GetUserPrmData(CByteArray * pUserPrms)
{
	if (!m_bHasExtUserPrm )
		return	m_pCfgUserData->GetUserPrmData(pUserPrms);
	else
		return	m_pModuleSpec->GetUserPrmData(pUserPrms);
}

//----------------------------- FUNCTION -------------------------------------*
LONG CSheetConfigGSD::GetIdentNumber()
{
	return m_pSlaveInfos->GetIdentNumber();
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::StationIsModular()
{
	return m_bIsModular;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::HasExtUserPrm()
{
	return m_bHasExtUserPrm;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CSheetConfigGSD::SetUserChanged(BOOL bNew)
{
	BOOL	bOld;
	if (m_bIsModular)	{
		bOld = m_pModuleSpec->SetUserChanged(bNew);
	}
	else	{
		bOld = m_pCfgUserData->SetUserChanged(bNew);
	}
	return bOld;
}

//----------------------------- FUNCTION -------------------------------------*
/*static*/
HRESULT CSheetConfigGSD::GetExtUserParams(IExtUserParam* pParam, ExtParam* pExtParam)
{
	if (pParam == NULL || pExtParam == NULL)
		return E_POINTER;

	BSTR    sName;
	USES_CONVERSION;
    HRESULT hr = pParam->get_Description(&sName);
    if (SUCCEEDED(hr)) {
		pExtParam->SetDescription(sName);
		::SysFreeString(sName);
    }
	else	{
		delete pExtParam;
		return hr;
	}

    long lTmp = 0;
    long lOffset = 0;
    hr = pParam->get_Offset(&lOffset);
    if (SUCCEEDED(hr)) {
		pExtParam->SetByteOffset(lOffset);
    }
	else	{
		delete pExtParam;
		return hr;
	}
    BOOL bConst = FALSE;
    BOOL bVarEnum = FALSE;
    BOOL bVarRange = FALSE;
    BOOL bBitArea = FALSE;
    hr = pParam->get_IsConstantParam(&bConst);
    if (SUCCEEDED(hr)) {
		pExtParam->SetIfConstant(bConst);
	}
	else	{
		delete pExtParam;
		return hr;
	}

    if (bConst)
    {
        CComVariant vtBytes;
        hr = pParam->get_ConstPrmBytes(&vtBytes);
        if (SUCCEEDED(hr)) {
			HRESULT     hr;
			// is it what we expected in VARIANT?
            ASSERT(vtBytes.vt == (VT_UI1 | VT_ARRAY));
			if (vtBytes.vt == (VT_UI1 | VT_ARRAY))
			{
				// now show values
				SAFEARRAY   *psaBytes = vtBytes.parray;

				long    lBound = 0, uBound = 0;
				hr = ::SafeArrayGetLBound(psaBytes, 1, &lBound);
				_ASSERTE(SUCCEEDED(hr));
				hr = ::SafeArrayGetUBound(psaBytes, 1, &uBound);
				_ASSERTE(SUCCEEDED(hr));
				// loop thru all objects in this level:
				CByteArray octets;
				octets.RemoveAll();
				for (long i = lBound; i <= uBound; i++)
				{
					BYTE    ucVal;
					hr = ::SafeArrayGetElement(psaBytes, &i, &ucVal);
					_ASSERTE(SUCCEEDED(hr));
					if (FAILED(hr)) {
						continue;
					}
					octets.Add(ucVal);
				}// next byte
				pExtParam->SetConstantBytes(octets.GetData(), octets.GetSize());
			}
		}
        pExtParam->SetParamType(Array);
        return S_OK;
    }

    hr = pParam->get_DefaultValue(&lTmp);
    if (SUCCEEDED(hr)) {
		pExtParam->SetDefaultValue(lTmp);
    }
    hr = pParam->get_IsVariableEnumParam(&bVarEnum);
    if (SUCCEEDED(hr)) {
		//pExtParam->SetIfVariableEnum(bVarEnum);
    }
	else	{
		delete pExtParam;
		return hr;
	}
    hr = pParam->get_IsVariableMinMaxParam(&bVarRange);
    if (SUCCEEDED(hr)) {
        hr = pParam->get_MinValue(&lTmp);
        if (SUCCEEDED(hr)) {
			pExtParam->SetLowerValue(lTmp);
        }
        hr = pParam->get_MaxValue(&lTmp);
        if (SUCCEEDED(hr)) {
			pExtParam->SetUpperValue(lTmp);
        }
    }
	else	{
		delete pExtParam;
		return hr;
	}
    hr = pParam->get_IsBitArea(&bBitArea);
    if (SUCCEEDED(hr)) {
		pExtParam->SetIfBitArea(bBitArea);
        // show bit area params
        hr = pParam->get_BitAreaStart(&lTmp);
        if (SUCCEEDED(hr)) {
			pExtParam->SetBitAreaStart(lTmp);
		}
        hr = pParam->get_BitAreaLength(&lTmp);
        if (SUCCEEDED(hr)) {
			pExtParam->SetBitAreaLength(lTmp);
        }
    }
	else	{
		delete pExtParam;
		return hr;
	}

	CComVariant vtValues, vtNames;
	hr = pParam->NamedEnumValues(&vtValues, &vtNames);
	if (SUCCEEDED(hr)) {
		HRESULT hr;
		// is it what we expected in VARIANT?
        ASSERT(vtValues.vt == (VT_I4 | VT_ARRAY));
        ASSERT(vtNames.vt == (VT_BSTR | VT_ARRAY));
		if (   (vtValues.vt == (VT_I4 | VT_ARRAY))
			&& (vtNames.vt == (VT_BSTR | VT_ARRAY))  )
		{
			// now show values and texts
			SAFEARRAY   *psaValues = vtValues.parray;
			SAFEARRAY   *psaNames = vtNames.parray;

			long    lBound = 0, uBound = 0;
			hr = ::SafeArrayGetLBound(psaValues, 1, &lBound);
			_ASSERTE(SUCCEEDED(hr));
			hr = ::SafeArrayGetUBound(psaValues, 1, &uBound);
			_ASSERTE(SUCCEEDED(hr));
			// loop thru all objects in this level:
			for (long i = lBound; i <= uBound; i++)
			{
				LONG    lVal;
				BSTR    sName;
				hr = ::SafeArrayGetElement(psaValues, &i, &lVal);
				_ASSERTE(SUCCEEDED(hr));
				hr = ::SafeArrayGetElement(psaNames, &i, &sName);
				_ASSERTE(SUCCEEDED(hr));
				if (FAILED(hr)) {
					continue;
				}
				NamedEnumValues* pElement = new NamedEnumValues;
				pElement->Value = lVal;
				pElement->szName = sName;
				pExtParam->AddNamedEnumVal(pElement);
                SysFreeString(sName);
			}// next
		}
	}// end if (SUCCEEDED(hr))

    if (bVarEnum && !pExtParam->IsVariableEnumParam())
    {
        CComVariant vtEnums;
        hr = pParam->EnumValues(&vtEnums);
        if (SUCCEEDED(hr)) {
            ASSERT(vtEnums.vt == (VT_I4 | VT_ARRAY));
			if (vtEnums.vt == (VT_I4 | VT_ARRAY))	{
				// now show values and texts
				SAFEARRAY   *psaValues = vtEnums.parray;

				long    lBound = 0, uBound = 0;
				hr = ::SafeArrayGetLBound(psaValues, 1, &lBound);
				_ASSERTE(SUCCEEDED(hr));
				hr = ::SafeArrayGetUBound(psaValues, 1, &uBound);
				_ASSERTE(SUCCEEDED(hr));
				// loop thru all objects in this level:
				for (long i = lBound; i <= uBound; i++)
				{
					LONG    lVal;
					CString szName;
					hr = ::SafeArrayGetElement(psaValues, &i, &lVal);
					_ASSERTE(SUCCEEDED(hr));
					if (FAILED(hr)) {
						continue;
					}
					szName.Format(_T("%d"), lVal);

					NamedEnumValues* pElement = new NamedEnumValues;
					pElement->Value = lVal;
					pElement->szName = szName;
					pExtParam->AddNamedEnumVal(pElement);
				} //next i
			}// end if
        }// end if
    }// end if

    SetExtParamType(pParam,pExtParam);
    return S_OK;
}

void CSheetConfigGSD::SetExtParamType(IExtUserParam* pParam, ExtParam* pExtParam)
{
    CComQIPtr<IExtUserParam2> p;
    GSDDATATYPE type;
    ParamType paramtype;

    _ASSERTE(pExtParam != NULL);
    _ASSERTE(pParam != NULL);
    
    p = pParam;
    if(p != NULL) {
        p->get_Datatype(&type);

        switch(type) {
        case octets:
        case bitArea:
            paramtype = Byte;
            pExtParam->SetIfBitArea(TRUE);
            break;
        case uSignedInt8:
        case signedInt8:
            paramtype = Byte;
            break;
        case uSignedInt16:
        case signedInt16:
            paramtype = Word;
            break;
        case uSignedInt32:
        case signedInt32:
            paramtype = DWord;
            break;
        }
    }
    else { 
        /*  QueryInterface does'nt work - old parser ? 
            Attention: guessing the datatype from the range is error prone
            Installation of new parser and using get_Datatype is recommended
        */
	    DWORD Upper = pExtParam->GetUpperValue();
	    if (Upper <= 0xFF) {
            paramtype = Byte;
	    }
	    else if (Upper <= 0xFFFF) {
            paramtype = Word;
	    }
	    else {
            paramtype = DWord;
	    }
    }
    
    pExtParam->SetParamType(paramtype);    

}

//-------------------------------  Function  -----------------------------------
BOOL CSheetConfigGSD::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CPropertySheet::OnHelpInfo(pHelpInfo);
}

//-------------------------------  Function  -----------------------------------
BOOL CSheetConfigGSD::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	WORD wSwitch = (WORD)wParam;
	switch (wSwitch)	{
	case ID_HELP: // F1 KEY
	case IDHELP:  // HELP BUTTON
		if (m_pHelpConnection)	{
			DWORD dwIndex = GetActiveIndex();
			if (dwIndex == 2 && !m_bHasExtUserPrm)
				dwIndex = 4;
			// dwIndex:	0 -> Slave Infos
			//			1 -> Modules
			//			2 -> Extended Parameters
			//			3 -> Error Messages
			//			4 -> Config and Userdata
			m_pHelpConnection->CallHelpWithIndex(dwIndex);
		}
		break;
	default:
		break;
	}
	return CPropertySheet::OnCommand(wParam, lParam);
}

//-------------------------------  Function  -----------------------------------
HRESULT CSheetConfigGSD::GetGSDObject(LPUNKNOWN * ppUnk)
{
	if (m_pUnknown != NULL)	{
		*ppUnk = m_pUnknown;
		m_pUnknown->AddRef();
	}
	else	{
		*ppUnk = NULL;
	}
	
	return (m_pUnknown == NULL)? E_FAIL : S_OK;
}

//-------------------------------  Function  -----------------------------------
void CSheetConfigGSD::SetHelpConnection(IConfigGSDHelp * pHelp)
{
	m_pHelpConnection = pHelp;
	m_psh.dwFlags |= PSH_HASHELP;
	m_pSlaveInfos->m_psp.dwFlags  |= PSP_HASHELP;
	m_pSlaveModule->m_psp.dwFlags |= PSP_HASHELP;
	m_pModuleSpec->m_psp.dwFlags  |= PSP_HASHELP;
	m_pCfgUserData->m_psp.dwFlags |= PSP_HASHELP;
	m_pErrorPage->m_psp.dwFlags   |= PSP_HASHELP;
}

//-------------------------------  Function  -----------------------------------
CString CSheetConfigGSD::GetState()
{
    CString retval;
    retval.Format(_T("%d;%d;"), CONFIG_GSD_STATE_VERSION, GetIdentNumber());
    retval += m_pModuleSpec->GetState();
    retval += m_pSlaveModule->GetState();
    return retval;
}

//-------------------------------  Function  -----------------------------------
HRESULT CSheetConfigGSD::SetState(CString &state)
{
    LPCTSTR buffer = state;
    LONG version = GetNextValue(buffer);
    LONG identnr = GetNextValue(buffer);
    if (version != CONFIG_GSD_STATE_VERSION ||
        identnr != GetIdentNumber())
        return E_INVALIDARG;

    BOOL bOK = m_pModuleSpec->SetState(buffer);
    bOK = bOK && m_pSlaveModule->SetState(buffer);
    bOK = bOK && !*buffer;
    return bOK ? S_OK : S_FALSE;
}

