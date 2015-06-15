

// ToolbarItemComObj.h : Declaration of the CToolbarItemComObj

#ifndef __TOOLBARITEMCOMOBJ_H_
#define __TOOLBARITEMCOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CToolbarItemComObj
class ATL_NO_VTABLE CToolbarItemComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IToolbarItem,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CToolbarItemComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CToolbarItemComObj)
COM_INTERFACE_ENTRY(IToolbarItem)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IToolbarItem
	STDMETHOD(GetMainMenuId)(BSTR * psDispatchId);
	STDMETHOD(GetIcon)(BSTR *psIcon);
	STDMETHOD(GetToolTip)(BSTR * psToolTip);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__TOOLBARITEMCOMOBJ_H_
