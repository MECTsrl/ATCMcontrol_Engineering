

#ifndef __DEBUGINFOSERVER_H_
#define __DEBUGINFOSERVER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DBIHolder.h"


//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// DebugInfoServer.h : Declaration of the CDebugInfoServer

/////////////////////////////////////////////////////////////////////////////
// CDebugInfoServer
#include "DbgInfoCP.h"
class ATL_NO_VTABLE CDebugInfoServer : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDebugInfoServer, &CLSID_DebugInfoServer>,
	public IDispatchImpl<IDebugInfo, &IID_IDebugInfo, &LIBID_DBGINFOLib>,
	public CProxyIDebugInfoNotify< CDebugInfoServer >,
	public IConnectionPointContainerImpl<CDebugInfoServer>
{
public:
    CDebugInfoServer ();
    virtual ~CDebugInfoServer ();

DECLARE_REGISTRY_RESOURCEID(IDR_DEBUGINFOSERVER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDebugInfoServer)
	COM_INTERFACE_ENTRY(IDebugInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDebugInfoServer)
	CONNECTION_POINT_ENTRY(IID_IDebugInfoNotify)
END_CONNECTION_POINT_MAP()

// IDebugInfo
public:
    STDMETHOD(Init)(/*[in, string]*/ BSTR sDebugInfo);
    STDMETHOD(Cleanup)();
    STDMETHOD(get_Project)(/*[out]*/ IUnknown **ppProject);
    STDMETHOD(GetSimpleType)(/*[in]*/ short sIECType, /*[out]*/ IUnknown **ppType);
    STDMETHOD(GetConfigurationType)(/*[out]*/ IUnknown **ppType);
    STDMETHOD(IsCorrupt)(/*[out]*/ BOOL *pbCorrupt);

protected:
    bool       m_bInitialized;
    CDBIHolder *m_pHolder;

    FC_CALLBACK (DebugInfoModifiedCB, CDebugInfoServer) m_ModifiedCB;

private:
    void OnDebugInfoModified ();
};

#endif //__DEBUGINFOSERVER_H_