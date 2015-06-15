/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/ControlMgt.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ControlMgt.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/ControlMgt.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef __CONTROL_MGT_H_
#define __CONTROL_MGT_H_

//------------------------------------------------------------------*
/**
*  class CControlMgt
*
*  @see    ....
*/
class ATL_NO_VTABLE CControlMgt : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CControlMgt, &CLSID_ControlMgt>,
	public IConnectionPointContainerImpl<CControlMgt>,
	public IDispatchImpl<IControlMgt, &IID_IControlMgt, &LIBID_CSC_OnlineServer, 2, 0>,
	public IDispatchImpl<IMessages, &IID_IMessages, &LIBID_CSC_OnlineServer, 2, 0>,
	public IDispatchImpl<IConnection, &IID_IConnection, &LIBID_CSC_OnlineServer, 2, 0>,
   	public IDispatchImpl<ICustOnlCmds, &IID_ICustOnlCmds, &LIBID_CSC_OnlineServerExt, 2, 0>,
    public CProxy_ICERemoteMessageEvent<CControlMgt>
{
// =Construction
public:
	                        CControlMgt();
                                // default ctor
    virtual                 ~CControlMgt();
                                // dtor

private:
                            CControlMgt(CControlMgt const & other);
                                // copy ctor
    CControlMgt const &    operator = (CControlMgt const & other);
                                // assignment


//--------------------------
// =Interface map
BEGIN_COM_MAP(CControlMgt)
	COM_INTERFACE_ENTRY(IControlMgt)
	COM_INTERFACE_ENTRY2(IDispatch, IControlMgt)
    COM_INTERFACE_ENTRY(IMessages)
    COM_INTERFACE_ENTRY(IConnection)
    COM_INTERFACE_ENTRY(ICustOnlCmds)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CControlMgt)
    CONNECTION_POINT_ENTRY(IID__ICERemoteMessageEvent)
END_CONNECTION_POINT_MAP()

// =Access
public:

// =Operations
public:
DECLARE_REGISTRY_RESOURCEID(IDR_DUMMY_REGSCRIPT)
    static HRESULT          SendMsgToEng(BSTR pMessage);

// =Overrides
public:
    HRESULT                 FinalConstruct();
    void                    FinalRelease();

protected:

//--------------------------
// =Interface
// IControlMgt
public:
    STDMETHOD(Login)(/*[in,string]*/ BSTR sUser, /*[in,string]*/ BSTR sPassword, /*[out,retval]*/ long* plKey); 
    STDMETHOD(Logout)(/*[in]*/ long lKey);
	STDMETHOD(ColdStartResource)(/*[in]*/ long lKey);
	STDMETHOD(WarmStartResource)(/*[in]*/ long lKey);
	STDMETHOD(PauseResource)(/*[in]*/ long lKey);
	STDMETHOD(ContinueResource)(/*[in]*/ long lKey);
	STDMETHOD(StartTaskOnResource)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTask);
	STDMETHOD(StopTaskOnResource)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTask);
	STDMETHOD(StartAllTasksOnResource)(/*[in]*/ long lKey);
	STDMETHOD(StopAllTasksOnResource)(/*[in]*/ long lKey);

// IMessages
public:
	STDMETHOD(ClearHistoryBuffer)(/*[in]*/ long lKey);
	STDMETHOD(get_MaxHistoryLength)(/*[in]*/ long lKey, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_MaxHistoryLength)(/*[in]*/ long lKey, /*[in]*/ long newVal);
	STDMETHOD(GetHistoryMessage)(/*[in]*/ long lKey, /*[in]*/ long idHistory, /*[out]*/ BSTR *sMessage, /*[out]*/ DATE *dt);
	STDMETHOD(GetMinMaxHistoryIDs)(/*[in]*/ long lKey, /*[out]*/ long *pFirst, /*[out]*/ long *pLast);
	STDMETHOD(Disconnect)(/*[in]*/ long lKey);
	STDMETHOD(Connect)(/*[in]*/ long lKey);

// IConnect
public:
    STDMETHOD(CheckConnection)();

// IDebugOutput
public:
    STDMETHOD(printString)(/*[in]*/ long id, /*[in]*/ BSTR message, /*[in]*/ DATE dt);

// ICustOnlCmds
    STDMETHOD(CustDownUpLoad)(/*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppDataDown, 
                              /*[out]*/ SAFEARRAY/*(unsigned char)*/ **ppDataUp);  
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

// =Helpers
private:
    HRESULT                 ConnectControlMessages(BOOL bConnect);
    HRESULT                 CheckKey(long lKey, BOOL bWriteAccess = FALSE);
    HRESULT                 DoSimpleCommand(UINT uCmd, UINT uTrials = 1);
    

private:
    HRESULT                 StartThread();
    HRESULT                 StopThread();
    static DWORD WINAPI     ThreadMain(LPVOID lpParam);
    HRESULT                 MessageThread();
    CCriticalSection        m_csMessageQueue;
    CList<BSTR, BSTR>       m_qMessageQueue;
    BOOL                    m_bShutdownRequest;
    HANDLE                  m_hWakeup;
    HANDLE                  m_hThread;
    DWORD                   m_dwThreadID;

// =Adaptation
protected:
    STDMETHOD(Adapt_Login)(/*[in,string]*/ BSTR sUser, /*[in,string]*/ BSTR sPassword, /*[out,retval]*/ long* plKey, BOOL *bpHandled); 
    STDMETHOD(Adapt_Logout)(/*[in]*/ long lKey, BOOL *bpHandled);
	STDMETHOD(Adapt_ColdStartResource)(/*[in]*/ long lKey, /*[out]*/ BOOL *bpHandled);
	STDMETHOD(Adapt_WarmStartResource)(/*[in]*/ long lKey, /*[out]*/ BOOL *bpHandled);
	STDMETHOD(Adapt_PauseResource)(/*[in]*/ long lKey, BOOL *bpHandled);
	STDMETHOD(Adapt_ContinueResource)(/*[in]*/ long lKey, BOOL *bpHandled);
	STDMETHOD(Adapt_StartTaskOnResource)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTask, BOOL *bpHandled);
	STDMETHOD(Adapt_StopTaskOnResource)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTask, BOOL *bpHandled);
	STDMETHOD(Adapt_StartAllTasksOnResource)(/*[in]*/ long lKey, BOOL *bpHandled);
	STDMETHOD(Adapt_StopAllTasksOnResource)(/*[in]*/ long lKey, BOOL *bpHandled);

// =Attributes
protected:

private:
    long                        m_lSessionKey;
    CSecurity::SECURITY_RIGHTS  m_SessionRights;

    static CList<CControlMgt*, CControlMgt*>  sm_ListControlMgt;
    static CCriticalSection                   sm_csListControlMgt;
};

#endif

/* ---------------------------------------------------------------------------- */
