
// MenuItemComObj.h : Declaration of the CMenuItemComObj

#ifndef __MENUITEMCOMOBJ_H_
#define __MENUITEMCOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuItemComObj
class ATL_NO_VTABLE CMenuItemComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IMenuItem,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CMenuItemComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMenuItemComObj)
COM_INTERFACE_ENTRY(IMenuItem)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IMenuItem
	STDMETHOD(GetId)(BSTR *psId);
	STDMETHOD(GetText)(BSTR *psText);
	STDMETHOD(GetHandler)(GUID *pHandlerId);
	STDMETHOD(GetExeHandler)(BSTR *psHandler);
	STDMETHOD(GetDispatchId)(BSTR *psDispatchId);
	STDMETHOD(GetStandardOperation)(BSTR *piStandardOperation);
	STDMETHOD(GetStatusBarText)(BSTR *psStatusBarText);
	STDMETHOD(GetOrderKey)(BSTR *psOrderKey);
    STDMETHOD(GetKeyMouseEvent)(BSTR *psKeyMouseEvent);
	STDMETHOD(GetEnableExpression)(BSTR *psEnableExpression);
	STDMETHOD(GetShowExpression)(BSTR *psShowExpression);
    STDMETHOD(GetAccelerator)(BSTR *psAccelerator);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__MENUITEMCOMOBJ_H_

