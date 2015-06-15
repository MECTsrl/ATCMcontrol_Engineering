/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/VarSession.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: VarSession.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/VarSession.h $
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
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef _VAR_SESSION_H_
#define _VAR_SESSION_H_

//----  Aggregate Includes:   --------------------------------------*
#include "CSC_Online.h"     // interfaces
#include "opcDA.h"
#include "CSC_OnlineServer.h"
#include "CSC_OnlineServerExt.h"
#include "CpSessionNotification.h"
#include "FCSession.h"

//----  Forward Class Definitions:   -------------------------------*
class CFCVariable;
class CFCVarSession;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//----  Typedefs:    -----------------------------------------------*

typedef CMap<long, long, CFCVariable*, CFCVariable*&>           varClientHandleMap;
typedef CList<CFCVariable*, CFCVariable*&> varHandleList;

class CCommandReq : public CRefObj
{
public:
    enum {
        None = 0,
        VarWriteReq = 1,
        ColdStart,
        WarmStart,
        Pause,
        Continue,
        StartAllTasks,
        StopAllTasks,
        StopAllTask,
        StartTask,
        StopTask,
        CustDownUpLoad,
        ProjectInfoUpload,
        ProjectDownload,
        ProjectUpload,
        CustCmd1,
        CustCmd2,
        CustCmd3,
        CustCmd4,
        CustCmd5,
        LoadFile,
        SaveFile,
        Dir,
    };

    CCommandReq(int iType, HANDLE hEvent = NULL)
    { 
        m_iType = iType; 
        m_hr = S_OK;
        m_hEvent = hEvent;
        AddRef();
    }

    virtual ~CCommandReq()
    {}

    int GetType() const
    { return m_iType; }

    HRESULT GetErrorCode() const
    { return m_hr; }

    void SetErrorCode(HRESULT hr)
    { m_hr = hr; }

    void SetEvent()
    {
        if(m_hEvent) {
            ::SetEvent(m_hEvent);
        }
    }

    HANDLE GetEvent() const
    { return m_hEvent; }

protected:
    void OnFinalRelease()
    {
        if(m_hEvent) {
            CloseHandle(m_hEvent);
            m_hEvent = NULL;
        }
    }

    /**Type according to the anonymous enumeration above*/
    int m_iType;
    /**HRESULT of operation*/
    HRESULT m_hr;
    /**Event handle for synchronization*/
    HANDLE  m_hEvent;
};

/////////////////////////////////////////////////////////////////////////////
// CFCVariableWriteReq
class CFCVariableWriteReq : public CCommandReq
{
public:
//--------------------------
// =Constructor
    CFCVariableWriteReq(CFCVariable *pVariable, VARIANT value);
    ~CFCVariableWriteReq();

public:
   CFCVariable *m_pVariable;
   CComVariant  m_Value;
};

class CTaskCommand : public CCommandReq
{
public:
    CTaskCommand(int iType, BSTR sTask, HANDLE hEvent = NULL) :
      CCommandReq(iType,hEvent), m_sTask(sTask)
    {
    }

    virtual ~CTaskCommand()
    {}

    CComBSTR    m_sTask;
};

class CProjectInfoCommand : public CCommandReq
{
public:
    CProjectInfoCommand(int iType,  
                         BSTR* psProjVers,
	                     BSTR* psProjName,
	                     BSTR* psModified,
	                     BSTR* psCreated,
                         BSTR* psOwner,
	                     BSTR* psComment1,
	                     BSTR* psComment2,
	                     BSTR* psComment3, 
                         HANDLE hEvent = NULL) :
      CCommandReq(iType,hEvent), 
            m_psProjVers(psProjVers),
            m_psProjName(psProjName),
            m_psModified(psModified),
            m_psCreated(psCreated),
            m_psOwner(psOwner),
            m_psComment1(psComment1),
            m_psComment2(psComment2),
            m_psComment3(psComment3) 
    {
    }

    virtual ~CProjectInfoCommand()
    {}

    BSTR* m_psProjVers;
    BSTR* m_psProjName;
    BSTR* m_psModified;
    BSTR* m_psCreated;
    BSTR* m_psOwner;
    BSTR* m_psComment1;
    BSTR* m_psComment2;
    BSTR* m_psComment3; 
};

class CCustomCommand : public CCommandReq
{
public:
    CCustomCommand(int iType, 
                   BSTR* psNameDown,
                   BSTR* psNameUp,
                   SAFEARRAY **ppDataDown,
                   SAFEARRAY **ppDataUp,
                   HANDLE hEvent = NULL) :
      CCommandReq(iType,hEvent), 
          m_bCancel(FALSE),
          m_psNameDown(psNameDown),
          m_psNameUp(psNameUp),
          m_ppDataDown(ppDataDown),
          m_ppDataUp(ppDataUp)
    {
    }

    virtual ~CCustomCommand()
    {}

    BOOL       m_bCancel;
    BSTR       *m_psNameDown;
    BSTR       *m_psNameUp;
    SAFEARRAY **m_ppDataDown;
    SAFEARRAY **m_ppDataUp;
};


typedef CMFCObjStore<CCommandReq> CommandReqList;
typedef CMFCObjStore<CFCVariableWriteReq> varWriteReqList;
/////////////////////////////////////////////////////////////////////////////
// CFCVarSessionThread
class ATL_NO_VTABLE CFCVarSessionThread : 
	public CComObjectRootEx<CComMultiThreadModel>,
    public IAdviseSink
{
public:
//--------------------------
// =Constructor
    CFCVarSessionThread();
    ~CFCVarSessionThread();


//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCVarSessionThread)
    COM_INTERFACE_ENTRY(IAdviseSink)
END_COM_MAP()

// IAdviseData
    STDMETHODIMP_(void)     OnDataChange(LPFORMATETC, LPSTGMEDIUM);
    STDMETHODIMP_(void)     OnViewChange(DWORD, LONG);
    STDMETHODIMP_(void)     OnRename(LPMONIKER);
    STDMETHODIMP_(void)     OnSave(void);
    STDMETHODIMP_(void)     OnClose(void);

public:
    HRESULT StartThread(CFCVarSession *pRawVarSession, long cookie);
    HRESULT StopThread();
    HRESULT SubscribeVariable(CFCVariable*);
    HRESULT UnSubscribeVariable(CFCVariable*);
    HRESULT SubmitWriteRequest(CFCVariableWriteReq*);
    HRESULT SubmitCommandRequest(CCommandReq*);

private:
    static DWORD WINAPI ThreadMain(LPVOID lpParam);
    HRESULT ConnectThread();
    HRESULT DoConnect();
    HRESULT ConnectControl();
    HRESULT ConnectOPC();
    HRESULT DoDisconnect();
    HRESULT DoCheckConnection();
    HRESULT DoWorkItem();
    void ExecuteCommand(CCommandReq* pReq);
    void InitiateConnection();
    void TerminateConnection();
    void ForceResubscription();
    HRESULT Login();
    HRESULT Logout();

    HRESULT CustDownUpLoad(CCustomCommand* pReq);  
    HRESULT ProjectInfoUpload(CProjectInfoCommand* pReq);
    HRESULT ProjectDownload(CCustomCommand* pReq);
    HRESULT ProjectUpload(CCustomCommand* pReq);
    HRESULT CustCmd1(CCustomCommand* pReq);  
    HRESULT CustCmd2(CCustomCommand* pReq);  
    HRESULT CustCmd3(CCustomCommand* pReq);  
    HRESULT CustCmd4(CCustomCommand* pReq);  
    HRESULT CustCmd5(CCustomCommand* pReq); 
    HRESULT LoadFile(CCustomCommand* pReq);
    HRESULT SaveFile(CCustomCommand* pReq);
    HRESULT Dir(CCustomCommand* pReq);


private:
    long          m_cookie;
    /** Login key 0 - invalid, else valide*/
    long          m_lKey;
    /** associated raw var session object*/
    CFCVarSession *m_pRawVarSession;
    /** Interface used to communicate notifications to var session over
        apartment boundary.
    */
    _FCVarSessionNotificationInternal *m_pVarSession;
    /**Stream used to pass interface _FCVarSessionNotificationInternal to thread*/
    IStream      *m_pVarSessionStream;
    /**Wake up thread due to a pending request*/
    HANDLE        m_hThreadWakeUp;
    BSTR          m_sMachineName;
    /**Interface ptr Control management COM object*/
    IControlMgt  *m_pControlMgt;
    /**Interface ptr only for check of connection */
    IConnection  *m_pCheckConnection;
    /**Interface ptr to OPC server COM object*/
    IOPCServer   *m_pOPCServer;
    /**Group within OPC server for variable management*/
    IOPCItemMgt  *m_pGroup1;
    /**Group handle*/
    OPCHANDLE     m_hGroup1;
    /**Shutdown/Disconnect request - thread termination*/
    bool          m_bShutdownRequest;
    /**Variables or OPC items to add to group pending - see m_addVarsList*/
    bool          m_bAddVarsRequest;
    /**Variables or OPC items to remove from group pending - see m_removeVarsList*/
    bool          m_bRemVarsRequest;
    /**Variables or OPC items to write request pending - see m_writeReqList*/                
    bool          m_bWriteVarsRequest;
    /**Command(s) request pending - see m_CommandList*/
    bool          m_bCommandRequest;
    /**Connected ?*/
    bool          m_bConnected;
    /**List of variables or OPC items to add to group per request*/
    varHandleList m_addVarsList;
    /**List of variables or OPC items to remove from group per request*/
    varHandleList m_removeVarsList;
    /**List of variables or OPC items to write per request*/
    varWriteReqList m_writeReqList;
    /**List of commands to perform with control*/
    CommandReqList m_CommandList;
};


/////////////////////////////////////////////////////////////////////////////
// CFCVarSession
class ATL_NO_VTABLE CFCVarSession : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IFCConnect, &IID_IFCConnect, &LIBID_CSC_ONLINELib>,
	public IDispatchImpl<IFCVarSession, &IID_IFCVarSession, &LIBID_CSC_ONLINELib>,
    public IDispatchImpl<IFCCustOnlCmds, &IID_IFCCustOnlCmds, &LIBID_CSC_ONLINELib>,
	public IConnectionPointContainerImpl<CFCVarSession>,
    public CProxy_FCSessionNotification<CFCVarSession>,
    public IProvideClassInfo2Impl<&CLSID_FCVarSession, &IID__FCSessionNotification, &LIBID_CSC_ONLINELib>,
    public IObjectSafetyImpl<CFCVarSession, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
    public _FCVarSessionNotificationInternal             // object implemented in main apartment, called via marshalled ptr from MTA
{

friend class CFCVarSessionThread;
public:
//--------------------------
// =Constructor
    CFCVarSession();
    ~CFCVarSession();

//--------------------------
// =Interface map
BEGIN_COM_MAP(CFCVarSession)
    COM_INTERFACE_ENTRY(IFCConnect)
	COM_INTERFACE_ENTRY(IFCVarSession)
    COM_INTERFACE_ENTRY(IFCCustOnlCmds)
	COM_INTERFACE_ENTRY2(IDispatch, IFCConnect)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(_FCVarSessionNotificationInternal)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CFCVarSession)
    CONNECTION_POINT_ENTRY(IID__FCSessionNotification)
END_CONNECTION_POINT_MAP()

//--------------------------
// =Operations


//--------------------------
// =Interface

// IFCConnect
	STDMETHOD(Connect)(/*[in, string]*/BSTR sUserName,/*[in, string]*/BSTR sPassword);
	STDMETHOD(Disconnect)();
	STDMETHOD(get_SessionState)(/*[out, retval]*/ CSC_SESSION_STATE *pVal);
	STDMETHOD(get_MachineName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsConnected)(/*[out, retval]*/ BOOL *pVal);
    STDMETHOD(get_CurrentUser)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(ForceDisconnect)(/*[in, string]*/BSTR sUserName,/*[in, string]*/BSTR sPassword,
        /*[in, string]*/ BSTR sEngMachineName);


// IFCVarSession

	STDMETHOD(StartResource)();
	STDMETHOD(StopResource)();
	STDMETHOD(ColdstartResource)();
	STDMETHOD(WarmstartResource)();
	STDMETHOD(StartTaskOnResource)(/*[in, string]*/ BSTR sTask);
	STDMETHOD(StopTaskOnResource)(/*[in, string]*/ BSTR sTask);
	STDMETHOD(StartAllTasksOnResource)();
	STDMETHOD(StopAllTasksOnResource)();


// IFCCustOnlCmds
    STDMETHOD(CustDownUpLoad)(/*[in]*/ SAFEARRAY **ppDataDown, /*[out, retval]*/ SAFEARRAY **ppDataUp);  
    STDMETHOD(ProjectInfoUpload)(/*[out, string]*/ BSTR* psProjVers,
	                            /*[out, string]*/ BSTR* psProjName,
	                            /*[out, string]*/ BSTR* psModified,
	                            /*[out, string]*/ BSTR* psCreated,
                                /*[out, string]*/ BSTR* psOwner,
	                            /*[out, string]*/ BSTR* psComment1,
	                            /*[out, string]*/ BSTR* psComment2,
	                            /*[out, string]*/ BSTR* psComment3);
    STDMETHOD(ProjectDownload)(/*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppDataDown);
    STDMETHOD(ProjectUpload)(/*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);
    STDMETHOD(CustCmd1)(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                        /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);  
    STDMETHOD(CustCmd2)(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                        /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);  
    STDMETHOD(CustCmd3)(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                        /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);  
    STDMETHOD(CustCmd4)(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                        /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);  
    STDMETHOD(CustCmd5)(/*[in]*/  SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                        /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);  
    STDMETHOD(LoadFile)(/*[in, string]*/ BSTR sName, /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppData);
    STDMETHOD(SaveFile)(/*[in, string]*/ BSTR sName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppData);
    STDMETHOD(Dir)(/*[out]*/ SAFEARRAY/*(BSTR)*/ **ppFileNames);



// _FCVarSessionNotificationInternal
    STDMETHOD(OnStateChanged)(/*[in]*/ long cookie, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError);
    STDMETHOD(OnDataReceived)(/*[in]*/ long cookie, /*[in]*/ long hglobal);


    void AddVariable(CFCVariable *pVar);
    void RemoveVariable(CFCVariable *pVar);
    HRESULT WriteValueAsync(CFCVariable *pVar, VARIANT val);
    void SetAddressInfo(CString &, CGuid& , CString &);
//--------------------------
// =Helpers
private:
    void                    SetAllQualitiesBad();
    void                    FireStateChangeHelper(CSC_SESSION_STATE newState, HRESULT hError = S_OK);
    HRESULT                 CreateCommand(int,CCommandReq*&);
    HRESULT                 CreateTaskCommand(int iType, CCommandReq* & pReq, BSTR sTask);
    HRESULT                 CreateProjectInfoCommand(CCommandReq* & pReq, 
                                                     BSTR* psProjVers,
	                                                 BSTR* psProjName,
	                                                 BSTR* psModified,
	                                                 BSTR* psCreated,
                                                     BSTR* psOwner,
	                                                 BSTR* psComment1,
	                                                 BSTR* psComment2,
	                                                 BSTR* psComment3);
    HRESULT                 CreateUpDownloadCommmand(int iType, CCommandReq* & pReq,
                                                    BSTR* psNameDown,
                                                    BSTR* psNameUp,
                                                    SAFEARRAY **ppDataDown, 
                                                    SAFEARRAY **ppDataUp);
//--------------------------
// =Attributes
private:
    CString                 m_sMachineName;
    /**User for login*/
    CComBSTR                m_sUser;
    /**Password for login*/
    CComBSTR                m_sPassword;
    /**Init string for custom connect via ICustomConnect*/
    CString                 m_strInit;
    /**default CLSID for OPC server used if m_clsidConn = NULLGUID*/
    CGuid                   m_clsidOPC;
    /**default CLSID for ControlMgt used if m_clsidConn = NULLGUID*/
    CGuid                   m_clsidCtrl;
    /**CLSID for coclass impl. ICustomConnect may be NULLGUID*/
    CGuid                   m_clsidConn;

    CSC_SESSION_STATE       m_state;

    OPCHANDLE               m_dwNextClientHandle;
    varClientHandleMap      m_varClientHandleMap;

    CFCVarSessionThread    *m_pVarSessionThread;
    long                    m_cookieThread;
    CCriticalSection        m_cs;
};

/////////////////////////////////////////////////////////////////////
#endif // _VAR_SESSION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: VarSession.h $
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
 * User: Jd           Date: 25.03.03   Time: 15:53
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Asynchron holding of safearrays.
 * 
 * *****************  Version 8  *****************
 * User: Jd           Date: 20.03.03   Time: 16:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * More custom online commands.
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 10.03.03   Time: 10:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Introduced FireStateChangeHelper to trace all state changes on session.
 * 
 * *****************  Version 6  *****************
 * User: Jd           Date: 25.02.03   Time: 11:49
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Custom Online commands in non exclusive sessions.
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 10.07.01   Time: 12:29
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 19.03.01   Time: 19:02
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision with respect to new target connection information
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:45
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 *==
 *----------------------------------------------------------------------------*
*/
