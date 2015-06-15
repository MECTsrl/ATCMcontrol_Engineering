

// ToolbarComObj.h : Declaration of the CToolbarComObj

#ifndef __TOOLBARCOMOBJ_H_
#define __TOOLBARCOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CToolbarComObj
class ATL_NO_VTABLE CToolbarComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IToolbar,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CToolbarComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CToolbarComObj)
COM_INTERFACE_ENTRY(IToolbar)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

// IToolbarComObj
public:
// IToolbar
	STDMETHOD(GetId)(BSTR * psId);
	STDMETHOD(GetHandler)(GUID * pHandlerId);
    STDMETHOD(GetToolbarItemEnumerator)(IEnumToolbarItems **ppIEnumToolbarItems);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__TOOLBARCOMOBJ_H_
