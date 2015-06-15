// CEExecute.h : Declaration of the CCEExecute

#ifndef __CEEXECUTE_H_
#define __CEEXECUTE_H_

#include "resource.h"       // main symbols
#include "ExeMaster.h"
#include "CEMsg.h"
#include "ExeDelegator.h"
#include "ExeDelegatorCP.h"

#include "cfacility.h"

/////////////////////////////////////////////////////////////////////////////
// CCEExecute

//------------------------------------------------------------------*
/**
 *  class CCEExecute.
 *
 *  - COM class encapsulating exe delegator interfaces
 *  - implements ICEExecute
 *  - implements connection point ICEMessageEvent
 *  - IExeMsgInternal is used to generate marshaling code for access to MTA
 *    (Multi Threaded Apartment)
 *
 *  @see    CExeThread
 */
class ATL_NO_VTABLE CCEExecute : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCEExecute, &CLSID_CEExecute>,
	public IDispatchImpl<ICEExecute, &IID_ICEExecute, &LIBID_EXEDELEGATORLib>,
	public CProxy_ICEMessageEvent< CCEExecute >,
	public IConnectionPointContainerImpl<CCEExecute>,
	public IDispatchImpl<IExeMsgInternal, &IID_IExeMsgInternal, &LIBID_EXEDELEGATORLib>
{
public:
	CCEExecute();

DECLARE_REGISTRY_RESOURCEID(IDR_CEEXECUTE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEExecute)
	COM_INTERFACE_ENTRY(ICEExecute)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IExeMsgInternal)
END_COM_MAP()

public:
    void FireMessage(const CString& strMessage, HRESULT hr = S_OK);

    virtual void FinalRelease();

    HRESULT GetInterfacePointer(IExeMsgInternal** ppIExeMsg);

// interface: ICEExecute 
public:

    STDMETHOD (ExecuteCommand)( /*[in,string]*/ BSTR            sCommand,
                                /*[in]*/        ICEProjInfo*    pICEProjectInfo,
                                /*[in]*/        DWORD           dwTimeout,
                                /*[in]*/        BOOL            bSynchronous,
                                /*[in]*/        BOOL            bShowWindow,
                                /*[out]*/       DWORD*          pdwReturnValue);

    STDMETHOD (OpenExeSession)( /*[out]*/   DWORD*          pdwSessionId,
                                /*[in]*/    ICEProjInfo*    pICEProjectInfo,
                                /*[in]*/    BOOL            bAutoClose);
    
    STDMETHOD (WriteExeCommand)(/*[in]*/        DWORD       dwSessionId,
                                /*[in,string]*/ BSTR        sCommand,
                                /*[in]*/        DWORD       dwTimeout);

    STDMETHOD (CloseExeSession)(/*[in]*/    DWORD           dwSessionId,
                                /*[in]*/    BOOL            bKill);

    STDMETHOD (CloseAllExeSessions)(/*[in]*/    BOOL        bKill);



protected:
    CExeMaster  m_tExeMaster;
    HANDLE      m_hSynchronizeEvent;

public :

BEGIN_CONNECTION_POINT_MAP(CCEExecute)
	CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()



// IExeMsgInternal
	STDMETHOD(NewExeMessage)(BSTR sMessage)
	{
        CComBSTR    sMachine;
        HRESULT     hr = S_OK;

		Fire_OnNewMessage(sMachine, E_FACILITY_CEEXE, hr, sMessage);
        return S_OK;
	}

    STDMETHOD(OnReady)()
    {
        ::SetEvent(m_hSynchronizeEvent);
        return S_OK;
    }
};

#endif //__CEEXECUTE_H_
