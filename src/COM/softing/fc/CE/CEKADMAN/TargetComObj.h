

// TargetComObj.h : Declaration of the CTargetComObj

#ifndef __TARGETCOMOBJ_H_
#define __TARGETCOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetComObj
class ATL_NO_VTABLE CTargetComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public ITarget,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CTargetComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTargetComObj)
	COM_INTERFACE_ENTRY(ITarget)
	COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// ITarget
	STDMETHOD(GetTypeString)(BSTR * psType);
    STDMETHOD(GetExt)(BSTR* psExt);
	STDMETHOD(GetType)(IType * * ppIType);
    STDMETHOD(GetFacilityCode)(WORD * pFacilityCode);
	STDMETHOD(GetTargetOnline)(ITargetOnline * * ppITargetOnline);
    STDMETHOD(GetIECFeatures)(IIECFeatures * * ppIIECFeatures);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__TARGETCOMOBJ_H_

