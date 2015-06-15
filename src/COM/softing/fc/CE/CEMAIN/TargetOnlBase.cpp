


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "TargetOnlBase.h"
#include "CECompMan.h"
#include "CEStateMan.h"

#include "CESysdef.h"
#include "XMLHelper/XMLNode.h"
#include "CEProjMan.h"
#include "toolbox.h"
#include "fc_todebug/fc_assert.h"
#include "utilif.h"
#include "resource.h"
#include "KADHelper/KADTargetInfo.h"
#include "ConnectDlg.h"
#include "CEProjInfo.h"
#include "CECompMan.h"
#include "CEAddonAction.h"

#include "CESysDef.h"
#include "traceif.h"

#include "vmSharedDef.h"

//----  Static Initializations:   ----------------------------------*
CSubscriptionManager* CTargetOnlBase::m_pSubscriptionManager;
unsigned int CTargetOnlBase::m_uiSMLockCount;

CTargetOnlBase::CTargetOnlBase
(
    const CString& crstrTargetId /*= ""*/,
    const CString& crstrInstIdPath /*= ""*/,
    const CString& crstrType /*=""*/,
    const CString& crstrAssignedResource /*=""*/
)
: m_bConnected(FALSE),
  m_SessionState(not_connected),
  m_SessionNotifyCB(this, OnSessionNotifyCB),
  m_dwSessionCookie(0),
  m_pSessionNotify(NULL),
  m_pStateSubscription(0),
  m_StateCB(this, OnResourceStateChanged),
  m_pProjectGUIDSubscription(NULL),
  m_bProjectGUIDValid(false),
  m_pProjectNameSubscription(NULL),
  m_bProjectNameValid(false),
  m_strId(crstrTargetId),
  m_strIdPath(crstrInstIdPath),
  m_strType(crstrType),
  m_strAssignedResource(crstrAssignedResource),
  m_eConnectState(eTOCS_not_connected),
  m_eOldConnectState(eTOCS_not_connected),
  m_eResourceState(eRS_on_creation),
  m_eOldResourceState(eRS_on_creation),
  m_strUserName(),
  m_strPassword(),
  m_bWaitForTargetConnection(TRUE)
{
    Init();
    LockSubscriptionManager();
}

CTargetOnlBase::~CTargetOnlBase()
{
	if(m_bConnected)
	{
		Disconnect();
	}

	if(m_pIFCVarSession != NULL)
	{
		CloseVarSession();
	}

    UnlockSubscriptionManager();
}


//------------------------------------------------------------------*
/**
 * set id of target.
*/
void CTargetOnlBase::SetId(const CString& crstrId)
{
    m_strId = crstrId;
}

//------------------------------------------------------------------*
/**
 * set id path of target in instance view.
 *
 *  is used to get node
*/
void CTargetOnlBase::SetIdPath(const CString& crstrInstIdPath)
{
    m_strIdPath = crstrInstIdPath;
}

//------------------------------------------------------------------*
/**
 * set control address.
 *
*/
void CTargetOnlBase::SetControlAddress(const CString& crstrAddress)
{
    m_strControlAddress = crstrAddress;
}

//------------------------------------------------------------------*
/**
 * set control address shown to user.
 *
*/
void CTargetOnlBase::SetUserControlAddress(const CString& crstrUserAddress)
{
    m_strUserControlAddress = crstrUserAddress;
}

//------------------------------------------------------------------*
/**
 * set control address.
 *
*/
void CTargetOnlBase::SetAssignedResource(const CString& crstrAssignedResource)
{
    m_strAssignedResource = crstrAssignedResource;
}

//------------------------------------------------------------------*
/**
 * set wait for target connection.
 *  for OSAI Target waitForTargetConnection will be FALSE. The connect dialog
 *  will not wait until the opc server has established a connection to the
 *  target itself. Opnly the connection to the OPC server has to be established.
 *  For the OSAI target thos is enough to do a download (only file copy over the
 *  network.)
 *
*/
void CTargetOnlBase::SetWaitForTargetConnection(const BOOL bWaitForTargetConnection)
{
    m_bWaitForTargetConnection = bWaitForTargetConnection;
}



//------------------------------------------------------------------*
/**
 * get id of target.
 * @see             SetId()
*/
CString CTargetOnlBase::GetId()
{
    return(m_strId);
}

//------------------------------------------------------------------*
/**
 * get id path of target node in instance tree.
 * @see             SetIdPath()
*/
CString CTargetOnlBase::GetIdPath()
{
    return(m_strIdPath);
}



//------------------------------------------------------------------*
/**
 * get type of target instance as specified in the 4CE KAD.
 * @return	CString - type identifier from 4CE KAD.
 * @see		4CE KAD description
*/
CString CTargetOnlBase::GetType()
{
    return(m_strType);
}

//------------------------------------------------------------------*
/**
 * get control address.
 *
 * @return          control address
 *
*/
CString CTargetOnlBase::GetControlAddress()
{
    return(m_strControlAddress);
}

//------------------------------------------------------------------*
/**
 * get control address shown to user.
 *
 * @return          user control address
 *
*/
CString CTargetOnlBase::GetUserControlAddress()
{
    return(m_strUserControlAddress);
}

//------------------------------------------------------------------*
/**
 * get assigned resource.
 *
 * @return          name of assigned resource
 *
*/
CString CTargetOnlBase::GetAssignedResource()
{
    return(m_strAssignedResource);
}

//------------------------------------------------------------------*
/**
 * initialization.
 *
 *  get user name from system
 *  load KAD info
 *
 * @return          successful?
*/
BOOL CTargetOnlBase::Init()
{
    // determine user name from system
    DWORD   dwLength = _MAX_PATH + 1;
    CString strTemp;

    ::WNetGetUser(NULL, strTemp.GetBuffer(_MAX_PATH + 1), &dwLength);
    strTemp.ReleaseBuffer();
    m_strUserName = strTemp;
    m_strPassword = UTIL_GetMasterPassword();

    if(!LoadKADInfo())
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * connect to target.
 *
 * @return          return codes:   S_OK
 *                                  E_CE_TARGETONL_ALREADYCONNECTED
 *                                  E_CE_TARGETONL_CONNECTFAILED
 * @see             
*/
HRESULT CTargetOnlBase::Connect()
{
	HRESULT hr;
    CCEStateManager *pStateManager;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Connect : TargetId: %s ; Address: %s\n"), m_strId, m_strControlAddress);

    if(m_bConnected)
    {
        return(MakeHResult(E_CE_TARGETONL_ALREADYCONNECTED));
    }

    DoPreConnectStep();

    pStateManager = CEGetStateManager();

    m_ConnectActionCBs.Execute(this, TargetPreConnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Connect : (TargetId: %s) TriggerTargetConnect : CEPreConnect \n"), m_strId);
        pStateManager->TriggerTargetConnect(this, CEPreConnect);
    }

	hr = OpenVarSession();
	if(FAILED(hr))
	{
        m_ConnectActionCBs.Execute(this, TargetConnectError);
        if(pStateManager != NULL)
        {
            pStateManager->TriggerTargetConnect(this, CEConnectError);
        }

		return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
	}


	{	//do it in a block to get hourglass destructed
		//at the end of the block.
		CHourglass hourglass;
        CComQIPtr<IFCConnect> pIFCConnect = m_pIFCVarSession;
        if(pIFCConnect)
        {
            CComBSTR sUserName = m_strUserName;
            CComBSTR sPassword = m_strPassword;
		    hr = pIFCConnect->Connect(sUserName,sPassword);
        }
	}

    if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Connect : (TargetId: %s) IFCConnect->Connect returned hr=0x%08X!\n"), m_strId, hr);

        m_pIFCVarSession = NULL;
        m_bConnected = FALSE;
        m_strControlAddressConn.Empty();

        m_ConnectActionCBs.Execute(this, TargetConnectError);
        if(pStateManager != NULL)
        {
            pStateManager->TriggerTargetConnect(this, CEConnectError);
        }

        return(MakeHResult(E_CE_TARGETONL_CONNECTFAILED));
    }

    m_bConnected = TRUE;
    m_strControlAddressConn = m_strControlAddress;

    m_ConnectActionCBs.Execute(this, TargetPostConnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Connect : (TargetId: %s) TriggerTargetConnect : CEPostConnect \n"), m_strId);
        pStateManager->TriggerTargetConnect(this, CEPostConnect);
    }

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * do pre connect step.
 *
 * @param           -
 * @return          proceed connecting?
 * @exception       -
 * @see             -
*/
BOOL CTargetOnlBase::DoPreConnectStep()
{
    GUID    sGuid;
    if(!m_tKADTargetData.GetPreConnectStepGuid(sGuid))
    {
        return TRUE;
    }

    HRESULT                 hr;
    CGuid                   tGuid(sGuid);
    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEProjInfo>  pProjInfo;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    if(!pProjInfo)
    {
        ASSERT(!"get project info IF failed");
        return TRUE;
    }
    pTmp.Release();

    CComBSTR    sSourceFile;
    CComBSTR    sIdPath(m_strIdPath);

    hr = pProjInfo->getFileNameFromIdPath(sIdPath, &sSourceFile);
    if(hr != S_OK)
    {
        ASSERT(!"could not get file name");
        return TRUE;
    }

    CComQIPtr<ICEAddonAction>   pAddOnIF;
    CString                     strGuid = tGuid;

    pTmp.Attach(CEGetCOMByCLSID(strGuid, true));

    pAddOnIF = pTmp;
    if(!pAddOnIF)
    {
        return TRUE;
    }

    // call do action
    CComBSTR    sMethodName(_T("PreConnectStep"));
    CComBSTR    sAdditionalInfo;

    hr = pAddOnIF->doAction(sMethodName, sSourceFile, sIdPath, pProjInfo, sAdditionalInfo);
    if(hr == S_OK)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * connect to target exclusively.
 *
 *  base class just returns S_FALSE
 *
 * @return          return codes:   S_FALSE
 * @see             
*/
HRESULT CTargetOnlBase::ConnectExclusive()
{
    return S_FALSE;
}

//------------------------------------------------------------------*
/**
 * disconnect target exclusively.
 *
 *  base class just returns S_FALSE
 *
 * @return          return codes:   S_FALSE
 * @see             
*/
HRESULT CTargetOnlBase::DisconnectExclusive(BOOL bForce /*=FALSE*/)
{
    return S_FALSE;
}

//------------------------------------------------------------------*
/**
 * disconnect from target.
 *
 * @return           return codes:  S_OK
 *                                  E_CE_TARGETONL_NOTCONNECTED
 *                                  E_CE_TARGETONL_DISCONNECTFAILED
 * @see             
*/
HRESULT CTargetOnlBase::Disconnect()
{
    HRESULT hr;
    CCEStateManager *pStateManager;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Disconnect : TargetId: %s ; Address: %s\n"), m_strId, m_strControlAddress);

    if(!m_bConnected)
    {
        return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    UnsubscribeForState();

    pStateManager = CEGetStateManager();

    m_ConnectActionCBs.Execute(this, TargetPreDisconnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Disconnect : (TargetId: %s) TriggerTargetConnect : CEPreDisconnect \n"), m_strId);
        pStateManager->TriggerTargetConnect(this, CEPreDisconnect);
    }

	ASSERT(m_pIFCVarSession != NULL);
    if(m_pIFCVarSession != NULL)
    {
        CComQIPtr<IFCConnect>   pIFCConnect = m_pIFCVarSession;
        if(pIFCConnect)
        {
	        hr = pIFCConnect->Disconnect();
            ASSERT(SUCCEEDED(hr));
            if(FAILED(hr))
            {
                TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Disconnect : (TargetId: %s) IFCConnect->Disconnect returned hr=0x%08X!\n"), m_strId, hr);

                m_ConnectActionCBs.Execute(this, TargetDisconnectError);
                if(pStateManager != NULL)
                {
                    pStateManager->TriggerTargetConnect(this, CEDisconnectError);
                }

                return(MakeHResult(E_CE_TARGETONL_DISCONNECTFAILED));
            }
        }

        CloseVarSession();
    }

    m_bConnected = FALSE;
    m_strControlAddressConn.Empty();

    m_ConnectActionCBs.Execute(this, TargetPostDisconnect);
    if(pStateManager != NULL)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::Disconnect : (TargetId: %s) TriggerTargetConnect : CEPostDisconnect \n"), m_strId);
        pStateManager->TriggerTargetConnect(this, CEPostDisconnect);
    }

    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * is connected.
 *
 * @return          is target connected?
 * @see             
*/
BOOL    CTargetOnlBase::IsConnected()
{
    return(m_bConnected);
}

//------------------------------------------------------------------*
/**
 * is connected exclusively?.
 *
 * @return          since target is not able to connected exclusively
 *                  return FALSE
*/
BOOL    CTargetOnlBase::IsConnectedExclusive()
{
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * is target connected.
 *
 *  is target connection OK
 *
 * @param           -
 * @return          is target connected?
 * @exception       -
 * @see             IsConnected()
*/
BOOL    CTargetOnlBase::IsTargetConnected()
{
    BOOL    bConnected = m_bConnected && (m_eConnectState >= eTOCS_connected);
    return bConnected;
}

//------------------------------------------------------------------*
/**
 * is target connected exclusive?
 *
 *  base class returns FALSE.
 *
 * @param           -
 * @return          FALSE.
 * @exception       -
 * @see             -
*/
BOOL    CTargetOnlBase::IsTargetConnectedExclusive()
{
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get session state.
 *
 * @return          session state.
 * @see             
*/
CSC_SESSION_STATE CTargetOnlBase::GetSessionState()
{
	return(m_SessionState);
}

//------------------------------------------------------------------*
/**
 * get exclusive session state.
 *
 *  since cannot connect exclusive, return 0 (not_connected)
 *
*/
CSC_SESSION_STATE CTargetOnlBase::GetExclusiveSessionState()
{
    return not_connected;
}



//------------------------------------------------------------------*
/**
 * subscribe variable.
 *
 * @param           [in] strVariableName: name of variable
 * @param           [in] pModifiedCB: callback function to be called when var changes
 * @param           [out] pSubscription: pointer to subscription object
 * @return          return codes:   S_OK
 *                                  E_FAIL
 *                                  E_UNEXPECTED
*/
HRESULT CTargetOnlBase::SubscribeVariable(const CString &strVariableName, FC_CALLBACK_BASE(SubscriptionModifiedCB) *pModifiedCB, CSubscription *&pSubscription)
{
    HRESULT hr;
    CString strAddress;
    LPTSTR pszAddress;
    CString strCompleteName;

    pSubscription = NULL;

    if(m_pSubscriptionManager == NULL)
    {
        return(E_FAIL);
    }

    strAddress = m_strControlAddress;
    pszAddress = strAddress.GetBuffer(strAddress.GetLength() + 20);
    hr = UTIL_MakeMachineName(pszAddress, strAddress.GetLength() + 20);
    if(FAILED(hr))
    {
        return(hr);
    }

    strCompleteName.Format("%s.%s", pszAddress,(LPCTSTR)strVariableName);

    hr = m_pSubscriptionManager->GetSubscription(strCompleteName, BL_ANY, pSubscription);
    if(FAILED(hr))
    {
        return(hr);
    }
    ASSERT(pSubscription != NULL);
    if(pSubscription == NULL)
    {
        return(E_UNEXPECTED);
    }

    pSubscription->RegisterModifiedCB(pModifiedCB);

    return(S_OK);
}

HRESULT CTargetOnlBase::UnsubscribeVariable(CSubscription *pSubscription, FC_CALLBACK_BASE(SubscriptionModifiedCB) *pModifiedCB)
{
    HRESULT hr;

    ASSERT(pSubscription != NULL);
    if(pSubscription == NULL)
    {
        return(E_INVALIDARG);
    }

	pSubscription->UnregisterModifiedCB(pModifiedCB);

    hr = pSubscription->Unreference();
    ASSERT(SUCCEEDED(hr));

    return(S_OK);
}

HRESULT CTargetOnlBase::ReadVariable(CSubscription *pSubscription, short &sQuality, CComVariant &varValue)
{
    ASSERT(pSubscription != NULL);
    if(pSubscription == NULL)
    {
        return(E_INVALIDARG);
    }

	if(m_pIFCVarSession == NULL)
	{
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
	}

    pSubscription->Read();

    varValue = pSubscription->GetVariantValue();
    sQuality = pSubscription->GetQuality();

	return(S_OK);
}

HRESULT CTargetOnlBase::GetCurrentUser(CString& rstrUser)
{
    HRESULT hr;
    CComQIPtr<IFCConnect> pIFCConnect = m_pIFCVarSession;
    if(pIFCConnect)
    {
        CComBSTR    bstrTmp;
		hr = pIFCConnect->get_CurrentUser(&bstrTmp);
        rstrUser = bstrTmp;
        return S_OK;
    }
    return E_FAIL;
}

HRESULT CTargetOnlBase::GetMachineName(CString& rstrMachineName)
{
    HRESULT hr;
    CComQIPtr<IFCConnect> pIFCConnect = m_pIFCVarSession;
    if(pIFCConnect)
    {
        CComBSTR    bstrTmp;
		hr = pIFCConnect->get_MachineName(&bstrTmp);
        rstrMachineName = bstrTmp;
        return S_OK;
    }
    return E_FAIL;
}


HRESULT CTargetOnlBase::StartResource()
{
	HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CString strHelp;
    strHelp.Format(IDS_RUNCONTROL, m_strAssignedResource);
    if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO))
    {
        return S_OK;
    }

    assert(m_pIFCVarSession);

	hr = m_pIFCVarSession->StartResource();
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_STARTRESOURCE);
    }
	if(FAILED(hr))
    {
        return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}


HRESULT CTargetOnlBase::StopResource()
{
	HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }
    // Currently (12.7.99) resource state variable has
    // to be resubscribed after clearing the resource.
    // This behaviour is not implemented => m_eResourceState not
    // reliable.
#if 0
    if(m_eResourceState != eRS_running)
    { //not started return
        return S_OK;
    }
#endif

    CString strHelp;
    strHelp.Format(IDS_STOPCONTROL, m_strAssignedResource);
    if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO))
    {
        return S_OK;
    }

    assert(m_pIFCVarSession);
	hr = m_pIFCVarSession->StopResource();
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_STOPRESOURCE);
    }
    if(FAILED(hr))
    {
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}


HRESULT CTargetOnlBase::ColdStartResource()
{
	HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CString strHelp;
    strHelp.Format(IDS_COLDSTART, m_strAssignedResource);
    if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO))
    {
        return S_OK;
    }

    assert(m_pIFCVarSession);
	hr = m_pIFCVarSession->ColdstartResource();
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_COLDSTART);
    }
    if(FAILED(hr))
    {
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}


HRESULT CTargetOnlBase::WarmStartResource()
{
	HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CString strHelp;
    strHelp.Format(IDS_WARMSTART, m_strAssignedResource);
    if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO))
    {
        return S_OK;
    }
    assert(m_pIFCVarSession);
	hr = m_pIFCVarSession->WarmstartResource();
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_WARMSTART);
    }
    if(FAILED(hr))
    {
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}


// task actions
HRESULT CTargetOnlBase::StartTask(LPCTSTR pszTask)
{
	HRESULT hr;
    BSTR szTask;
	CString strTask = pszTask;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }
    assert(m_pIFCVarSession);
	szTask = strTask.AllocSysString();
	hr = m_pIFCVarSession->StartTaskOnResource(szTask);
	::SysFreeString(szTask);
	
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_STARTTASK);
    }
	if(FAILED(hr))
    {
        return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}

HRESULT CTargetOnlBase::StopTask(LPCTSTR pszTask)
{
	HRESULT hr;
    BSTR szTask;
	CString strTask = pszTask;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    // TODO: RTCE-task: can be stopped?
//    BOOL bRTCETask = FALSE;
//    IsRTCETask(strTask);
//    if(SUCCEEDED(hr) && bRTCETask)
//    {
//        AfxMessageBox(IDS_NOSTOP_RTCETASK, MB_ICONEXCLAMATION | MB_OK);
//        return(S_FALSE);
//    }

    assert(m_pIFCVarSession);
	szTask = strTask.AllocSysString();
	hr = m_pIFCVarSession->StopTaskOnResource(szTask);
	::SysFreeString(szTask);
	
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_STOPTASK);
    }
	if(FAILED(hr))
    {
        return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}


HRESULT CTargetOnlBase::StartAllTasks()
{
	HRESULT hr;

    if(!m_bConnected) 
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CString strHelp;
    strHelp.Format(IDS_STARTALLTASKS, m_strAssignedResource);
    if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO))
    {
        return S_OK;
    }
    assert(m_pIFCVarSession);
	hr = m_pIFCVarSession->StartAllTasksOnResource();
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_STARTALLTASKS);
    }
    if(FAILED(hr))
    {
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}


HRESULT CTargetOnlBase::StopAllTasks()
{
	HRESULT hr;
    BOOL bRTCETask = FALSE;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

// TODO: stopping of RTCE-tasks
//    CMainFrame *pFrame;
//    CPrjDoc *pProject;
//
//    pFrame = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
//    ASSERT(NULL != pFrame);
//    if(NULL == pFrame)
//    {
//        return(S_FALSE);
//    }
//    pProject = pFrame->GetProjectDocument();
//    ASSERT(NULL != pProject);
//    if(NULL == pProject)
//    {
//        return(S_FALSE);
//    }
//
//    hr = pProject->HasPCAnyRTCETasks(m_strAssignedResource, bRTCETask);
//    if(SUCCEEDED(hr) && bRTCETask)
//    {
//        if(IDYES != AfxMessageBox(IDS_NOSTOP_ALLRTCETASKS, MB_ICONEXCLAMATION | MB_YESNO))
//        {
//            return(S_FALSE);
//        }
//    }
//    else
//    {
        CString strHelp;
        strHelp.Format(IDS_STOPALLTASKS, m_strAssignedResource);
        if(IDYES != AfxMessageBox(strHelp, MB_ICONQUESTION | MB_YESNO)) 
        {
            return S_OK;
        }
//    }

    assert(m_pIFCVarSession);
	hr = m_pIFCVarSession->StopAllTasksOnResource();
    if(hr != S_OK)
    {
        AfxMessageBox(IDS_ERROR_STOPALLTASKS);
    }
    if(FAILED(hr)) 
    {
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
    return(S_OK);
}

// state queries
bool CTargetOnlBase::IsResourceStarted()
{
    return(m_eResourceState == eRS_running);
}

bool CTargetOnlBase::IsResourceStopped()
{
    return(m_eResourceState == eRS_stopped);
}

bool CTargetOnlBase::IsResourceError()
{
    return(m_eResourceState == eRS_general_error);
}

bool CTargetOnlBase::IsServiceStopped()
{
    return(m_eResourceState == eRS_service_stopped);
}

E_ResourceState CTargetOnlBase::GetResourceState()
{
    return(m_eResourceState);
}

// register / unregister callbacks
HRESULT CTargetOnlBase::RegisterSessionCB(FC_CALLBACK_BASE(TargetOnlSessionCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_SessionCBs.Find(pCallback);
	assert(pPosition == NULL);
	if(pPosition != NULL)
	{
		return(E_FAIL);
	}

	m_SessionCBs.AddTail(pCallback);

	return(S_OK);
}

HRESULT CTargetOnlBase::UnregisterSessionCB(FC_CALLBACK_BASE(TargetOnlSessionCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_SessionCBs.Find(pCallback);
	assert(pPosition != NULL);
	if(pPosition == NULL)
	{
		return(E_FAIL);
	}

	m_SessionCBs.RemoveAt(pPosition);

	return(S_OK);
}

HRESULT CTargetOnlBase::RegisterConnectActionCB(FC_CALLBACK_BASE(TargetOnlConnectActionCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ConnectActionCBs.Find(pCallback);
	assert(pPosition == NULL);
	if(pPosition != NULL)
	{
		return(E_FAIL);
	}

    m_ConnectActionCBs.AddTail(pCallback);

    return(S_OK);
}

HRESULT CTargetOnlBase::UnregisterConnectActionCB(FC_CALLBACK_BASE(TargetOnlConnectActionCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ConnectActionCBs.Find(pCallback);
	assert(pPosition != NULL);
	if(pPosition == NULL)
	{
		return(E_FAIL);
	}

	m_ConnectActionCBs.RemoveAt(pPosition);

	return(S_OK);
}


//------------------------------------------------------------------*
/**
 * register callback function for changes of connect state.
 *
 * @param           [in] pCallback: pointer to callback function
 * @return          S_OK,
 *                  E_FAIL
 * @see             UnregisterConnectStateCB()
*/
HRESULT CTargetOnlBase::RegisterConnectStateCB(FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ConnectStateCBs.Find(pCallback);
	assert(pPosition == NULL);
	if(pPosition != NULL)
	{
		return(E_FAIL);
	}

    m_ConnectStateCBs.AddTail(pCallback);

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * unregister callback function for changes of connect state.
 *
 * @param           [in] pCallback: pointer to callback function
 * @return          S_OK,
 *                  E_FAIL
 * @see             RegisterConnectStateCB()
*/
HRESULT CTargetOnlBase::UnregisterConnectStateCB (FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ConnectStateCBs.Find(pCallback);
	assert(pPosition != NULL);
	if(pPosition == NULL)
	{
		return(E_FAIL);
	}

	m_ConnectStateCBs.RemoveAt(pPosition);

	return(S_OK);
}

//------------------------------------------------------------------*
/**
 * register callback function for changes of resource state.
 *
 * @param           [in] pCallback: pointer to callback function
 * @return          S_OK,
 *                  E_FAIL
 * @see             UnregisterResourceStateCB()
*/
HRESULT CTargetOnlBase::RegisterResourceStateCB (FC_CALLBACK_BASE (TargetOnlResourceStateCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ResourceStateCBs.Find(pCallback);
	assert(pPosition == NULL);
	if(pPosition != NULL)
	{
		return(E_FAIL);
	}

    m_ResourceStateCBs.AddTail(pCallback);

    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * unregister callback function for changes of resource state.
 *
 * @param           [in] pCallback: pointer to callback function
 * @return          S_OK,
 *                  E_FAIL
 * @see             RegisterResourceStateCB()
*/
HRESULT CTargetOnlBase::UnregisterResourceStateCB (FC_CALLBACK_BASE (TargetOnlResourceStateCB) *pCallback)
{
	POSITION pPosition;

	pPosition = m_ResourceStateCBs.Find(pCallback);
	assert(pPosition != NULL);
	if(pPosition == NULL)
	{
		return(E_FAIL);
	}

	m_ResourceStateCBs.RemoveAt(pPosition);

	return(S_OK);
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
E_TargetOnlConnectState CTargetOnlBase::CalculateConnectState()
{
    E_TargetOnlConnectState eConnState = eTOCS_exception;

    switch(m_SessionState)
    {
    case not_connected:
    case waiting_for_connect:
    case regular_shutdown:
        eConnState = eTOCS_not_connected;
        break;
    case RW_established:
        // variable session OK
        if( m_eResourceState == eRS_not_connected ||
            m_eResourceState == eRS_service_stopped ||
            m_eResourceState == eRS_general_error)
        {
            // target connection not OK
            eConnState = eTOCS_targ_con_error;
        }
        else
        {
            // target connection OK
            eConnState = eTOCS_connected;
        }
        break;
    case interrupted:
        eConnState = eTOCS_csc_con_error;
        break;
    }
    return eConnState;
}

//------------------------------------------------------------------*
/**
 * get new connect state.
 *
 *  set new connect state using virtual CalculateConnectState() method.
 *  if value changes inform registered clients about change
 *
 * @see             CalculateConnectState(), ExecuteConnectChangeCallbacks()
*/
void CTargetOnlBase::GetNewConnectState()
{
    m_eConnectState = CalculateConnectState();

//    if(m_eConnectState != m_eOldConnectState)
//    {
        m_eOldConnectState = m_eConnectState;
        ExecuteConnectStateChangeCallbacks();
//    }
}


//------------------------------------------------------------------*
/**
 * get connect state text.
 *
 * @return          connect state text
*/
CString CTargetOnlBase::GetConnectStateText()
{
    CString strText;

    switch(m_eConnectState)
    {
    case eTOCS_exception:
        strText.LoadString(IDS_TOCS_EXCEPTION);
        break;
    case eTOCS_targ_con_ex_error:
        strText.LoadString(IDS_TOCS_TARG_CON_EX_ERR);
        break;
    case eTOCS_targ_con_error:
        strText.LoadString(IDS_TOCS_TARG_CON_ERR);
        break;
    case eTOCS_csc_con_ex_error:
        strText.LoadString(IDS_TOCS_CSC_CON_EX_ERR);
        break;
    case eTOCS_csc_con_error:
        strText.LoadString(IDS_TOCS_CSC_CON_ERR);
        break;
    case eTOCS_not_connected:
        strText.LoadString(IDS_TOCS_NOT_CON);
        break;
    case eTOCS_connected:
        strText.LoadString(IDS_TOCS_CON);
        break;
    case eTOCS_connected_ex:
        strText.LoadString(IDS_TOCS_CON_EX);
        break;
    }
    return(strText);
}

//------------------------------------------------------------------*
/**
 * get session state text.
 *
 *  helper function to generate text output for CSC_SESSION_STATE.
 *
 * @param           [in] state: session state
 * @return          description text
*/
CString CTargetOnlBase::GetSessionStateText(CSC_SESSION_STATE state)
{
    CString strText;

    switch(state)
    {
    case not_connected:
        strText.LoadString(IDS_CSC_NOT_CON);
        break;
    case waiting_for_connect:
        strText.LoadString(IDS_CSC_WAIT_CON);
        break;
    case RW_established:
        strText.LoadString(IDS_CSC_CON);
        break;
    case interrupted:
        strText.LoadString(IDS_CSC_INTERRUPTED);
        break;
    case waiting_for_exclusive:
        strText.LoadString(IDS_CSC_WAIT_CON_EX);
        break;
    case exclusive_established:
        strText.LoadString(IDS_CSC_CON_EX);
        break;
    case regular_shutdown:
        strText.LoadString(IDS_CSC_SHUTDOWN);
        break;
    }
    return(strText);
}

CString CTargetOnlBase::GetResourceStateText()
{
    CString strText;

    switch(m_eResourceState)
    {
    case eRS_not_connected:
        strText.LoadString(IDS_RS_NOT_CON);
        break;
    case eRS_version_conflict:
        strText.LoadString(IDS_RS_VERSION_CONFLICT);
        break;
    case eRS_service_stopped:
        strText.LoadString(IDS_RS_SERVICE_STOPPED);
        break;
    case eRS_general_error:
        strText.LoadString(IDS_RS_ERROR);
        break;
    case eRS_on_creation:   // not used -> no string
        break;
    case eRS_stopped:
        strText.LoadString(IDS_RS_STOPPED);
        break;
    case eRS_running:
        strText.LoadString(IDS_RS_RUNNING);
        break;
    }
    return(strText);
}


//------------------------------------------------------------------*
/**
 * get diagnosis text.
 *
 *  create a diagnosis text for the target built up by address, connect
 *  and resource info.
 *
 * @return          diagnosis text
*/
CString CTargetOnlBase::GetDiagnosisText()
{
    CString strUserConnect = GetUserControlAddress();
    CString strConnectState = GetConnectStateText();
    CString strResourceState = GetResourceStateText();
    CString strDiagnosis;

    if(!strResourceState.IsEmpty())
    {
        strResourceState = _T(" (") + strResourceState + _T(")");
    }
    strConnectState += strResourceState;
    if(!strConnectState.IsEmpty() && !strUserConnect.IsEmpty())
    {
        strConnectState = _T(", ") + strConnectState;
    }
    strDiagnosis = strUserConnect + strConnectState;
    return strDiagnosis;
}



//------------------------------------------------------------------*
/**
 * get target online connect state.
 *
 * @return          target online connect state
*/
E_TargetOnlConnectState CTargetOnlBase::GetConnectState()
{
    return m_eConnectState;
}


//------------------------------------------------------------------*
/**
 * execute connect state change callbacks.
 *
*/
void CTargetOnlBase::ExecuteConnectStateChangeCallbacks()
{
    POSITION    pPosition;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::ExecuteConnectStateChangeCallbacks : TargetId: %s ; ConnectState: %s\n"), m_strId, GetConnectStateText());

	pPosition = m_ConnectStateCBs.GetHeadPosition ();
	while (pPosition != NULL)
	{
		FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pCallback;

		pCallback = m_ConnectStateCBs.GetNext (pPosition);
		assert (pCallback != NULL);
		if (pCallback == NULL)
			continue;

		pCallback->Execute(this);
	}
}

//------------------------------------------------------------------*
/**
 * execute resource state change callbacks.
 *
*/
void CTargetOnlBase::ExecuteResourceConnectChangeCallbacks()
{
    POSITION    pPosition;

	pPosition = m_ResourceStateCBs.GetHeadPosition ();
	while (pPosition != NULL)
	{
		FC_CALLBACK_BASE (TargetOnlResourceStateCB) *pCallback;

		pCallback = m_ResourceStateCBs.GetNext (pPosition);
		assert (pCallback != NULL);
		if (pCallback == NULL)
			continue;

		pCallback->Execute(this);
	}
}



BOOL CTargetOnlBase::LoadKADInfo()
{
    CKADTargetData* pTargetData = NULL;

    CComPtr<IUnknown>    pTmp;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_KADMAN));

    CKADTargetInfo  tKADInfo;
    tKADInfo.Load(pTmp);
    pTargetData = tKADInfo.GetTargetDataForType(m_strType);
    if(pTargetData)
    {
        m_tKADTargetData = *pTargetData;
        return TRUE;
    }
    return FALSE;
}


HRESULT CTargetOnlBase::MakeHResult(long lCode)
{
    return(MAKE_HRESULT(0x1, m_tKADTargetData.m_lFacilityCode, lCode));
}

HRESULT CTargetOnlBase::OpenVarSession()
{
	// build up the session IF
	CComPtr<IFCSessions> pIFCSessions; 
	LPUNKNOWN pUnk;
	HRESULT hr;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OpenVarSession : TargetId: %s\n"), m_strId);

    if(m_pIFCVarSession)
    {
        return S_OK;
    }
        
    hr = CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (void**)&pIFCSessions);
	assert(SUCCEEDED(hr));
    if(FAILED(hr))
    {
        CString strMessage;

        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OpenVarSession : (TargetId: %s) CoCreateInstance CLSID_FCSessions returned hr=0x%08X!\n"), m_strId, hr);

        strMessage.Format(IDS_NO_COMPONENT, _T("Sessions"));
        UTIL_FatalError(strMessage, hr);
        return(MakeHResult(E_CE_TARGETONL_GENERAL));
    }
	BSTR szMachine = m_strControlAddress.AllocSysString();
	BSTR szConfig = m_strAssignedResource.AllocSysString(); 

	hr = pIFCSessions->OpenVarSession(szMachine, &pUnk);
	assert(SUCCEEDED(hr));
	::SysFreeString(szMachine);
	::SysFreeString(szConfig);
	if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OpenVarSession : (TargetId: %s) IFCSessions->OpenVarSession returned hr=0x%08X!\n"), m_strId, hr);

		AfxMessageBox(IDS_ERROR_CONNECT);
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
	}

    hr = pUnk->QueryInterface(IID_IFCVarSession, (void**)&m_pIFCVarSession);
    pUnk->Release();
	assert(SUCCEEDED(hr));
	if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OpenVarSession : (TargetId: %s) QueryInterface IFCVarSession returned hr=0x%08X!\n"), m_strId, hr);

		AfxMessageBox(IDS_ERROR_CONNECT);
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
	}

	// now try to advise the notification IF
	hr = CComObject<CImplSessionNotify>::CreateInstance(&m_pSessionNotify);
	assert(SUCCEEDED(hr));
	if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OpenVarSession : (TargetId: %s) CreateInstance CImplSessionNotify returned hr=0x%08X!\n"), m_strId, hr);

		AfxMessageBox(IDS_ERROR_CONNECT);
		return(MakeHResult(E_CE_TARGETONL_GENERAL));
	}
	m_pSessionNotify->SetCallback(&m_SessionNotifyCB);
	hr = AtlAdvise(m_pIFCVarSession, m_pSessionNotify->GetUnknown(), IID__FCSessionNotification, &m_dwSessionCookie);
	assert(SUCCEEDED(hr));
	if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OpenVarSession : (TargetId: %s) AtlAdvise SessionNotify returned hr=0x%08X!\n"), m_strId, hr);

		AfxMessageBox(IDS_ERROR_CONNECT);
		return(MakeHResult(E_CE_TARGETONL_ADVISEFAILED));
	}
	return(S_OK);
}


HRESULT CTargetOnlBase::CloseVarSession()
{
	HRESULT hr;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::CloseVarSession : TargetId: %s\n"), m_strId);

    if(!m_pIFCVarSession) 
    {
        return S_OK;
    }
	// detatch from 4CSC_Proxy
	hr = AtlUnadvise(m_pIFCVarSession, IID__FCSessionNotification, m_dwSessionCookie);
	assert(SUCCEEDED(hr));
	//!!!m_pIFCVarSession->Release();
	m_pIFCVarSession = NULL;
	if(FAILED(hr))
    {
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::CloseVarSession : (TargetId: %s) AtlUnadvise FCSessionNotification returned hr=0x%08X!\n"), m_strId, hr);

		AfxMessageBox(IDS_ERROR_DISCONNECT);
		return(MakeHResult(E_CE_TARGETONL_UNADVISEFAILED));
	}

	return S_OK;
}


HRESULT CTargetOnlBase::SubscribeForState()
{
    HRESULT hr;
    CString strName;
    long lState=-1;
    short sQuality;
    CComVariant varValue;

    if(m_pStateSubscription != NULL)
    {
        return(S_FALSE);
    }

    strName.Format("__srt0.%s", CE_STATE_RESOURCE);

    hr = SubscribeVariable(strName, &m_StateCB, m_pStateSubscription);
    if(FAILED(hr))
    {
        return(hr);
    }

    // Under certain circumstances (control service not running)
    // we will get no initial notification. For these cases we
    // read synchroneously here.

    hr = ReadVariable(m_pStateSubscription, sQuality, varValue);
    if(FAILED(hr))
    {
        // Variables are subscribed. Don't care if initial read
        // fails because hopefully later there will be notifications.
        return(S_OK);
    }

    if((sQuality & OPC_QUALITY_MASK) != OPC_QUALITY_BAD)
    {
        ASSERT(varValue.vt == VT_I4);
        if(varValue.vt == VT_I4)
        {
            lState = varValue.lVal;
        }
    }
    m_eResourceState = (E_ResourceState)lState;
    GetNewConnectState();
    return(S_OK);
}


HRESULT CTargetOnlBase::UnsubscribeForState()
{
    HRESULT hr;

    if(m_pStateSubscription == NULL)
    {
        return(S_FALSE);
    }

    hr = UnsubscribeVariable(m_pStateSubscription, &m_StateCB);
    m_pStateSubscription = 0;
    m_eResourceState = eRS_on_creation;
    GetNewConnectState();
    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * subscribe for download info.
 *
 *  base class returns E_FAIL
 *
 * @see             CTargetOnl::SubscribeForDownloadInfo()
*/
HRESULT CTargetOnlBase::SubscribeForDownloadInfo()
{
    return E_FAIL;
}

//------------------------------------------------------------------*
/**
 * unsubscribe for download info.
 *
 *  base class returns E_FAIL
 *
 * @see             CTargetOnl::UnsubscribeForDownloadInfo()
*/
HRESULT CTargetOnlBase::UnsubscribeForDownloadInfo()
{
    return E_FAIL;
}


void CTargetOnlBase::OnResourceStateChanged(CSubscription *pSubscription)
{
    CComVariant varValue;

    ASSERT(pSubscription != NULL);
    if(pSubscription == NULL)
    {
        return;
    }

    if((pSubscription->GetQuality() & OPC_QUALITY_MASK) != OPC_QUALITY_GOOD)
    {
        m_eResourceState = eRS_general_error;
        GetNewConnectState();
        return;
    }

    varValue = pSubscription->GetVariantValue();
    ASSERT(varValue.vt == VT_I4);
    if(varValue.vt != VT_I4)
    {
        return;
    }

    m_eResourceState = (E_ResourceState)varValue.lVal;
    GetNewConnectState();
}


void CTargetOnlBase::OnUserLoggedIn(const CString &strUserName, const CString &strHostName, bool bLoggedIn)
{
    CCEStateManager *pStateManager;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::OnUserLoggedIn : TargetId: %s ; UserName: %s ; HostName: %s ; Login: %s\n"), 
                            m_strId, strUserName, strHostName, bLoggedIn?_T("TRUE"):_T("FALSE"));


    pStateManager = CEGetStateManager();
    if(pStateManager == NULL)
        return;

    pStateManager->TriggerTargetLogin(this, bLoggedIn, strUserName, strHostName);
}


void CTargetOnlBase::LockSubscriptionManager()
{
    if(m_pSubscriptionManager == NULL)
    {
        m_pSubscriptionManager = new CSubscriptionManager;
        ASSERT(m_pSubscriptionManager != NULL);

        if(m_pSubscriptionManager != NULL)
        {
            HRESULT hr;

            hr = m_pSubscriptionManager->Start();
            ASSERT(SUCCEEDED(hr));

            if(FAILED(hr))
            {
                delete m_pSubscriptionManager;
                m_pSubscriptionManager = NULL;
            }
        }
    }

    ++m_uiSMLockCount;
}


void CTargetOnlBase::UnlockSubscriptionManager()
{
    ASSERT(m_uiSMLockCount != 0);
    if(m_uiSMLockCount == 0)
    {
        return;
    }

    if(--m_uiSMLockCount == 0)
    {
        if(m_pSubscriptionManager != NULL)
        {
            HRESULT hr;

            hr = m_pSubscriptionManager->Stop();
            ASSERT(SUCCEEDED(hr));

            delete m_pSubscriptionManager;
            m_pSubscriptionManager = NULL;
        }
    }
}


HRESULT CTargetOnlBase::SetPassword(const CString &strPassword)
{
	HRESULT hr;
	CString strVariableName;
	CComVariant varPassword;
    CSubscription *pSubscription;

	ASSERT(m_pIFCVarSession != NULL);
	if(m_pIFCVarSession == NULL)
	{
		return(E_FAIL);
	}

    if(m_pSubscriptionManager == NULL)
    {
        return(E_FAIL);
    }

	strVariableName.Format(_T("[%s].__system.security.password"), (LPCTSTR)m_strControlAddress);
	varPassword = strPassword;

    hr = m_pSubscriptionManager->GetSubscription(strVariableName, BL_ANY, pSubscription);
    if(FAILED(hr))
    {
        return(hr);
    }

    hr = pSubscription->Write(varPassword);
	if(FAILED(hr))
	{
        (void)pSubscription->Unreference();
		return(hr);
	}

	(void)pSubscription->Unreference();
	return(S_OK);
}

void CTargetOnlBase::OnSessionNotifyCB(CSC_SESSION_STATE sessionState, HRESULT hrError)
{
	POSITION pPosition;

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::ConOnSessionNotifyCBnect : TargetId: %s ; SessionState: %s, HRESULT: 0x%08X\n"), 
                         m_strId, GetSessionStateText(sessionState), hrError);


    // Set state before executing callbacks. Some clients
    // may read the state from TargetOnl rather than using the
    // passed parameter.

	m_SessionState = sessionState;

    GetNewConnectState();

	if (sessionState == RW_established)
	{
        CString strPassword = UTIL_GetMasterPassword ();
		SetPassword (strPassword);

        SubscribeForState();
        SubscribeForDownloadInfo();
	}
    else if (sessionState == not_connected)
    {
        UnsubscribeForState ();
        UnsubscribeForDownloadInfo ();
    }

    // TODO: own method:
	pPosition = m_SessionCBs.GetHeadPosition ();
	while (pPosition != NULL)
	{
		FC_CALLBACK_BASE (TargetOnlSessionCB) *pCallback;

		pCallback = m_SessionCBs.GetNext (pPosition);
		assert (pCallback != NULL);
		if (pCallback == NULL)
			continue;

		pCallback->Execute (this, sessionState, hrError);
	}
}


//------------------------------------------------------------------*
/**
 * show connect dialog.
 *
 *  shows progress bar for connect
 *
 * @param            [out] strErrorMessage: error message returned from the dialog
 * @return           return value from dialog
 *
*/
HRESULT CTargetOnlBase::ShowConnectDialog(CString& strErrorMessage, BOOL bExclusive /*= FALSE*/)
{
    UINT uiRet;
    HRESULT hr = S_FALSE;
    //int iConnectTimeOut = 100;  // TODO: get this value from settings
    int iConnectTimeOut = GetConnectTimeout();

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::ShowConnectDialog : TargetId: %s ; Timeout: %i\n"), m_strId, iConnectTimeOut);

    CConnectDlg dlg(this, iConnectTimeOut, bExclusive);
    uiRet = dlg.DoModal();

    TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::ShowConnectDialog : TargetId: %s ; Return: %s\n"), 
        m_strId, (uiRet==IDOK)?_T("OK"):_T("Cancel"));

    hr = (IDOK == uiRet) ? S_OK : S_FALSE;
    if(hr == S_FALSE) 
    {
        dlg.GetStateText(strErrorMessage);
        TrcPrint(TRC_COMM_EVENT, _T("CTargetOnlBase::ShowConnectDialog : TargetId: %s ; ErrorMsg: %s\n"), m_strId, strErrorMessage);
    }
    return hr;
}



int CTargetOnlBase::GetConnectTimeout()
{
    HRESULT hr;

    CString strType;
    strType = GetType();

    CString strValue;
    int iValue = -1;

    CString strSection = (CString)CE_CP_SETTINGS_TARGET + _T(".") + strType;

    CComBSTR sSection = strSection;
    CComBSTR sKey = CE_CP_SETTINGS_CONNECTTIMEOUT;
    CComBSTR sValue;

    // get project manager
    CComPtr<IUnknown>    pTmp;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    CComQIPtr<ICEProjInfo> pProjInfo;
    pProjInfo = pTmp;

    if (!pProjInfo)
    {
        return 100;
    }
    
    hr = pProjInfo-> CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    if (hr!=S_OK)
    {
        return 100;
    }

    strValue = sValue;
    iValue = _ttoi(strValue);

    // standard value if no valid value has been found
    if (iValue<=0)
    {
        iValue = 100;
    }

    return iValue;
}

CString CTargetOnlBase::GetConnectedControlAddress()
{
    return m_strControlAddressConn;
}


BOOL CTargetOnlBase::IsConsoleType()
{
    return m_tKADTargetData.GetConsoleFlag();
}

BOOL CTargetOnlBase::IsDebugEnabled()
{
    return m_tKADTargetData.GetDebugFlag();
}

BOOL CTargetOnlBase::IsMultitaskEnabled()
{
    return m_tKADTargetData.GetMultitaskFlag();
}

BOOL CTargetOnlBase::IsWarmstartEnabled()
{
    return m_tKADTargetData.GetWarmstartFlag();
}

BOOL CTargetOnlBase::IsInitClearEnabled()
{
    return m_tKADTargetData.GetInitClearFlag();
}

BOOL CTargetOnlBase::IsDLChangesEnabled()
{
    return m_tKADTargetData.GetDLChangesFlag();
}


CKADTargetData* CTargetOnlBase::GetKADTargetData()
{
    return &m_tKADTargetData;
}



HRESULT CTargetOnlBase::CustomOnlCmd(SAFEARRAY **ppDataDown, SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->CustDownUpLoad(ppDataDown, ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::ProjectInfoUpload(BSTR* psProjVers,
	                                      BSTR* psProjName,
	                                      BSTR* psModified,
	                                      BSTR* psCreated,
                                          BSTR* psOwner,
	                                      BSTR* psComment1,
	                                      BSTR* psComment2,
	                                      BSTR* psComment3)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->ProjectInfoUpload(psProjVers,
                                         psProjName,
                                         psModified,
	                                     psCreated,
                                         psOwner,
	                                     psComment1,
	                                     psComment2,
	                                     psComment3); 
    
    return hr;
}


HRESULT CTargetOnlBase::ClearFlash()
{
    HRESULT hr = EstablishConnection(FALSE);
    if(hr != S_OK)
    {
        return hr;
    }

    return CustomCommandSimple(CMD_CLEAR_FLASH, 100);
}


//------------------------------------------------------------------*
/**
 * EstablishConnection
 *
 *  build up a connection using the connect dialog
 *  this call is synchronous
 *
 * @param           bExclusive: exclusive connection?
 * @return          S_OK:       ok
 *                  S_FALSE:    abort
 *                  E_FAIL:     fail
 *                  hr from connect calls
 * @exception       -
 * @see             
*/
HRESULT CTargetOnlBase::EstablishConnection(BOOL bExclusive)
{
    HRESULT hr = S_OK;

    // check for connection to old target address
    // and disconnect
    if(IsConnected() && m_strControlAddress.CompareNoCase(m_strControlAddressConn) != 0)
    {
        CString strMessage;
        strMessage.Format(IDS_DL_DISCONN_OLD, m_strControlAddressConn);
        if(::AfxMessageBox(strMessage, MB_YESNO) == IDNO)
        {
            return S_FALSE;
        }
        if(IsConnectedExclusive())
        {
            hr = DisconnectExclusive();
        }
        if(hr == S_OK)
        {
            hr = Disconnect();
        }
        if(hr != S_OK)
        {
            return hr;
        }
    }

    // check for connection
    if(!IsTargetConnected())
    {
        // OPC server is connected but not target -> disconnect and retry
        if(IsConnected())
        {
            Disconnect();
        }
    }

    if(bExclusive && !IsTargetConnectedExclusive())
    {
        // OPC server is connected ex but not target -> disconnect ex and retry
        if(IsConnectedExclusive())
        {
            DisconnectExclusive();
        }
    }

    if(!IsConnected() || (bExclusive && !IsConnectedExclusive()))
    {
        CString strMessage;
        if(bExclusive)
        {
            strMessage.Format(IDS_CONNECTION_EX_REQUIRED, m_strId);
            if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
            {
                return S_FALSE;
            }
        }
        else
        {
            strMessage.Format(IDS_CONNECTION_REQUIRED, m_strId);
            if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
            {
                return S_FALSE;
            }
        }
    }
    CString strErrMessage;
    hr = ShowConnectDialog(strErrMessage, bExclusive);
    
    return hr;
}

HRESULT CTargetOnlBase::CustomCommandSimple(unsigned char ucCommand, unsigned char ucTrials /*=1*/)
{
    HRESULT hr = S_OK;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    SAFEARRAYBOUND  bound;
    SAFEARRAY*      pInput;
    LONG            lIndex(0);

    bound.lLbound = 0;
    bound.cElements = 2;
    
    pInput = ::SafeArrayCreate(VT_UI1, 1, &bound);

    hr = ::SafeArrayPutElement(pInput, &lIndex, (void*)(&ucCommand));
    if(hr != S_OK)
    {
        ::SafeArrayDestroy(pInput);
        return hr;
    }
    ++lIndex;
    hr = ::SafeArrayPutElement(pInput, &lIndex, (void*)(&ucTrials));
    if(hr != S_OK)
    {
        ::SafeArrayDestroy(pInput);
        return hr;
    }

	hr = pCustOnlCmds->CustCmd1(&pInput, NULL); 

    if (FAILED(hr))
    {
        ::SafeArrayDestroy(pInput);
    }

    return hr;
}


HRESULT CTargetOnlBase::ProjectDownload(SAFEARRAY **ppDataDown)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->ProjectDownload(ppDataDown);
    
    return hr;
}


HRESULT CTargetOnlBase::ProjectUpload(SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->ProjectUpload(ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::CustCmd1(SAFEARRAY **ppDataDown, 
                                 SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->CustCmd1(ppDataDown, ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::CustCmd2(SAFEARRAY **ppDataDown, 
                                 SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->CustCmd2(ppDataDown, ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::CustCmd3(SAFEARRAY **ppDataDown, 
                                 SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->CustCmd3(ppDataDown, ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::CustCmd4(SAFEARRAY **ppDataDown, 
                                 SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->CustCmd4(ppDataDown, ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::CustCmd5(SAFEARRAY **ppDataDown, 
                                 SAFEARRAY **ppDataUp)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->CustCmd5(ppDataDown, ppDataUp);
    
    return hr;
}


HRESULT CTargetOnlBase::LoadFile(BSTR sName, 
                                 SAFEARRAY **ppData)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->LoadFile(sName, ppData);
    
    return hr;
}


HRESULT CTargetOnlBase::SaveFile(BSTR sName, 
                                 SAFEARRAY **ppData)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->SaveFile(sName, ppData);
    
    return hr;
}


HRESULT CTargetOnlBase::Dir(SAFEARRAY **ppFileNames)
{
    HRESULT hr;

    if(!m_bConnected)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

    CComQIPtr<IFCCustOnlCmds> pCustOnlCmds = m_pIFCVarSession;
    if(!pCustOnlCmds)
    {
		return(MakeHResult(E_CE_TARGETONL_NOTCONNECTED));
    }

	hr = pCustOnlCmds->Dir(ppFileNames);
    
    return hr;
}
