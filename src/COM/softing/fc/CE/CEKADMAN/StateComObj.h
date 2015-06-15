// StateComObj.h : Declaration of the CStateComObj

#ifndef __STATECOMOBJ_H_
#define __STATECOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CStateComObj
class ATL_NO_VTABLE CStateComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IState,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CStateComObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STATECOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStateComObj)
COM_INTERFACE_ENTRY(IState)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IState
	STDMETHOD(GetText)(BSTR *psText);
	STDMETHOD(GetIcon)(BSTR *psIcon);
	STDMETHOD(GetOverlay)(BSTR *psOverlay);
    STDMETHOD(GetStateValue)(BSTR *psStateValue);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__STATECOMOBJ_H_
