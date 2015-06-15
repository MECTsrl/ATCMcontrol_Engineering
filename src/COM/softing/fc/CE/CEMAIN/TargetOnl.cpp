


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "Stdafx.h"
#include "TargetOnl.h"
#include "CEStateMan.h"
#include "CECompMan.h"
#include "CESysdef.h"
#include "XMLHelper/XMLNode.h"
#include "CEProjMan.h"
#include "toolbox.h"
#include "fc_todebug/fc_assert.h"
#include "utilif.h"
#include "resource.h"
#include "ImplSessNot.h"
#include "connectdlg.h"
#include "BrkPtMan.h"
#include "traceif.h"


//----  Static Initializations:   ----------------------------------*
CTargetOnl::CTargetOnl
(
    const CString& crstrTargetId /*= ""*/,
    const CString& crstrInstIdPath /*= ""*/,
    const CString& crstrType /*=""*/,
    const CString& crstrAssignedResource /*=""*/
)
: CTargetOnlBase(crstrTargetId, crstrInstIdPath, crstrType, crstrAssignedResource),
  m_ExclusiveSessionState (not_connected),
  m_ExclusiveHrError(S_OK),
  m_dwExclusiveSessionCookie(0),
  m_bExclusiveConnected(FALSE),
  m_pExclusiveSession(NULL),
  m_pExclusiveNotify(NULL),
  m_pTransferNotify(NULL),
  m_dwTransferCookie(0),
  m_pBreakpointNotify(NULL),
  m_dwBreakpointCookie(0),
  m_pDownloadGUIDSubscription(NULL),
  m_bDownloadGUIDValid(false),
  m_DownloadInfoCB(this, OnDownloadInfoChanged),
  m_pDownloadDateSubscription(NULL),
  m_bDownloadDateValid(false),
  m_pDownloadHostSubscription(NULL),
  m_bDownloadHostValid(false),
  m_bTaskInBreakPoint(FALSE)
{
}

CTargetOnl::~CTargetOnl()
{
	if (m_bExclusiveConnected)
	{
		DisconnectExclusive();
	}

	if(m_pExclusiveSession != NULL)
	{
		CloseExclusiveSession();
    }
}

// connect exclusive
HRESULT CTargetOnl::ConnectExclusive()
{
    HRESULT                 hr;
    CString                 strAssignedResource;
    bool                    bReopenSession;
    CCEStateManager*        pStateManager;
	CComPtr<IBreakPoint>    pIBreakPoint;
	CComPtr<IUnknown>       pISession;
    IUnknown*               pXMLNode;
    CXMLNode                tNode;


    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s) \n"), m_strId);

    // get assigned resource:
    // get project manager and from this get the node
    CComQIPtr<ICEProjInfo>  pProjMan;
    CComPtr<IUnknown>       pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;

    CComBSTR    sIdPath = m_strIdPath;
    hr = pProjMan->getXMLNodePtr(sIdPath, &pXMLNode);
    if(hr == S_OK)
    {
        tNode.SetIXMLNode(pXMLNode);
        pXMLNode->Release();
    }
    else
    {
        return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
    }

    tNode.GetAttribute(CE_XMLATTR_RESOURCE, strAssignedResource);

    // whenever a Target is assigned to a resource.
    if(FAILED(hr))
    {
        return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
    }
    bReopenSession = strAssignedResource != m_strAssignedResource;
    m_strAssignedResource = strAssignedResource;

    if(m_strAssignedResource == _T(""))
    {
        CString strMessage;

        strMessage.Format(IDS_DENY_DEBUG_WITHOUT_ASSIGN,(LPCTSTR)m_strId);
        AfxMessageBox(strMessage);
        return(S_FALSE);
    }

    // assure that already connected to target
//    if(!IsConnected())
//    {
//        hr = Connect();
//        if(hr != S_OK)
//        {
//            return hr;
//        }
//    }

    pStateManager = CEGetStateManager();

    m_ConnectActionCBs.Execute(this, TargetPreExclusiveConnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s) : TriggerTargetDebugConnect: CEPreConnect\n"), m_strId);

        pStateManager->TriggerTargetDebugConnect(this, CEPreConnect);
    }


    // If session is already open and resource name has
    // changed we have to close and reopen the session.

    if(m_pExclusiveSession != NULL && bReopenSession)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s) Reopen Session\n"), m_strId);

        hr = CloseExclusiveSession();
		if(FAILED(hr))
		{
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s)  CloseExclusiveSession returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            m_ConnectActionCBs.Execute(this, TargetExclusiveConnectError);
            if(pStateManager != NULL)
            {
                pStateManager->TriggerTargetDebugConnect(this, CEConnectError);
            }

			return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
		}
    }

	if(m_pExclusiveSession == NULL)
	{
		hr = OpenExclusiveSession();
		if(FAILED(hr))
		{
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s)  OpenExclusiveSession returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            m_ConnectActionCBs.Execute(this, TargetExclusiveConnectError);
            if(pStateManager != NULL)
            {
                pStateManager->TriggerTargetDebugConnect(this, CEConnectError);
            }

			return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
		}
	}

	{	//do it in a block to get hourglass destructed
		//at the end of the block.
		CHourglass hourglass;

		FC_DEBUGPRINT(_T("CETARGETONL> Connecting ...\n"));

        CComBSTR sUser = m_strUserName;
        CComBSTR sPassword = m_strPassword;
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s)  connect exclusive session \n"), 
                                        m_strId);
		hr = m_pExclusiveSession->Connect(sUser,sPassword);
	}

    if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s)  m_pExclusiveSession->Connect returned: hr=0x%08X\n"), 
                                        m_strId, hr);

		FC_DEBUGPRINT1(_T("CETARGETONL> connect sess. failed hr=0x%x\n"),hr);
        m_pExclusiveSession = NULL;
        m_bExclusiveConnected = FALSE;
        m_strControlAddressConn.Empty();

        m_ConnectActionCBs.Execute(this, TargetExclusiveConnectError);
        if(pStateManager != NULL)
        {
            pStateManager->TriggerTargetDebugConnect(this, CEConnectError);
        }

        return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
    }

    m_bExclusiveConnected = TRUE;
    m_strControlAddressConn = m_strControlAddress;

	pIBreakPoint = GetBreakpointManager();
	if(pIBreakPoint != NULL)
	{
		hr = m_pExclusiveSession->QueryInterface(IID_IUnknown,(LPVOID *)&pISession);
		if(SUCCEEDED(hr))
		{
			hr = pIBreakPoint->Attach(CComBSTR(m_strId), pISession);
		}

		if(FAILED(hr))
		{
			// INCOMPLETE: Debug output, message box ???
		}
	}

    m_ConnectActionCBs.Execute(this, TargetPostExclusiveConnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::ConnectExclusive : (TargetId: %s) : TriggerTargetDebugConnect: CEPreConnect\n"), m_strId);

        pStateManager->TriggerTargetDebugConnect(this, CEPostConnect);
    }

    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * disconnect from target.
 *
 *  assure that exclusive connection is released first.
 *
 * @return           return codes:  S_OK
 *                                  E_CE_TARGETONL_NOTCONNECTED
 *                                  E_CE_TARGETONL_DISCONNECTFAILED
 * @see             
*/
HRESULT CTargetOnl::Disconnect()
{
//    HRESULT hr;

//    if(m_bExclusiveConnected)
//    {
//        hr = DisconnectExclusive();
//        if(hr != S_OK)
//        {
//            return hr;
//        }
//    }
    return(CTargetOnlBase::Disconnect());
}


//------------------------------------------------------------------*
/**
 * Disconnect exclusive.
 *
 * @param            [in] bForce: force disconnect?
 * @return           S_OK | 
                     E_CE_TARGETONL_DISCONNECTFAILED |
                     E_CE_TARGETONL_FORCEDISCONNECTFAILED |
                     E_CE_TARGETONL_NOTCONNECTED
 * @exception        -
 * @see              
*/
HRESULT CTargetOnl::DisconnectExclusive(BOOL bForce /*=FALSE*/)
{
    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s) (bForce = %s)\n"), 
                                 m_strId, bForce?_T("TRUE"):_T("FALSE"));

    if(bForce)
    {
	    assert(m_pExclusiveSession != NULL);
    
        DWORD   dwLength = _MAX_PATH;
        CString strTemp;

        CComBSTR sUserName = m_strUserName;
        CComBSTR sPassword = m_strPassword;
        CComBSTR sMachineName;

        UTIL_GetLocalMachineName(strTemp.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
        strTemp.ReleaseBuffer();
        sMachineName = strTemp; 

        if(m_pExclusiveSession != NULL) {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s) exclusive session force disconnect\n"), 
                                 m_strId);

	        HRESULT hr = m_pExclusiveSession->ForceDisconnect(sUserName,sPassword, sMachineName);
    
	        assert(SUCCEEDED(hr));
            if(SUCCEEDED(hr)) {
                return(S_OK);
            }
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s)  m_pExclusiveSession->ForceDisconnect returned: hr=0x%08X\n"), 
                                        m_strId, hr);

        }
        return(MakeHResult(E_CE_TARGETONL_FORCEDISCONNECTFAILED));
    }

//    if(m_bTaskInBreakPoint)
//    {
//        if(::AfxMessageBox(IDS_WARN_DISCONNECT_IN_BP, MB_YESNO) == IDNO)
//        {
//            return S_FALSE;
//        }
//    }
    HRESULT hr;
    CCEStateManager *pStateManager;
	CComPtr<IBreakPoint> pIBreakPoint;
	CComPtr<IUnknown> pISession;

    if(!m_bExclusiveConnected) 
        return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));

    pStateManager = CEGetStateManager();

    m_ConnectActionCBs.Execute(this, TargetPreExclusiveDisconnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s) : TriggerTargetDebugConnect: CEPreDisconnect\n"), m_strId);
        pStateManager->TriggerTargetDebugConnect(this, CEPreDisconnect);
    }

	pIBreakPoint = GetBreakpointManager();
	if(pIBreakPoint != NULL)
	{
		hr = pIBreakPoint->Detach(CComBSTR(m_strId));

		if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s)  IBreakPoint->Detach returned: hr=0x%08X\n"), 
                                        m_strId, hr);

			FC_DEBUGPRINT1("CETARGETONL> Failed to detach commissioning session from breakpoint manager (hr=%08lx)\n", hr);
		}
	}

	ASSERT(m_pExclusiveSession != NULL);
    if(m_pExclusiveSession != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s)  disconnect exclusive session\n"), 
                                        m_strId);

		hr = m_pExclusiveSession->Disconnect();
        ASSERT(SUCCEEDED(hr));
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s)  m_pExclusiveSession->Disconnect returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            m_ConnectActionCBs.Execute(this, TargetExclusiveDisconnectError);
            if(pStateManager != NULL)
            {
                pStateManager->TriggerTargetDebugConnect(this, CEDisconnectError);
            }

            return(MakeHResult(E_CE_TARGETONL_DISCONNECTFAILED));
        }

        CloseExclusiveSession();
    }

    m_bExclusiveConnected = FALSE;
    m_strControlAddressConn.Empty();

    m_ConnectActionCBs.Execute(this, TargetPostExclusiveDisconnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::DisconnectExclusive : (TargetId: %s) : TriggerTargetDebugConnect: CEPostDisconnect\n"), m_strId);
        pStateManager->TriggerTargetDebugConnect(this, CEPostDisconnect);
    }

    UnsubscribeForDownloadInfo();

    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * is connected exclusive.
 *
 *  is CSC online debug connection established?
 *
 * @param           -
 * @return          is connected exclusively?
 * @exception       -
 * @see             
*/
BOOL    CTargetOnl::IsConnectedExclusive()
{
    return(m_bExclusiveConnected);
}


//------------------------------------------------------------------*
/**
 * is target connected exclusive.
 *
 *  is exclusive target connection OK
 *
 * @param           -
 * @return          is target connected exclusive?
 * @exception       -
 * @see             IsConnectedExclusive()
*/
BOOL    CTargetOnl::IsTargetConnectedExclusive()
{
    BOOL    bConnected = (m_eConnectState == eTOCS_connected_ex);
    return bConnected;
}


// debug / download connections
HRESULT CTargetOnl::GetExclusiveSessionHResult()
{
    return m_ExclusiveHrError;
}


CSC_SESSION_STATE CTargetOnl::GetExclusiveSessionState()
{
	return(m_ExclusiveSessionState);
}


LPUNKNOWN CTargetOnl::GetExclusiveSession()
{
	LPUNKNOWN pUnk;

	if(!m_pExclusiveSession)
    {
		return NULL;
	}
	m_pExclusiveSession->QueryInterface(IID_IUnknown,(void**)&pUnk);
	return pUnk;
}


// resource actions
HRESULT CTargetOnl::ClearResource(LPCTSTR pszResource)
{
	HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CString strHelp;
    strHelp.Format(IDS_RESOURCECLEAR, pszResource);
    if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO))
    {
        return S_OK;
    }

    assert(m_pExclusiveSession);
    if(!m_pExclusiveSession)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCDownload> pDLoad = m_pExclusiveSession;
    if(!pDLoad)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pDLoad->ClearResource(ProgramDomains);
    if(!FAILED(hr))
    {
        // if target has console -> clear visual domains
        if(m_tKADTargetData.GetConsoleFlag())
        {
    	    hr = pDLoad->ClearResource(VisualDomains);
        }
    }

    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_CLEAR);
    }

    if(FAILED(hr))
    {
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}







// download connections
bool CTargetOnl::IsDownloadInfoAvailable()
{
    return(m_bDownloadGUIDValid && m_bDownloadDateValid && m_bDownloadHostValid && m_bProjectGUIDValid && m_bProjectNameValid);
}

bool CTargetOnl::GetDownloadGUID(CString &strDownloadGUID)
{
    if(!m_bDownloadGUIDValid)
        return(false);

    strDownloadGUID = m_strDownloadGUID;
    return(true);
}

bool CTargetOnl::GetDownloadDate(CString &strDownloadDate)
{
    if(!m_bDownloadDateValid)
        return(false);

    strDownloadDate = m_strDownloadDate;
    return(true);
}

bool CTargetOnl::GetDownloadHost(CString &strDownloadHost)
{
    if(!m_bDownloadHostValid)
        return(false);

    strDownloadHost = m_strDownloadHost;
    return(true);
}

bool CTargetOnl::GetProjectGUID(CString &strProjectGUID)
{
    if(!m_bProjectGUIDValid)
        return(false);

    strProjectGUID = m_strProjectGUID;
    return(true);
}

bool CTargetOnl::GetProjectName(CString &strProjectName)
{
    if(!m_bProjectNameValid)
        return(false);

    strProjectName = m_strProjectName;
    return(true);
}


// register / unregister callbacks
HRESULT CTargetOnl::RegisterExclusiveSessionCB(FC_CALLBACK_BASE(TargetOnlExclusiveSessionCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ExclusiveSessionCBs.Find(pCallback);
	assert(pPosition == NULL);
	if(pPosition != NULL)
	{
		return(E_FAIL);
	}

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::RegisterExclusiveSessionCB : (TargetId: %s) \n"), m_strId);

	m_ExclusiveSessionCBs.AddTail(pCallback);

	return(S_OK);
}

HRESULT CTargetOnl::UnregisterExclusiveSessionCB(FC_CALLBACK_BASE(TargetOnlExclusiveSessionCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ExclusiveSessionCBs.Find(pCallback);
	assert(pPosition != NULL);
	if(pPosition == NULL)
	{
		return(E_FAIL);
	}

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::UnregisterExclusiveSessionCB : (TargetId: %s) \n"), m_strId);


	m_ExclusiveSessionCBs.RemoveAt(pPosition);

	return(S_OK);
}


HRESULT CTargetOnl::OpenExclusiveSession()
{
	HRESULT hr = S_OK;
	LPUNKNOWN pUnk;
	IFCSessions * pISessions; 
    int iStep = 0;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s) \n"), m_strId);

	if(m_pExclusiveSession) //already opened
    {
		return hr;
    }

    try
    {
	    hr = CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, 
                              (void**)&pISessions);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  CoCreateInstance FCSessions returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            CString strMessage;

            strMessage.Format(IDS_NO_COMPONENT, _T("Sessions"));
            UTIL_FatalError(strMessage, hr);
            AfxThrowOleException(GetScode(hr));
        }

		//Open the Exclusive session
	    BSTR sMachine = m_strControlAddress.AllocSysString();
	    BSTR sConfig = m_strAssignedResource.AllocSysString(); 

        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  FCSessions->OpenCommissioningSession: Machine address: %s\n"), 
                                        m_strId, m_strControlAddress);

		hr = pISessions->OpenCommissioningSession(sMachine, &pUnk);
        pISessions->Release();
		::SysFreeString(sMachine);
		::SysFreeString(sConfig);

		if(FAILED(hr)) 
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  FCSessions->OpenCommissioningSession returned: hr=0x%08X\n"), 
                                        m_strId, hr);

			FC_DEBUGPRINT1(_T("CETARGETONL> OpenExclusiveSession failed hr=0x%x\n"),hr);
			AfxThrowOleException(GetScode(hr));
		}

		hr = pUnk->QueryInterface(IID_IFCConnect,(void **)&m_pExclusiveSession);
		pUnk->Release();

		if(FAILED(hr)) 
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  QueryInterface IFCConnect returned: hr=0x%08X\n"), 
                                        m_strId, hr);

			FC_DEBUGPRINT1(_T("CETARGETONL> QueryIf failed hr=0x%x\n"),hr);
			AfxThrowOleException(GetScode(hr));
		}

        
		iStep++; //Step = 1 Release session ptr
		
		// create the session notification interface 
		hr = CComObject<CExclusiveSessionNotify>::CreateInstance(&m_pExclusiveNotify);
		if(FAILED(hr)) 
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  create CComObject CExclusiveSessionNotify returned: hr=0x%08X\n"), 
                                        m_strId, hr);

			FC_DEBUGPRINT1(_T("CETARGETONL> create of notification if. failed hr=0x%x\n"),hr);
			AfxThrowOleException(GetScode(hr));
		}
		
		m_pExclusiveNotify->m_pTargetOnl = this;

		iStep++; //Step=2 release session ptr, release notification interface

		hr = AtlAdvise(m_pExclusiveSession, m_pExclusiveNotify->GetUnknown(),
			IID__FCSessionNotification, &m_dwExclusiveSessionCookie);
		if(FAILED(hr)) 
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  AtlAdvise FCSessionNotification returned: hr=0x%08X\n"), 
                                        m_strId, hr);

			FC_DEBUGPRINT1(_T("CETARGETONL> Advise failed hr=0x%x\n"),hr);
			AfxThrowOleException(GetScode(hr));
		}

		/*Step = 3 release session ptr, release session notify if, release transfer 
          notify if AtlUnadvise session if*/
		iStep++ ; 

        hr = CComObject<CTransferNotify>::CreateInstance(&m_pTransferNotify);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  create CComObject CTransferNotify returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            FC_DEBUGPRINT1(_T("CETARGETONL> create of transfer notification object failed hr=0x%x\n"), hr);
            AfxThrowOleException(GetScode(hr));
        }
        ASSERT(m_pTransferNotify != NULL);
        if(m_pTransferNotify == NULL)
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  create CComObject CTransferNotify returned: hr=0x%08X\n"), 
                                      m_strId, hr);

            FC_DEBUGPRINT(_T("CETARGETONL> create of transfer notification object failed\n"));
            AfxThrowOleException(GetScode(E_FAIL));
        }
        m_pTransferNotify->AddRef();
        m_pTransferNotify->SetTarget(this);

        ++iStep;

        hr = AtlAdvise(m_pExclusiveSession, m_pTransferNotify->GetUnknown(), IID__FCTransferNotification, &m_dwTransferCookie);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  AtlAdvise FCTransferNotification  returned: hr=0x%08X\n"), 
                                      m_strId, hr);

            FC_DEBUGPRINT1(_T("CETARGETONL> failed to advise transfer notification hr=0x%x\n"), hr);
            AfxThrowOleException(GetScode(hr));
        }

		iStep++ ; 

        hr = CComObject<CBreakpointNotify>::CreateInstance(&m_pBreakpointNotify);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  create CComObject CBreakpointNotify returned: hr=0x%08X\n"), 
                                      m_strId, hr);

            FC_DEBUGPRINT1(_T("CETARGETONL> create of breakpoint notification object failed hr=0x%x\n"), hr);
            AfxThrowOleException(GetScode(hr));
        }
        ASSERT(m_pBreakpointNotify != NULL);
        if(m_pBreakpointNotify == NULL)
        {
             TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  create CComObject CBreakpointNotify returned: hr=0x%08X\n"), 
                                      m_strId, hr);

            FC_DEBUGPRINT(_T("CETARGETONL> create of breakpoint notification object failed\n"));
            AfxThrowOleException(GetScode(E_FAIL));
        }
        m_pBreakpointNotify->AddRef();
        m_pBreakpointNotify->SetTarget(this);

        ++iStep;

        hr = AtlAdvise(m_pExclusiveSession, m_pBreakpointNotify->GetUnknown(), IID__FCBreakpointNotification, &m_dwBreakpointCookie);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OpenExclusiveSession : (TargetId: %s)  AtlAdvise FCBreakpointNotification  returned: hr=0x%08X\n"), 
                                      m_strId, hr);

            FC_DEBUGPRINT1(_T("CETARGETONL> failed to advise transfer notification hr=0x%x\n"), hr);
            AfxThrowOleException(GetScode(hr));
        }

        ++iStep;
    }

	catch(COleException *e)
    {
        if(iStep >= 7)
        {
            AtlUnadvise(m_pExclusiveSession, IID__FCBreakpointNotification, m_dwBreakpointCookie);
            m_dwBreakpointCookie = 0;
        }

        if(iStep >= 6)
        {
            m_pBreakpointNotify->Release();
            m_pBreakpointNotify = NULL;
        }

        if(iStep >= 5)
        {
            AtlUnadvise(m_pExclusiveSession, IID__FCTransferNotification, m_dwTransferCookie);
            m_dwTransferCookie = 0;
        }

        if(iStep >= 4)
        {
            m_pTransferNotify->Release();
            m_pTransferNotify = NULL;
        }

		if(iStep >=	3) 
        {   //AtlUnadvise session notify if
            if(m_pExclusiveSession) 
            {            
			    AtlUnadvise(m_pExclusiveSession, IID__FCSessionNotification, m_dwExclusiveSessionCookie);
			    //do not check for errors
			    m_dwExclusiveSessionCookie = 0;
            }
		}

		if(iStep >= 1) 
        {   //Release Session Ptr
            if(m_pExclusiveSession) 
            {
			    m_pExclusiveSession->Release();
			    m_pExclusiveSession = NULL;
            }
		}

		hr = ResultFromScode(e->m_sc);
		e->Delete();
    }

	return hr;
}

HRESULT CTargetOnl::CloseExclusiveSession()
{
    HRESULT hr;
	HRESULT hrRes = S_OK;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::CloseExclusiveSession : (TargetId: %s) \n"), m_strId);


	if(m_pExclusiveSession) 
    {
        hr = AtlUnadvise(m_pExclusiveSession, IID__FCBreakpointNotification, m_dwBreakpointCookie);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::CloseExclusiveSession : (TargetId: %s)  AtlUnadvise FCBreakpointNotification returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            FC_DEBUGPRINT1(_T("CETARGETONL> failed to unadvise breakpoint notification hr=0x%x\n"), hr);
            if(hrRes == S_OK)
            {
                hrRes = hr;
            }
        }

        if(m_pBreakpointNotify != NULL)
        {
            m_pBreakpointNotify->Release();
            m_pBreakpointNotify = NULL;
        }

        hr = AtlUnadvise(m_pExclusiveSession, IID__FCTransferNotification, m_dwTransferCookie);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::CloseExclusiveSession : (TargetId: %s)  AtlUnadvise FCTransferNotification returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            FC_DEBUGPRINT1(_T("CETARGETONL> failed to unadvise transfer notification hr=0x%x\n"), hr);
            if(hrRes == S_OK)
            {
                hrRes = hr;
            }
        }

        if(m_pTransferNotify != NULL)
        {
            m_pTransferNotify->Release();
            m_pTransferNotify = NULL;
        }

		// unadvise the notification object from connection point
		hr = AtlUnadvise(m_pExclusiveSession, IID__FCSessionNotification, m_dwExclusiveSessionCookie);

		if(FAILED(hr)) 
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::CloseExclusiveSession : (TargetId: %s)  AtlUnadvise FCSessionNotification returned: hr=0x%08X\n"), 
                                        m_strId, hr);

			FC_DEBUGPRINT1(_T("CETARGETONL> Unadvise failed hr=0x%x\n"),hr);
			hrRes =(hrRes == S_OK) ? hr : hrRes;
			//overwrite hrRes only iff S_OK - first error counts
		}

		m_dwExclusiveSessionCookie = 0;
        m_pExclusiveSession->Release();
        m_pExclusiveSession = NULL;

	}

	return hrRes;
}


HRESULT CTargetOnl::SubscribeForDownloadInfo()
{
    HRESULT hr;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::SubscribeForDownloadInfo : (TargetId: %s)  \n"), m_strId);


    if(m_pDownloadGUIDSubscription == NULL)
    {
        hr = SubscribeDownloadInfoStringVariable("__srt0.__system.DLoad.DLoadGUID",
                                                  m_pDownloadGUIDSubscription,
                                                  m_bDownloadGUIDValid,
                                                  m_strDownloadGUID);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::SubscribeForDownloadInfo : (TargetId: %s) subscribe __srt0.__system.DLoad.DLoadGUID returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            UnsubscribeForDownloadInfo();
            return(hr);
        }
    }

    if(m_pDownloadDateSubscription == NULL)
    {
        hr = SubscribeDownloadInfoStringVariable("__srt0.__system.DLoad.Date",
                                                  m_pDownloadDateSubscription,
                                                  m_bDownloadDateValid,
                                                  m_strDownloadDate);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::SubscribeForDownloadInfo : (TargetId: %s) subscribe __srt0.__system.DLoad.DLoadGUID returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            UnsubscribeForDownloadInfo();
            return(hr);
        }
    }

    if(m_pDownloadHostSubscription == NULL)
    {
        hr = SubscribeDownloadInfoStringVariable("__srt0.__system.DLoad.Host",
                                                  m_pDownloadHostSubscription,
                                                  m_bDownloadHostValid,
                                                  m_strDownloadHost);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::SubscribeForDownloadInfo : (TargetId: %s) subscribe __srt0.__system.DLoad.DLoadGUID returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            UnsubscribeForDownloadInfo();
            return(hr);
        }
    }

    if(m_pProjectGUIDSubscription == NULL)
    {
        hr = SubscribeDownloadInfoStringVariable("__srt0.__system.DLoad.PrjGUID",
                                                  m_pProjectGUIDSubscription,
                                                  m_bProjectGUIDValid,
                                                  m_strProjectGUID);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::SubscribeForDownloadInfo : (TargetId: %s) subscribe __srt0.__system.DLoad.DLoadGUID returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            UnsubscribeForDownloadInfo();
            return(hr);
        }
    }

    if(m_pProjectNameSubscription == NULL)
    {
        hr = SubscribeDownloadInfoStringVariable("__system.ProjectName",
                                                  m_pProjectNameSubscription,
                                                  m_bProjectNameValid,
                                                  m_strProjectName);
        if(FAILED(hr))
        {
            TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::SubscribeForDownloadInfo : (TargetId: %s) subscribe __srt0.__system.DLoad.DLoadGUID returned: hr=0x%08X\n"), 
                                        m_strId, hr);

            UnsubscribeForDownloadInfo();
            return(hr);
        }
    }

    if(m_bDownloadGUIDValid || m_bDownloadDateValid || m_bDownloadHostValid || m_bProjectGUIDValid || m_bProjectNameValid)
    {
        CCEStateManager *pStateManager;

        pStateManager = CEGetStateManager();
        if(pStateManager != NULL)
        {
            pStateManager->TriggerDownloadInfo(this);
        }
    }

    return(S_OK);
}

HRESULT CTargetOnl::SubscribeDownloadInfoStringVariable(const CString &strName, CSubscription *&pSubscription, bool &bValid, CString &strValue)
{
    HRESULT hr;
    CComVariant varValue;

    strValue = "";

    hr = SubscribeDownloadInfoVariable(strName, pSubscription, bValid, varValue);
    if(FAILED(hr))
        return(hr);
    if(hr == S_FALSE)
        return(S_OK);

    ASSERT(varValue.vt == VT_BSTR);
    if(varValue.vt == VT_BSTR)
    {
        strValue = varValue.bstrVal;
    }

    return(S_OK);
}

HRESULT CTargetOnl::SubscribeDownloadInfoVariable(const CString &strName, CSubscription *&pSubscription, bool &bValid, CComVariant &varValue)
{
    HRESULT hr;
    short sQuality;

    bValid = false;

    hr = SubscribeVariable(strName, &m_DownloadInfoCB, pSubscription);
    if(FAILED(hr))
        return(hr);
    ASSERT(pSubscription != NULL);
    if(pSubscription == NULL)
        return(E_FAIL);


    // Under certain circumstances (control service not running)
    // we will get no initial notification. For these cases we
    // read synchroneously here.

    hr = ReadVariable(pSubscription, sQuality, varValue);

    // Variable is subscribed. Don't care if initial read fails
    // because hopefully later there will be notifications.

    if(FAILED(hr))
        return(S_FALSE);

    if((sQuality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
        return(S_FALSE);

    bValid = true;

    return(S_OK);
}

HRESULT CTargetOnl::UnsubscribeForDownloadInfo()
{
    HRESULT hr;
    bool bNotify;

    bNotify = m_bDownloadGUIDValid || m_bDownloadDateValid || m_bDownloadHostValid;

    if(m_pDownloadGUIDSubscription != NULL)
    {
        hr = UnsubscribeVariable(m_pDownloadGUIDSubscription, &m_DownloadInfoCB);

        m_pDownloadGUIDSubscription = NULL;
        m_bDownloadGUIDValid = false;
        m_strDownloadGUID = "";
    }

    if(m_pDownloadDateSubscription != NULL)
    {
        hr = UnsubscribeVariable(m_pDownloadDateSubscription, &m_DownloadInfoCB);

        m_pDownloadDateSubscription = NULL;
        m_bDownloadDateValid = false;
        m_strDownloadDate = "";
    }

    if(m_pDownloadHostSubscription != NULL)
    {
        hr = UnsubscribeVariable(m_pDownloadHostSubscription, &m_DownloadInfoCB);

        m_pDownloadHostSubscription = NULL;
        m_bDownloadHostValid = false;
        m_strDownloadHost = "";
    }

    if(m_pProjectGUIDSubscription != NULL)
    {
        hr = UnsubscribeVariable(m_pProjectGUIDSubscription, &m_DownloadInfoCB);

        m_pProjectGUIDSubscription = NULL;
        m_bProjectGUIDValid = false;
        m_strProjectGUID = "";
    }

    if(m_pProjectNameSubscription != NULL)
    {
        hr = UnsubscribeVariable(m_pProjectNameSubscription, &m_DownloadInfoCB);

        m_pProjectNameSubscription = NULL;
        m_bProjectNameValid = false;
        m_strProjectName = "";
    }

    if(bNotify)
    {
        CCEStateManager *pStateManager;

        pStateManager = CEGetStateManager();
        if(pStateManager != NULL)
        {
            pStateManager->TriggerDownloadInfo(this);
        }
    }

    return(S_OK);
}

void CTargetOnl::OnDownloadInfoChanged(CSubscription *pSubscription)
{
    CComVariant varValue;
    bool bNotify;

   TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OnDownloadInfoChanged : (TargetId: %s)  \n"), 
                                        m_strId);


    ASSERT(pSubscription != NULL);
    if(pSubscription == NULL)
        return;

    if(pSubscription == m_pDownloadGUIDSubscription)
    {
        if((pSubscription->GetQuality() & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
        {
            bNotify = m_bDownloadGUIDValid;

            m_bDownloadGUIDValid = false;
            m_strDownloadGUID = "";
        }
        else
        {
            varValue = pSubscription->GetVariantValue();
            ASSERT(varValue.vt == VT_BSTR);
            if(varValue.vt != VT_BSTR)
                return;

            bNotify = !m_bDownloadGUIDValid;

            m_bDownloadGUIDValid = true;
            m_strDownloadGUID = varValue.bstrVal;
        }
    }
    else if(pSubscription == m_pDownloadDateSubscription)
    {
        if((pSubscription->GetQuality() & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
        {
            bNotify = m_bDownloadDateValid;

            m_bDownloadDateValid = false;
            m_strDownloadDate = "";
        }
        else
        {
            varValue = pSubscription->GetVariantValue();
            ASSERT(varValue.vt == VT_BSTR);
            if(varValue.vt != VT_BSTR)
                return;

            bNotify = !m_bDownloadDateValid;

            m_bDownloadDateValid = true;
            m_strDownloadDate = varValue.bstrVal;
        }
    }
    else if(pSubscription == m_pDownloadHostSubscription)
    {
        if((pSubscription->GetQuality() & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
        {
            bNotify = m_bDownloadHostValid;

            m_bDownloadHostValid = false;
            m_strDownloadHost = "";
        }
        else
        {
            varValue = pSubscription->GetVariantValue();
            ASSERT(varValue.vt == VT_BSTR);
            if(varValue.vt != VT_BSTR)
                return;

            bNotify = !m_bDownloadHostValid;

            m_bDownloadHostValid = true;
            m_strDownloadHost = varValue.bstrVal;
        }
    }
    else if(pSubscription == m_pProjectGUIDSubscription)
    {
        if((pSubscription->GetQuality() & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
        {
            bNotify = m_bProjectGUIDValid;

            m_bProjectGUIDValid = false;
            m_strProjectGUID = "";
        }
        else
        {
            varValue = pSubscription->GetVariantValue();
            ASSERT(varValue.vt == VT_BSTR);
            if(varValue.vt != VT_BSTR)
                return;

            bNotify = !m_bProjectGUIDValid;

            m_bProjectGUIDValid = true;
            m_strProjectGUID = varValue.bstrVal;
        }
    }
    else if(pSubscription == m_pProjectNameSubscription)
    {
        if((pSubscription->GetQuality() & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
        {
            bNotify = m_bProjectNameValid;

            m_bProjectNameValid = false;
            m_strProjectName = "";
        }
        else
        {
            varValue = pSubscription->GetVariantValue();
            ASSERT(varValue.vt == VT_BSTR);
            if(varValue.vt != VT_BSTR)
                return;

            bNotify = !m_bProjectNameValid;

            m_bProjectNameValid = true;
            m_strProjectName = varValue.bstrVal;
        }
    }

    if(bNotify)
    {
        CCEStateManager *pStateManager;

        pStateManager = CEGetStateManager();
        if(pStateManager != NULL)
        {
            pStateManager->TriggerDownloadInfo(this);
        }
    }
}


CComPtr<IBreakPoint> CTargetOnl::GetBreakpointManager()
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IBreakPoint> pIBreakPoint;

    pUnknown.Attach(CEGetCOM(CE_COMPMAN_BREAKPTMAN, false));
    if(pUnknown == NULL)
    {
        return(NULL);
    }

    hr = pUnknown->QueryInterface(IID_IBreakPoint,(LPVOID *)&pIBreakPoint);
    ASSERT(SUCCEEDED(hr));
    if(FAILED(hr))
    {
        return(NULL);
    }

    return(pIBreakPoint);
}


void CTargetOnl::OnExclusiveSessionNotifyCB(CSC_SESSION_STATE sessionState, HRESULT hrError)
{
	POSITION pPosition;

    // Set state before executing callbacks. Some clients
    // may read the state from TargetOnl rather than using the
    // passed parameter.
     TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::OnExclusiveSessionNotifyCB : (TargetId: %s)  sessionState: %s; hrError: 0x%08X \n"), 
                                        m_strId, GetSessionStateText(sessionState), hrError);


    m_ExclusiveHrError = hrError;

	m_ExclusiveSessionState = sessionState;
    GetNewConnectState();

	if (sessionState == exclusive_established)
	{
		CString strPassword = UTIL_GetMasterPassword ();
		SetPassword (strPassword);
        SubscribeForDownloadInfo();         // resubscribe download info if needed
	}

	pPosition = m_ExclusiveSessionCBs.GetHeadPosition ();
	while (pPosition != NULL)
	{
		FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB) *pCallback;

		pCallback = m_ExclusiveSessionCBs.GetNext(pPosition);
		assert (pCallback != NULL);
		if (pCallback == NULL)
			continue;

		pCallback->Execute (this, sessionState, hrError);
	}
}


//------------------------------------------------------------------*
/**
 * on breakpoint notify.
 *
 * @param           [in] crstrTaskName: task name (not used up to now)
 * @param           [in] crstrInstName: instance name (not used up to now)
 * @param           [in] bBPReached: breakpoint reached?
 * @return          -
 * @exception       -
 * @see             -
*/
void CTargetOnl::OnBreakpointNotify(const CString& crstrTaskName, const CString& crstrInstName, BOOL bBPReached)
{
    if(bBPReached)
    {
        m_bTaskInBreakPoint = TRUE;
    }
    else
    {
        m_bTaskInBreakPoint = FALSE;
    }
}

//------------------------------------------------------------------*
/**
 * is task in breakpoint.
 *
 * @param           -
 * @return          is task in breakpoint?
 * @exception       -
 * @see             -
*/
BOOL CTargetOnl::IsTaskInBreakpoint()
{
    return m_bTaskInBreakPoint;
}

//------------------------------------------------------------------*
/**
 * calculate connect state.
 *
 *  the base class version only takes the variable connection and the
 *  resource state into account
 *
 * @return          target online connect state
 * @see             CTargetOnl::CalculateConnectState()
*/
E_TargetOnlConnectState CTargetOnl::CalculateConnectState()
{
    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnl::CalculateConnectState : (TargetId: %s) \n"), 
                                        m_strId);


    E_TargetOnlConnectState eConnStateBase = CTargetOnlBase::CalculateConnectState();
    E_TargetOnlConnectState eConnState = eConnStateBase;

    switch(eConnStateBase)
    {
    // in these cases dont override base class
    case eTOCS_exception:
    case eTOCS_csc_con_error:
    case eTOCS_not_connected:
        break;
    case eTOCS_connected:
        switch(m_ExclusiveSessionState)
        {
        case exclusive_established:
            eConnState = eTOCS_connected_ex;
            break;
        case interrupted:
            eConnState = eTOCS_csc_con_ex_error;
            break;
        }
        break;
    case eTOCS_targ_con_error:
        switch(m_ExclusiveSessionState)
        {
        case exclusive_established:
            eConnState = eTOCS_targ_con_ex_error;
            break;
        case interrupted:
            eConnState = eTOCS_csc_con_ex_error;
            break;
        }
        break;
    }
    return eConnState;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// CExclusiveSessionNotify
CExclusiveSessionNotify::CExclusiveSessionNotify() 
{
	m_pTargetOnl = NULL;
}

CExclusiveSessionNotify::~CExclusiveSessionNotify()
{
	if(m_pTargetOnl) 
    {
		m_pTargetOnl = NULL;
	}
}

STDMETHODIMP_(HRESULT) CExclusiveSessionNotify::OnChanged()
{
    return S_OK;
}

STDMETHODIMP_(HRESULT) CExclusiveSessionNotify::OnStateChanged(/*[in]*/ LPUNKNOWN pSession, /*[in]*/ CSC_SESSION_STATE newState,/*[in]*/ HRESULT hrError) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState())

	AddRef();

    TrcPrint(TRC_COMM_EVENT, _T("CExclusiveSessionNotify::OnStateChanged : (TargetId: %s)  Connect state: %d hr = 0x%08X \n"), 
                                        m_pTargetOnl->GetId() ,newState,hrError);


	FC_DEBUGPRINT2(_T("CETARGETONL> Connect state: %d hr = 0x%x\n"),newState,hrError);
    
	if(!m_pTargetOnl) 
    {
        Release();
		return E_FAIL;
    }

    if(newState != m_pTargetOnl->m_ExclusiveSessionState || hrError != m_pTargetOnl->m_ExclusiveHrError)
    {
        m_pTargetOnl->OnExclusiveSessionNotifyCB(newState, hrError);
    }

    Release();
    return S_OK;
}


// CTransferNotify
CTransferNotify::CTransferNotify()
: m_pTarget(NULL)
{
}


CTransferNotify::~CTransferNotify()
{
}


void CTransferNotify::SetTarget(CTargetOnl *pTarget)
{
    m_pTarget = pTarget;
}


STDMETHODIMP CTransferNotify::OnUserLoggedIn(BSTR sUserName, BSTR sHostName, BOOL bLoggedIn)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState())

    if(m_pTarget != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTransferNotify::OnUserLoggedIn : (TargetId: %s) User: %s  -  Machine: %s  -  Login: %s\n"), 
                                        m_pTarget->GetId(), (CString)sUserName, (CString)sHostName, bLoggedIn ? "TRUE" : "FALSE");

        TRACE("********** Login : User: %s  -  Machine: %s  -  Login: %s\n", 
            (CString)sUserName, (CString)sHostName, bLoggedIn ? "TRUE" : "FALSE");
        m_pTarget->OnUserLoggedIn(sUserName, sHostName, bLoggedIn == TRUE);
    };

    return(S_OK);
}


STDMETHODIMP CTransferNotify::OnTransferStateChanged(DWORD dwCookie, BSTR sCurrentDomainName, CSC_TRANSFER_STATE state, float fltPercentDone)
{
    return(S_OK);
}


STDMETHODIMP CTransferNotify::OnTransferError(DWORD dwCookie, BSTR sCurrentDomainName, CSC_TRANSFER_STATE state, HRESULT hrError)
{
    return(S_OK);
}


// Implemented breakpoint notification interface just for
// some test. Left it in here so anybody who needs it may
// use it. It is ready to use, i.e. already advised to
// CSC_Online.

// CBreakpointNotify
CBreakpointNotify::CBreakpointNotify()
: m_pTarget(NULL)
{
}


CBreakpointNotify::~CBreakpointNotify()
{
}


void CBreakpointNotify::SetTarget(CTargetOnl *pTarget)
{
    m_pTarget = pTarget;
}


STDMETHODIMP_ (HRESULT) CBreakpointNotify::OnBreakpointReached(BSTR sInstanceName, long lBreakpointNumber,
                                                             long bpQuality, BSTR sTaskName)
{
    if(m_pTarget)
    {
        CString strTaskName(sTaskName);
        CString strInstanceName(sInstanceName);
        BOOL    bReached = (bpQuality == 0) ? TRUE : FALSE;
        m_pTarget->OnBreakpointNotify(strTaskName, strInstanceName, bReached);
    }
    return S_OK;
}


STDMETHODIMP_ (HRESULT) CBreakpointNotify::OnConfigurationChanged(BSTR sAddress)
{
    return S_OK;
}


