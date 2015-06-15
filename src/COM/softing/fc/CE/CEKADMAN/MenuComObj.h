
// MenuComObj.h : Declaration of the CMenuComObj

#ifndef __MENUCOMOBJ_H_
#define __MENUCOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuComObj
class ATL_NO_VTABLE CMenuComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IMenu,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CMenuComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMenuComObj)
COM_INTERFACE_ENTRY(IMenu)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IMenu
	STDMETHOD(GetId)(BSTR *psId);
	STDMETHOD(GetTitle)(BSTR *psTitle);
	STDMETHOD(GetHandler)(GUID *pHandlerId);
	STDMETHOD(GetMenuItemEnumerator)(IEnumMenuItems **ppEnumMenuItems);
	STDMETHOD(GetItemByText)(BSTR sText, IMenuItem **ppIMenuItem);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__MENUCOMOBJ_H_

