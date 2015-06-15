/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/MsgSession.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: MsgSession.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/MsgSession.h $
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
#ifndef _MSG_SESSION_H_
#define _MSG_SESSION_H_

//----  Aggregate Includes:   --------------------------------------*
#include "CEMsg.h"
#include "CSC_Online.h"     // interfaces
#include "CSC_OnlineServer.h"
#include "CpSessionNotification.h"
#include "CpCEMessageEvent.h"

//----  Forward Class Definitions:   -------------------------------*
class CFCMessageSession;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



/////////////////////////////////////////////////////////////////////////////
// CFCMessageSessionThread
class ATL_NO_VTABLE CFCMessageSessionThread : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<_ICERemoteMessageEvent, &IID__ICERemoteMessageEvent, &LIBID_CSC_OnlineServer,2,0>
{
//--------------------------
// =Constructor
public:
    CFCMessageSessionThread();
    ~CFCMessageSessionThread();


//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCMessageSessionThread)
	COM_INTERFACE_ENTRY(_ICERemoteMessageEvent)
END_COM_MAP()

//--------------------------
// =Helpers
public:
    HRESULT StartThread(CFCMessageSession *pRawMsgSession, long cookie);
    HRESULT StopThread();

public:
// _ICERemoteMessageEvent
	STDMETHOD(OnNewMessage)(/*[in, string]*/BSTR sMessage);

//--------------------------
// Helpers
private:
    HRESULT                 ConnectToOnlineServer(IMessages **ppMessages);
    static DWORD WINAPI     ThreadMain(LPVOID lpParam);
    HRESULT                 ConnectThread();
    HRESULT                 DoConnect();
    HRESULT                 DoDisconnect();
    HRESULT                 DoCheckConnection();
    HRESULT                 Login();
    HRESULT                 Logout();

private:
    long                    m_cookie;
    BSTR                    m_sNodeName;
    HANDLE                  m_hShutdownRequest;
    bool                    m_bShutdownRequest;
    _FCMessageSessionNotificationInternal* m_pMsgSession;
    IStream*                m_pMsgSessionStream;

    DWORD                   m_dwMsgNotifyCookie;
    IMessages*              m_pMessage;
    /** Only used for check of connection */
    IConnection*            m_pCheckConnection;
    /** Raw message session object*/
    CFCMessageSession*      m_pRawMsgSession;
    /** Login key 0 - invalid, else valide*/
    long                    m_lKey;
};

/////////////////////////////////////////////////////////////////////////////
// CFCMessageSession
class ATL_NO_VTABLE CFCMessageSession : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IFCConnect, &IID_IFCConnect, &LIBID_CSC_ONLINELib>,
	public IConnectionPointContainerImpl<CFCMessageSession>,
    public CProxy_ICEMessageEvent<CFCMessageSession>,
    public CProxy_FCSessionNotification<CFCMessageSession>,
    public _FCMessageSessionNotificationInternal     // object implemented in main apartment, called via marshalled ptr from MTA
{
friend class CFCMessageSessionThread;
//--------------------------
// =Constructor
public:
    CFCMessageSession();
    ~CFCMessageSession();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCMessageSession)
	COM_INTERFACE_ENTRY(IFCConnect)
	COM_INTERFACE_ENTRY2(IDispatch, IFCConnect)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(_FCMessageSessionNotificationInternal)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CFCMessageSession)
    CONNECTION_POINT_ENTRY(IID__FCSessionNotification)
    CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()

//--------------------------
// =Operations
public:
// _FCMessageSessionNotificationInternal
    STDMETHOD(OnStateChanged)(/*[in]*/ long cookie, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError);
    STDMETHOD(OnMessageReceived)(/*[in]*/ long cookie, /*[in]*/ BSTR sMessage);
    void SetAddressInfo(CString &, CString &, CGuid& , CString &);

//--------------------------
// =Interface
// IFCConnect
public:
	STDMETHOD(Connect)(/*[in, string]*/BSTR sUserName,/*[in, string]*/BSTR sPassword);
	STDMETHOD(Disconnect)(void);
	STDMETHOD(get_MachineName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsConnected)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_SessionState)(/*[out, retval]*/CSC_SESSION_STATE *);
    STDMETHOD(get_CurrentUser)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(ForceDisconnect)(/*[in, string]*/ BSTR sUserName, /*[in, string]*/BSTR sPassword,
        /*[in, string]*/ BSTR sEngMachineName);

// IFCMessageHistory
public:
	STDMETHOD(ClearHistoryBuffer)();
	STDMETHOD(get_MaxHistoryLength)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaxHistoryLength)(/*[in]*/long newVal);
	STDMETHOD(GetHistoryMessage)(/*[in]*/long idHistory,/*[out]*/ BSTR *sMessage, /*[out]*/ DATE *dt);
	STDMETHOD(GetMinMaxHistoryIDs)(/*[out]*/long *pFirst, /*[out]*/ long *pLast);

// Helpers
private:
    void FireStateChangeHelper(CSC_SESSION_STATE newState, HRESULT hError = S_OK);

//--------------------------
// =Attributes
private:
    /**full address: host machine name, clsid of server, init string*/
    CString                 m_strAddress;
    /**host machine name*/
    CString                 m_strMachineName;
    /**User name for login*/
    CComBSTR                m_sUser;
    /**Password for login*/
    CComBSTR                m_sPassword;
    /**Init string for custom connect*/
    CString                 m_strInit;
    /** default CLSID for ControlMgt used if m_clsidConn = NULLGUID*/
    CGuid                   m_clsidCtrl;
    /** CLSID for coclass impl. ICustomConnect may be NULLGUID*/
    CGuid                   m_clsidConn;
    /***/
    CSC_SESSION_STATE       m_state;
    long                    m_cookieThread;
    CFCMessageSessionThread *m_pSessionThread;
};

/////////////////////////////////////////////////////////////////////
#endif // _MSG_SESSION_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: MsgSession.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 9  *****************
 * User: Su           Date: 10.03.03   Time: 10:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Introduced FireStateChangeHelper to trace all state changes on session.
 * 
 * *****************  Version 8  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 10.07.01   Time: 12:25
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 9.05.01    Time: 12:51
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * CFCMessageSession::SetAddressInfo needs full address as well.
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 25.04.01   Time: 11:40
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
 * Replaced IFCMessageSession by IFCConnect.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:45
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/
