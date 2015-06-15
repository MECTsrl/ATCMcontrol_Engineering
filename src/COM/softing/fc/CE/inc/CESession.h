
#ifndef CESession_H
#define CESession_H

#include "CSC_Online.h"
#include "fccallback.h"

class CCESession;


// Any session managed by the session manager has changed state
FC_CALLBACK_DEF3 (SessionStateNotifyCB, CCESession *, bool, CSC_SESSION_STATE)

// Session object has been modified
FC_CALLBACK_DEF2 (SessionModifiedCB, CCESession *, bool)

// Session object is about to be destroyed
FC_CALLBACK_DEF1 (SessionReleaseCB, CCESession *)

// for internal use only
FC_CALLBACK_DEF2 (SessionNotifyCB, const CComPtr<IUnknown> &, CSC_SESSION_STATE)


class CCESessionNotify : public IDispatchImpl<_FCSessionNotification, &IID__FCSessionNotification, &LIBID_CSC_ONLINELib,2,0>,
                         public CComObjectRoot
{
private:
    FC_CALLBACK_BASE (SessionNotifyCB) *m_pCallback;

public:
    CCESessionNotify() { m_pCallback = NULL; };
    ~CCESessionNotify() {};

    STDMETHOD_(HRESULT, OnChanged)();
    STDMETHOD_(HRESULT, OnStateChanged)(/*[in]*/ LPUNKNOWN pSession, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError);

    void SetCallback (FC_CALLBACK_BASE (SessionNotifyCB) *pCallback) { m_pCallback = pCallback; }

    BEGIN_COM_MAP(CCESessionNotify)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_FCSessionNotification)
    END_COM_MAP()
};


class CCESession : public CObject
{
public:
    CCESession (const CString &strMachine, const CComPtr<IFCSessions> &pSessions, CCESessionNotify *pSessionNotify);
    virtual ~CCESession ();

    void Reference ();
    void Unreference ();

    void RegisterModifiedCB (FC_CALLBACK_BASE (SessionModifiedCB) *pModifiedCB);
    void UnregisterModifiedCB (FC_CALLBACK_BASE (SessionModifiedCB) *pModifiedCB);

    void RegisterReleaseCB (FC_CALLBACK_BASE (SessionReleaseCB) *pReleaseCB);
    void UnregisterReleaseCB (FC_CALLBACK_BASE (SessionReleaseCB) *pReleaseCB);

    CString GetMachine ();

    CComPtr<IUnknown> GetSession ();
    CComPtr<IUnknown> GetDebugSession ();

    CSC_SESSION_STATE GetState ();
    void SetState (CSC_SESSION_STATE state);
    HRESULT ReadState ();

    CSC_SESSION_STATE GetDebugState ();
    void SetDebugState (CSC_SESSION_STATE state);
    HRESULT ReadDebugState ();

    bool IsOpen ();
    HRESULT Open ();
    HRESULT Close ();

    bool IsDebugOpen ();
    HRESULT CloseDebug ();
    HRESULT OpenDebug ();

    bool IsConnected ();
    HRESULT Connect ();
    HRESULT Disconnect ();

    bool IsDebugConnected ();
    HRESULT ConnectDebug ();
    HRESULT DisconnectDebug ();

protected:
    unsigned int m_uiReferences;

    CString              m_strMachine;
    CComPtr<IFCSessions> m_pSessions;
    CComPtr<CCESessionNotify> m_pSessionNotify;
    DWORD                m_dwCookieVS, m_dwCookieDS;

    CComPtr<IUnknown>      m_pVSUnknown;
    CComPtr<IFCVarSession> m_pIFCVarSession;
    CComPtr<IFCConnect>    m_pIFCVarConnect;
    bool                   m_bConnected;
    CSC_SESSION_STATE      m_State;

    CComPtr<IUnknown>   m_pDSUnknown;
    CComPtr<IFCConnect> m_pIFCConnect;
    bool                m_bDebugConnected;
    CSC_SESSION_STATE   m_DebugState;

    FC_CALLBACK_HOLDER (SessionReleaseCB)  m_ReleaseCBs;
    FC_CALLBACK_HOLDER (SessionModifiedCB) m_ModifiedCBs;
};


class CCESessionManager : public CObject
{
public:
    CCESessionManager (bool bKeepSessions);
    ~CCESessionManager ();

    HRESULT GetSession (const CString &strMachine, CCESession *&pSession);
    CCESession *FindSession (const CString &strMachine);

    void RegisterStateCB (FC_CALLBACK_BASE (SessionStateNotifyCB)   *pSessionStateCB);
    void UnregisterStateCB (FC_CALLBACK_BASE (SessionStateNotifyCB) *pSessionStateCB);

protected:
    bool m_bKeepSessions;

    CComPtr<IFCSessions>           m_pIFCSessions;
    CComPtr<IFCVariableCollection> m_pIFCVarCollection;

    CList<CCESession *, CCESession *> m_Sessions;

    CComObject<CCESessionNotify> *m_pSessionNotify;

    FC_CALLBACK (SessionNotifyCB, CCESessionManager)  m_SessionNotifyCB;
    FC_CALLBACK (SessionReleaseCB, CCESessionManager) m_SessionReleaseCB;

    FC_CALLBACK_HOLDER (SessionStateNotifyCB) m_SessionStateCBHolder;

private:
    HRESULT CreateSessionsObject ();
    HRESULT CreateVarCollection ();
    void OnSessionNotify (const CComPtr<IUnknown> &pSessionUnknown, CSC_SESSION_STATE state);
    void DisposeAllSessions ();
    HRESULT CreateSession (const CString &strMachine, CCESession *&pSession);
    CCESession *FindSession (const CComPtr<IUnknown> &pSessionUnknown);
    HRESULT SetPassword (CCESession *pSession, const CString &strPassword);
    void OnSessionReleased (CCESession *pSession);
};

#endif

