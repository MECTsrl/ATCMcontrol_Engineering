

#include "stdafx.h"

#include "atlbase.h"

#include "cewatchview.h"

#include "CeWatchElement.h"
#include "CeDisplayElement.h"
#include "CeWatchType.h"
#include "CeSymbol.h"

#include "CeWatchBackEnd.h"

#include "CESysDef.h"
#include "baselib.h"

#include "cewatchbe_i.c"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//*****************************************************************************
CCeWatchBackEnd::CCeWatchBackEnd(LPCTSTR pSymbolFile, CWnd* pWndParent)
//*****************************************************************************
{
    m_cRef = 0;
	m_AdviseCookie = 0;
	m_pWndParent = pWndParent;

	m_pBackEnd  = NULL;
	m_pICEWatch = NULL;
    HRESULT hr = CoCreateInstance (CLSID_CEWatchBE, NULL, CLSCTX_ALL, IID_ICEWatch, (LPVOID *)&m_pBackEnd);
    if (SUCCEEDED(hr))
	{
		hr = m_pBackEnd->QueryInterface(IID_ICEWatch, (void **)&m_pICEWatch );
		ASSERT(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			CString str(pSymbolFile);
            BSTR bstrSymbolFile = str.AllocSysString();
			hr = m_pICEWatch->Init(bstrSymbolFile);
            SysFreeString(bstrSymbolFile);
			ASSERT(SUCCEEDED(hr));
			if (SUCCEEDED(hr))
			{
				BOOL bhr = AfxConnectionAdvise(m_pICEWatch, IID_ICEWatchNotify, (LPUNKNOWN)this, TRUE, &m_AdviseCookie);
				ASSERT(bhr);
			}
		}
	}
}

//*****************************************************************************
CCeWatchBackEnd::~CCeWatchBackEnd()
//*****************************************************************************
{
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL)
	{
		AfxConnectionUnadvise(m_pICEWatch, IID_ICEWatchNotify, (LPUNKNOWN)this, TRUE, m_AdviseCookie);
				
		HRESULT hr = m_pICEWatch->Cleanup();
		ASSERT(SUCCEEDED(hr));       
		m_pICEWatch->Release();
		m_pICEWatch = NULL;
	}

	ASSERT(m_pBackEnd != NULL);
	if (m_pBackEnd != NULL)
	{
		m_pBackEnd->Release();
		m_pBackEnd = NULL;
	}
	ASSERT(m_cRef == 0);
	
//	m_MapBeH2pVar.RemoveAll();
}

BOOL IsValidBackEnd(const CCeWatchBackEnd* pObj) 
{
	return AfxIsValidAddress(pObj, sizeof(CCeWatchBackEnd));
}

//*****************************************************************************
LONG CCeWatchBackEnd::AddExpression(LPCTSTR pExpr, CCeDisplayElement *pData) 
//*****************************************************************************
{
	LONG hExpr = -1;
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL && pExpr != NULL)
	{
		CString str = pExpr;
        BSTR bstrExpr = str.AllocSysString();
		HRESULT hr = m_pICEWatch->AddItemExpand (bstrExpr, CEExpandFirst, &hExpr);
        SysFreeString(bstrExpr);
		ASSERT(SUCCEEDED(hr));   
		if (SUCCEEDED(hr))
		{
//			m_MapBeH2pVar.SetAt(hExpr, pData);
			hr = m_pICEWatch->SetClientData(hExpr, (long)pData);
			ASSERT(SUCCEEDED(hr));   
			if (SUCCEEDED(hr))
			{
				pData->SetExpressionHandle(hExpr);
				hr = m_pICEWatch->Connect(hExpr);
				ASSERT(SUCCEEDED(hr));   
			}
			if (!SUCCEEDED(hr))
			{
				m_pICEWatch->RemoveItem(hExpr);
//				m_MapBeH2pVar.RemoveKey(hExpr);
				hExpr = -1;
			}
		}
		else
		{
			hExpr = -1;
		}
	}
	return hExpr;
}

//*****************************************************************************
void CCeWatchBackEnd::RemoveVar(LONG hExpr) 
//*****************************************************************************
{
//	m_MapBeH2pVar.RemoveKey(hExpr);
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL)
	{
		HRESULT hr;
		hr = m_pICEWatch->SetClientData(hExpr, (long)NULL);
		ASSERT(SUCCEEDED(hr));
		hr = m_pICEWatch->Disconnect(hExpr);
		ASSERT(SUCCEEDED(hr));
		hr = m_pICEWatch->RemoveItem (hExpr);
		ASSERT(SUCCEEDED(hr));   
	}
}

//*****************************************************************************
bool CCeWatchBackEnd::WriteVar(CCeWatchElement* pVar, LPCTSTR strNewValue) 
//*****************************************************************************
{
	LONG hExpr = -1;
	HRESULT hr = E_FAIL;
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL)
	{
        BSTR bstrVarName = pVar->GetName().AllocSysString();
		hr = m_pICEWatch->AddItem (bstrVarName, &hExpr);
        SysFreeString(bstrVarName);
		ASSERT(SUCCEEDED(hr));   
		if (SUCCEEDED(hr))
		{
			hr = m_pICEWatch->Connect(hExpr);
			ASSERT(SUCCEEDED(hr));   
			if (SUCCEEDED(hr))
			{
				VARIANT va;
				BSTR bstrValue = CleanValue(pVar, strNewValue).AllocSysString();
				hr = BL_StringToVariant(bstrValue, (BL_IEC_TYP)pVar->GetType().GetIECType(), NULL, BL_USE_IEC_FORMAT, &va);
                SysFreeString(bstrValue);
				if (SUCCEEDED(hr))
				{
					hr = m_pICEWatch->Write (hExpr, va);
				}
                VariantClear(&va);
				m_pICEWatch->Disconnect(hExpr);
			}
			m_pICEWatch->RemoveItem (hExpr);
		}
	}
	return SUCCEEDED(hr);
}

//*****************************************************************************
bool CCeWatchBackEnd::GetType(LONG hExpr, CCeWatchType& type)
//*****************************************************************************
{
	if (m_pICEWatch != NULL && hExpr >= 0)
	{
		IUnknown* pIType = NULL;

		HRESULT hr = m_pICEWatch->GetType(hExpr, &pIType);
		if (pIType != NULL)
		{
			long MinIndex;
			unsigned long ChildrenCount;
			bool rv = CCeSymbol::ConvertType(pIType, type, MinIndex, ChildrenCount);
			pIType->Release();
			return rv;
		}
	}
	return false;
}

//*****************************************************************************
bool CCeWatchBackEnd::GetType(LPCTSTR pExpr, CCeWatchType& type)
//*****************************************************************************
{
	LONG hExpr = -1;
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL && pExpr != NULL)
	{
		CString str = pExpr;
        BSTR bstrExpr = str.AllocSysString();
		HRESULT hr = m_pICEWatch->AddItemExpand (bstrExpr, CEExpandFirst, &hExpr);;
        SysFreeString(bstrExpr);
		ASSERT(SUCCEEDED(hr));   
		if (SUCCEEDED(hr))
		{
			bool rv = GetType(hExpr, type);
			m_pICEWatch->RemoveItem (hExpr);
			return rv;
		}
	}
	return false;
}


//*****************************************************************************
bool CCeWatchBackEnd::GetTypeAndText(LPCTSTR pExpr, CCeWatchType& type, CString &strText)
//*****************************************************************************
{
	LONG hExpr = -1;
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL && pExpr != NULL)
	{
		CString str = pExpr;
        BSTR bstrExpr = str.AllocSysString();
		HRESULT hr = m_pICEWatch->AddItemExpand (bstrExpr, CEExpandFirst, &hExpr);;
        SysFreeString(bstrExpr);
		ASSERT(SUCCEEDED(hr));   
		if (SUCCEEDED(hr))
		{
            CComBSTR sText;

			bool rv = GetType(hExpr, type);

            hr = m_pICEWatch->GetItemText (hExpr, &sText);
            if (SUCCEEDED (hr))
                strText = sText;
            else
                strText = pExpr;

			m_pICEWatch->RemoveItem (hExpr);
			return rv;
		}
	}
	return false;
}


//*****************************************************************************
CString CCeWatchBackEnd::GetFormat(LONG hExpr)
//*****************************************************************************
{
	CString strFormat;
	if (m_pICEWatch != NULL && hExpr >= 0)
	{
		BSTR bstrValue;
		HRESULT hr = m_pICEWatch->GetFormatText(hExpr, &bstrValue);
		if (SUCCEEDED(hr))
		{
			strFormat = bstrValue;
			::SysFreeString(bstrValue);
		}
	}
	return strFormat;
}

//*****************************************************************************
bool CCeWatchBackEnd::SetFormat(LONG hExpr, const CString& strFormat)
//*****************************************************************************
{
	if (hExpr >= 0)
	{
		void* pData = NULL;
//		m_MapBeH2pVar.Lookup(hExpr, pData);
		HRESULT hr = m_pICEWatch->GetClientData(hExpr, (long*)&pData);
		ASSERT(SUCCEEDED(hr) && pData != NULL); 
		if (SUCCEEDED(hr) && pData != NULL)
		{
            BSTR bstrFormat = strFormat.AllocSysString();
			hr = m_pICEWatch->SetFormatText(hExpr, bstrFormat);
            SysFreeString(bstrFormat);
			if (SUCCEEDED(hr))
			{
				if (m_pWndParent != NULL && IsWindow(m_pWndParent->GetSafeHwnd()))
					m_pWndParent->SendMessage(WM_CEWATCH_VARCHANGED, 0, (LPARAM)pData);
				return true;
			}
		}
	}
	return false;
}

//*****************************************************************************
CString CCeWatchBackEnd::ScopeExpression (const CString &strExpr, const CString &strScope)
//*****************************************************************************
{
    LONG hExpr;
    HRESULT hr = m_pICEWatch->AddItemWithScope (CComBSTR(strExpr), CComBSTR(strScope), &hExpr);
    if (SUCCEEDED(hr))
	{
	    CComBSTR bstrScopedExpr;
		hr = m_pICEWatch->GetItemText(hExpr, &bstrScopedExpr);
		ASSERT(SUCCEEDED(hr));
        m_pICEWatch->RemoveItem (hExpr);
		if (SUCCEEDED(hr))
			return bstrScopedExpr;
	}
	return strExpr;
}


//*****************************************************************************
CString CCeWatchBackEnd::CleanValue(CCeWatchElement* pVar, LPCTSTR strNewValue) 
//*****************************************************************************
{
	CString sValue;
	if (pVar != NULL && strNewValue != NULL)
	{
		sValue = strNewValue;
		ASSERT(IsValidWatchElement(pVar)); 
		CCeWatchType type = pVar->GetType();
		if (type.IsSimpleType() && type.IsIECType())
		{
			if (type.GetIECType() != BL_WSTRING && type.GetIECType() != BL_ANY) 
			{
				sValue.TrimLeft();
				sValue.TrimRight();
			}
			if (type.GetIECType() == BL_LREAL || type.GetIECType() == BL_REAL) 	
			{
				int len = sValue.GetLength();
				if (len > 0)
				{
					int iDot = sValue.ReverseFind('.');
					if (iDot >= 0) 
					{
						if (len > 1) 
						{
							if (iDot == 0 && sValue[1] >= '0' && sValue[1] <= '9') // dot at the beginning e.g. ".2"
								sValue = "0" + sValue;
							else if (iDot == (len - 1) && sValue[len-2] >= '0' && sValue[len-2] <= '9') // dot at the end, e.g. "3."
								sValue += "0";
						}
					}
					else // not dot found eg 33
					{
						if (sValue[len-1] >= '0' && sValue[len-1] <= '9') 
							sValue += ".0";
					}
				}
			}
		}
	}
	return sValue;
}

//*****************************************************************************
bool CCeWatchBackEnd::IsValidValue(CCeWatchElement* pVar, LPCTSTR strNewValue) 
//*****************************************************************************
{
	if (pVar != NULL && strNewValue != NULL)
	{
		ASSERT(IsValidWatchElement(pVar)); 
		CCeWatchType type = pVar->GetType();
		if (type.IsSimpleType())
		{
			if (type.IsIECType())
			{
				VARIANT va;
				BSTR bstrValue = CleanValue(pVar, strNewValue).AllocSysString();
				HRESULT hr = BL_StringToVariant(bstrValue, (BL_IEC_TYP)type.GetIECType(), NULL, BL_USE_IEC_FORMAT, &va);
                SysFreeString(bstrValue);
                VariantClear(&va);
				return SUCCEEDED(hr);
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

//*****************************************************************************
STDMETHODIMP CCeWatchBackEnd::QueryInterface(REFIID iid, void **ppvObject)
//*****************************************************************************
{
	*ppvObject = NULL;
	if (IID_ICEWatchNotify == iid || IID_IUnknown == iid)
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

//*****************************************************************************
STDMETHODIMP_(ULONG) CCeWatchBackEnd::AddRef()
//*****************************************************************************
{
    return ++m_cRef;
}

//*****************************************************************************
STDMETHODIMP_(ULONG) CCeWatchBackEnd::Release()
//*****************************************************************************
{
	ASSERT(m_cRef > 0);
    --m_cRef;
    return m_cRef;
}

//*****************************************************************************
STDMETHODIMP CCeWatchBackEnd::OnExpressionChanged (LONG hExpr)
//*****************************************************************************
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL)
	{
		void* pData = NULL;
//		m_MapBeH2pVar.Lookup(hExpr, pData);
		HRESULT hr = m_pICEWatch->GetClientData(hExpr, (long*)&pData);
//		ASSERT(SUCCEEDED(hr) && pData != NULL); 
		if (SUCCEEDED(hr) && pData != NULL)
		{
			if (m_pWndParent != NULL && IsWindow(m_pWndParent->GetSafeHwnd()))
				m_pWndParent->SendMessage(WM_CEWATCH_VARCHANGED, 0, (LPARAM)pData);
		}
	}
	return S_OK;
}

//*****************************************************************************
void CCeWatchBackEnd::UpdateVar(LONG hExpr, CCeWatchElement *pVar)
//*****************************************************************************
{
	if (m_pICEWatch != NULL && pVar != NULL)
	{
		ASSERT(IsValidWatchElement(pVar)); 
		bool bOK = false;
		CEExpressionState state = CEEmptyValue;
		if (hExpr >= 0)
		{
			HRESULT hr = m_pICEWatch->GetExpressionState (hExpr, &state);
			if (SUCCEEDED(hr))
			{
				if (state == CEGoodExpression || state == CEReloadingValue)
				{
					CComBSTR bstrValue;
					DATE Date;
					short Quality;
					hr = m_pICEWatch->GetDataText (hExpr, TRUE, &bstrValue, &Quality, &Date);
					if (SUCCEEDED(hr))
					{
						CString strValue = bstrValue;
						pVar->SetValue(strValue);
						pVar->SetTime(CCeWatchBackEnd::UTCtoLocalTime(Date));
						pVar->SetQuality(Quality);
						bOK = true;
					}
				}
			}
		}

		if (!bOK)
		{
			pVar->SetQuality(OPC_QUALITY_BAD | OPC_QUALITY_NOT_CONNECTED);
		}
	}
}

//*****************************************************************************
CString CCeWatchBackEnd::GetStateText (CEExpressionState state)
//*****************************************************************************
{
    CString strState;
	int IdStr;

    switch (state)
    {
		case CEEmptyValue              : IdStr = IDS_STATE_EMPTYVALUE             ; break;
		case CELoadingValue            : IdStr = IDS_STATE_LOADINGVALUE           ; break;
		case CEReloadingValue          : IdStr = IDS_STATE_RELOADINGVALUE         ; break;
		case CENotConnected            : IdStr = IDS_STATE_NOTCONNECTED           ; break;
		case CESubscribeError          : IdStr = IDS_STATE_SUBSCRIBEERROR         ; break;
		case CEUnknownSubscription     : IdStr = IDS_STATE_UNKNOWNSUBSCRIPTION    ; break;
		case CEValueTypeError          : IdStr = IDS_STATE_VALUETYPEERROR         ; break;
		case CEInvalidAggregateType    : IdStr = IDS_STATE_INVALIDAGGREGATETYPE   ; break;
		case CEUnknownField            : IdStr = IDS_STATE_UNKNOWNFIELD           ; break;
		case CEContainsInOut           : IdStr = IDS_STATE_CONTAINSINOUT          ; break;
		case CENotArrayType            : IdStr = IDS_STATE_NOTARRAYTYPE           ; break;
		case CEUnknownElementType      : IdStr = IDS_STATE_UNKNOWNELEMENTTYPE     ; break;
		case CEInvalidIndexType        : IdStr = IDS_STATE_INVALIDINDEXTYPE       ; break;
		case CEIndexRangeError         : IdStr = IDS_STATE_INDEXRANGEERROR        ; break;
		case CEInvalidPlusOperandType  : IdStr = IDS_STATE_INVALIDPLUSOPERANDTYPE ; break;
		case CEInvalidMinusOperandType : IdStr = IDS_STATE_INVALIDMINUSOPERANDTYPE; break;
		case CEInvalidNotOperandType   : IdStr = IDS_STATE_INVALIDNOTOPERANDTYPE  ; break;
		case CEInvalidAddLeftType      : IdStr = IDS_STATE_INVALIDADDLEFTTYPE     ; break;
		case CEInvalidAddRightType     : IdStr = IDS_STATE_INVALIDADDRIGHTTYPE    ; break;
		case CEInvalidAddTypeCombi     : IdStr = IDS_STATE_INVALIDADDTYPECOMBI    ; break;
		case CEInvalidSubLeftType      : IdStr = IDS_STATE_INVALIDSUBLEFTTYPE     ; break;
		case CEInvalidSubRightType     : IdStr = IDS_STATE_INVALIDSUBRIGHTTYPE    ; break;
		case CEInvalidSubTypeCombi     : IdStr = IDS_STATE_INVALIDSUBTYPECOMBI    ; break;
		case CEInvalidMultLeftType     : IdStr = IDS_STATE_INVALIDMULTLEFTTYPE    ; break;
		case CEInvalidMultRightType    : IdStr = IDS_STATE_INVALIDMULTRIGHTTYPE   ; break;
		case CEInvalidMultTypeCombi    : IdStr = IDS_STATE_INVALIDMULTTYPECOMBI   ; break;
		case CEInvalidDivLeftType      : IdStr = IDS_STATE_INVALIDDIVLEFTTYPE     ; break;
		case CEInvalidDivRightType     : IdStr = IDS_STATE_INVALIDDIVRIGHTTYPE    ; break;
		case CEInvalidDivTypeCombi     : IdStr = IDS_STATE_INVALIDDIVTYPECOMBI    ; break;
		case CEDivisionByZero          : IdStr = IDS_STATE_DIVISIONBYZERO         ; break;
		case CEInvalidModLeftType      : IdStr = IDS_STATE_INVALIDMODLEFTTYPE     ; break;
		case CEInvalidModRightType     : IdStr = IDS_STATE_INVALIDMODRIGHTTYPE    ; break;
		case CEInvalidModTypeCombi     : IdStr = IDS_STATE_INVALIDMODTYPECOMBI    ; break;
		case CEModuloZero              : IdStr = IDS_STATE_MODULOZERO             ; break;
		case CEInvalidEQLeftType       : IdStr = IDS_STATE_INVALIDEQLEFTTYPE      ; break;
		case CEInvalidEQRightType      : IdStr = IDS_STATE_INVALIDEQRIGHTTYPE     ; break;
		case CEInvalidEQTypeCombi      : IdStr = IDS_STATE_INVALIDEQTYPECOMBI     ; break;
		case CEInvalidNELeftType       : IdStr = IDS_STATE_INVALIDNELEFTTYPE      ; break;
		case CEInvalidNERightType      : IdStr = IDS_STATE_INVALIDNERIGHTTYPE     ; break;
		case CEInvalidNETypeCombi      : IdStr = IDS_STATE_INVALIDNETYPECOMBI     ; break;
		case CEInvalidLTLeftType       : IdStr = IDS_STATE_INVALIDLTLEFTTYPE      ; break;
		case CEInvalidLTRightType      : IdStr = IDS_STATE_INVALIDLTRIGHTTYPE     ; break;
		case CEInvalidLTTypeCombi      : IdStr = IDS_STATE_INVALIDLTTYPECOMBI     ; break;
		case CEInvalidLELeftType       : IdStr = IDS_STATE_INVALIDLELEFTTYPE      ; break;
		case CEInvalidLERightType      : IdStr = IDS_STATE_INVALIDLERIGHTTYPE     ; break;
		case CEInvalidLETypeCombi      : IdStr = IDS_STATE_INVALIDLETYPECOMBI     ; break;
		case CEInvalidGTLeftType       : IdStr = IDS_STATE_INVALIDGTLEFTTYPE      ; break;
		case CEInvalidGTRightType      : IdStr = IDS_STATE_INVALIDGTRIGHTTYPE     ; break;
		case CEInvalidGTTypeCombi      : IdStr = IDS_STATE_INVALIDGTTYPECOMBI     ; break;
		case CEInvalidGELeftType       : IdStr = IDS_STATE_INVALIDGELEFTTYPE      ; break;
		case CEInvalidGERightType      : IdStr = IDS_STATE_INVALIDGERIGHTTYPE     ; break;
		case CEInvalidGETypeCombi      : IdStr = IDS_STATE_INVALIDGETYPECOMBI     ; break;
		case CEInvalidAndLeftType      : IdStr = IDS_STATE_INVALIDANDLEFTTYPE     ; break;
		case CEInvalidAndRightType     : IdStr = IDS_STATE_INVALIDANDRIGHTTYPE    ; break;
		case CEInvalidAndTypeCombi     : IdStr = IDS_STATE_INVALIDANDTYPECOMBI    ; break;
		case CEInvalidOrLeftType       : IdStr = IDS_STATE_INVALIDORLEFTTYPE      ; break;
		case CEInvalidOrRightType      : IdStr = IDS_STATE_INVALIDORRIGHTTYPE     ; break;
		case CEInvalidOrTypeCombi      : IdStr = IDS_STATE_INVALIDORTYPECOMBI     ; break;
		case CEInvalidXorLeftType      : IdStr = IDS_STATE_INVALIDXORLEFTTYPE     ; break;
		case CEInvalidXorRightType     : IdStr = IDS_STATE_INVALIDXORRIGHTTYPE    ; break;
		case CEInvalidXorTypeCombi     : IdStr = IDS_STATE_INVALIDXORTYPECOMBI    ; break;
		case CEInvalidPowerLeftType    : IdStr = IDS_STATE_INVALIDPOWERLEFTTYPE   ; break;
		case CEInvalidPowerRightType   : IdStr = IDS_STATE_INVALIDPOWERRIGHTTYPE  ; break;
		default                        : IdStr = -1                               ; break;
	}

	if (IdStr > 0)
		strState.LoadString(IdStr);
	else
		strState.Format(IDS_STATE_UNKNOWN, state);

    return strState;
}

//*****************************************************************************
CString CCeWatchBackEnd::GetQualityText(WORD wQuality)
//*****************************************************************************
{
#define OPC_QUALITY_SUBSTATUS_MASK (OPC_STATUS_MASK) //  ^ OPC_QUALITY_MASK)
    CString sQual;
    CString sTxt;
    // first analyze quality itself:
    switch (wQuality & OPC_QUALITY_MASK) 
	{
    // bad  -----------------------------------
    case OPC_QUALITY_BAD:
        sQual.LoadString(IDS_QUAL_BAD);
        sQual += _T(": ");
        switch (wQuality & OPC_QUALITY_SUBSTATUS_MASK) 
		{
        case OPC_QUALITY_CONFIG_ERROR:
            sTxt.LoadString(IDS_QUAL_BAD_CONFIG_ERROR);
            break;
        case OPC_QUALITY_NOT_CONNECTED:
            sTxt.LoadString(IDS_QUAL_BAD_NOT_CONNECTED);
            break;
        case OPC_QUALITY_DEVICE_FAILURE:
            sTxt.LoadString(IDS_QUAL_BAD_DEV_FAILURE);
            break;
        case OPC_QUALITY_SENSOR_FAILURE:
            sTxt.LoadString(IDS_QUAL_BAD_SENSOR_FAILURE);
            break;
        case OPC_QUALITY_LAST_KNOWN:
            sTxt.LoadString(IDS_QUAL_BAD_LAST_KNOWN_VALUE);
            break;
        case OPC_QUALITY_COMM_FAILURE:
            sTxt.LoadString(IDS_QUAL_BAD_COMM_FAILURE);
            break;
        case OPC_QUALITY_OUT_OF_SERVICE:
            sTxt.LoadString(IDS_QUAL_BAD_OUT_OF_SERVICE);
            break;
        }
		sQual += sTxt;
        break;
    // uncertain  -----------------------------
    case OPC_QUALITY_UNCERTAIN:
        sQual.LoadString(IDS_QUAL_UNCERTAIN);
        sQual += _T(": ");
        switch (wQuality & OPC_QUALITY_SUBSTATUS_MASK) 
		{
        case OPC_QUALITY_LAST_USABLE:
            sTxt.LoadString(IDS_QUAL_UNC_LAST_USABLE_VALUE);
            break;
        case OPC_QUALITY_SENSOR_CAL:
            sTxt.LoadString(IDS_QUAL_UNC_SENSOR_NOT_ACCURATE);
            break;
        case OPC_QUALITY_EGU_EXCEEDED:
            sTxt.LoadString(IDS_QUAL_UNC_ENG_UNITS_EXCEEDED);
            break;
        case OPC_QUALITY_SUB_NORMAL:
            sTxt.LoadString(IDS_QUAL_UNC_SUB_NORMAL);
            break;
        }
        sQual += sTxt;
        break;

    // Good  ----------------------------------
    case OPC_QUALITY_GOOD:
        sQual.LoadString(IDS_QUAL_GOOD);
        sQual += _T(": ");
        switch (wQuality & OPC_QUALITY_SUBSTATUS_MASK) 
		{
        case OPC_QUALITY_LOCAL_OVERRIDE:
            sTxt.LoadString(IDS_QUAL_GOOD_LOCAL_OVERRIDE);
            sQual += sTxt;
            break;
        }
        break;

	default:
        sQual.LoadString(IDS_QUAL_UNKNOWN);
        sQual += _T(": ");
		sTxt.Format("0x%02X", wQuality);
        sQual += sTxt;
        ASSERT(FALSE);
        break;
    }

    return sQual;
}


//*****************************************************************************
CString CCeWatchBackEnd::UTCtoLocalTime (DATE date)
//*****************************************************************************
{
	VARIANT varDate;
	OLECHAR szBuffer[100];

	varDate.vt = VT_DATE;
	varDate.date = date;

	HRESULT hr = BL_VariantToString (&varDate, BL_DT, NULL, BL_USE_WINDOWS_FORMAT, szBuffer, sizeof(szBuffer)/sizeof(OLECHAR));
	if (FAILED (hr))
		return "";

	CString strTime = szBuffer;

	// BL_USE_WINDOWS_FORMAT currently not supported.
	// Strip "dt#" prefix at least.

	int iPos;
	if ((iPos = strTime.Find (_T ('#'))) != -1)
		strTime = strTime.Mid (iPos + 1);

	return strTime;
}

//*****************************************************************************
IDebugInfo* CCeWatchBackEnd::GetDebugInfo() 
//*****************************************************************************
{
	ASSERT(m_pICEWatch != NULL);
	if (m_pICEWatch != NULL)
	{
		IDebugInfo* pDebugInfo = NULL;
		HRESULT hr = m_pICEWatch->GetDebugInfo((IUnknown **)&pDebugInfo);
		ASSERT(SUCCEEDED(hr));
		ASSERT(pDebugInfo != NULL);
		if (SUCCEEDED(hr))
			return pDebugInfo;
	}
	return NULL;
}

