/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CommissioningMgt.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommissioningMgt.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CommissioningMgt.h $
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

#ifndef __COMMISSIONING_MGT_H_
#define __COMMISSIONING_MGT_H_

//------------------------------------------------------------------*
/**
*  class CCommissioningMgt
*
*  @see    ....
*/
class ATL_NO_VTABLE CCommissioningMgt : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCommissioningMgt, &CLSID_CommissioningMgt>,
	public IConnectionPointContainerImpl<CCommissioningMgt>,
	public IDispatchImpl<ICommissioningMgt, &IID_ICommissioningMgt, &LIBID_CSC_OnlineServer, 2, 0>,
	public IDispatchImpl<IConnection, &IID_IConnection, &LIBID_CSC_OnlineServer, 2, 0>,
    public CProxy_FCCommissioningMgtNotification<CCommissioningMgt>
{
        friend class CCommissioningSession;
// =Construction
public:
	                        CCommissioningMgt();
                                // default ctor
    virtual                 ~CCommissioningMgt();
                                // dtor

private:
                            CCommissioningMgt(CCommissioningMgt const & other);
                                // copy ctor
    CCommissioningMgt const &    operator = (CCommissioningMgt const & other);
                                // assignment


//--------------------------
// =Interface map
BEGIN_COM_MAP(CCommissioningMgt)
	COM_INTERFACE_ENTRY(ICommissioningMgt)
	COM_INTERFACE_ENTRY2(IDispatch, ICommissioningMgt)
    COM_INTERFACE_ENTRY(IConnection)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CCommissioningMgt)
    CONNECTION_POINT_ENTRY(IID__FCCommissioningMgtNotification)
END_CONNECTION_POINT_MAP()

// =Access
public:

// =Operations
public:
DECLARE_REGISTRY_RESOURCEID(IDR_DUMMY_REGSCRIPT)
//DECLARE_CLASSFACTORY_SINGLETON(CCommissioningMgt)

// =Helpers
public:
	ULONG InternalRelease();

private:
    HRESULT                 UpdateSystemVariables(BOOL bLogOn);
    static DWORD WINAPI     LogOutHelper(LPVOID lpParam);

public:
// IBreakpointNotify (sink from Control)
	static HRESULT          ibnBreakPointNotify(BSTR taskName, LONG bpQuality, BSTR instanceName, LONG breakpointNumber);
    static HRESULT          ibnConfigurationChanged(void);

//--------------------------
// =Interface
// ICommissioningSession
public:
	STDMETHOD(ForceLogout)(/*[in, string]*/ BSTR sUserName, /*[in, string]*/ BSTR sPassword, /*[in, string]*/ BSTR sEngMachineName);
	STDMETHOD(Logout)(/*[in]*/ LONG lKey);
	STDMETHOD(get_CurrentUser)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(Login)(/*[in, string]*/ BSTR sUserName, /*[in, string]*/ BSTR sPassword, /*[in, string]*/ BSTR sEngMachineName, /*[out]*/ LONG *plKey, /*[out, retval]*/ LPUNKNOWN *pUnkSession);

// IConnection
public:
    STDMETHOD(CheckConnection)();

// =Attributes
protected:

private:
    static CCriticalSection   m_cs;
    static CCommissioningMgt* sm_pCommissioningMgt;

    LONG                    m_lKey;
    CString                 m_strLoggedInUserName;
    CString                 m_strEngMachineName;
    CCommissioningSession  *m_pCommissioningSession;
};

#endif

/* ---------------------------------------------------------------------------- */
