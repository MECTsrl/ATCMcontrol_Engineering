
#ifndef __DRAGDROPACTIONCOMOBJ_H_
#define __DRAGDROPACTIONCOMOBJ_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CDragDropActionComObj
class ATL_NO_VTABLE CDragDropActionComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDragDropAction,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CDragDropActionComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDragDropActionComObj)
COM_INTERFACE_ENTRY(IDragDropAction)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IDragDropAction
    STDMETHOD(GetDragType)(BSTR* psDragType);
	STDMETHOD(GetDropType)(BSTR* psDropType);
	STDMETHOD(GetHandler)(GUID *pHandlerId);
	STDMETHOD(GetEnableExpression)(BSTR* psEnableExpression);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif // __DRAGDROPACTIONCOMOBJ_H_

