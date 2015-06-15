

// ClassComObj.h : Declaration of the CClassComObj

#ifndef __CLASSCOMOBJ_H_
#define __CLASSCOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CClassComObj
class ATL_NO_VTABLE CClassComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IClass,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CClassComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CClassComObj)
COM_INTERFACE_ENTRY(IClass)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IClass
	STDMETHOD(GetSection)(BSTR * psSection);
	STDMETHOD(GetGroupText)(BSTR * psGroupText);
	STDMETHOD(GetClassType)(BSTR * psClassType);
	STDMETHOD(GetTypeString)(BSTR * psType);
	STDMETHOD(GetType)(IType * * ppIType);
    STDMETHOD(GetPouSel)(BOOL *pbPouSelFlag);

// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__CLASSCOMOBJ_H_

