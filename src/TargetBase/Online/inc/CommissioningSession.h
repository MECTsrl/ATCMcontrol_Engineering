/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CommissioningSession.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommissioningSession.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CommissioningSession.h $
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

#ifndef __COMMISSIONING_SESSION_H_
#define __COMMISSIONING_SESSION_H_

//----  Aggregate Includes:   --------------------------------------*
#include "OnlineChangesHelper.h"

//----  Forward Class Definitions:   -------------------------------*
class   CInternetSession;
class   CFtpConnection;
class   CVisTransferFTP;
class   CVisTransferLocal;


//------------------------------------------------------------------*
/**
*  class CCommissioningSession
*
*  @see    ....
*/
class ATL_NO_VTABLE CCommissioningSession : 
	public CComObjectRootEx<CComMultiThreadModel>,
//	public CComCoClass<CCommissioningSession, &CLSID_CommissioningMgt>,
	public IDispatchImpl<ITransfer, &IID_ITransfer, &LIBID_CSC_OnlineServer, 2, 0>,
	public IDispatchImpl<IBreakpoint, &IID_IBreakpoint, &LIBID_CSC_OnlineServer, 2, 0>,
	public IDispatchImpl<IConnection, &IID_IConnection, &LIBID_CSC_OnlineServer, 2, 0>,
	public IConnectionPointContainerImpl<CCommissioningSession>,
	public CProxy_FCRemoteBreakpointNotification< CCommissioningSession >
{
// easier to initialize attributes of session
friend  class CCommissioningMgt;

// =Construction
public:
	                        CCommissioningSession();
                                // default ctor
    virtual                 ~CCommissioningSession();
                                // dtor

private:
                            CCommissioningSession(CCommissioningSession const & other);
                                // copy ctor
    CCommissioningSession const &    operator = (CCommissioningSession const & other);
                                // assignment


//--------------------------
// =Interface map
BEGIN_COM_MAP(CCommissioningSession)
	COM_INTERFACE_ENTRY(ITransfer)
	COM_INTERFACE_ENTRY2(IDispatch, ITransfer)
	COM_INTERFACE_ENTRY(IBreakpoint)
    COM_INTERFACE_ENTRY(IConnection)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CCommissioningSession)
    CONNECTION_POINT_ENTRY(IID__FCRemoteBreakpointNotification)
END_CONNECTION_POINT_MAP()

// =Access
public:

// =Operations
public:
    HRESULT                 Connect();
    HRESULT                 Disconnect();

//--------------------------
// =Interface
// ITransfer
public:
	STDMETHOD(StartOnlineChange)(long lKey, CSC_TRANSFER_TARGET target);
	STDMETHOD(CommitOnlineChange)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sResource, /*[in, string] */ BSTR sProjectName, /*[in, string]*/ BSTR sProjectGUID, /*[in, string]*/ BSTR sDownloadGUID);
	STDMETHOD(CancelOnlineChange)(long lKey, CSC_TRANSFER_TARGET target);
	STDMETHOD(DownloadDomain)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppContent);
	STDMETHOD(UploadDomain)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, /*[out, retval]*/ SAFEARRAY/*(unsigned char)*/ **ppContent);
	STDMETHOD(DeleteAllDomains)(long lKey, CSC_TRANSFER_TARGET target);
	STDMETHOD(DeleteDomain)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName);
	STDMETHOD(GetDomainList)(long lKey, CSC_TRANSFER_TARGET target, /*[out, retval]*/ SAFEARRAY **ppListDomainNames);
	STDMETHOD(get_DLoadAttribute)(/*[in]*/ long lKey, /*[in]*/ CSC_TRANSFER_TARGET src, /*[in, string]*/ BSTR sAtributeName, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(ClearResource)(long lKey, CSC_TRANSFER_TARGET target);
    STDMETHOD(FlashResource)(/*[in]*/ long lKey);



// IBreakpoint
public:
	STDMETHOD(SetBreakpoint)(long lKey, BSTR sInstanceName, long lBreakpointNumber, long * hBP);
	STDMETHOD(ClearBreakpoint)(long lKey, long hBP);
	STDMETHOD(ClearAllBreakpoints)(long lKey);
	STDMETHOD(ContinueTask)(long lKey, BSTR sTaskName);
    STDMETHOD(SingleStep)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTaskName);
    STDMETHOD(StepOver)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTaskName);
    STDMETHOD(StepOut)(/*[in]*/ long lKey, /*[in, string]*/ BSTR sTaskName);

// IConnection
public:
    STDMETHOD(CheckConnection)();

// internal interfaces
protected:
	STDMETHOD(ibnBreakPointNotify)(BSTR taskName, LONG bpQuality, BSTR instanceName, LONG breakpointNumber);
    STDMETHOD(ibnConfigurationChanged)(void);
	STDMETHOD(DropSession)();
	STDMETHOD(CheckSession)();

// =Helpers
private:
    HRESULT                 ConnectToControlResource();
    HRESULT                 ConnectToControlService();
    HRESULT                 ConnectToConsole();
    HRESULT                 CloseControl();
    HRESULT                 CloseConsole();
    HRESULT                 CheckKey(long lKey);

    HRESULT                 UpdateSystemVariables(CSC_TRANSFER_TARGET target, BOOL bClearedResource, LPCTSTR pszResource, LPCTSTR pszProjectName, LPCTSTR pszProjectGUID, LPCTSTR pszDownloadGUID);
    HRESULT                 DownloadToControl(BSTR sDownloadGuid);

    HRESULT                 DL_Download_20x(BSTR sDownloadGuid);
    HRESULT                 DL_Download_210(BSTR sDownloadGuid);
	
	HRESULT					GetCompiledFWVersion(UINT *upFirmware);

	HRESULT					DL_Changes(BSTR sDownloadGuid);
	
	HRESULT					DL_BuildFlashFile(BSTR sDownloadGuid);
	HRESULT					DL_Flash();

	HRESULT					DL_ClearFlash();

	BOOL					AppendData(BYTE **ppData, UINT *pLen, BYTE *pAppendData, UINT uAppendLen);
	BOOL					AppendFile(BYTE **ppData, UINT *pLen, LPCTSTR lpFileName);
	BOOL					LoadFile(BYTE **ppData, UINT *pLen, LPCTSTR lpFileName);

	HRESULT					PollControl(BOOL bLogon, UINT uRetry, UINT uCmd);

	int						GetNumberOfTasks();

	HRESULT					SendMsgToEng(BSTR pszMessage);

// =Attributes
protected:

private:
    CString                 m_strConfigName;
    CString                 m_strResourceName;
                                // current config/resource name as in IEC program, set after first Breakpoint!!

    long                    m_lSessionKey;

    CVisTransferLocal      *m_pResource;
    CVisTransferLocal      *m_pVisTransfer;     // use local copy as transport to console

    CString                 m_strLoggedInUserName;
    CString                 m_strEngMachineName;
    BOOL                    m_bCtrlDebugSessionOn;
    BOOL                    m_bFlashResource;
    CCriticalSection        m_cs;
	BOOL					m_bIsFullDownload;
	COnlineChangesHelper*	m_pOCHlp;
// =Adaptation
protected:
	STDMETHOD(Adapt_StartOnlineChange)(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled);
	STDMETHOD(Adapt_CommitOnlineChange)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sResource, /*[in, string] */ BSTR sProjectName, /*[in, string]*/ BSTR sProjectGUID, /*[in, string]*/ BSTR sDownloadGUID, BOOL *bpHandled);
	STDMETHOD(Adapt_CancelOnlineChange)(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled);
	STDMETHOD(Adapt_DownloadDomain)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppContent, BOOL *bpHandled);
	STDMETHOD(Adapt_UploadDomain)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, /*[out, retval]*/ SAFEARRAY/*(unsigned char)*/ **ppContent, BOOL *bpHandled);
	STDMETHOD(Adapt_DeleteAllDomains)(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled);
	STDMETHOD(Adapt_DeleteDomain)(long lKey, CSC_TRANSFER_TARGET target, /*[in, string]*/ BSTR sDomainName, BOOL *bpHandled);
	STDMETHOD(Adapt_GetDomainList)(long lKey, CSC_TRANSFER_TARGET target, /*[out, retval]*/ SAFEARRAY **ppListDomainNames, BOOL *bpHandled);
	STDMETHOD(Adapt_get_DLoadAttribute)(/*[in]*/ long lKey, /*[in]*/ CSC_TRANSFER_TARGET src, /*[in, string]*/ BSTR sAtributeName, /*[out, retval]*/ VARIANT *pVal, BOOL *bpHandled);
	STDMETHOD(Adapt_ClearResource)(long lKey, CSC_TRANSFER_TARGET target, BOOL *bpHandled);
    STDMETHOD(Adapt_FlashResource)(/*[in]*/ long lKey, BOOL *bpHandled);

	STDMETHOD(Adapt_SetBreakpoint)(long lKey, BSTR sInstanceName, long lBreakpointNumber, long * hBP, BOOL *bpHandled);
	STDMETHOD(Adapt_ClearBreakpoint)(long lKey, long hBP, BOOL *bpHandled);
	STDMETHOD(Adapt_ClearAllBreakpoints)(long lKey, BOOL *bpHandled);
	STDMETHOD(Adapt_ContinueTask)(long lKey, BSTR sTaskName, BOOL *bpHandled);
    STDMETHOD(Adapt_SingleStep)(long lKey, BSTR sTaskName, BOOL *bpHandled);
    STDMETHOD(Adapt_StepOver)(long lKey, BSTR sTaskName, BOOL *bpHandled);
    STDMETHOD(Adapt_StepOut)(long lKey, BSTR sTaskName, BOOL *bpHandled);
	
	HRESULT Adapt_DownloadToControl(BSTR sDownloadGuid, BOOL *bpHandled);
};

#endif

/* ---------------------------------------------------------------------------- */
