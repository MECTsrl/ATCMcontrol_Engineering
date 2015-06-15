
//----  Local Defines:   -------------------------------------------*
#include "stdafx.h"
#include "opcda.h"
#include "cedlerror.h"
#include "cedl.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "dlstinfo.h"
//----  Includes:   -------------------------------------------*

static CEDL_VARDESR VarDescrTbl[] = {
    {ProjectName,_T("[{%s}].__system.ProjectName"),_T("ProjectName")},
    {DloadDate,_T("{%s}.__srt0.__system.Dload.Date"),_T("")},
    {DloadHost,_T("{%s}.__srt0.__system.Dload.Host"),_T("")},
    {DloadGUID,_T("{%s}.__srt0.__system.Dload.DLoadGUID"),_T("DLoadGUID")},
    {Cleared,_T("{%s}.__srt0.__system.dload.cleared"),_T("cleared")},
    {ProjectGUID,_T("{%s}.__srt0.__system.dload.PrjGUID"),_T("PrjGUID")},
};


CStationInfo::CStationInfo(void)
	: m_pStation (NULL),m_strAddress(),
      m_NotifyCB (this, OnSubscriptionNotify)
{
    int i;

	for(i= 0;i<FC_ARRAY_LEN(m_pSubscriptionArr);i++) {
		m_pSubscriptionArr[i] = NULL;
	}

    for(i= 0;i<FC_ARRAY_LEN(m_strRead);i++) {
        m_strRead[i].Empty();
    }


	m_bConnect = false;
    m_bCleared = false;
    m_hWnd = NULL;
}


CStationInfo::~CStationInfo(void)
{
}

HRESULT CStationInfo::Connect(void)
{
	HRESULT hr;
	CComBSTR sMachine;
	CComBSTR sConfiguration;
	CComPtr<IUnknown> pVCUnknown;
    CComPtr<IFCSessions> pIFCSessions;	


    try {

        m_strAddress = m_pStation->GetAddress();

        hr = m_SubscriptionManager.Start ();
		if (FAILED (hr)){
			FC_DEBUGPRINT1 (_T("CEDL> initialization of subscription manager failed: hr=%08x\n"), hr);
			AfxThrowOleException (hr);
		}

        m_SubscriptionManager.SetNotifyCB (&m_NotifyCB);


        hr = Subscribe();
        if(FAILED(hr)) {
            //do not return fail here
            hr = S_OK;
        }
    
    }
	catch(COleException *e) {
	
        Disconnect();

		hr = ResultFromScode(e->m_sc);
		e->Delete();
	}
    
    pIFCSessions = NULL;
	return hr;
}



void CStationInfo::Disconnect(void)
{
    HRESULT hr = S_OK;

    UnSubscribe();

   
    if(m_SubscriptionManager.IsInitialized ()) {
        m_SubscriptionManager.SetNotifyCB (NULL);
		m_SubscriptionManager.Stop ();
	}

    m_bConnect = false;
}

HRESULT CStationInfo::Subscribe(void)
{
	HRESULT hr,hrRes;
	CString strSubscribeName,strHelp;
	bool bResult=true;

    hrRes = S_OK;

	if(!m_pStation) {
		assert(m_pStation);
		return E_FAIL;
	}

	for(int i = 0;i<FC_ARRAY_LEN(m_pSubscriptionArr);i++) {
        strHelp = GetVarAddress((CEDL_VAR)i);
        if(strHelp.IsEmpty()) {
            //don't worry this var shall not be subscribed
            continue;
        }

	    strSubscribeName.Format (strHelp,m_strAddress );

        m_pSubscriptionArr[i] = NULL;

        hr = m_SubscriptionManager.GetSubscription (strSubscribeName, BL_ANY, m_pSubscriptionArr[i]);
        if (SUCCEEDED (hr) && !m_pSubscriptionArr[i]->IsKnown ())
        {
            hr = S_FALSE;
        };
        switch(GetScode(hr)) {
            case S_OK:
                break;
            case S_FALSE:
                if(!FAILED(hrRes)) { //if not already failed - set hrRes 
                    hrRes = hr;
                }
                break;
            default:
                m_pSubscriptionArr[i] = 0L;
                FC_DEBUGPRINT2(_T("CEDL> Failed to subscribe for %s: hr=%08x\n"), (LPCTSTR)strSubscribeName, hr);
                m_pStation->FireErrorMessage(E_CE_DL_SUBSCRIBE,strSubscribeName);
                /*  if not already failed - set hrRes - previous S_FALSE will be
                    overwritten
                */
                if(!FAILED(hrRes)) { 
                    hrRes = hr;
                }
                break;
        }
    
    }
	
	return hrRes;
}

HRESULT CStationInfo::UnSubscribe(void)
{
    HRESULT hr,hrRes = S_OK;

	for(int i = 0;i<FC_ARRAY_LEN(m_pSubscriptionArr);i++) {
        if(m_pSubscriptionArr[i]) {
            hr = m_pSubscriptionArr[i]->Unreference ();
            if(!SUCCEEDED(hr)) {
                hrRes = hr;
            }
            m_pSubscriptionArr[i] = 0L;
        }

    }

    return hrRes;
}

int CStationInfo::GetNumberOfVars(void)
{
   return FC_ARRAY_LEN(m_pSubscriptionArr);
}

int CStationInfo::State(void)
//thows CDaoException must be catched
{
    CDomRS rs;

    if(!IsConnected()) {
        return notconnected;
    }

    if(m_strRead[DloadGUID].IsEmpty()) {
        return unknown;
    }

    if(IsCleared()) {
        return cleared;
    }

    if(m_strRead[ProjectGUID].IsEmpty()) {
        if( m_strRead[ProjectName] != m_pStation->GetProjectName()) {
            return project;
        }
    }
    else {    
        if(m_strRead[ProjectGUID] != 
            m_pStation->GetVersion()) {
            return project;
        }
    }
   
    if(m_strRead[DloadGUID] == m_pStation->GetDLGuid()) {
        return sync;
    }

    return unsync;
}

int CStationInfo::CheckVar(CSubscription *pSubscription)
{
    for(int i = 0;i<FC_ARRAY_LEN(m_pSubscriptionArr);i++) {
        if(m_pSubscriptionArr[i] == pSubscription) {
            return i;
        }
    }

    return -1;
}

void CStationInfo::UpdateVar(CEDL_VAR iVarId,CString & strValue,short nQuality)
{
    //check the connection state first
    switch (nQuality & OPC_QUALITY_MASK) {
        case OPC_QUALITY_BAD:
            switch (nQuality & OPC_STATUS_MASK) {
                case OPC_QUALITY_NOT_CONNECTED:
                    m_bConnect = false;    
                    break;
                default:
                    m_bConnect = false;    
                    break;
            }
            break;
        default:
            m_bConnect = true;    
            break;
    }

    switch(iVarId) {
        case ProjectName:
        case DloadDate:
        case DloadHost:
        case DloadGUID:
        case ProjectGUID:
            m_strRead[iVarId] = strValue;            
            break;
        case Cleared:
            m_strRead[iVarId] = strValue;
            if(!strValue.CompareNoCase(_T("True"))) {
                m_bCleared = true;
            }
            else {
                m_bCleared = false;
            }
            break;
        default:
			FC_DEBUGPRINT(_T("CEDL> unknown index \n"));
            return;
    }

    if(m_hWnd) {
        ::PostMessage(m_hWnd,CEDL_MSG_VAR_CHANGED,iVarId,(LONG)m_pStation);
    }

}

HRESULT CStationInfo::GetDLoadAttributes(CString & strResource, IFCDownload* pDLSession)
//throws HR, CMemoryException
{
    BSTR sAttrib;
    CString strHelp;
    HRESULT hr;
	COleVariant vtVar;
	
	assert(pDLSession);
	strHelp = GetVarName(DloadGUID);
	sAttrib = strHelp.AllocSysString();

    hr = pDLSession->get_DLoadAttribute(ProgramDomains,sAttrib,(LPVARIANT)vtVar);
	if(FAILED(hr)) {
        SysFreeString(sAttrib);
        return hr;
	}

	assert(vtVar.vt == VT_BSTR);
	m_strRead[DloadGUID] = vtVar.bstrVal;
	vtVar.Clear();

	SysFreeString(sAttrib);

	strHelp = GetVarName(ProjectGUID);
	sAttrib = strHelp.AllocSysString();

	hr = pDLSession->get_DLoadAttribute(ProgramDomains,sAttrib,(LPVARIANT)vtVar);
	if(FAILED(hr)) {
        SysFreeString(sAttrib);
        return hr;
	}

	assert(vtVar.vt == VT_BSTR);
	m_strRead[ProjectGUID] = vtVar.bstrVal;
	vtVar.Clear();

	SysFreeString(sAttrib);

	strHelp = GetVarName(ProjectName);
	sAttrib = strHelp.AllocSysString();

	hr = pDLSession->get_DLoadAttribute(ProgramDomains,sAttrib,(LPVARIANT)vtVar);
	if(FAILED(hr)) {
        SysFreeString(sAttrib);
        return hr;
	}

	assert(vtVar.vt == VT_BSTR);
	m_strRead[ProjectName] = vtVar.bstrVal;
	vtVar.Clear();
    
	SysFreeString(sAttrib);

	strHelp = GetVarName(Cleared);
	sAttrib = strHelp.AllocSysString();

	hr = pDLSession->get_DLoadAttribute(ProgramDomains,sAttrib,(LPVARIANT)vtVar);
	if(FAILED(hr)) {
        SysFreeString(sAttrib);
        return hr;
	}

	assert(vtVar.vt == VT_BOOL);

    m_bCleared = vtVar.boolVal != VARIANT_FALSE;

	SysFreeString(sAttrib);
    m_bConnect = true;
    return hr;
}

LPCTSTR CStationInfo::GetVarAddress(CEDL_VAR iVarId)
{
    static TCHAR szEmpty[] = _T("");
    int nEntries,i;
    nEntries  = sizeof(VarDescrTbl)/sizeof(CEDL_VARDESR);

    for(i = 0;i<nEntries;i++) {
        if(VarDescrTbl[i].iVarId == iVarId) {
            return VarDescrTbl[i].pszAddr;
        }
    }

    return szEmpty;
}

LPCTSTR CStationInfo::GetVarName(CEDL_VAR iVarId)
{
    static TCHAR szEmpty[] = _T("");
    int nEntries,i;
    nEntries  = sizeof(VarDescrTbl)/sizeof(CEDL_VARDESR);

    for(i = 0;i<nEntries;i++) {
        if(VarDescrTbl[i].iVarId == iVarId) {
            return VarDescrTbl[i].pszVarName;
        }
    }

    return szEmpty;
}


void CStationInfo::OnSubscriptionNotify (CSubscription *pSubscription)
{
    int iIndex;
    CComVariant varValue;
    CString strValue;

    iIndex = CheckVar (pSubscription);

    if (iIndex == -1)
    {
        return;
    };

    varValue = pSubscription->GetVariantValue ();

    if (varValue.vt != VT_BSTR)
    {
        HRESULT hr;

        hr = ::VariantChangeType (&varValue, &varValue, VARIANT_ALPHABOOL | VARIANT_NOUSEROVERRIDE, VT_BSTR);
    };

    strValue = varValue.bstrVal;

    UpdateVar ((CEDL_VAR)iIndex, strValue, pSubscription->GetQuality ());
}

