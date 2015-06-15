#ifndef __TARGETONL_H_
#define __TARGETONL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "TargetOnlBase.h"

//----  Forward Class Definitions:   -------------------------------*
class CStation;
class CTargetOnl;
interface IBreakPoint;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CExclusiveSessionNotify : 
	public IDispatchImpl<_FCSessionNotification, &IID__FCSessionNotification, &LIBID_CSC_ONLINELib,2,0>, 
	public CComObjectRoot
{
public:
    CExclusiveSessionNotify();
    virtual ~CExclusiveSessionNotify();

    STDMETHOD_(HRESULT, OnChanged)();
    STDMETHOD_(HRESULT, OnStateChanged)(/*[in]*/ LPUNKNOWN pSession,/*[in]*/CSC_SESSION_STATE newState,/*[in]*/HRESULT hrError);

    BEGIN_COM_MAP(CExclusiveSessionNotify)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_FCSessionNotification)
    END_COM_MAP()

public: //attributes
    CTargetOnl* m_pTargetOnl;

private:
    void OnInterrupted(void);
    void OnExclusive(void);
    void OnConnectError(HRESULT hrError);
    BOOL IsRequestPending(void);
};

class CTransferNotify : public IDispatchImpl<_FCTransferNotification, &IID__FCTransferNotification, &LIBID_CSC_ONLINELib,2,0>,
                        public CComObjectRoot
{
public:
    CTransferNotify ();
    virtual ~CTransferNotify ();

    void SetTarget (CTargetOnl *pTarget);

    BEGIN_COM_MAP (CTransferNotify)
    COM_INTERFACE_ENTRY (IDispatch)
    COM_INTERFACE_ENTRY (_FCTransferNotification)
    END_COM_MAP ()

protected:
    CTargetOnl *m_pTarget;

private:
    STDMETHOD(OnUserLoggedIn)(/*[in, string]*/ BSTR sUserName, /*[in, string]*/ BSTR sHostName, /*[in]*/ BOOL bLoggedIn);
    STDMETHOD(OnTransferStateChanged)(/*[in]*/ DWORD dwCookie, /*[in, string]*/ BSTR sCurrentDomainName, /*[in]*/ CSC_TRANSFER_STATE state, /*[in]*/ float fltPercentDone);
    STDMETHOD(OnTransferError)(/*[in]*/ DWORD dwCookie, /*[in, string]*/ BSTR sCurrentDomainName, /*[in]*/ CSC_TRANSFER_STATE state, /*[in]*/ HRESULT hError);
};

class CBreakpointNotify : public IDispatchImpl<_FCBreakpointNotification, &IID__FCBreakpointNotification, &LIBID_CSC_ONLINELib,2,0>,
                          public CComObjectRoot
{
public:
    CBreakpointNotify ();
    virtual ~CBreakpointNotify ();

    void SetTarget (CTargetOnl *pTarget);

    BEGIN_COM_MAP (CBreakpointNotify)
    COM_INTERFACE_ENTRY (IDispatch)
    COM_INTERFACE_ENTRY (_FCBreakpointNotification)
    END_COM_MAP ()

protected:
    CTargetOnl *m_pTarget;

private:
    STDMETHOD_ (HRESULT, OnBreakpointReached)(/*[in]*/ BSTR sInstanceName, /*[in]*/ long lBreakpointNumber,
                                              /*[in]*/ long bpQuality, /*[in]*/ BSTR sTaskName);
    STDMETHOD_ (HRESULT, OnConfigurationChanged)(/*[in, string]*/ BSTR sAddress);
};


FC_CALLBACK_DEF3 (TargetOnlExclusiveSessionCB, CTargetOnl *, CSC_SESSION_STATE, HRESULT)


//------------------------------------------------------------------*
/**
 *  class CTargetOnl
 *
 *  - online connection to control for debug and download
 *  - online connection to start / stop task / resource is inherited
 *    from CTargetOnlBase
 *
 *  @see    CTargetOnlBase
 */
class CTargetOnl : public CTargetOnlBase
{
//friend class CTransferNotify;
//friend class CBreakpointNotify;

public:
    CTargetOnl( const CString& crstrTargetId = "",
                const CString& crstrInstIdPath = "",
                const CString& crstrType = "",
                const CString& crstrAssignedResource = "");

    virtual ~CTargetOnl();

    // connect
    virtual HRESULT Disconnect();
    virtual HRESULT ConnectExclusive();
    virtual HRESULT DisconnectExclusive(BOOL bForce = FALSE);
    virtual BOOL IsConnectedExclusive();        // CSC debug connection?
    virtual BOOL IsTargetConnectedExclusive();  // target connection also?
    virtual CSC_SESSION_STATE GetExclusiveSessionState();
    
    // debug / download connections
    HRESULT GetExclusiveSessionHResult();
    LPUNKNOWN GetExclusiveSession();

    // resource actions
    HRESULT ClearResource(LPCTSTR pszResource);



    // download connections
    bool IsDownloadInfoAvailable ();
    bool GetDownloadGUID (CString &strDownloadGUID);
    bool GetDownloadDate (CString &strDownloadDate);
    bool GetDownloadHost (CString &strDownloadHost);
    bool GetProjectGUID (CString &strProjectGUID);
    bool GetProjectName (CString &strProjectName);
    
    // register / unregister callbacks
    HRESULT RegisterExclusiveSessionCB (FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB) *pCallback);
    HRESULT UnregisterExclusiveSessionCB (FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB) *pCallback);

    BOOL IsTaskInBreakpoint();

protected:
    HRESULT OpenExclusiveSession();
    HRESULT CloseExclusiveSession();

    virtual HRESULT SubscribeForDownloadInfo();
    virtual HRESULT UnsubscribeForDownloadInfo();

    HRESULT SubscribeDownloadInfoStringVariable (const CString &strName, CSubscription *&pSubscription, bool &bValid, CString &strValue);
    HRESULT SubscribeDownloadInfoVariable (const CString &strName, CSubscription *&pSubscription, bool &bValid, CComVariant &varValue);
    void OnDownloadInfoChanged (CSubscription *pSubscription);

    void OnExclusiveSessionNotifyCB(CSC_SESSION_STATE sessionState, HRESULT hrError);
    void OnBreakpointNotify(const CString& crstrTaskName, const CString& crstrInstName, BOOL bBPReached);

    CComPtr<IBreakPoint> GetBreakpointManager ();

    virtual E_TargetOnlConnectState CalculateConnectState();

protected:

    typedef CList<FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB)* , FC_CALLBACK_BASE (TargetOnlExclusiveSessionCB) *> ExclusiveSessionCBList;
    ExclusiveSessionCBList                  m_ExclusiveSessionCBs;

    // Exclusive session
    IFCConnect*                             m_pExclusiveSession;
    CComObject<CExclusiveSessionNotify>*    m_pExclusiveNotify;
    DWORD	                                m_dwExclusiveSessionCookie;
    BOOL                                    m_bExclusiveConnected; // TRUE if Exclusive connected
    CSC_SESSION_STATE                       m_ExclusiveSessionState;
    HRESULT                                 m_ExclusiveHrError;

    CComObject<CTransferNotify>*            m_pTransferNotify;
    DWORD                                   m_dwTransferCookie;

    CComObject<CBreakpointNotify>*          m_pBreakpointNotify;
    DWORD                                   m_dwBreakpointCookie;
    FC_CALLBACK (SubscriptionModifiedCB, CTargetOnl)   m_DownloadInfoCB;
    CSubscription*                          m_pDownloadGUIDSubscription;
    bool                                    m_bDownloadGUIDValid;
    CString                                 m_strDownloadGUID;
    CSubscription*                          m_pDownloadDateSubscription;
    bool                                    m_bDownloadDateValid;
    CString                                 m_strDownloadDate;
    CSubscription*                          m_pDownloadHostSubscription;
    bool                                    m_bDownloadHostValid;
    CString                                 m_strDownloadHost;

    BOOL                                    m_bTaskInBreakPoint;

friend class CExclusiveSessionNotify;
friend class CBreakpointNotify;

};

#endif // __TARGETONL_H_