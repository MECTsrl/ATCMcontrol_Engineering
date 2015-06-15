
 


#include "stdafx.h"
#include "CEStateMan.h"
#include "CEMAINError.h"
#include "TargetOnlBase.h"
//#include "TargetOnl.h"


CCEStateManager *CCEStateManager::m_pMe;


CCEStateManager::CCEStateManager ()
: m_bSystemStarted (false),
  m_bFrameCreated (false),
  m_bProjectOpen (false),
  m_bNotifying (false),
  m_uiNextDelayCookie (1),
  m_strTargetName(),
  m_PumpCB (this, OnPump)
{
    int i;
    HRESULT hr;

    for (i = 0; i <= CELastStateNotify; ++i)
    {
        m_uiStages[i] = 1;
    };

    ASSERT (m_pMe == NULL);
    if (m_pMe == NULL)
    {
        m_pMe = this;
    };

    m_Pump.RegisterCB (&m_PumpCB);
    hr = m_Pump.Create (_T ("ATCM State Manager Pump"));
    ASSERT (SUCCEEDED (hr));
}


CCEStateManager::~CCEStateManager ()
{
    m_Pump.Destroy ();
    m_Pump.UnregisterCB (&m_PumpCB);
}


HRESULT CCEStateManager::RegisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB)
{
    if (m_StateChangeCBs.Find (pCB) != NULL)
    {
        return (E_CESTATEMAN_CALLBACK_ALREADY_REGISTERED);
    };

    m_StateChangeCBs.AddTail (pCB);
    return (S_OK);
}


HRESULT CCEStateManager::UnregisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB)
{
    POSITION pPosition;

    pPosition = m_StateChangeCBs.Find (pCB);
    if (pPosition == NULL)
    {
        return (E_CESTATEMAN_CALLBACK_NOT_REGISTERED);
    };

    m_StateChangeCBs.RemoveAt (pPosition);
    return (S_OK);
}


void CCEStateManager::SetStages (CEStateNotify stateNotify, unsigned int uiStages)
{
    m_uiStages[stateNotify] = uiStages;
}


void CCEStateManager::TriggerSystemStartup ()
{
    m_bSystemStarted = true;
    ExecuteStages (CESystemStartupNotify);
}


void CCEStateManager::TriggerSystemShutdown ()
{
    ExecuteStages (CESystemShutdownNotify);
    m_bSystemStarted = false;
}


void CCEStateManager::TriggerCreateFrame ()
{
    m_bFrameCreated = true;
    ExecuteStages (CECreateFrameNotify);
}


void CCEStateManager::TriggerDestroyFrame ()
{
    ExecuteStages (CEDestroyFrameNotify);
    m_bFrameCreated = false;
}


void CCEStateManager::TriggerPreOpenProject ()
{
    ExecuteStages (CEPreOpenProjectNotify);
}


void CCEStateManager::TriggerPostOpenProject ()
{
    ExecuteStages (CEPostOpenProjectNotify);
}


void CCEStateManager::TriggerPreCloseProject ()
{
    ExecuteStages (CEPreCloseProjectNotify);
}


void CCEStateManager::TriggerPostCloseProject ()
{
    ExecuteStages (CEPostCloseProjectNotify);
}


void CCEStateManager::TriggerCreateComponent (const CString &strName)
{
    CString strSavedName=m_strComponentName;

    m_strComponentName = strName;
    ExecuteStages (CECreateComponentNotify);
    m_strComponentName = strSavedName;
}


void CCEStateManager::TriggerDestroyComponent (const CString &strName)
{
    CString strSavedName=m_strComponentName;

    m_strComponentName = strName;
    ExecuteStages (CEDestroyComponentNotify);
    m_strComponentName = strSavedName;
}


void CCEStateManager::TriggerComponentAccessible (const CString &strName)
{
    CString strSavedName=m_strComponentName;

    m_strComponentName = strName;
    ExecuteStages (CEComponentAccessibleNotify);
    m_strComponentName = strSavedName;
}


void CCEStateManager::TriggerHandsOffComponent (const CString &strName)
{
    CString strSavedName=m_strComponentName;

    m_strComponentName = strName;
    ExecuteStages (CEHandsOffComponentNotify);
    m_strComponentName = strSavedName;
}


void CCEStateManager::TriggerNewSettings ()
{
    ExecuteStages (CENewSettingsNotify);
}


void CCEStateManager::TriggerFillSettings ()
{
    ExecuteStages (CEFillSettingsNotify);
}


void CCEStateManager::TriggerAddTarget (CTargetOnlBase *pTarget)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;

    m_pTarget = pTarget;
    ExecuteStages (CEAddTargetNotify);
    m_pTarget = pSavedTarget;
}


void CCEStateManager::TriggerRemoveTarget (CTargetOnlBase *pTarget)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;

    m_pTarget = pTarget;
    ExecuteStages (CERemoveTargetNotify);
    m_pTarget = pSavedTarget;
}


void CCEStateManager::TriggerRenameTarget (CTargetOnlBase *pTarget, const CString &strOldName)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;
    CString strSavedOldTargetName=m_strOldTargetName;

    m_pTarget = pTarget;
    m_strOldTargetName = strOldName;
    ExecuteStages (CERenameTargetNotify);
    m_pTarget = pSavedTarget;
    m_strOldTargetName = strSavedOldTargetName;
}


void CCEStateManager::TriggerTargetAddressModify(CTargetOnlBase *pTarget)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;

    m_pTarget = pTarget;
    ExecuteStages (CETargetAddressModifyNotify);
    m_pTarget = pSavedTarget;
}


void CCEStateManager::TriggerDownloadInitiated (const CString & strTargetName)
{
    CString strSaved=m_strTargetName;

    m_strTargetName = strTargetName;
    ExecuteStages (CEDownloadInitiatedNotify);
    m_strTargetName = strSaved;
}


void CCEStateManager::TriggerDownloadStarted (const CString &strTargetName)
{
    CString strSaved=m_strTargetName;

    m_strTargetName = strTargetName;
    ExecuteStages (CEDownloadStartedNotify);
    m_strTargetName = strSaved;
}

void CCEStateManager::TriggerDownloadAborted (const CString &strTargetName)
{
    CString strSaved=m_strTargetName;

    m_strTargetName = strTargetName;
    ExecuteStages (CEDownloadAbortedNotify);
    m_strTargetName = strSaved;
}


void CCEStateManager::TriggerDownloadComplete (const CString &strTargetName)
{
    CString strSaved=m_strTargetName;

    m_strTargetName = strTargetName;
    ExecuteStages (CEDownloadCompleteNotify);
    m_strTargetName = strSaved;
}


void CCEStateManager::TriggerTargetLogin (CTargetOnlBase *pTarget, bool bLogin, const CString &strUser, const CString &strHost)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;
    bool bSavedLogin=m_bTargetLogin;
    CString strSavedUser=m_strTargetLoginUser;
    CString strSavedHost=m_strTargetLoginHost;

    m_pTarget = pTarget;
    m_bTargetLogin = bLogin;
    m_strTargetLoginUser = strUser;
    m_strTargetLoginHost = strHost;
    ExecuteStages (CETargetLoginNotify);
    m_pTarget = pSavedTarget;
    m_bTargetLogin = bSavedLogin;
    m_strTargetLoginUser = strSavedUser;
    m_strTargetLoginHost = strSavedHost;
}


void CCEStateManager::TriggerTargetConnect (CTargetOnlBase *pTarget, CEConnectAction connectAction)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;
    CEConnectAction savedConnectAction=m_ConnectAction;

    m_pTarget = pTarget;
    m_ConnectAction = connectAction;
    ExecuteStages (CETargetConnectNotify);
    m_pTarget = pSavedTarget;
    m_ConnectAction = savedConnectAction;
}


void CCEStateManager::TriggerTargetDebugConnect (CTargetOnlBase *pTarget, CEConnectAction connectAction)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;
    CEConnectAction savedConnectAction=m_ConnectAction;

    m_pTarget = pTarget;
    m_ConnectAction = connectAction;
    ExecuteStages (CETargetDebugConnectNotify);
    m_pTarget = pSavedTarget;
    m_ConnectAction = savedConnectAction;
}


void CCEStateManager::TriggerDownloadInfo (CTargetOnlBase *pTarget)
{
    CTargetOnlBase *pSavedTarget=m_pTarget;

    m_pTarget = pTarget;
    ExecuteStages (CEDownloadInfoNotify);
    m_pTarget = pSavedTarget;
}


unsigned int CCEStateManager::TriggerDelay (void *pClientData)
{
    unsigned int uiDelayCookie;
    DelayInfo delayInfo;

    uiDelayCookie = m_uiNextDelayCookie++;

    delayInfo.m_uiCookie = uiDelayCookie;
    delayInfo.m_pClientData = pClientData;
    delayInfo.m_uiPumpCookie = 0;

    m_Pump.Trigger ();

    m_DelayInfos.AddTail (delayInfo);

    return (uiDelayCookie);
}


unsigned int CCEStateManager::TriggerDelay (unsigned int uiDelayMillis, void *pClientData)
{
    unsigned int uiDelayCookie;
    DelayInfo delayInfo;

    uiDelayCookie = m_uiNextDelayCookie++;

    delayInfo.m_uiCookie = uiDelayCookie;
    delayInfo.m_pClientData = pClientData;

    m_Pump.Delay (uiDelayMillis, delayInfo.m_uiPumpCookie);

    m_DelayInfos.AddTail (delayInfo);

    return (uiDelayCookie);
}


bool CCEStateManager::IsSystemStarted ()
{
    return (m_bSystemStarted);
}


bool CCEStateManager::IsFrameCreated ()
{
    return (m_bFrameCreated);
}


bool CCEStateManager::IsProjectOpen ()
{
    return (m_bProjectOpen);
}


CString CCEStateManager::GetComponentName ()
{
    ASSERT (m_bNotifying);
    if (!m_bNotifying)
        return ("");

    ASSERT (m_CurrentNotify == CECreateComponentNotify ||
            m_CurrentNotify == CEDestroyComponentNotify ||
            m_CurrentNotify == CEComponentAccessibleNotify ||
            m_CurrentNotify == CEHandsOffComponentNotify);

    if (m_CurrentNotify != CECreateComponentNotify &&
        m_CurrentNotify != CEDestroyComponentNotify &&
        m_CurrentNotify != CEComponentAccessibleNotify &&
        m_CurrentNotify != CEHandsOffComponentNotify)
        return ("");

    return (m_strComponentName);
}


CTargetOnlBase *CCEStateManager::GetTarget ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CEAddTargetNotify || m_CurrentNotify == CERemoveTargetNotify ||
            m_CurrentNotify == CERenameTargetNotify || m_CurrentNotify == CETargetAddressModifyNotify ||
            m_CurrentNotify == CETargetLoginNotify ||
            m_CurrentNotify == CETargetConnectNotify || m_CurrentNotify == CETargetDebugConnectNotify ||
            m_CurrentNotify == CEDownloadInfoNotify);

    if (!m_bNotifying)
        return (NULL);

    if (m_CurrentNotify != CEAddTargetNotify && m_CurrentNotify != CERemoveTargetNotify &&
        m_CurrentNotify != CERenameTargetNotify && m_CurrentNotify != CETargetAddressModifyNotify &&
        m_CurrentNotify != CETargetLoginNotify &&
        m_CurrentNotify != CETargetConnectNotify && m_CurrentNotify != CETargetDebugConnectNotify &&
        m_CurrentNotify != CEDownloadInfoNotify)
        return (NULL);

    return (m_pTarget);
}


CString CCEStateManager::GetOldTargetName ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CERenameTargetNotify);

    if (!m_bNotifying)
        return ("");

    if (m_CurrentNotify != CERenameTargetNotify)
        return ("");

    return (m_strOldTargetName);
}


CString CCEStateManager::GetTargetName ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CEDownloadInitiatedNotify ||
            m_CurrentNotify == CEDownloadStartedNotify ||
            m_CurrentNotify == CEDownloadCompleteNotify ||
            m_CurrentNotify == CEDownloadAbortedNotify);

    if (!m_bNotifying)
        return ("");

    if (m_CurrentNotify != CEDownloadInitiatedNotify &&
        m_CurrentNotify != CEDownloadStartedNotify &&
        m_CurrentNotify != CEDownloadCompleteNotify &&
        m_CurrentNotify != CEDownloadAbortedNotify)
        return ("");

    return (m_strTargetName);
}


bool CCEStateManager::IsTargetLogin ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CETargetLoginNotify);

    if (!m_bNotifying)
        return (false);

    if (m_CurrentNotify != CETargetLoginNotify)
        return (false);

    return (m_bTargetLogin);
}


CString CCEStateManager::GetTargetLoginUser ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CETargetLoginNotify);

    if (!m_bNotifying)
        return ("");

    if (m_CurrentNotify != CETargetLoginNotify)
        return ("");

    return (m_strTargetLoginUser);
}


CString CCEStateManager::GetTargetLoginHost ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CETargetLoginNotify);

    if (!m_bNotifying)
        return ("");

    if (m_CurrentNotify != CETargetLoginNotify)
        return ("");

    return (m_strTargetLoginHost);
}


CEConnectAction CCEStateManager::GetConnectAction ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CETargetConnectNotify || m_CurrentNotify == CETargetDebugConnectNotify);

    if (!m_bNotifying)
        return (CEConnectError);

    if (m_CurrentNotify != CETargetConnectNotify && m_CurrentNotify != CETargetDebugConnectNotify)
        return (CEConnectError);

    return (m_ConnectAction);
}


unsigned int CCEStateManager::GetDelayCookie ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CEDelayNotify);

    if (!m_bNotifying)
        return (0);

    if (m_CurrentNotify != CEDelayNotify)
        return (0);

    return (m_DelayInfo.m_uiCookie);
}


void *CCEStateManager::GetDelayClientData ()
{
    ASSERT (m_bNotifying);
    ASSERT (m_CurrentNotify == CEDelayNotify);

    if (!m_bNotifying)
        return (0);

    if (m_CurrentNotify != CEDelayNotify)
        return (0);

    return (m_DelayInfo.m_pClientData);
}


void CCEStateManager::ExecuteStages (CEStateNotify stateNotify)
{
    unsigned int uiStage;
    bool bSavedNotifying;
    CEStateNotify savedCurrentNotify;

    bSavedNotifying = m_bNotifying;
    savedCurrentNotify = m_CurrentNotify;
    m_bNotifying = true;
    m_CurrentNotify = stateNotify;

    for (uiStage = 0; uiStage < m_uiStages[stateNotify]; ++uiStage)
    {
        m_StateChangeCBs.Execute (stateNotify, uiStage);
    };

    m_bNotifying = bSavedNotifying;
    m_CurrentNotify = savedCurrentNotify;
}


void CCEStateManager::OnPump ()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    CList<DelayInfo, const DelayInfo &> delayInfos;
    UINT uiPumpCookie;
    POSITION pPosition;

    uiPumpCookie = m_Pump.GetDelayCookie ();


    // Very important: make a copy of the delay infos in question. If we did
    // not do so, some client of delay notification could add an entry to the
    // delay info list which would be handled immediately in here (without
    // delay!).

    pPosition = m_DelayInfos.GetHeadPosition ();
    while (pPosition != NULL)
    {
        POSITION pRemovePosition=NULL;
        DelayInfo delayInfo;

        delayInfo = m_DelayInfos.GetAt (pPosition);
        if (delayInfo.m_uiPumpCookie == uiPumpCookie)
        {
            delayInfos.AddTail (delayInfo);
            pRemovePosition = pPosition;
        };

        m_DelayInfos.GetNext (pPosition);

        if (pRemovePosition != NULL)
        {
            m_DelayInfos.RemoveAt (pRemovePosition);
        };
    };

    while (!delayInfos.IsEmpty ())
    {
        DelayInfo savedDelayInfo=m_DelayInfo;

        m_DelayInfo = delayInfos.RemoveHead ();
        ExecuteStages (CEDelayNotify);
        m_DelayInfo = savedDelayInfo;
    };
}


CCEStateManager *CCEStateManager::GetMe ()
{
    return (m_pMe);
}


CCEStateManager *CEGetStateManager ()
{
    CCEStateManager *pStateManager;

    pStateManager = CCEStateManager::GetMe ();
    ASSERT (pStateManager != NULL);
    return (pStateManager);
}


void CESMRegisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB)
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return;

    pStateManager->RegisterCallback (pCB);
}


void CESMUnregisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB)
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return;

    pStateManager->UnregisterCallback (pCB);
}


bool CEIsProjectOpen ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return (false);

    return (pStateManager->IsProjectOpen ());
}


CString CESMGetComponentName ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return ("");

    return (pStateManager->GetComponentName ());
}


CTargetOnlBase *CESMGetTarget ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return (NULL);

    return (pStateManager->GetTarget ());
}


CString CESMGetOldTargetName ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return ("");

    return (pStateManager->GetOldTargetName ());
}


CString CESMGetTargetName ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return ("");

    return (pStateManager->GetTargetName());
}


bool CESMIsTargetLogin ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return (false);

    return (pStateManager->IsTargetLogin ());
}


CString CESMGetTargetLoginUser ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return ("");

    return (pStateManager->GetTargetLoginUser ());
}


CString CESMGetTargetLoginHost ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return ("");

    return (pStateManager->GetTargetLoginHost ());
}


CEConnectAction CESMGetConnectAction ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return (CEConnectError);

    return (pStateManager->GetConnectAction ());
}


unsigned int CESMGetDelayCookie ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return (0);

    return (pStateManager->GetDelayCookie ());
}


void *CESMGetDelayClientData ()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return (NULL);

    return (pStateManager->GetDelayClientData ());
}

