#ifndef __BREAKPOINTMAN_H_
#define __BREAKPOINTMAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// BreakPointMan.h : Declaration of the CBreakPointMan

#include "resource.h"       // main symbols
#include "BrkPtMan.h"
#include "BrkPtManCP.h"
#include "CSC_Online.h"
#include "DbgInfo.h"
#include "fccallback.h"


struct StopData;
struct DownloadData;
struct ConfigChangeData;

// Callback definition for internal use
FC_CALLBACK_DEF1 (StopNotifyCB, StopData *);
FC_CALLBACK_DEF1 (DownloadNotifyCB, DownloadData *);
FC_CALLBACK_DEF1 (ConfigChangeNotifyCB, ConfigChangeData *);
FC_CALLBACK_DEF2 (BPManExclusiveSessionCB, LPUNKNOWN, CSC_SESSION_STATE);


/////////////////////////////////////////////////////////////////////////////
// CBreakPointInfo

class ATL_NO_VTABLE CBreakPointInfo : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public IDispatchImpl<IBreakPointInfo, &IID_IBreakPointInfo, &LIBID_BRKPTMANLib>
{
public:
    CBreakPointInfo ()
    : m_lX    (-1),
      m_lY    (-1),
      m_lCX   (-1),
      m_lCY   (-1),
      m_PosQual (EDP_STLINE),
      m_lNumber (0),
      m_bSubscribed (false),
      m_lHandle (NULL),
      m_bActive (true),
      m_lQuality (0)
    {
    }

    virtual ~CBreakPointInfo()
    {
    
    }

DECLARE_PROTECT_FINAL_CONSTRUCT()



BEGIN_COM_MAP(CBreakPointInfo)
    COM_INTERFACE_ENTRY(IBreakPointInfo)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

    STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_PosQualifier)(/*[out, retval]*/ EDP_POS_QUALIFIER *pVal);
    STDMETHOD(get_X)(/*[out, retval]*/ long *pVal);
    STDMETHOD(get_Y)(/*[out, retval]*/ long *pVal);
    STDMETHOD(get_CX)(/*[out, retval]*/ long *pCX);
    STDMETHOD(get_CY)(/*[out, retval]*/ long *pCY);
    STDMETHOD(get_ClassName)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_Language)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_Quality)(/*[out, retval]*/ long *pVal);
    STDMETHOD(get_Instance)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(get_Active)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_Number)(/*[out, retval]*/ long *pNumber);

public:
    CString           m_strInstance;
    CString           m_strFileName;
    long              m_lX;
    long              m_lY;
    long              m_lCX;
    long              m_lCY;
    EDP_POS_QUALIFIER m_PosQual;
    CString           m_strClassName;
    CString           m_strLanguage;
    long              m_lNumber;
    CString           m_strTask;
    bool              m_bSubscribed;
    long              m_lHandle;
    bool              m_bActive;
    long              m_lQuality;
};


class ATL_NO_VTABLE CCallStackEntry : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public IDispatchImpl<ICallStackEntry, &IID_ICallStackEntry, &LIBID_BRKPTMANLib>
{
public:
    CCallStackEntry ()
    {
    }


DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCallStackEntry)
    COM_INTERFACE_ENTRY(ICallStackEntry)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

    STDMETHOD(get_Instance)(/*[out, retval]*/ BSTR *pVal);

public:
    CString m_strInstance;
};


class CExclusiveSessionNotifyBPMan : public IDispatchImpl<_FCSessionNotification, &IID__FCSessionNotification, &LIBID_CSC_ONLINELib,2,0>,
                               public CComObjectRoot
{
public:
    CExclusiveSessionNotifyBPMan ();

    void SetNotifyCB (FC_CALLBACK_BASE (BPManExclusiveSessionCB) *pSessionCB);

protected:
    FC_CALLBACK_BASE (BPManExclusiveSessionCB) *m_pSessionCB;

private:
    STDMETHOD_(HRESULT, OnChanged)();
    STDMETHOD_(HRESULT, OnStateChanged)(/*[in]*/ LPUNKNOWN pSession,/*[in]*/CSC_SESSION_STATE newState,/*[in]*/HRESULT hrError);

    BEGIN_COM_MAP(CExclusiveSessionNotifyBPMan)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_FCSessionNotification)
    END_COM_MAP()
};


class CBreakpointNotify : public IDispatchImpl<_FCBreakpointNotification, &IID__FCBreakpointNotification, &LIBID_CSC_ONLINELib,2,0>,
                          public CComObjectRoot
{
public:
    CBreakpointNotify::CBreakpointNotify ();

    void SetNotifyCBs (FC_CALLBACK_BASE (StopNotifyCB) *pStopNotifyCB,
                       FC_CALLBACK_BASE (DownloadNotifyCB) *pDownloadNotifyCB);

protected:
    FC_CALLBACK_BASE (StopNotifyCB)         *m_pStopNotifyCB;
    FC_CALLBACK_BASE (DownloadNotifyCB)     *m_pDownloadNotifyCB;

private:
    STDMETHOD_ (HRESULT, OnBreakpointReached)(/*[in]*/ BSTR sInstance,
                                           /*[in]*/ long lNumber,
                                           /*[in]*/ long lQuality,
                                           /*[in]*/ BSTR sTask);
    STDMETHOD_ (HRESULT, OnConfigurationChanged)(/*[in, string]*/ BSTR sAddress);

    BEGIN_COM_MAP(CBreakpointNotify)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_FCBreakpointNotification)
    END_COM_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CBreakPointMan
class ATL_NO_VTABLE CBreakPointMan : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CBreakPointMan, &CLSID_BreakPointMan>,
    public IConnectionPointContainerImpl<CBreakPointMan>,
    public IDispatchImpl<IBreakPoint, &IID_IBreakPoint, &LIBID_BRKPTMANLib>,
    public CProxy_IBreakPointNotify< CBreakPointMan >,
    public CProxy_ICEMessageEvent< CBreakPointMan >
{
public:
    struct SessionInfo
    {
        SessionInfo ()
        : m_dwBreakpointCookie (0),
          m_dwSessionCookie (0),
          m_bEstablished (false)
        {
        }

        CComPtr<IUnknown>      m_pIUnknown;
        CComPtr<IFCBreakpoint> m_pIFCBreakpoint;
        DWORD                  m_dwBreakpointCookie;
        DWORD                  m_dwSessionCookie;
        bool                   m_bEstablished;
        CString                m_strTarget;

        CList<CComObject<CBreakPointInfo> *, CComObject<CBreakPointInfo> *> m_Breakpoints;
        CList<CComObject<CBreakPointInfo> *, CComObject<CBreakPointInfo> *> m_Stoppoints;

        CString m_strDIIConfiguration;
        CString m_strDIIResource;
    };

    CBreakPointMan ();
    ~CBreakPointMan ();

DECLARE_REGISTRY_RESOURCEID(IDR_BREAKPOINTMAN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBreakPointMan)
    COM_INTERFACE_ENTRY(IBreakPoint)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CBreakPointMan)
CONNECTION_POINT_ENTRY(IID__IBreakPointNotify)
CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()


// IBreakPoint
public:
    STDMETHOD(Init)(/*[in, string]*/ BSTR sProjectPath);
    STDMETHOD(Cleanup)();
    STDMETHOD(SetBP)(
        /*[in, string]*/ BSTR              sInstanceName, 
        /*[in]*/         EDP_POS_QUALIFIER PosQual, 
        /*[in]*/         long              X, 
        /*[in]*/         long              Y,
        /*[in]*/         long              CX, 
        /*[in]*/         long              CY,
        /*[in]*/         BOOL              bActive
    );
    STDMETHOD(ResetBP)(
        /*[in, string]*/ BSTR              sInstanceName, 
        /*[in]*/         EDP_POS_QUALIFIER PosQual, 
        /*[in]*/         long              X, 
        /*[in]*/         long              Y,
        /*[in]*/         long              CX, 
        /*[in]*/         long              CY
    );
    STDMETHOD(ActivateBP)(
        /*[in, string]*/ BSTR              sInstanceName, 
        /*[in]*/         EDP_POS_QUALIFIER PosQual, 
        /*[in]*/         long              X, 
        /*[in]*/         long              Y,
        /*[in]*/         long              CX, 
        /*[in]*/         long              CY
    );
    STDMETHOD(DeactivateBP)(
        /*[in, string]*/ BSTR              sInstanceName, 
        /*[in]*/         EDP_POS_QUALIFIER PosQual, 
        /*[in]*/         long              X, 
        /*[in]*/         long              Y,
        /*[in]*/         long              CX, 
        /*[in]*/         long              CY
    );
    STDMETHOD(UpdateAllBPs)(/*[in, string]*/ BSTR sInstanceName);
    STDMETHOD(Attach)(/*[in]*/ BSTR sTarget, /*[in]*/ LPUNKNOWN pUnk);
    STDMETHOD(Detach)(/*[in, string]*/ BSTR sTarget);
    STDMETHOD(ContinueTask)(/*[in, string]*/ BSTR sInstanceName, /*[in]*/ long mode);
    STDMETHOD(GetCallStackEntry)(/*[in, string]*/ BSTR sInstanceName, /*[in]*/ long lLevel, /*[out]*/ LPUNKNOWN *pEntry);
    STDMETHOD(GetBPs)(/*[out]*/ SAFEARRAY/*(IDispatch)*/ **pBPInfos);
    STDMETHOD(ResetAllBPs)(/*[in]*/ BSTR sInstanceName);
    STDMETHOD(ShowBP)(/*[in]*/ BSTR sInstanceName, /*[in]*/ long lNumber);
    STDMETHOD(GetSyncMode)(/*[out]*/ BOOL *pbSyncOnConnect, /*[out]*/ BOOL *pbSyncOnStateChange);
    STDMETHOD(SetSyncMode)(/*[in]*/ BOOL bSyncOnConnect, /*[in]*/ BOOL bSyncOnStateChange);
    STDMETHOD(Synchronize)(/*[in, string]*/ BSTR sTarget);
    STDMETHOD(ActivateAllConfigBPs)(/*[in, string]*/ BSTR sTarget);
    STDMETHOD(DeactivateAllConfigBPs)(/*[in, string]*/ BSTR sTarget);

protected:
    bool                 m_bInitialized;
    CString              m_strProjectPath;
    CComPtr<IDebugInfo>  m_pDebugInfo;
    CMapStringToPtr      m_SessionInfos;
    bool                 m_bSyncOnConnect;
    bool                 m_bSyncOnStateChange;

    CComObject<CBreakpointNotify>                      *m_pBreakpointNotify;
    FC_CALLBACK (StopNotifyCB, CBreakPointMan)         m_StopNotifyCB;
    FC_CALLBACK (DownloadNotifyCB, CBreakPointMan)     m_DownloadNotifyCB;

    CComObject<CExclusiveSessionNotifyBPMan>              *m_pSessionNotify;
    FC_CALLBACK (BPManExclusiveSessionCB, CBreakPointMan) m_SessionNotifyCB;

    virtual void OnStopNotify (StopData *pData);
    virtual void OnDownloadNotify (DownloadData *pData);
    virtual void OnSessionNotify (LPUNKNOWN pSession, CSC_SESSION_STATE state);

private:
    HRESULT GetTypeDBIForInstance (const CString &strInstance, CComPtr<ITypeDBI> &pTypeDBI);

    HRESULT SessionInfoFromInstance (const CString &strInstance, SessionInfo *&pSessionInfo);
    HRESULT SessionInfoFromTarget (const CString &strTarget, SessionInfo *&pSessionInfo);
    HRESULT SessionInfoFromUnknown (LPUNKNOWN pUnknown, SessionInfo *&pSessionInfo);
    HRESULT LoadDII (SessionInfo *pSessionInfo, const CString &strInstance);
    CString GetTarget (const CString &strInstance);
    CString GetResource (const CString &strInstance);

    HRESULT CreateSession (const CString &strTarget, SessionInfo *&pSessionInfo);
    void RemoveAllSessions ();
    void ClearSession (SessionInfo *pSessionInfo);

    void SynchronizeBreakpoints (SessionInfo *pSessionInfo);
    void UnsubscribeBreakpoints (SessionInfo *pSessionInfo);
    HRESULT ReadBreakpoint (
        const CString&               strInstance, 
        EDP_POS_QUALIFIER            PosQual, 
        long                         lX, 
        long                         lY, 
        long                         lCX, 
        long                         lCY, 
        CComObject<CBreakPointInfo>* pBPInfo
    );
    HRESULT CheckBreakpoint (CComObject<CBreakPointInfo> *pBPInfo);
    void ClearBreakpoints (SessionInfo *pSessionInfo, const CString &strInstance);
    HRESULT SetBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo, bool bActivate);
    HRESULT ClearBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo);
    void FireBPStateChange (CComObject<CBreakPointInfo> *pBPInfo, bool bUpdate);
    void FireBPRemoved (CComObject<CBreakPointInfo> *pBPInfo);
    void AddBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo);
    void RemoveBreakpoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo);
    CComObject<CBreakPointInfo>* LookupBreakpoint (
        SessionInfo*      pSessionInfo, 
        const CString&    strInstance, 
        EDP_POS_QUALIFIER PosQual,
        long              lX, 
        long              lY, 
        long              lCX, 
        long              lCY 
    );
    CComObject<CBreakPointInfo> *LookupDuplicateBreakpoint (SessionInfo *pSessionInfo, const CString &strInstance, long lBPNumber);

    void TaskContinued (StopData *pData);
    HRESULT AddStoppoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo);
    
    //mode: 0=continue task, 1=single step, 2=step over, 3=step out 
    HRESULT ContinueFromStoppoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo, long mode);
    void RemoveStoppoints (SessionInfo *pSessionInfo);
    void RemoveStoppoint (SessionInfo *pSessionInfo, CComObject<CBreakPointInfo> *pBPInfo);
    CComObject<CBreakPointInfo> *LookupStoppoint (SessionInfo *pSessionInfo, const CString &strInstance);

    HRESULT UpdateSingle (const CString &strInstance);
    HRESULT UpdateAll ();
    void UpdateBPs (SessionInfo *pSessionInfo);
    void UpdateSPs (SessionInfo *pSessionInfo);

    HRESULT ResetSingle (const CString &strInstance);
    HRESULT ResetAll ();

    void FireMessage (HRESULT hr);
    bool GetMessageText (HRESULT hr, CString &strMessage);

    //mode: 0=continue task, 1=single step, 2=step over, 3=step out 
    HRESULT ContinueUnambigous(long mode);

};

#endif // __BREAKPOINTMAN_H_


