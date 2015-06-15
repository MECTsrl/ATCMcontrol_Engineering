


#include "stdafx.h"
#include "fc_todebug\fc_assert.h"
#include "CESession.h"
#include "utilif.h"

#define WHO_FILE_NAME   _T ("who.list")
#define WHO_ENGINEERING _T ("Engineering")


static CString GetMasterPassword ();


CCESessionManager::CCESessionManager (bool bKeepSessions)
: m_bKeepSessions (bKeepSessions),
  m_SessionNotifyCB (this, OnSessionNotify),
  m_pSessionNotify (NULL),
  m_SessionReleaseCB (this, OnSessionReleased)
{
}


CCESessionManager::~CCESessionManager ()
{
	DisposeAllSessions ();

	if (m_pSessionNotify != NULL)
	{
        m_pSessionNotify->SetCallback(NULL);
		m_pSessionNotify->Release ();
	};
}


HRESULT CCESessionManager::GetSession (const CString &strMachine, CCESession *&pSession)
{
    HRESULT hr;

	pSession = FindSession (strMachine);
	if (pSession != NULL)
	{
        return (S_OK);
	};

    hr = CreateSession (strMachine, pSession);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


CCESession *CCESessionManager::FindSession (const CString &strMachine)
{
	POSITION pPosition;

	pPosition = m_Sessions.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CCESession *pSession;

		pSession = m_Sessions.GetNext (pPosition);
		ASSERT (pSession != NULL);
		if (pSession == NULL)
			continue;

		if (strMachine.CompareNoCase (pSession->GetMachine ()) == 0)
		{
            pSession->Reference ();
			return (pSession);
		};
	};

	return (NULL);
}


void CCESessionManager::OnSessionReleased (CCESession *pSession)
{
    POSITION pPosition;

	pPosition = m_Sessions.Find (pSession);
	ASSERT (pPosition != NULL);
	if (pPosition != NULL)
	{
    	m_Sessions.RemoveAt (pPosition);
	};
}


void CCESessionManager::RegisterStateCB (FC_CALLBACK_BASE (SessionStateNotifyCB) *pSessionStateCB)
{
	m_SessionStateCBHolder.Add (pSessionStateCB);
}


void CCESessionManager::UnregisterStateCB (FC_CALLBACK_BASE (SessionStateNotifyCB) *pSessionStateCB)
{
	m_SessionStateCBHolder.Remove (pSessionStateCB);
}


HRESULT CCESessionManager::CreateSessionsObject ()
{
	HRESULT hr;

	if (m_pIFCSessions != NULL)
	{
		return (S_OK);
	};

	hr = CoCreateInstance (CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (LPVOID *)&m_pIFCSessions);
	if (FAILED (hr))
	{
        UTIL_FatalError (_T ("Failed to create Sessions component"), hr);
		FC_DEBUGPRINT1 ("CESUBSCR> CreateSessionsObject: failed to create FCSessions object (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = CComObject<CCESessionNotify>::CreateInstance (&m_pSessionNotify);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> CreateSessionsObject: failed to create session notification sink (hr=0x%08lx)\n", hr);
		m_pIFCSessions = NULL;
		return (hr);
	};
	m_pSessionNotify->AddRef ();
	m_pSessionNotify->SetCallback (&m_SessionNotifyCB);

	return (S_OK);
}


HRESULT CCESessionManager::CreateVarCollection ()
{
    HRESULT hr;

    if (m_pIFCVarCollection != NULL)
    {
        return (S_OK);
    };

	hr = CoCreateInstance (CLSID_FCVariableCollection, NULL, CLSCTX_ALL, IID_IFCVariableCollection, (LPVOID *)&m_pIFCVarCollection);
	if (FAILED (hr))
	{
        UTIL_FatalError (_T ("Failed to create VariableCollection component"), hr);
		FC_DEBUGPRINT1 (_T ("CESUBSCR> CreateVarCollection: failed to create variable collection object (hr=0x%08lx)\n"), (long)hr);
		return (hr);
	};

    return (S_OK);
}


void CCESessionManager::OnSessionNotify (const CComPtr<IUnknown> &pSessionUnknown, CSC_SESSION_STATE state)
{
	CCESession *pSession;
    bool bDebug;

	pSession = FindSession (pSessionUnknown);
	if (pSession == NULL)
	{
		return;
	};

    bDebug = pSessionUnknown == pSession->GetDebugSession ();

    if (bDebug)
    {
        if (state == waiting_for_connect)
        {
            HRESULT hr;

            hr = pSession->ReadDebugState ();
            if (hr == S_OK)
            {
                state = pSession->GetDebugState ();
            };
        }
        else
        {
            pSession->SetDebugState (state);
        };
    }
    else
    {
        if (state == waiting_for_connect)
        {
            HRESULT hr;

            hr = pSession->ReadState ();
            if (hr == S_OK)
            {
                state = pSession->GetState ();
            };
        }
        else
        {
	        pSession->SetState (state);
        };
    };

	if (state == RW_established || state == exclusive_established)
	{
		CString strPassword;

		strPassword = GetMasterPassword ();
		SetPassword (pSession, strPassword);
	};

	m_SessionStateCBHolder.Execute (pSession, bDebug, state);
}


HRESULT CCESessionManager::CreateSession (const CString &strMachine, CCESession *&pSession)
{
    HRESULT hr;

	hr = CreateSessionsObject ();
	if (FAILED (hr))
	{
		return (hr);
	};
	ASSERT (m_pIFCSessions != NULL);
	if (m_pIFCSessions == NULL)
	{
		return (E_FAIL);
	};

	pSession = new CCESession (strMachine, m_pIFCSessions, m_pSessionNotify);
	ASSERT (pSession != NULL);
	if (pSession == NULL)
	{
		return (E_OUTOFMEMORY);
	};

    pSession->RegisterReleaseCB (&m_SessionReleaseCB);

    if (m_bKeepSessions)
    {
        pSession->Reference ();
    };

    m_Sessions.AddTail (pSession);

    return (S_OK);
}


void CCESessionManager::DisposeAllSessions ()
{
    POSITION pPosition;
    CList<CCESession *, CCESession *> sessions;


    // Calling Unreference on a session may or may not remove the
    // session from m_Sessions (depending on whether clients still
    // reference the sessin or not). So we first copy m_Sessions
    // and operate on the copy which will never be concerned by
    // unreferencing the session.

    pPosition = m_Sessions.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CCESession *pSession;

        pSession = m_Sessions.GetNext (pPosition);
        ASSERT (pSession != NULL);
        if (pSession == NULL)
            continue;

        sessions.AddTail (pSession);
    };

    while (!sessions.IsEmpty ())
    {
        CCESession *pSession;

        pSession = sessions.RemoveHead ();
        ASSERT (pSession != NULL);
        if (pSession == NULL)
            continue;

        pSession->UnregisterReleaseCB (&m_SessionReleaseCB);

        if (m_bKeepSessions)
        {
            pSession->Unreference ();
        };
    };

    m_Sessions.RemoveAll ();
}


CCESession *CCESessionManager::FindSession (const CComPtr<IUnknown> &pSessionUnknown)
{
	POSITION pPosition;

	pPosition = m_Sessions.GetHeadPosition ();
	while (pPosition != NULL)
	{
		CCESession *pSession;

		pSession = m_Sessions.GetNext (pPosition);
		ASSERT (pSession != NULL);
		if (pSession == NULL)
			continue;

        if (pSessionUnknown == pSession->GetDebugSession () || pSessionUnknown == pSession->GetSession ())
		{
			return (pSession);
		};
	};

	return (NULL);
}


HRESULT CCESessionManager::SetPassword (CCESession *pSession, const CString &strPassword)
{
	HRESULT hr;
	CString strMachine;
	CString strVariableName;
	CComVariant varPassword;
	long lHandle;

    hr = CreateVarCollection ();
    if (FAILED (hr))
    {
        return (hr);
    };

	strMachine = pSession->GetMachine ();

	strVariableName.Format (_T ("[%s].__system.security.password"), (LPCTSTR)strMachine);
	varPassword = strPassword;

	hr = m_pIFCVarCollection->Subscribe (CComBSTR (strVariableName), &lHandle);
	if (FAILED (hr))
	{
		return (hr);
	};

	hr = m_pIFCVarCollection->WriteWithHandle (lHandle, varPassword);
	if (FAILED (hr))
	{
		(void)m_pIFCVarCollection->UnsubscribeWithHandle (lHandle);
		return (hr);
	};

	(void)m_pIFCVarCollection->UnsubscribeWithHandle (lHandle);

	return (S_OK);
}


CCESession::CCESession (const CString &strMachine, const CComPtr<IFCSessions> &pSessions, CCESessionNotify *pSessionNotify)
: m_uiReferences (1),
  m_strMachine (strMachine),
  m_pSessions (pSessions),
  m_pSessionNotify (pSessionNotify),
  m_bConnected (false),
  m_State (not_connected),
  m_bDebugConnected (false),
  m_DebugState (not_connected),
  m_pIFCConnect(),
  m_pIFCVarConnect()
{
}

CCESession::~CCESession ()
{
    m_ReleaseCBs.Execute (this);
    CloseDebug();
    Close();
}


void CCESession::Reference ()
{
    ++m_uiReferences;
}


void CCESession::Unreference ()
{
    if (--m_uiReferences == 0)
    {
        delete this;
    };
}


void CCESession::RegisterModifiedCB (FC_CALLBACK_BASE (SessionModifiedCB) *pModifiedCB)
{
    m_ModifiedCBs.Add (pModifiedCB);
}


void CCESession::UnregisterModifiedCB (FC_CALLBACK_BASE (SessionModifiedCB) *pModifiedCB)
{
    m_ModifiedCBs.Remove (pModifiedCB);
}


void CCESession::RegisterReleaseCB (FC_CALLBACK_BASE (SessionReleaseCB) *pReleaseCB)
{
    m_ReleaseCBs.Add (pReleaseCB);
}


void CCESession::UnregisterReleaseCB (FC_CALLBACK_BASE (SessionReleaseCB) *pReleaseCB)
{
    m_ReleaseCBs.Remove (pReleaseCB);
}


CString CCESession::GetMachine ()
{
    return (m_strMachine);
}


CComPtr<IUnknown> CCESession::GetSession ()
{
    return (m_pVSUnknown);
}


CComPtr<IUnknown> CCESession::GetDebugSession ()
{
    return (m_pDSUnknown);
}


CSC_SESSION_STATE CCESession::GetState ()
{
    return (m_State);
}


void CCESession::SetState (CSC_SESSION_STATE state)
{
    m_State = state;
    m_ModifiedCBs.Execute (this, false);
}


HRESULT CCESession::ReadState ()
{
    HRESULT hr;
    CSC_SESSION_STATE state;

    hr = m_pIFCVarConnect->get_SessionState (&state);
    if (hr != S_OK)
    {
        return (hr);
    };

    m_State = state;
    return (S_OK);
}


CSC_SESSION_STATE CCESession::GetDebugState ()
{
    return (m_DebugState);
}


void CCESession::SetDebugState (CSC_SESSION_STATE state)
{
    m_DebugState = state;
    m_ModifiedCBs.Execute (this, true);
}


HRESULT CCESession::ReadDebugState ()
{
    HRESULT hr;
    CSC_SESSION_STATE state;

    hr = m_pIFCConnect->get_SessionState (&state);
    if (hr != S_OK)
    {
        return (hr);
    };

    m_DebugState = state;
    return (S_OK);
}


bool CCESession::IsOpen ()
{
    return (m_pVSUnknown != NULL);
}


HRESULT CCESession::Open ()
{
	HRESULT hr;
	CComPtr<IUnknown> pVS;
	CComPtr<IUnknown> pVSUnknown;
	CComPtr<IFCVarSession> pIFCVarSession;
    CSC_SESSION_STATE state;

    if (m_pVSUnknown != NULL)
    {
        return (S_FALSE);
    };

    // NEW_CSC: no configuration parameter needed, only one address parameter
	hr = m_pSessions->OpenVarSession (CComBSTR (m_strMachine), &pVS);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> Open: failed to open variable session (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = pVS->QueryInterface (IID_IUnknown, (LPVOID *)&pVSUnknown);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> Open: failed to get interface IUnknown (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = pVS->QueryInterface (IID_IFCVarSession, (LPVOID *)&pIFCVarSession);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> Open: failed to get interface IFCVarSession (hr=0x%08lx)\n", hr);
		return (hr);
	};

    hr = pVS->QueryInterface(__uuidof(IFCConnect),(LPVOID *)&m_pIFCVarConnect);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> Open: failed to get interface IFCConnect (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = AtlAdvise (pVSUnknown, m_pSessionNotify->GetUnknown (), IID__FCSessionNotification, &m_dwCookieVS);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> Open: failed to advise sessions object (hr=0x%08lx)\n", hr);
		return (hr);
	};

    m_pVSUnknown = pVSUnknown;
    m_pIFCVarSession = pIFCVarSession;

    hr = m_pIFCVarConnect->get_SessionState (&state);
    ASSERT (SUCCEEDED (hr));
    if (SUCCEEDED (hr))
    {
        m_State = state;
    };

	return (S_OK);
}


HRESULT CCESession::Close ()
{
    if (m_pVSUnknown != NULL) {
	    HRESULT hr = AtlUnadvise (m_pVSUnknown, IID__FCSessionNotification, m_dwCookieVS);
	    if (FAILED (hr))
	    {
		    FC_DEBUGPRINT1 ("CESUBSCR> ~CCESession: failed to unadvise sessions object (hr=0x%08lx)\n", hr);
	    }
    }

    m_pVSUnknown = NULL;
    m_pIFCVarSession = NULL;
    m_pIFCVarConnect = NULL;

	return (S_OK);
}


bool CCESession::IsDebugOpen ()
{
    return (m_pDSUnknown != NULL);
}


HRESULT CCESession::OpenDebug ()
{
	HRESULT hr;
	CComPtr<IUnknown> pDS;
	CComPtr<IUnknown> pDSUnknown;
    CComPtr<IFCConnect> pIFCConnect;
    CSC_SESSION_STATE state;

    if (m_pDSUnknown != NULL)
    {
        return (S_FALSE);
    };

    // NEW_CSC: no configuration parameter needed, only one address parameter
	hr = m_pSessions->OpenCommissioningSession (CComBSTR (m_strMachine), &pDS);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> OpenDebug: failed to open debug session (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = pDS->QueryInterface (IID_IUnknown, (LPVOID *)&pDSUnknown);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> OpenDebug: failed to get interface IUnknown (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = pDS->QueryInterface (IID_IFCConnect, (LPVOID *)&pIFCConnect);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> OpenDebug: failed to get interface IFCConnect (hr=0x%08lx)\n", hr);
		return (hr);
	};

	hr = AtlAdvise (pDSUnknown, m_pSessionNotify->GetUnknown (), IID__FCSessionNotification, &m_dwCookieDS);
	if (FAILED (hr))
	{
		FC_DEBUGPRINT1 ("CESUBSCR> OpenDebug: failed to advise sessions object (hr=0x%08lx)\n", hr);
		return (hr);
	};

    m_pDSUnknown = pDSUnknown;
    m_pIFCConnect = pIFCConnect;

    hr = pIFCConnect->get_SessionState (&state);
    ASSERT (SUCCEEDED (hr));
    if (SUCCEEDED (hr))
    {
        m_DebugState = state;
    };

	return (S_OK);
}


HRESULT CCESession::CloseDebug ()
{
    if (m_pDSUnknown != NULL) {
	    HRESULT hr = AtlUnadvise (m_pDSUnknown, IID__FCSessionNotification, m_dwCookieDS);
	    if (FAILED (hr))
	    {
		    FC_DEBUGPRINT1 ("CESUBSCR> ~CCESession: failed to unadvise sessions object (hr=0x%08lx)\n", hr);
	    }
    }

    m_pDSUnknown = NULL;
    m_pIFCConnect = NULL;

	return (S_OK);
}


bool CCESession::IsConnected ()
{
    return (m_bConnected);
}


HRESULT CCESession::Connect ()
{
	HRESULT hr;
    CSC_SESSION_STATE state;
    //TODO user name and password must be specified here
    CComBSTR sUser = _T("");
    CComBSTR sPassword = _T("");

    if (m_pVSUnknown == NULL)
    {
        return (S_FALSE);
    };

	if (m_bConnected)
	{
		return (S_FALSE);
	};

    ASSERT (m_pIFCVarConnect != NULL);
    if (m_pIFCVarConnect == NULL)
    {
        return (E_UNEXPECTED);
    };

	hr = m_pIFCVarConnect->Connect(sUser,sPassword);
	if (FAILED (hr))
	{
		return (hr);
	};

	m_bConnected = true;

    hr = m_pIFCVarConnect->get_SessionState (&state);
    ASSERT (SUCCEEDED (hr));
    if (SUCCEEDED (hr))
    {
        m_State = state;
    };

	return (S_OK);
}


HRESULT CCESession::Disconnect ()
{
    HRESULT hr;
    CSC_SESSION_STATE state;

    if (m_pVSUnknown == NULL)
    {
        return (S_FALSE);
    };

	if (!m_bConnected)
	{
		return (S_FALSE);
	};

    ASSERT (m_pIFCVarConnect != NULL);
    if (m_pIFCVarConnect == NULL)
    {
        return (E_UNEXPECTED);
    };

	hr = m_pIFCVarConnect->Disconnect ();

	m_bConnected = false;

    hr = m_pIFCVarConnect->get_SessionState (&state);
    ASSERT (SUCCEEDED (hr));
    if (SUCCEEDED (hr))
    {
        m_State = state;
    };

	return (hr);
}


bool CCESession::IsDebugConnected ()
{
    return (m_bDebugConnected);
}


HRESULT CCESession::ConnectDebug ()
{
	HRESULT hr;
    CSC_SESSION_STATE state;
    //TODO Username and Password missing.
    CComBSTR sUser = _T("");
    CComBSTR sPassword = _T("");

    if (m_pDSUnknown == NULL)
    {
        return (S_FALSE);
    };

    if (m_bDebugConnected)
    {
        return (S_FALSE);
    };

    ASSERT (m_pIFCConnect != NULL);
    if (m_pIFCConnect == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pIFCConnect->Connect (sUser,sPassword);
   	if (FAILED (hr))
    {
        return (hr);
    };

    m_bDebugConnected = true;

    hr = m_pIFCConnect->get_SessionState (&state);
    ASSERT (SUCCEEDED (hr));
    if (SUCCEEDED (hr))
    {
        m_DebugState = state;
    };

    return (S_OK);
}


HRESULT CCESession::DisconnectDebug ()
{
    HRESULT hr;
    CSC_SESSION_STATE state;

    if (m_pDSUnknown == NULL)
    {
        return (S_FALSE);
    };

    if (!m_bDebugConnected)
    {
        return (S_FALSE);
    };

    ASSERT (m_pIFCConnect != NULL);
    if (m_pIFCConnect == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pIFCConnect->Disconnect ();

    m_bDebugConnected = false;

    hr = m_pIFCConnect->get_SessionState (&state);
    ASSERT (SUCCEEDED (hr));
    if (SUCCEEDED (hr))
    {
        m_DebugState = state;
    };

    return (hr);
}


STDMETHODIMP_(HRESULT) CCESessionNotify::OnChanged ()
{
    return S_OK;
}


STDMETHODIMP_(HRESULT) CCESessionNotify::OnStateChanged (LPUNKNOWN pSession, CSC_SESSION_STATE newState, HRESULT hrError)
{
	AFX_MANAGE_STATE (AfxGetAppModuleState ());

	if (m_pCallback == NULL)
	{
		return S_OK;
	};

	m_pCallback->Execute (pSession, newState);
    return S_OK;
}


static CString GetMasterPassword ()
{
	HMODULE hModule;
	CStdioFile file;
	CString strPassword;
	TCHAR szWhoFile[_MAX_PATH + 1];
	LPTSTR psz;
	CString strWhoFile;

	hModule = ::GetModuleHandle (NULL);
	::GetModuleFileName (hModule, szWhoFile, _MAX_PATH);
	psz = _tcsrchr (szWhoFile, _T('\\'));
	if (psz != NULL)
	{
		*psz = _T ('\0');
	};
	strWhoFile = CString (szWhoFile) + _T ("\\") + WHO_FILE_NAME;

	if (!file.Open (strWhoFile, CFile::modeRead | CFile::shareDenyWrite))
	{
		return ("");
	};

	TRY
	{
		CString strLine;

		while (file.ReadString (strLine))
		{
			LPTSTR pszLine;
			LPTSTR pszField;

			pszLine = strLine.GetBuffer (0);

			pszField = _tcstok (pszLine, _T ("="));
			if (pszField != NULL)
			{
				if (_tcsicmp (pszField, WHO_ENGINEERING) == NULL)
				{
					pszField = _tcstok (NULL, _T ("=;"));
					if (pszField != NULL)
					{
						strPassword = pszField;
						break;
					};
				};
			};

			strLine.ReleaseBuffer ();
		};
	}
	CATCH_ALL (e)
	{
		strPassword = "";
	}
	END_CATCH_ALL

	file.Close ();
	return (strPassword);
}
