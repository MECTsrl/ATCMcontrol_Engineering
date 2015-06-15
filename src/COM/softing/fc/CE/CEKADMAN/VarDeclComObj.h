


#if !defined(AFX_VARDECLCOMOBJ_H__2EFBDA1D_5607_4E7B_80E4_4BEFDFE61C13__INCLUDED_)
#define AFX_VARDECLCOMOBJ_H__2EFBDA1D_5607_4E7B_80E4_4BEFDFE61C13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuItemComObj
class ATL_NO_VTABLE CVarDeclComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IVarDecl,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CVarDeclComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVarDeclComObj)
COM_INTERFACE_ENTRY(IVarDecl)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IVarDecl
	STDMETHOD(GetHandler)(GUID *pHandlerId);
	STDMETHOD(GetRetain)(BOOL *pbRetainFlag);
	STDMETHOD(GetInput)(BOOL *pbInputFlag);
	STDMETHOD(GetOutput)(BOOL *pbOutpuFlag);
	STDMETHOD(GetInOut)(BOOL *pbInOutFlag);
// IXmlNodeRef
    XML_NODE_REF_DECL
};



#endif // !defined(AFX_VARDECLCOMOBJ_H__2EFBDA1D_5607_4E7B_80E4_4BEFDFE61C13__INCLUDED_)




