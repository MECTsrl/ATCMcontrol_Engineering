

// TypeComObj.h : Declaration of the CTypeComObj

#ifndef __TYPECOMOBJ_H_
#define __TYPECOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CTypeComObj
class ATL_NO_VTABLE CTypeComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IType,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CTypeComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTypeComObj)
COM_INTERFACE_ENTRY(IType)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

// ITypeComObj
public:
// IType
	STDMETHOD(GetIcon)(BSTR * psIcon);
	STDMETHOD(GetId)(BSTR * psId);
	STDMETHOD(GetVisibilityFlag)(BOOL * pbVisibilityFlag);
	STDMETHOD(GetMenu)(IMenu ** ppIMenu);
    STDMETHOD(GetText)(BSTR *psText);
    STDMETHOD(GetFilterText)(BSTR *psFilterText);
    STDMETHOD(GetView)(BSTR *psView);
    STDMETHOD(GetStateEnumerator)(IEnumStates **ppEnumStates);
    STDMETHOD(GetProjectListViewFlag)(BOOL *pbProjectListViewFlag);
    STDMETHOD(GetVarDecl)(IVarDecl **ppIVarDecl);

// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__TYPECOMOBJ_H_

