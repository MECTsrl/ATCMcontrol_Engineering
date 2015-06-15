

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// BreakPointMan.cpp : Implementation of CBreakPointMan
#include "stdafx.h"
#include "fc_todebug/fc_assert.h"
#include "utilif.h"
#include "BrkPtMan.h"
#include "BreakPointMan.h"
#include "CSC_Online_i.c"
#include "DbgInfo_i.c"
#include "CEMsg_i.c"
#include "CEBPManError.h"


struct StopData
{
	CString m_strInstance;
	long    m_lNumber;
	long    m_lQuality;
	CString m_strTask;
};


struct DownloadData
{
    CString m_strAddress;
};

/////////////////////////////////////////////////////////////////////////////
// CBreakPointMan

CBreakPointMan::CBreakPointMan ()
: m_StopNotifyCB (this, OnStopNotify),
  m_DownloadNotifyCB (this, OnDownloadNotify),
  m_pBreakpointNotify (NULL),
  m_SessionNotifyCB (this, OnSessionNotify),
  m_pSessionNotify (NULL),
  m_bInitialized (false),
  m_bSyncOnConnect (true),
  m_bSyncOnStateChange (true)
{
}


CBreakPointMan::~CBreakPointMan ()
{
	if (m_bInitialized)
	{
		Cleanup ();
	}
}


STDMETHODIMP CBreakPointMan::Init (BSTR sProjectPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr;
    CString strDebugInfoPath;
    CComPtr<IUnknown> pProjectDBI;

    if (m_bInitialized)
    {
        return (E_FAIL);
    }

	m_strProjectPath = sProjectPath;
    if (m_strProjectPath.IsEmpty ())
    {
        return (E_INVALIDARG);
    }

    while (!m_strProjectPath.IsEmpty () && 
           m_strProjectPath[m_strProjectPath.GetLength () - 1] == _T ('\\'))
    {
        m_strProjectPath = m_strProjectPath.Left (m_strProjectPath.GetLength () - 1);
    }

	hr = CoCreateInstance (CLSID_DebugInfoServer, NULL, CLSCTX_ALL, IID_IDebugInfo, (LPVOID *)&m_pDebugInfo);
	if (FAILED (hr))
	{
        CString strMessage;

        strMessage.Format (IDS_NO_COMPONENT, _T ("DebugInfo"));
        UTIL_FatalError (strMessage, hr);
		return (hr);
	}

    strDebugInfoPath = m_strProjectPath + _T ("\\gen");
	hr = m_pDebugInfo->Init (CComBSTR (strDebugInfoPath));
	if (FAILED (hr))
	{
        m_pDebugInfo = NULL;
		return (hr);
	}

	if (m_pBreakpointNotify == NULL)
	{
		hr = CComObject<CBreakpointNotify>::CreateInstance (&m_pBreakpointNotify);
		if (FAILED (hr))
		{
			FC_DEBUGPRINT1(_T("CEBPMAN> Failed to create notification object (hr=0x%08lx)\n"), hr);
            m_pDebugInfo = NULL;
			return (hr);
		}

		m_pBreakpointNotify->AddRef ();
	}
	m_pBreakpointNotify->SetNotifyCBs (&m_StopNotifyCB, &m_DownloadNotifyCB);

	if (m_pSessionNotify == NULL)
	{
		hr = CComObject<CExclusiveSessionNotifyBPMan>::CreateInstance (&m_pSessionNotify);
		if (FAILED (hr))
		{
			FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to create session notification object (hr=%0x08lx)\n"), hr);
            m_pDebugInfo = NULL;
			return (hr);
		}

		m_pSessionNotify->AddRef ();
	}
	m_pSessionNotify->SetNotifyCB (&m_SessionNotifyCB);

	RemoveAllSessions ();

	m_bInitialized = true;

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::Cleanup ()
{
	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	RemoveAllSessions ();

	m_pBreakpointNotify->SetNotifyCBs (NULL, NULL);
	m_pBreakpointNotify->Release ();
	m_pBreakpointNotify = NULL;

	m_pSessionNotify->SetNotifyCB (NULL);
	m_pSessionNotify->Release ();
	m_pSessionNotify = NULL;

    if (m_pDebugInfo != NULL)
    {
        m_pDebugInfo->Cleanup ();
    }

    m_pDebugInfo = NULL;

	m_bInitialized = false;
	return (S_OK);
}


STDMETHODIMP CBreakPointMan::SetBP(
    BSTR              sInstanceName, 
    EDP_POS_QUALIFIER PosQual, 
    long              lX,
    long              lY, 
    long              lCX,
    long              lCY, 
    BOOL              bActive
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;
    SessionInfo *pSessionInfo;
    CComObject<CBreakPointInfo> *pBPInfo;

    if (!m_bInitialized)
    {
        return (E_FAIL);
    }
    hr = SessionInfoFromInstance (sInstanceName, pSessionInfo);
    if (hr == S_FALSE)
    {
        hr = CreateSession (GetTarget (sInstanceName), pSessionInfo);
    }
    if (FAILED (hr))
    {
        return (hr);
    }


    // Create and partly initialize the breakpoint information object

    hr = CComObject<CBreakPointInfo>::CreateInstance (&pBPInfo);
    if (FAILED (hr))
    {
        return (hr);
    }
    pBPInfo->AddRef ();


    // Check if debug information knows about this breakpoint.
    // If it does not, we will not activate the breakpoint.
    // Also read breakpoint information and store it in pBPInfo.

    hr = ReadBreakpoint (sInstanceName, PosQual, lX, lY, lCX, lCY, pBPInfo);
    if (FAILED (hr))
    {
        return (hr);
    }


    // If the breakpoint is already set, do not try to set it again

    if (LookupDuplicateBreakpoint (pSessionInfo, sInstanceName, pBPInfo->m_lNumber) != NULL)
    {
        pBPInfo->Release ();
        return (S_FALSE);
    }


	// Complete initialization of breakpoint information object.
    // Also reinitialize qualifier, x and y because breakpoint
    // info is always right even if it said yes to the information
    // we have been passed into.



    // Add the breakpoint information object to the breakpoint list

    AddBreakpoint (pSessionInfo, pBPInfo);


    // Now try to set the breakpoint at the control.

    hr = SetBreakpoint (pSessionInfo, pBPInfo, bActive == TRUE);
    if (FAILED (hr))
    {
        RemoveBreakpoint (pSessionInfo, pBPInfo);
        pBPInfo->Release ();
        return (hr);
    }


    // Notify about breakpoint state change

    FireBPStateChange (pBPInfo, false);

    pBPInfo->Release ();
    return (S_OK);
}


STDMETHODIMP CBreakPointMan::ResetBP(
    BSTR              sInstanceName, 
    EDP_POS_QUALIFIER PosQual, 
    long              lX,
    long              lY, 
    long              lCX,
    long              lCY
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr;
	SessionInfo *pSessionInfo;
	CComObject<CBreakPointInfo> *pBPInfo;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	hr = SessionInfoFromInstance (sInstanceName, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return (hr);
	}

	pBPInfo = LookupBreakpoint (pSessionInfo, sInstanceName, PosQual, lX, lY, lCX, lCY);
	if (pBPInfo == NULL)
	{
		return (S_FALSE);
	}

	hr = ClearBreakpoint (pSessionInfo, pBPInfo);
	if (FAILED (hr))
	{
		return (hr);
	}

	FireBPRemoved (pBPInfo);

	RemoveBreakpoint (pSessionInfo, pBPInfo);

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::ActivateBP(
    BSTR              sInstanceName, 
    EDP_POS_QUALIFIER PosQual, 
    long              lX,
    long              lY, 
    long              lCX,
    long              lCY
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr;
	SessionInfo *pSessionInfo;
	CComObject<CBreakPointInfo> *pBPInfo;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	hr = SessionInfoFromInstance (sInstanceName, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return (hr);
	}


	// Do we know about a breakpoint at the passed position.
	// If we do not there is no way to activate the breakpoint.

	pBPInfo = LookupBreakpoint (pSessionInfo, sInstanceName, PosQual, lX, lY, lCX, lCY);
	if (pBPInfo == NULL)
	{
		return (S_FALSE);
	}


	// Do we believe the breakpoint is already active.
	// If we do there is no need to activate the breakpoint.

	if (pBPInfo->m_bActive)
	{
		return (S_FALSE);
	}


	// Check if debug information knows about this breakpoint.
	// If it does not, we will not activate the breakpoint.

	hr = CheckBreakpoint (pBPInfo);
	if (FAILED (hr))
	{
		return (hr);
	}


	// Now try to set the breakpoint at the control.

	hr = SetBreakpoint (pSessionInfo, pBPInfo, true);
	if (FAILED (hr))
	{
		return (hr);
	}


	// Notify about breakpoint state change

	FireBPStateChange (pBPInfo, false);

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::DeactivateBP(
    BSTR              sInstanceName, 
    EDP_POS_QUALIFIER PosQual, 
    long              lX,
    long              lY, 
    long              lCX,
    long              lCY
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr;
	SessionInfo *pSessionInfo;
	CComObject<CBreakPointInfo> *pBPInfo;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	hr = SessionInfoFromInstance (sInstanceName, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return (hr);
	}

	pBPInfo = LookupBreakpoint (pSessionInfo, sInstanceName, PosQual, lX, lY, lCX, lCY);
	if (pBPInfo == NULL)
	{
		return (S_FALSE);
	}

	if (!pBPInfo->m_bActive)
	{
		return (S_FALSE);
	}

	hr = ClearBreakpoint (pSessionInfo, pBPInfo);
	if (FAILED (hr))
	{
		return (hr);
	}

	FireBPStateChange (pBPInfo, false);

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::UpdateAllBPs(BSTR sInstanceName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr;
	CString strInstance;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	if (sInstanceName == NULL)
	{
		strInstance = "";
	}
	else
	{
		strInstance = sInstanceName;
	}

	if (strInstance != _T (""))
	{
		hr = UpdateSingle (strInstance);
	}
	else
	{
		hr = UpdateAll ();
	}

	return (hr);
}


HRESULT CBreakPointMan::UpdateSingle (const CString &strInstance)
{
	HRESULT hr;
	SessionInfo *pSessionInfo;

	hr = SessionInfoFromInstance (strInstance, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return (hr);
	}

	UpdateBPs (pSessionInfo);
	UpdateSPs (pSessionInfo);

	return (S_OK);
}


HRESULT CBreakPointMan::UpdateAll ()
{
	POSITION pPosition;

	pPosition = m_SessionInfos.GetStartPosition ();
	while (pPosition != NULL)
	{
		CString str;
		void *pVoid;
		SessionInfo *pSessionInfo;

		m_SessionInfos.GetNextAssoc (pPosition, str, pVoid);

		pSessionInfo = (SessionInfo *)pVoid;
		assert (pSessionInfo != NULL);
		if (pSessionInfo == NULL)
			continue;

		UpdateBPs (pSessionInfo);
		UpdateSPs (pSessionInfo);
	}

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::Attach(BSTR sTarget, LPUNKNOWN pUnk)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr;
	CComPtr<IFCBreakpoint> pIFCBreakpoint;
	CString strTarget (sTarget);
	SessionInfo *pSessionInfo;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}


	// Look if there already is a session
	// info for our configuration.

	hr = SessionInfoFromTarget (strTarget, pSessionInfo);
	if (hr == S_FALSE)
	{
		hr = CreateSession (strTarget, pSessionInfo);
	}
	if (FAILED (hr))
	{
		return (hr);
	}


	// If the session info was attached
	// do not try to attach another time.

	if (pSessionInfo->m_pIUnknown != NULL)
	{
		return (S_FALSE);
	}


	pSessionInfo->m_pIUnknown = pUnk;


	// Get IFCBreakpoint interface pointer
	// and store it in session info

	hr = pUnk->QueryInterface (IID_IFCBreakpoint, (LPVOID *)&pIFCBreakpoint);
	if (FAILED (hr))
	{
		return (hr);
	}
	pSessionInfo->m_pIFCBreakpoint = pIFCBreakpoint;


	// Advise breakpoint notification source
    pIFCBreakpoint->put_ConnectionID(sTarget);
	hr = AtlAdvise (pIFCBreakpoint, m_pBreakpointNotify->GetUnknown (), IID__FCBreakpointNotification, &pSessionInfo->m_dwBreakpointCookie);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to advise breakpoint notification (hr=0x%08lx)\n"), hr);
		pSessionInfo->m_pIUnknown = NULL;
		pSessionInfo->m_pIFCBreakpoint = NULL;
		return (hr);
	}


	// Advise session notification source

	hr = AtlAdvise (pIFCBreakpoint, m_pSessionNotify->GetUnknown (), IID__FCSessionNotification, &pSessionInfo->m_dwSessionCookie);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to advise session notification (hr=%08lx)\n"), hr);

		hr = AtlUnadvise (pSessionInfo->m_pIFCBreakpoint, IID__FCBreakpointNotification, pSessionInfo->m_dwBreakpointCookie);
		if (FAILED (hr))
		{
			FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to unadvise breakpoint notification (hr=0x%08lx)\n"), hr);
		}
		pSessionInfo->m_dwBreakpointCookie = NULL;

		pSessionInfo->m_pIUnknown = NULL;
		pSessionInfo->m_pIFCBreakpoint = NULL;
		return (hr);
	}

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::Detach(BSTR sTarget)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SessionInfo *pSessionInfo;
	HRESULT hr;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	hr = SessionInfoFromTarget (sTarget, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return (hr);
	}

	if (pSessionInfo->m_pIUnknown == NULL)
	{
		return (S_FALSE);
	}

	UnsubscribeBreakpoints (pSessionInfo);

	RemoveStoppoints (pSessionInfo);

	hr = AtlUnadvise (pSessionInfo->m_pIFCBreakpoint, IID__FCBreakpointNotification, pSessionInfo->m_dwBreakpointCookie);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to unadvise breakpoint notification (hr=0x%08lx)\n"), hr);
	}
	pSessionInfo->m_dwBreakpointCookie = NULL;

	hr = AtlUnadvise (pSessionInfo->m_pIFCBreakpoint, IID__FCSessionNotification, pSessionInfo->m_dwSessionCookie);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to unadvise session notification (hr=0x%08lx)\n"), hr);
	}
	pSessionInfo->m_dwSessionCookie = NULL;

	pSessionInfo->m_pIUnknown = NULL;
	pSessionInfo->m_pIFCBreakpoint = NULL;
	pSessionInfo->m_bEstablished = false;

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::ContinueTask (BSTR sInstanceName, long mode)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	HRESULT hr;
	SessionInfo *pSessionInfo;
	CComObject<CBreakPointInfo> *pBPInfo;
    CString strInstanceName(sInstanceName);

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

    if(strInstanceName.IsEmpty())
    {
        return ContinueUnambigous(mode);
    }

	hr = SessionInfoFromInstance (sInstanceName, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return (hr);
	}

	pBPInfo = LookupStoppoint (pSessionInfo, sInstanceName);
	if (pBPInfo == NULL)
	{
		return (S_FALSE);
	}

	hr = ContinueFromStoppoint (pSessionInfo, pBPInfo, mode);

	return (hr);
}


STDMETHODIMP CBreakPointMan::GetCallStackEntry (BSTR sInstanceName, long lLevel, LPUNKNOWN *pEntry)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	CString strInstance (sInstanceName);
	HRESULT hr;
	CComObject<CCallStackEntry> *pCallStackEntry;

	if (lLevel < 0)
	{
		return (E_FAIL);
	}

	while (lLevel != 0)
	{
		int iPos;

		iPos = strInstance.ReverseFind (_T ('.'));

		if (iPos < 0)
		{
			*pEntry = NULL;
			return (S_FALSE);
		}

		strInstance = strInstance.Mid (iPos);

		--lLevel;
	}

	hr = CComObject<CCallStackEntry>::CreateInstance (&pCallStackEntry);
	if (FAILED (hr))
	{
		return (hr);
	}

	pCallStackEntry->m_strInstance = strInstance;

	pCallStackEntry->AddRef ();
	*pEntry = pCallStackEntry->GetUnknown ();

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::GetBPs (SAFEARRAY **pBPInfos)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	SAFEARRAYBOUND bound;
	POSITION pPosition;
	long lIndex=0;

	bound.lLbound = 0;
	bound.cElements = 0;

	pPosition = m_SessionInfos.GetStartPosition ();
	while (pPosition != NULL)
	{
		CString strConfig;
		void *pVoid;
		SessionInfo *pSessionInfo;

		m_SessionInfos.GetNextAssoc (pPosition, strConfig, pVoid);

		pSessionInfo = (SessionInfo *)pVoid;
		assert (pSessionInfo != NULL);
		if (pSessionInfo == NULL)
			continue;

		bound.cElements += pSessionInfo->m_Breakpoints.GetCount ();
	}

	*pBPInfos = ::SafeArrayCreate (VT_DISPATCH, 1, &bound);
	assert (*pBPInfos != NULL);
	if (*pBPInfos == NULL)
	{
		FC_DEBUGPRINT("CEBPMAN> CBreakPointMan::GetBPs: failed to create breakpoint SAFEARRAY\n");
		return (E_FAIL);
	}

	pPosition = m_SessionInfos.GetStartPosition ();
	while (pPosition != NULL)
	{
		CString strConfig;
		void *pVoid;
		SessionInfo *pSessionInfo;
		POSITION pBPPosition;

		m_SessionInfos.GetNextAssoc (pPosition, strConfig, pVoid);

		pSessionInfo = (SessionInfo *)pVoid;
		assert (pSessionInfo != NULL);
		if (pSessionInfo == NULL)
			continue;

		pBPPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
		while (pBPPosition != NULL)
		{
			CComObject<CBreakPointInfo> *pBPInfo;
			HRESULT hr;
			CComQIPtr<IDispatch, &IID_IDispatch> pIDispatch;

			pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pBPPosition);
			assert (pBPInfo != NULL);
			if (pBPInfo == NULL)
				continue;

			pIDispatch = pBPInfo->GetUnknown ();

			hr = ::SafeArrayPutElement (*pBPInfos, &lIndex, (void *)(LPDISPATCH)pIDispatch);
			if (FAILED (hr))
			{
				FC_DEBUGPRINT1("CEBPMAN> CBreakPointMan::GetBPs: failed to put breakpoint SAFEARRAY element (hr=0x%08lx)\n", hr);
				::SafeArrayDestroy (*pBPInfos);
				*pBPInfos = NULL;
				return (hr);
			}

			++lIndex;
		}
	}

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::ResetAllBPs (BSTR sInstanceName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr;
	CString strInstance;

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	if (sInstanceName == NULL)
	{
		strInstance = "";
	}
	else
	{
		strInstance = sInstanceName;
	}

	if (strInstance != _T (""))
	{
		hr = ResetSingle (strInstance);
	}
	else
	{
		hr = ResetAll ();
	}

	return (hr);
}


HRESULT CBreakPointMan::ResetSingle (const CString &strInstance)
{
	HRESULT hr;
	SessionInfo *pSessionInfo;

	hr = SessionInfoFromInstance (strInstance, pSessionInfo);
	if (FAILED (hr))
	{
		return (hr);
	}

	if (hr == S_FALSE)
	{
		return (S_OK);
	}

	ClearBreakpoints (pSessionInfo, strInstance);

	return (S_OK);
}


HRESULT CBreakPointMan::ResetAll ()
{
	POSITION pPosition;

	pPosition = m_SessionInfos.GetStartPosition ();
	while (pPosition != NULL)
	{
		CString strConfig;
		void *pVoid;
		SessionInfo *pSessionInfo;

		m_SessionInfos.GetNextAssoc (pPosition, strConfig, pVoid);
		pSessionInfo = (SessionInfo *)pVoid;
		assert (pSessionInfo != NULL);
		if (pSessionInfo == NULL)
			continue;

		ClearBreakpoints (pSessionInfo, "");
	}

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::ShowBP (BSTR sInstanceName, long lNumber)
{
	HRESULT hr;
	SessionInfo *pSessionInfo;
	POSITION pPosition;
	CString strInstance;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!m_bInitialized)
	{
		return (E_FAIL);
	}

	strInstance = sInstanceName;

	hr = SessionInfoFromInstance (strInstance, pSessionInfo);
	if (hr == S_FALSE)
	{
		return (E_FAIL);
	}
	if (FAILED (hr))
	{
		return (hr);
	}

	pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;

		pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		if (pBPInfo->m_lNumber == lNumber &&
		    strInstance.CompareNoCase (pBPInfo->m_strInstance) == 0)
		{
			Fire_OnShowBP (sInstanceName, pBPInfo->GetUnknown ());
			return (S_OK);
		}
	}

	pPosition = pSessionInfo->m_Stoppoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;

		pBPInfo = pSessionInfo->m_Stoppoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		if (pBPInfo->m_lNumber == lNumber &&
		    strInstance.CompareNoCase (pBPInfo->m_strInstance) == 0)
		{
			Fire_OnShowBP (sInstanceName, pBPInfo->GetUnknown ());
			return (S_OK);
		}
	}

	return (S_OK);
}


STDMETHODIMP CBreakPointMan::GetSyncMode (BOOL *pbSyncOnConnect, BOOL *pbSyncOnStateChange)
{
    *pbSyncOnConnect = m_bSyncOnConnect ? TRUE : FALSE;
    *pbSyncOnStateChange = m_bSyncOnStateChange ? TRUE : FALSE;
    return (S_OK);
}


STDMETHODIMP CBreakPointMan::SetSyncMode (BOOL bSyncOnConnect, BOOL bSyncOnStateChange)
{
    m_bSyncOnConnect = bSyncOnConnect == TRUE;
    m_bSyncOnStateChange = bSyncOnStateChange == TRUE;
    return (S_OK);
}


STDMETHODIMP CBreakPointMan::Synchronize (BSTR sTarget)
{
    HRESULT hr;
    SessionInfo *pSessionInfo;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	hr = SessionInfoFromTarget (sTarget, pSessionInfo);
	if (hr == S_FALSE || FAILED (hr))
		return (hr);

    SynchronizeBreakpoints (pSessionInfo);

    return (S_OK);
}


STDMETHODIMP CBreakPointMan::ActivateAllConfigBPs (BSTR sTarget)
{
    return (E_NOTIMPL);
}


STDMETHODIMP CBreakPointMan::DeactivateAllConfigBPs (BSTR sTarget)
{
    HRESULT hr;
    SessionInfo *pSessionInfo;
    POSITION pPosition;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    hr = SessionInfoFromTarget (sTarget, pSessionInfo);
    if (hr == S_FALSE || FAILED (hr))
        return (hr);

    pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
    while (pPosition != NULL)
    {
    	CComObject<CBreakPointInfo> *pBPInfo;

        pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
        ASSERT (pBPInfo != NULL);
        if (pBPInfo == NULL)
            continue;

        if (!pBPInfo->m_bActive)
            continue;

        hr = ClearBreakpoint (pSessionInfo, pBPInfo);
	    if (FAILED (hr))
            continue;

        FireBPStateChange (pBPInfo, false);
    }

    return (S_OK);
}


void CBreakPointMan::OnStopNotify (StopData *pData)
{
	HRESULT hr;
	SessionInfo *pSessionInfo;
	CComBSTR sClassName;
	CComBSTR sLanguage;
	long lX=-1;
	long lY=-1;
	long lCX=-1;
	long lCY=-1;
	EDP_POS_QUALIFIER PosQual=EDP_STLINE;
	CComObject<CBreakPointInfo> *pBPInfo;
    CComBSTR sSource;
    CComPtr<IBreakpointDBI> pBreakpointDBI;

	if (pData->m_lQuality == -1)
	{
		TaskContinued (pData);
		return;
	}

	hr = SessionInfoFromInstance (pData->m_strInstance, pSessionInfo);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT2("CEBPMAN> OnStopNotify: failed to get SessionInfo from path (path=%s, hr=0x%08lx)\n",
		                (LPCTSTR)pData->m_strInstance, hr);
        return;
	}
	else if (hr == S_FALSE)
	{
		FC_DEBUGPRINT1("CEBPMAN> OnStopNotify: called with unknown configuration (path=%s)\n",
		                (LPCTSTR)pData->m_strInstance);
        return;
	}
	else
	{
        CComPtr<ITypeDBI> pTypeDBI;

        hr = GetTypeDBIForInstance (pData->m_strInstance, pTypeDBI);
        if (FAILED (hr))
        {
	        FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint information (path=%s, bp=%ld, hr=0x%08lx)\n",
				            (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
        }
        if (hr == S_OK)
        {
            CComPtr<IUnknown> pUnknown;

            hr = pTypeDBI->get_Source (&sSource);
            if (FAILED (hr))
            {
			    FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint information (path=%s, bp=%ld, hr=0x%08lx)\n",
				                (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }

            hr = pTypeDBI->get_BreakpointByNumber (pData->m_lNumber, &pUnknown);
            if (hr == S_OK)
            {
                hr = pUnknown->QueryInterface (IID_IBreakpointDBI, (LPVOID *)&pBreakpointDBI);
            }
            if (FAILED (hr))
            {
			    FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint information (path=%s, bp=%ld, hr=0x%08lx)\n",
				                (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }
        }

		if (pData->m_lQuality == 0 && pBreakpointDBI != NULL)
		{

			// Try to get positional info for the stoppoint

            hr = pBreakpointDBI->get_Qualifier (&PosQual);
            if (FAILED (hr))
            {
				FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint position (path=%s, bp=%ld, hr=0x%08lx)\n",
					            (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }

            hr = pBreakpointDBI->get_X (&lX);
            if (FAILED (hr))
            {
				FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint position (path=%s, bp=%ld, hr=0x%08lx)\n",
					            (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }

            hr = pBreakpointDBI->get_Y (&lY);
            if (FAILED (hr))
            {
				FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint position (path=%s, bp=%ld, hr=0x%08lx)\n",
					            (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }
            hr = pBreakpointDBI->get_CX (&lCX);
            if (FAILED (hr))
            {
				FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint position (path=%s, bp=%ld, hr=0x%08lx)\n",
					            (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }
            hr = pBreakpointDBI->get_CY (&lCY);
            if (FAILED (hr))
            {
				FC_DEBUGPRINT3("CEBPMAN> OnStopNotify: failed to get breakpoint position (path=%s, bp=%ld, hr=0x%08lx)\n",
					            (LPCTSTR)pData->m_strInstance, pData->m_lNumber, hr);
            }

		}
	}

	hr = CComObject<CBreakPointInfo>::CreateInstance (&pBPInfo);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1("CEBPMAN> OnStopNotify: failed to create BreakPointInfo COM object (hr=0x%08lx)\n", hr);
		return;
	}
	pBPInfo->AddRef ();

	pBPInfo->m_strInstance = pData->m_strInstance;
	pBPInfo->m_lNumber = pData->m_lNumber;
	pBPInfo->m_strTask = pData->m_strTask;
	pBPInfo->m_lQuality = pData->m_lQuality;
	pBPInfo->m_PosQual = PosQual;
	pBPInfo->m_lX      = lX;
	pBPInfo->m_lY      = lY;
	pBPInfo->m_lCX     = lCX;
	pBPInfo->m_lCY     = lCY;
	pBPInfo->m_strFileName = sSource;
	pBPInfo->m_strClassName = sClassName;
	pBPInfo->m_strLanguage = sLanguage;

    //give this reference to the stop point list
	AddStoppoint (pSessionInfo, pBPInfo);

}


HRESULT CBreakPointMan::GetTypeDBIForInstance (const CString &strInstance, CComPtr<ITypeDBI> &pTypeDBI)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IProjectDBI> pProjectDBI;
    CComPtr<IInstanceDBI> pInstanceDBI;

    pTypeDBI = NULL;

    hr = m_pDebugInfo->get_Project (&pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    }
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    }

    hr = pUnknown->QueryInterface (IID_IProjectDBI, (LPVOID *)&pProjectDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pProjectDBI != NULL);
    if (pProjectDBI == NULL)
    {
        return (E_UNEXPECTED);
    }

	hr = pProjectDBI->get_InstanceByPath (CComBSTR (strInstance), &pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    }
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    }

    hr = pUnknown->QueryInterface (IID_IInstanceDBI, (LPVOID *)&pInstanceDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pInstanceDBI != NULL);
    if (pInstanceDBI == NULL)
    {
        return (E_UNEXPECTED);
    }

    hr = pInstanceDBI->get_Type (&pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    }
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    }

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&pTypeDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    }

    return (S_OK);
}


void CBreakPointMan::OnDownloadNotify (DownloadData *pData)
{
	HRESULT hr=S_OK;
	SessionInfo *pSessionInfo = NULL;
    CComQIPtr<IProjectDBI> pProjectDBI;
    CComPtr<IUnknown> pUnknown;
    CComBSTR s;

    //try to syncronize the breakpoints
    try {
        //get the project debug information
        hr = m_pDebugInfo->get_Project (&pUnknown);
        if (hr != S_OK)
        {
            throw hr;
        }

        ASSERT (pUnknown != NULL);
        if (pUnknown == NULL)
        {
            throw E_UNEXPECTED;
        }

        pProjectDBI = pUnknown;
        if(!pProjectDBI) {
            throw E_NOINTERFACE;
        }

        s = pData->m_strAddress;
        pUnknown = NULL;
        //this is actually the target instance
        hr = pProjectDBI->get_ConfigurationByAddress(s,&pUnknown);
        if(FAILED(hr)) {
            throw hr;
        }

        CComQIPtr<IConfigurationDBI> pConfig;
        pConfig = pUnknown;
        if(!pConfig) {
            throw E_NOINTERFACE;
        }

        s.Empty();
        //get the target name
        pConfig->get_Name(&s);

        CString strTarget = s;
        //get the session info by the target name
        hr = SessionInfoFromTarget (strTarget, pSessionInfo);
        if(FAILED(hr)) {
            throw hr;
        }

		//>> ## LN040709
		RemoveStoppoints(pSessionInfo);

        if (m_bSyncOnStateChange)
        {
            //try to synchronize the breakpoints.
	        SynchronizeBreakpoints (pSessionInfo);
        }
    }
    catch(HRESULT hrRes) {
		FC_DEBUGPRINT2("CEBPMAN> OnConfigChangeNotify: failed to get SessionInfo from address (address=%s, hr=0x%08lx)\n",
		                (LPCTSTR)pData->m_strAddress, hrRes);
    }
    catch(CMemoryException* e) {
        e->Delete();
    }
}

void CBreakPointMan::OnSessionNotify (LPUNKNOWN pSession, CSC_SESSION_STATE state)
{
	HRESULT hr;
	SessionInfo *pSessionInfo;

	if (state != exclusive_established)
	{
		return;
	}

	hr = SessionInfoFromUnknown (pSession, pSessionInfo);
	if (FAILED (hr) || hr == S_FALSE)
	{
		return;
	}

	assert (pSessionInfo != NULL);
	if (pSessionInfo == NULL)
	{
		return;
	}

	pSessionInfo->m_bEstablished = true;
    if (m_bSyncOnConnect)
    {
	    SynchronizeBreakpoints (pSessionInfo);
    }
}


HRESULT CBreakPointMan::SessionInfoFromInstance (const CString &strInstance, SessionInfo *&pSessionInfo)
{
	CString strTarget;

	strTarget = GetTarget (strInstance);
	if (strTarget == _T (""))
	{
		return (E_FAIL);
	}

	return (SessionInfoFromTarget (strTarget, pSessionInfo));
}


HRESULT CBreakPointMan::SessionInfoFromTarget (const CString &strTarget, SessionInfo *&pSessionInfo)
{
	void *pVoid;
	CString strTemp;

	strTemp = strTarget;
	strTemp.MakeLower ();
	if (!m_SessionInfos.Lookup (strTemp, pVoid))
	{
		pSessionInfo = NULL;
		return (S_FALSE);
	}

	pSessionInfo = (SessionInfo *)pVoid;
	assert (pSessionInfo != NULL);
	if (pSessionInfo == NULL)
	{
		return (E_FAIL);
	}

	return (S_OK);
}


HRESULT CBreakPointMan::SessionInfoFromUnknown (LPUNKNOWN pIUnknown, SessionInfo *&pSessionInfo)
{
	POSITION pPosition;

	pPosition = m_SessionInfos.GetStartPosition ();
	while (pPosition != NULL)
	{
		CString strConfiguration;
		void *pVoid;

		m_SessionInfos.GetNextAssoc (pPosition, strConfiguration, pVoid);
		assert (pVoid != NULL);
		if (pVoid == NULL)
			continue;

		pSessionInfo = (SessionInfo *)pVoid;
		if (pIUnknown == pSessionInfo->m_pIUnknown)
		{
			return (S_OK);
		}
	}

	pSessionInfo = NULL;
	return (S_FALSE);
}



//------------------------------------------------------------------*
/**
 * Retrieve the target name from the instance path. The instance path
 * has the form: target name.resourcename.taskname.programname... In
 * order to identify the sessioninfo the target name will be used as
 * key to the map m_SessionInfos. Hence the target name has to be 
 * separated from the instance path.
 * @param			const CString &strInstance - fully qualified 
 *                  instance path.
 * @return			CString containing the target name
 * @see			
*/
CString CBreakPointMan::GetTarget (const CString &strInstance)
{
	int iPos;

	iPos = strInstance.Find (_T ('.'));
	if (iPos == -1)
	{
		return ("");
	}

	return (strInstance.Left (iPos));
}


CString CBreakPointMan::GetResource (const CString &strInstance)
{
	int iPos;
	int iEndPos;

	iPos = strInstance.Find (_T ('.'));
	if (iPos == -1)
	{
		return ("");
	}

	iEndPos = strInstance.Find (_T ('.'), iPos + 1);
	if (iEndPos == -1)
	{
		return ("");
	}

	return (strInstance.Mid (iPos + 1, iEndPos - iPos - 1));
}


HRESULT CBreakPointMan::CreateSession (const CString &strTarget, SessionInfo *&pSessionInfo)
{
	CString strTemp;

	pSessionInfo = new SessionInfo;

	pSessionInfo->m_bEstablished = false;
	pSessionInfo->m_strTarget = strTarget;

	strTemp = strTarget;
	strTemp.MakeLower ();
	m_SessionInfos.SetAt (strTemp, pSessionInfo);

	return (S_OK);
}


void CBreakPointMan::RemoveAllSessions ()
{
	while (!m_SessionInfos.IsEmpty ())
	{
		POSITION pPosition;
		CString strKey;
		void *pValue;
		SessionInfo *pSessionInfo;

		pPosition = m_SessionInfos.GetStartPosition ();
		m_SessionInfos.GetNextAssoc (pPosition, strKey, pValue);
		m_SessionInfos.RemoveKey (strKey);

		assert (pValue != NULL);
		if (pValue == NULL)
			continue;

		pSessionInfo = (SessionInfo *)pValue;
		ClearSession (pSessionInfo);

		delete pSessionInfo;
	}
}


void CBreakPointMan::ClearSession (SessionInfo *pSessionInfo)
{
	ClearBreakpoints (pSessionInfo, "");
	RemoveStoppoints (pSessionInfo);
}


void CBreakPointMan::SynchronizeBreakpoints (SessionInfo *pSessionInfo)
{
	POSITION pPosition;

	// Advise control to clear all breakpoints

	pSessionInfo->m_pIFCBreakpoint->ClearAllBreakpoints ();


	pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;
		HRESULT hr=S_OK;

		pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		if (pBPInfo->m_bActive)
		{

			// Check if debug information knows about this breakpoint.
			// If it does not do not subscribe the breakpoint.

			hr = CheckBreakpoint (pBPInfo);
			if (FAILED (hr))
			{
				if (pBPInfo->m_bSubscribed)
				{
					pBPInfo->m_bSubscribed = false;
					FireBPStateChange (pBPInfo, false);
				}

				FireMessage (E_CE_BPMAN_INVALID_BREAKPOINT_POSITION);
				continue;
			}


			// Now try to set the breakpoint at the control.

			hr = pSessionInfo->m_pIFCBreakpoint->SetBreakpoint (CComBSTR (pBPInfo->m_strInstance), pBPInfo->m_lNumber, &pBPInfo->m_lHandle);
			if (FAILED (hr))
			{
				if (pBPInfo->m_bSubscribed)
				{
					pBPInfo->m_bSubscribed = false;
					FireBPStateChange (pBPInfo, false);
				}

				FireMessage (E_CE_BPMAN_CANNOT_SUBSCRIBE_BREAKPOINT);
				continue;
			}

			pBPInfo->m_bSubscribed = true;
			FireBPStateChange (pBPInfo, false);
		}
	}
}


void CBreakPointMan::UnsubscribeBreakpoints (SessionInfo *pSessionInfo)
{
	POSITION pPosition;

    // ED, 4.3.2000: since connection is being Closed,
    // all Breakpoints are dismissed anyways.
    // But this call may block the GUI Thread!

	// pSessionInfo->m_pIFCBreakpoint->ClearAllBreakpoints (CComBSTR (""));


	pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;

		pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		if (pBPInfo->m_bSubscribed)
		{
			pBPInfo->m_bSubscribed = false;
			FireBPStateChange (pBPInfo, false);
		}
	}
}


HRESULT CBreakPointMan::ReadBreakpoint (
    const CString&               strInstance, 
    EDP_POS_QUALIFIER            PosQual, 
    long                         lX, 
    long                         lY, 
    long                         lCX, 
    long                         lCY, 
    CComObject<CBreakPointInfo>* pBPInfo
)
{
    HRESULT hr;
    CComPtr<ITypeDBI> pTypeDBI;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IBreakpointDBI> pBreakpointDBI;
    CComBSTR sSource;
    CComBSTR sTypeName;
    long lNumber;


    pBPInfo->m_lNumber = -1;

    hr = GetTypeDBIForInstance (strInstance, pTypeDBI);
    if (hr == S_FALSE)
    {
        hr = E_CE_BPMAN_INVALID_BREAKPOINT_POSITION;
    }
    if (FAILED (hr))
    {
        FC_DEBUGPRINT7("CEBPMAN> ReadBreakpoint: failed to get breakpoint information (path=%s, q=%d, x=%ld, y=%ld, cx=%ld, cy=%ld, hr=0x%08lx)\n",
                        (LPCTSTR)strInstance, PosQual, lX, lY, lCX, lCY, hr);
        return (hr);
    }
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    }

    hr = pTypeDBI->get_Source (&sSource);
    if (FAILED (hr))
    {
        return (hr);
    }

    hr = pTypeDBI->get_TypeName (&sTypeName);
    if (FAILED (hr))
    {
        return (hr);
    }

    hr = pTypeDBI->get_BreakpointByPositionEx(PosQual, lX, lY, lCX, lCY, &pUnknown);
    if (hr == S_FALSE)
    {
        hr = E_CE_BPMAN_INVALID_BREAKPOINT_POSITION;
    }
    if (FAILED (hr))
    {
        return (hr);
    }

    hr = pUnknown->QueryInterface (IID_IBreakpointDBI, (LPVOID *)&pBreakpointDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        FC_DEBUGPRINT7("CEBPMAN> ReadBreakpoint: failed to get breakpoint information (path=%s, q=%d, x=%ld, y=%ld, cx=%ld, cy=%ld, hr=0x%08lx)\n",
                        (LPCTSTR)strInstance, PosQual, lX, lY, lCX, lCY, hr);
    }
    ASSERT (pBreakpointDBI != NULL);
    if (pBreakpointDBI == NULL)
    {
        return (E_UNEXPECTED);
    }
        
    hr = pBreakpointDBI->get_Number (&lNumber);
    if (FAILED (hr))
    {
        pBreakpointDBI = NULL;
        return (hr);
    }

    hr = pBreakpointDBI->get_Qualifier (&PosQual);
    if (FAILED (hr))
    {
        pBreakpointDBI = NULL;
        pBPInfo->Release ();
        return (hr);
    }

    hr = pBreakpointDBI->get_X (&lX);
    if (FAILED (hr))
    {
        pBreakpointDBI = NULL;
        pBPInfo->Release ();
        return (hr);
    }

    hr = pBreakpointDBI->get_Y (&lY);
    if (FAILED (hr))
    {
        pBreakpointDBI = NULL;
        pBPInfo->Release ();
        return (hr);
    }

    hr = pBreakpointDBI->get_CX (&lCX);
    if (FAILED (hr))
    {
        pBreakpointDBI = NULL;
        pBPInfo->Release ();
        return (hr);
    }

    hr = pBreakpointDBI->get_CY (&lCY);
    if (FAILED (hr))
    {
        pBreakpointDBI = NULL;
        pBPInfo->Release ();
        return (hr);
    }


    pBPInfo->m_lNumber = lNumber;
    pBPInfo->m_strInstance = strInstance;
    pBPInfo->m_PosQual = PosQual;
    pBPInfo->m_lX    = lX;
    pBPInfo->m_lY    = lY;
    pBPInfo->m_lCX   = lCX;
    pBPInfo->m_lCY   = lCY;
    pBPInfo->m_strFileName = sSource;
    pBPInfo->m_strClassName = sTypeName;

    return (S_OK);
}


HRESULT CBreakPointMan::CheckBreakpoint (CComObject<CBreakPointInfo> *pBPInfo)
{
	HRESULT hr;
    CComPtr<ITypeDBI> pTypeDBI;
    CComPtr<IUnknown> pIUnknown;
    CComQIPtr<IBreakpointDBI> pIBreakpoint;

    hr = GetTypeDBIForInstance (pBPInfo->m_strInstance, pTypeDBI);
    if (hr == S_FALSE)
    {
		hr = E_CE_BPMAN_INVALID_BREAKPOINT_POSITION;
    }
    if (FAILED (hr))
    {
	    FC_DEBUGPRINT7("CEBPMAN> CheckBreakpoint: failed to get breakpoint information (path=%s, q=%d, x=%ld, y=%ld, cx=%ld, cy=%ld, hr=0x%08lx)\n",
				        (LPCTSTR)pBPInfo->m_strInstance, pBPInfo->m_PosQual, pBPInfo->m_lX, pBPInfo->m_lY, pBPInfo->m_lCX, pBPInfo->m_lCY, hr);
        return (hr);
    }
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    }

    hr = pTypeDBI->get_BreakpointByPositionEx (pBPInfo->m_PosQual, 
         pBPInfo->m_lX, pBPInfo->m_lY, pBPInfo->m_lCX, pBPInfo->m_lCY, &pIUnknown);

    if (hr == S_FALSE)
    {
		hr = E_CE_BPMAN_INVALID_BREAKPOINT_POSITION;
    }
    if (FAILED (hr))
    {
        return (hr);
    }

    // SIS: check breakpoint number
    pIBreakpoint = pIUnknown;
    if(pIBreakpoint)
    {
        long    lNumber;
        hr = pIBreakpoint->get_Number(&lNumber);
        if(hr == S_OK)
        {
            pBPInfo->m_lNumber = lNumber;
        }
    }

	return (S_OK);
}


void CBreakPointMan::ClearBreakpoints (SessionInfo *pSessionInfo, const CString &strInstance)
{
	bool bRemovedBP;

	do
	{
		POSITION pPosition;

		bRemovedBP = false;

		pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
		while (pPosition != NULL && !bRemovedBP)
		{
			CComObject<CBreakPointInfo> *pBPInfo;
			HRESULT hr;

			pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
			assert (pBPInfo != NULL);
			if (pBPInfo == NULL)
				continue;

			if (strInstance != _T ("") && strInstance.CompareNoCase (pBPInfo->m_strInstance) != 0)
				continue;

			hr = ClearBreakpoint (pSessionInfo, pBPInfo);
			if (FAILED (hr))
			{
				// INCOMPLETE: Debug output
			}

			FireBPRemoved (pBPInfo);

			RemoveBreakpoint (pSessionInfo, pBPInfo);
			bRemovedBP = true;
		}
	} while (bRemovedBP);
}


HRESULT CBreakPointMan::SetBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo, bool bActivate)
{
	HRESULT hr=S_OK;

	if (bActivate && pSessionInfo->m_pIFCBreakpoint != NULL && pSessionInfo->m_bEstablished)
	{
		hr = pSessionInfo->m_pIFCBreakpoint->SetBreakpoint (CComBSTR (pBPInfo->m_strInstance), pBPInfo->m_lNumber, &pBPInfo->m_lHandle);
		if (FAILED (hr))
		{
			return (E_CE_BPMAN_CANNOT_SUBSCRIBE_BREAKPOINT);
		}

		pBPInfo->m_bSubscribed = true;
	}

	pBPInfo->m_bActive = bActivate;

	return (S_OK);
}


HRESULT CBreakPointMan::ClearBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo)
{
	HRESULT hr=S_OK;

	if (pSessionInfo->m_pIFCBreakpoint != NULL && pBPInfo->m_bSubscribed)
	{
		hr = pSessionInfo->m_pIFCBreakpoint->ClearBreakpoint (pBPInfo->m_lHandle);
		// Currently assume clearing a breakpoint always
		// succeeds even if an error code is returned.
		hr = S_OK;
		if (FAILED (hr))
		{
			return (E_CE_BPMAN_CANNOT_UNSUBSCRIBE_BREAKPOINT);
		}

		pBPInfo->m_bSubscribed = false;
	}

	pBPInfo->m_bActive = false;

	return (S_OK);
}


void CBreakPointMan::FireBPStateChange (CComObject<CBreakPointInfo> *pBPInfo, bool bUpdate)
{
	BPM_BRKPT_STATE state;
	HRESULT hr;

	if (!pBPInfo->m_bActive)
	{
		state = BPM_BPINACTIVE;
	}
	else
	{
		if (pBPInfo->m_bSubscribed)
		{
			state = BPM_BPACTIVE;
		}
		else
		{
			state = BPM_BPACTIVEUNSET;
		}
	}

	hr = Fire_OnBPStateChanged (state, CComBSTR (pBPInfo->m_strInstance), pBPInfo->GetUnknown (), bUpdate ? TRUE : FALSE);
	if (FAILED (hr))
	{
		// INCOMPLETE: Debug output
	}
}


void CBreakPointMan::FireBPRemoved (CComObject<CBreakPointInfo> *pBPInfo)
{
	HRESULT hr;

	hr = Fire_OnBPStateChanged (BPM_NOBP, CComBSTR (pBPInfo->m_strInstance), pBPInfo->GetUnknown (), false);
	if (FAILED (hr))
	{
		// INCOMPLETE: Debug output
	}
}


void CBreakPointMan::AddBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo)
{
	pSessionInfo->m_Breakpoints.AddTail (pBPInfo);
	pBPInfo->AddRef ();
}


void CBreakPointMan::RemoveBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo)
{
	POSITION pPosition;

	pPosition = pSessionInfo->m_Breakpoints.Find (pBPInfo);

	if (pPosition != NULL)
	{
		pSessionInfo->m_Breakpoints.RemoveAt (pPosition);
	}

	pBPInfo->Release ();
}


CComObject<CBreakPointInfo>* CBreakPointMan::LookupBreakpoint (
    SessionInfo*      pSessionInfo, 
    const CString&    strInstance, 
    EDP_POS_QUALIFIER PosQual,
    long              lX, 
    long              lY, 
    long              lCX, 
    long              lCY 
)
{
	POSITION pPosition;

	pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo>* pBPInfo;

		pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

        if (strInstance.CompareNoCase (pBPInfo->m_strInstance)) 
            continue;

        if(PosQual != pBPInfo->m_PosQual)
            continue;

        switch(PosQual)
        {
        case EDP_STLINE:  
            if(lY==pBPInfo->m_lY)
                return pBPInfo;
        break;
        case EDP_GRBOX:  
            if(lX==pBPInfo->m_lX && lY==pBPInfo->m_lY)
                return pBPInfo;
        break;
        case EDP_GRSTBOX:
            if(lX==pBPInfo->m_lX && lY==pBPInfo->m_lY && lCY==pBPInfo->m_lCY)
                return pBPInfo;
        break;
        case EDP_GRREGION:
            assert(!"CBreakPointMan::LookupBreakpoint: EDP_GRREGION not implemented");
            return NULL;
        case EDP_GRBEGIN:  
        case EDP_GREND:  
            return pBPInfo;
        break;
        default: 
            assert(!"bad BP PosQual enum");
            return NULL;
        }
	}

	return (NULL);
}


CComObject<CBreakPointInfo> *CBreakPointMan::LookupDuplicateBreakpoint (SessionInfo *pSessionInfo, const CString &strInstance, long lBPNumber)
{
    POSITION pPosition;

    pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CComObject<CBreakPointInfo> *pBPInfo;

        pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
        assert (pBPInfo != NULL);
        if (pBPInfo == NULL)
            continue;

        if (strInstance.CompareNoCase (pBPInfo->m_strInstance) == 0 &&
            lBPNumber == pBPInfo->m_lNumber)
        {
            HRESULT hr;

            // Invalid breakpoints can never be duplicates.

            hr = CheckBreakpoint (pBPInfo);
            if (FAILED (hr))
            {
                return (NULL);
            }

            return (pBPInfo);
        }
    }

    return (NULL);
}


HRESULT CBreakPointMan::AddStoppoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo)
{
	HRESULT hr;

	pSessionInfo->m_Stoppoints.AddTail (pBPInfo);

	hr = Fire_OnSPStateChanged (BPM_STOPPED, CComBSTR (pBPInfo->m_strInstance), pBPInfo->GetUnknown (), false);
	if (FAILED (hr))
	{
		// INCOMPLETE: Debug output
	}

	return (S_OK);
}


HRESULT CBreakPointMan::ContinueFromStoppoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo, long mode)
{
	HRESULT hr;

    //mode: 0=continue task, 1=single step, 2=step over, 3=step out 
    if(mode==0) 
    {
	    hr = pSessionInfo->m_pIFCBreakpoint->ContinueTask (CComBSTR (pBPInfo->m_strTask));
    }
    else if(mode==1)
    {
        hr = pSessionInfo->m_pIFCBreakpoint->SingleStep (CComBSTR (pBPInfo->m_strTask));
    }
    else if(mode==2)
    {
        hr = pSessionInfo->m_pIFCBreakpoint->StepOver (CComBSTR (pBPInfo->m_strTask));
    }
    else if(mode==3)
    {
        hr = pSessionInfo->m_pIFCBreakpoint->StepOut (CComBSTR (pBPInfo->m_strTask));
    }
    else
    {
        assert(!"bad mode");
        hr = E_INVALIDARG;
    }

    if (FAILED (hr))
	{
		return (hr);
	}

	return (S_OK);
}


//------------------------------------------------------------------*
/**
 * continue unambigous.
 *
 *  continue will only performed if:
 *  - there is only one session
 *  - there is only one task
 *
 * @param           mode
 * @return          S_OK: ok
 *                  S_FALSE: not unambious
 *                  E_FAIL: error
 *                  result of ContinueTask() or Step function.
 * @exception       -
 * @see             -
*/
HRESULT CBreakPointMan::ContinueUnambigous(long mode)
{
	HRESULT hr;

    SessionInfo *pSessionInfo;
    CComObject<CBreakPointInfo> *pBPInfo;

    int iSessions = m_SessionInfos.GetCount();

    if(iSessions != 1)
    {
        return S_FALSE;
    }

    // now get the only session object
    POSITION    sessionpos = m_SessionInfos.GetStartPosition();
    CString     strTargetName;
    void *pVoid;

    m_SessionInfos.GetNextAssoc(sessionpos, strTargetName, pVoid);
	pSessionInfo = (SessionInfo *)pVoid;

    ASSERT(pSessionInfo);
    if(!pSessionInfo)
    {
        return E_FAIL;
    }

    int iStoppoints = pSessionInfo->m_Stoppoints.GetCount();
    if(iStoppoints != 1)
    {
        return S_FALSE;
    }

	POSITION stoppointpos = pSessionInfo->m_Stoppoints.GetHeadPosition ();
	pBPInfo = pSessionInfo->m_Stoppoints.GetNext(stoppointpos);

    ASSERT(pBPInfo);
    if(!pBPInfo)
    {
        return E_FAIL;
    }

    if(mode==0) 
    {
	    hr = pSessionInfo->m_pIFCBreakpoint->ContinueTask (CComBSTR (pBPInfo->m_strTask));
    }
    else if(mode==1)
    {
        hr = pSessionInfo->m_pIFCBreakpoint->SingleStep (CComBSTR (pBPInfo->m_strTask));
    }
    else if(mode==2)
    {
        hr = pSessionInfo->m_pIFCBreakpoint->StepOver (CComBSTR (pBPInfo->m_strTask));
    }
    else if(mode==3)
    {
        hr = pSessionInfo->m_pIFCBreakpoint->StepOut (CComBSTR (pBPInfo->m_strTask));
    }
    else
    {
        assert(!"bad mode");
        hr = E_INVALIDARG; 
    }


	if (FAILED (hr))
	{
		return (hr);
	}
	return (S_OK);
}


void CBreakPointMan::TaskContinued (StopData *pData)
{
	HRESULT hr;
	SessionInfo *pSessionInfo;
	CComObject<CBreakPointInfo> *pBPInfo;

	hr = SessionInfoFromInstance (pData->m_strInstance, pSessionInfo);
	if (FAILED (hr))
	{
		// INCOMPLETE: Debug output
		return;
	}
	if (hr == S_FALSE)
	{
		// INCOMPLETE: Debug output
		return;
	}

	pBPInfo = LookupStoppoint (pSessionInfo, pData->m_strInstance);
	//assert (pBPInfo != NULL);
	if (pBPInfo == NULL)
	{
		return;
	}

	RemoveStoppoint (pSessionInfo, pBPInfo);
}


void CBreakPointMan::RemoveStoppoints (SessionInfo *pSessionInfo)
{
	while (!pSessionInfo->m_Stoppoints.IsEmpty ())
	{
		CComObject<CBreakPointInfo> *pBPInfo;

		pBPInfo = pSessionInfo->m_Stoppoints.GetHead ();
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		RemoveStoppoint (pSessionInfo, pBPInfo);
	}
}


void CBreakPointMan::RemoveStoppoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo)
{
	HRESULT hr;
	POSITION pPosition;

	hr = Fire_OnSPStateChanged (BPM_CONTINUED, CComBSTR (pBPInfo->m_strInstance), pBPInfo->GetUnknown (), false);
	if (FAILED (hr))
	{
		// INCOMPLETE: Debug output
	}

	pPosition = pSessionInfo->m_Stoppoints.Find (pBPInfo);
	if (pPosition != NULL)
	{
		pSessionInfo->m_Stoppoints.RemoveAt (pPosition);
		pBPInfo->Release ();
	}
}


CComObject<CBreakPointInfo> *CBreakPointMan::LookupStoppoint (SessionInfo *pSessionInfo, const CString &strInstance)
{
	POSITION pPosition;

	pPosition = pSessionInfo->m_Stoppoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;

		pBPInfo = pSessionInfo->m_Stoppoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		if (strInstance.CompareNoCase (pBPInfo->m_strInstance) == 0)
		{
			return (pBPInfo);
		}
	}

	return (NULL);
}


void CBreakPointMan::UpdateBPs (SessionInfo *pSessionInfo)
{
	POSITION pPosition;

	pPosition = pSessionInfo->m_Breakpoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;

		pBPInfo = pSessionInfo->m_Breakpoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		FireBPStateChange (pBPInfo, true);
	}
}


void CBreakPointMan::UpdateSPs (SessionInfo *pSessionInfo)
{
	POSITION pPosition;

	pPosition = pSessionInfo->m_Stoppoints.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CComObject<CBreakPointInfo> *pBPInfo;
		HRESULT hr;

		pBPInfo = pSessionInfo->m_Stoppoints.GetNext (pPosition);
		assert (pBPInfo != NULL);
		if (pBPInfo == NULL)
			continue;

		hr = Fire_OnSPStateChanged (BPM_STOPPED, CComBSTR (pBPInfo->m_strInstance), pBPInfo->GetUnknown (), true);
		if (FAILED (hr))
		{
			// INCOMPLETE: Debug output
		}
	}
}


void CBreakPointMan::FireMessage (HRESULT hr)
{
	CString strMessage;

	if (!GetMessageText (hr, strMessage))
	{
		return;
	}

	Fire_OnNewMessage (CComBSTR (""), E_FACILITY_CEBPMAN, hr, CComBSTR (strMessage));
}


bool CBreakPointMan::GetMessageText (HRESULT hr, CString &strMessage)
{
	HINSTANCE hInstance;
	DWORD dwResult;
	LPVOID pBuffer;

	hInstance = AfxGetResourceHandle ();
	assert (hInstance != NULL);
	if (hInstance == NULL)
	{
		return (false);
	}

	dwResult = FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
	                          hInstance,
	                          hr,
	                          UTIL_GetInstallLanguage (),
	                          (LPTSTR)&pBuffer,
	                          0,
	                          NULL);

	if (dwResult == 0)
	{
		return (false);
	}

	assert (pBuffer != NULL);
	if (pBuffer == NULL)
	{
		return (false);
	}

	strMessage = (LPCTSTR)pBuffer;

	::LocalFree (pBuffer);

	return (true);
}


/////////////////////////////////////////////////////////////////////////////
// CBreakPointInfo

STDMETHODIMP CBreakPointInfo::get_FileName (BSTR *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	TRY
	{
		*pVal = m_strFileName.AllocSysString ();
	}
	CATCH (CMemoryException, e)
	{
		return (E_OUTOFMEMORY);
	}
	END_CATCH

	return (S_OK);
}

STDMETHODIMP CBreakPointInfo::get_PosQualifier (EDP_POS_QUALIFIER *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_PosQual;

	return (S_OK);
}

STDMETHODIMP CBreakPointInfo::get_X (long *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_lX;

	return (S_OK);
}


STDMETHODIMP CBreakPointInfo::get_Y (long *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_lY;

	return (S_OK);
}

STDMETHODIMP CBreakPointInfo::get_CX (long *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_lCX;

	return (S_OK);
}

STDMETHODIMP CBreakPointInfo::get_CY (long *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_lCY;

	return (S_OK);
}






STDMETHODIMP CBreakPointInfo::get_ClassName (BSTR *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	TRY
	{
		*pVal = m_strClassName.AllocSysString ();
	}
	CATCH (CMemoryException, e)
	{
		return (E_OUTOFMEMORY);
	}
	END_CATCH

	return (S_OK);
}


STDMETHODIMP CBreakPointInfo::get_Language (BSTR *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	TRY
	{
		*pVal = m_strLanguage.AllocSysString ();
	}
	CATCH (CMemoryException, e)
	{
		return (E_OUTOFMEMORY);
	}
	END_CATCH

	return (S_OK);
}


STDMETHODIMP CBreakPointInfo::get_Quality (long *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_lQuality;

	return (S_OK);
}


STDMETHODIMP CBreakPointInfo::get_Instance (BSTR *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	TRY
	{
		*pVal = m_strInstance.AllocSysString ();
	}
	CATCH (CMemoryException, e)
	{
		return (E_OUTOFMEMORY);
	}
	END_CATCH

	return (S_OK);
}


STDMETHODIMP CBreakPointInfo::get_Active (BOOL *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_bActive ? TRUE : FALSE;

	return (S_OK);
}


STDMETHODIMP CBreakPointInfo::get_Number (long *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	*pVal = m_lNumber;

	return (S_OK);
}


/////////////////////////////////////////////////////////////////////////////
// CCallStackEntry

STDMETHODIMP CCallStackEntry::get_Instance (BSTR *pVal)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (pVal == NULL)
	{
		return (E_POINTER);
	}

	TRY
	{
		*pVal = m_strInstance.AllocSysString ();
	}
	CATCH (CMemoryException, e)
	{
		return (E_OUTOFMEMORY);
	}
	END_CATCH

	return (S_OK);
}


/////////////////////////////////////////////////////////////////////////////
// CBreakpointNotify

CBreakpointNotify::CBreakpointNotify ()
: m_pStopNotifyCB (NULL),
  m_pDownloadNotifyCB (NULL)
{
}


void CBreakpointNotify::SetNotifyCBs (FC_CALLBACK_BASE (StopNotifyCB) *pStopNotifyCB,
                                      FC_CALLBACK_BASE (DownloadNotifyCB) *pDownloadNotifyCB)
{
	m_pStopNotifyCB = pStopNotifyCB;
	m_pDownloadNotifyCB = pDownloadNotifyCB;
}


STDMETHODIMP_ (HRESULT) CBreakpointNotify::OnBreakpointReached (BSTR sInstance, long lNumber, long lQuality, BSTR sTask)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());

	if (m_pStopNotifyCB != NULL)
	{
		StopData data;

		data.m_lNumber = lNumber;
		data.m_lQuality = lQuality;
		data.m_strInstance = sInstance;
		data.m_strTask = sTask;

		m_pStopNotifyCB->Execute (&data);
	}
    return(S_OK);
}


STDMETHODIMP_ (HRESULT) CBreakpointNotify::OnConfigurationChanged (BSTR sAddress)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());

	if (m_pDownloadNotifyCB != NULL)
	{
		DownloadData data;

        data.m_strAddress = sAddress;

		m_pDownloadNotifyCB->Execute (&data);
	}
    return(S_OK);
}

CExclusiveSessionNotifyBPMan::CExclusiveSessionNotifyBPMan ()
: m_pSessionCB (NULL)
{
}


void CExclusiveSessionNotifyBPMan::SetNotifyCB (FC_CALLBACK_BASE (BPManExclusiveSessionCB) *pSessionCB)
{
	m_pSessionCB = pSessionCB;
}


STDMETHODIMP_(HRESULT) CExclusiveSessionNotifyBPMan::OnChanged()
{
    return(S_OK);
}


STDMETHODIMP_(HRESULT) CExclusiveSessionNotifyBPMan::OnStateChanged (LPUNKNOWN pSession, CSC_SESSION_STATE newState, HRESULT hrError)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	if (m_pSessionCB != NULL)
	{
		m_pSessionCB->Execute (pSession, newState);
	}
    return(S_OK);
}


