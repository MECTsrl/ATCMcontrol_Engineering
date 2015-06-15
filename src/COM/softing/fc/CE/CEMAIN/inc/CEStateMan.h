
#ifndef __CESTATEMAN_H_
#define __CESTATEMAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#ifndef CESTATEMAN_H
#define CESTATEMAN_H

class CTargetOnlBase;

#include "fccallback.h"
#include "fcpump.h"


enum CEStateNotify
{
    CESystemStartupNotify,
    CESystemShutdownNotify,
    CECreateFrameNotify,
    CEDestroyFrameNotify,
    CEPreOpenProjectNotify,
    CEPostOpenProjectNotify,
    CEPreCloseProjectNotify,
    CEPostCloseProjectNotify,
    CECreateComponentNotify,
    CEDestroyComponentNotify,
    CEComponentAccessibleNotify,
    CEHandsOffComponentNotify,
    CENewSettingsNotify,
    CEFillSettingsNotify,
    CEAddTargetNotify,
    CERemoveTargetNotify,
    CERenameTargetNotify,
    CETargetAddressModifyNotify,
    CEDownloadInitiatedNotify,
    CEDownloadStartedNotify,
    CEDownloadReadyNotify,
    CEDownloadAbortedNotify,
    CEDownloadCompleteNotify,
    CETargetLoginNotify,
    CETargetConnectNotify,
    CETargetDebugConnectNotify,
    CEDownloadInfoNotify,
    CEDelayNotify,

    CELastStateNotify = CEDelayNotify
};


enum CEConnectAction
{
    CEPreConnect,
    CEPostConnect,
    CEConnectError,
    CEPreDisconnect,
    CEPostDisconnect,
    CEDisconnectError
};


FC_CALLBACK_DEF2 (CEStateChangeCB, CEStateNotify, unsigned int);


class CCEStateManager : public CObject
{
public:
    CCEStateManager ();
    ~CCEStateManager ();

    HRESULT RegisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB);
    HRESULT UnregisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB);

    void SetStages (CEStateNotify stateNotify, unsigned int uiStages);

    void TriggerSystemStartup ();
    void TriggerSystemShutdown ();
    void TriggerCreateFrame ();
    void TriggerDestroyFrame ();
    void TriggerPreOpenProject ();
    void TriggerPostOpenProject ();
    void TriggerPreCloseProject ();
    void TriggerPostCloseProject ();
    void TriggerCreateComponent (const CString &strComponent);
    void TriggerDestroyComponent (const CString &strComponent);
    void TriggerComponentAccessible (const CString &strComponent);
    void TriggerHandsOffComponent (const CString &strComponent);
    void TriggerNewSettings ();
    void TriggerFillSettings ();
    void TriggerAddTarget (CTargetOnlBase *pTarget);
    void TriggerRemoveTarget (CTargetOnlBase *pTarget);
    void TriggerRenameTarget (CTargetOnlBase *pTarget, const CString &strOldName);
    void TriggerTargetAddressModify(CTargetOnlBase *pTarget);
    void TriggerDownloadInitiated (const CString &);
    void TriggerDownloadStarted (const CString &);
    void TriggerDownloadComplete(const CString &);
    void TriggerDownloadAborted (const CString &);
    void TriggerTargetLogin (CTargetOnlBase *pTarget, bool bLoggedIn, const CString &strUser, const CString &strHost);
    void TriggerTargetConnect (CTargetOnlBase *pTarget, CEConnectAction connectAction);
    void TriggerTargetDebugConnect (CTargetOnlBase *pTarget, CEConnectAction connectAction);
    void TriggerDownloadInfo (CTargetOnlBase *pTarget);
    unsigned int TriggerDelay (void *pClientData);
    unsigned int TriggerDelay (unsigned int uiDelayMillis, void *pClientData);

    bool IsSystemStarted ();
    bool IsFrameCreated ();
    bool IsProjectOpen ();

    CString GetComponentName ();
    CTargetOnlBase *GetTarget ();
    CString GetOldTargetName ();
    CString GetTargetName ();
    bool IsTargetLogin ();
    CString GetTargetLoginUser ();
    CString GetTargetLoginHost ();
    CEConnectAction GetConnectAction ();
    unsigned int GetDelayCookie ();
    void *GetDelayClientData ();

    static CCEStateManager *GetMe ();

protected:
    struct DelayInfo
    {
        unsigned int m_uiCookie;
        void         *m_pClientData;
        UINT         m_uiPumpCookie;
    };

    FC_CALLBACK_HOLDER (CEStateChangeCB) m_StateChangeCBs;

    unsigned int m_uiStages[CELastStateNotify+1];

    bool m_bSystemStarted;
    bool m_bFrameCreated;
    bool m_bProjectOpen;

    bool            m_bNotifying;
    CEStateNotify   m_CurrentNotify;
    CString         m_strComponentName;
    CTargetOnlBase          *m_pTarget;
    CString         m_strTargetName;
    CString         m_strOldTargetName;
    CString         m_strDownloadMachine;
    bool            m_bTargetLogin;
    CString         m_strTargetLoginUser;
    CString         m_strTargetLoginHost;
    CEConnectAction m_ConnectAction;
    DelayInfo       m_DelayInfo;

    CFCPump                                 m_Pump;
    FC_CALLBACK (FCPumpCB, CCEStateManager) m_PumpCB;

    unsigned int                        m_uiNextDelayCookie;
    CList<DelayInfo, const DelayInfo &> m_DelayInfos;

    static CCEStateManager *m_pMe;

private:
    void ExecuteStages (CEStateNotify stateNotify);

    void OnPump ();
};


CCEStateManager *CEGetStateManager ();

void CESMRegisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB);
void CESMUnregisterCallback (FC_CALLBACK_BASE (CEStateChangeCB) *pCB);

bool CEIsProjectOpen ();

CString CESMGetComponentName ();
CTargetOnlBase *CESMGetTarget ();
CString CESMGetOldTargetName ();
CString CESMGetTargetName ();
bool CESMIsTargetLogin ();
CString CESMGetTargetLoginUser ();
CString CESMGetTargetLoginHost ();
CEConnectAction CESMGetConnectAction ();
unsigned int CESMGetDelayCookie ();
void *CESMGetDelayClientData ();

#endif

#endif // __CESTATEMAN_H_

