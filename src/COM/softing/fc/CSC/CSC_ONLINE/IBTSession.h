/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/IBTSession.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: IBTSession.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/IBTSession.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  14.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __IBTSESSION_H_
#define __IBTSESSION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "CSC_OnlineServer.h"     // interfaces
#include "CSC_Online.h"
#include "CpTransferNotification.h"
#include "CpSessionNotification.h"
#include "CpBreakpointNotification.h"



//----  Forward Class Definitions:   -------------------------------*
class   CDomainsTransfer;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CFCCommissioningSession
class ATL_NO_VTABLE CFCCommissioningSession : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IFCConnect, &IID_IFCConnect, &LIBID_CSC_ONLINELib>,
	public IDispatchImpl<IFCDownload, &IID_IFCDownload, &LIBID_CSC_ONLINELib>,
	public IDispatchImpl<IFCBreakpoint, &IID_IFCBreakpoint, &LIBID_CSC_ONLINELib>,
	public IConnectionPointContainerImpl<CFCCommissioningSession>,
    public CProxy_FCSessionNotification<CFCCommissioningSession>,
    public CProxy_FCTransferNotification<CFCCommissioningSession>,
	public IDispatchImpl<_FCCommissioningMgtNotification, &IID__FCCommissioningMgtNotification, &LIBID_CSC_OnlineServer,2,0>,
    public _FCInternalTransferProgress,
    public CProxy_FCBreakpointNotification< CFCCommissioningSession >,
	public IDispatchImpl<_FCRemoteBreakpointNotification, &IID__FCRemoteBreakpointNotification, &LIBID_CSC_OnlineServer,2,0>  // STA sink from CSC_OnlSrv!
{
//--------------------------
// =Constructor
public:
    CFCCommissioningSession();
    ~CFCCommissioningSession();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCCommissioningSession)
    COM_INTERFACE_ENTRY(IFCConnect)
	COM_INTERFACE_ENTRY(IFCDownload)
    COM_INTERFACE_ENTRY(IFCBreakpoint)
	COM_INTERFACE_ENTRY2(IDispatch, IFCConnect)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(_FCCommissioningMgtNotification)
	COM_INTERFACE_ENTRY(_FCInternalTransferProgress)
    COM_INTERFACE_ENTRY(_FCRemoteBreakpointNotification)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CFCCommissioningSession)
    CONNECTION_POINT_ENTRY(IID__FCSessionNotification)
    CONNECTION_POINT_ENTRY(IID__FCTransferNotification)
    CONNECTION_POINT_ENTRY(IID__FCBreakpointNotification)
END_CONNECTION_POINT_MAP()

//--------------------------
// =Operations
public:
    void SetAddressInfo(CString &, CString &, CGuid& , CString &);
//--------------------------
// =Interface
// IFCConnect
	STDMETHOD(Connect)(/*[in, string]*/ BSTR sUserName,/*[in, string]*/ BSTR sPassword);
	STDMETHOD(Disconnect)(void);
	STDMETHOD(ForceDisconnect)(/*[in, string]*/ BSTR sUserName, /*[in, string]*/ BSTR sPassword, /*[in, string]*/ BSTR sEngMachineName);
	STDMETHOD(get_SessionState)(/*[out, retval]*/ CSC_SESSION_STATE *pVal);
	STDMETHOD(get_CurrentUser)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_MachineName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsConnected)(/*[out, retval]*/ BOOL *pVal);

// IFCDownload
	STDMETHOD(DownloadDomainList)(/*[in]*/ CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sListFileName, /*[out, retval]*/ DWORD *dwCookie);
	STDMETHOD(UploadDomainList)(/*[in]*/ CSC_TRANSFER_TARGET source,/*[in, string]*/ BSTR sListFileName, /*[out, retval]*/ DWORD *dwCookie);
	STDMETHOD(DeleteDomainList)(/*[in]*/ CSC_TRANSFER_TARGET src,/*[in, string]*/ BSTR sListFileName, /*[out, retval]*/ DWORD *dwCookie);
	STDMETHOD(DeleteAllDomains)(/*[in]*/ CSC_TRANSFER_TARGET target);
	STDMETHOD(AbortTransfer)(/*[in]*/ DWORD dwCookie);
	STDMETHOD(StartOnlineChange)(/*[in]*/ CSC_TRANSFER_TARGET target);
	STDMETHOD(CommitOnlineChange)(/*[in]*/ CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sResource, /*[in, string]*/BSTR sProjectName,/*[in, string]*/ BSTR sProjectGUID, /*[in, string]*/ BSTR sDownloadGUID);
	STDMETHOD(CancelOnlineChange)(/*[in]*/ CSC_TRANSFER_TARGET target);
	STDMETHOD(get_DLoadAttribute)(/*[in]*/ CSC_TRANSFER_TARGET src, /*[in, string]*/ BSTR sAttributeName, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(ClearResource)(/*[in]*/ CSC_TRANSFER_TARGET target);
    STDMETHOD(FlashResource)();
// IFCBreakpoint
	STDMETHOD(SetBreakpoint)(/*[in,string]*/ BSTR sInstanceName, /*[in]*/ long lBreakpointNumber, /*[out, retval]*/ long *hBP);
	STDMETHOD(ClearBreakpoint)(/*[in]*/ long hBP);
	STDMETHOD(ClearAllBreakpoints)();
    STDMETHOD(ContinueTask)(/*[in, string]*/ BSTR sTaskName);
    STDMETHOD(SingleStep)(/*[in, string]*/ BSTR sTaskName);
    STDMETHOD(StepOver)(/*[in, string]*/ BSTR sTaskName);
    STDMETHOD(StepOut)(/*[in, string]*/ BSTR sTaskName);
    STDMETHOD(put_ConnectionID)(/* [string][in] */ BSTR sId);




// _FCCommissioningMgtNotification
    STDMETHOD(OnUserLoggedin)(/*[in, string]*/ BSTR sUserName, /*[in, string]*/ BSTR sEngMachineName, BOOL bLoggingIn);

// _FCInternalTransferProgress
    STDMETHOD(OnTransferProgress)(/*[in]*/ DWORD dwCookie, /*[in, string]*/ BSTR sCurrentDomainName, /*[in]*/ CSC_TRANSFER_STATE state, /*[in]*/ float fltPercentDone);
    STDMETHOD(OnTransferError)(/*[in]*/ DWORD dwCookie, /*[in, string]*/ BSTR sCurrentDomainName, /*[in]*/ CSC_TRANSFER_STATE state, /*[in]*/ HRESULT hError);

// _IFCSessionConnControlInternal
	STDMETHOD(DoConnect)();
	STDMETHOD(DoDisconnect)(CSC_SESSION_STATE newState = not_connected, HRESULT hError = S_OK);
	STDMETHOD(DoCheckConnection)();


// _FCRemoteBreakpointNotification
    STDMETHOD(OnBreakpointReached)(/* [in] */ BSTR sTaskName,/* [in] */ long bpQuality,
            /* [in] */ SAFEARRAY* arrsInstances,
            /* [in] */ SAFEARRAY* arrlBreakpointNumber);
	STDMETHOD(OnConfigurationChanged)();


// Helpers
private:
    HRESULT                 ConnectToOnlineServer(ICommissioningMgt **ppUnk);
    void                    FireStateChangeHelper(CSC_SESSION_STATE newState, HRESULT hError = S_OK);
    HRESULT                 TryLoginForCommissioning(void);
    HRESULT                 GetResourcePtr(ITransfer **ppRes);

static DWORD WINAPI         ThreadMain(LPVOID lpParam);
    HRESULT                 ConnectThread();


// Attributes
    /** Fully qualified address e.g. {w-xy,GUID,initstring}*/
    CString                 m_strAddress;
    /** host machine name to connect to*/
    CString                 m_strMachineName;
    /** Initialization string for custom connect*/
    CString                 m_strInit;
    /** Connection ID, usually PC1 */
    CString                 m_Id;
    /** default CLSID for CommissioningMgt used if m_clsidConn = NULLGUID*/
    CGuid                   m_clsidComm;
    /** CLSID for coclass impl. ICustomConnect may be NULLGUID*/
    CGuid                   m_clsidConn;
    /** Actual session state*/
    CSC_SESSION_STATE       m_state;
    /** Currently requesting user*/
    CString                 m_strRequestingUser;
    /** User actually logged in, if any*/
    CString                 m_strCurrentUser;
    /**Password for login*/
    CComBSTR                m_sPassword;
    /** iff m_state >= waiting_for_exclusive*/
    ICommissioningMgt      *m_pCommMgt;  
    /**Interface ptr only for check of connection */
    IConnection            *m_pCheckConnection;
    /**iff m_state == exclusive_established*/
    ITransfer              *m_pResource;
    /** transfer session key as authorization*/
    LONG                    m_lKey;
    DWORD                   m_dwTSessionNotifyCookie;
    DWORD                   m_dwBreakpointNotifyCookie;

    CDomainsTransfer       *m_pCurrentJob;

    /** Critical section used by _IFCSessionConnControlInternal in MTA context/CheckThread context*/
    CCriticalSection        m_cs;
    bool                    m_bRunning;
    bool                    m_bConnect;
    HANDLE                  m_hWakeup;
};

/////////////////////////////////////////////////////////////////////

#endif // __IBTSESSION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: IBTSession.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 13  *****************
 * User: Su           Date: 10.03.03   Time: 10:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 12  *****************
 * User: Jd           Date: 25.02.03   Time: 11:49
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Custom Online commands in non exclusive sessions.
 * 
 * *****************  Version 11  *****************
 * User: Jd           Date: 4.02.03    Time: 14:40
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 10  *****************
 * User: Jd           Date: 4.02.03    Time: 9:21
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Changes for new callback interface into engineering (custom online
 * commands).
 * 
 * *****************  Version 9  *****************
 * User: Ed           Date: 6/14/02    Time: 11:45a
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 8  *****************
 * User: Su           Date: 17.07.01   Time: 17:59
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 10.07.01   Time: 12:23
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 25.04.01   Time: 11:39
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 19.03.01   Time: 19:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision with respect to new target connection information
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 15.03.01   Time: 19:44
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Removed not used/implemented IF methods.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:46
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/
