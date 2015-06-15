
#ifndef __IECFEATURESCOMOBJ_H_
#define __IECFEATURESCOMOBJ_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetOnlineComObj
class ATL_NO_VTABLE CIECFeaturesComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IIECFeatures,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CIECFeaturesComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIECFeaturesComObj)
COM_INTERFACE_ENTRY(IIECFeatures)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

public:
// IIECFeatures
    STDMETHOD(GetDataTypes)(BSTR *psDataTypes);
    STDMETHOD(GetLanguages)(BSTR *psLanguages);
    STDMETHOD(GetRetain)(BOOL *pbRetain);
    STDMETHOD(GetAutoDecl)(GUID* pAutoDeclComponent,
                           BSTR* pINIFileWithFullPath);

// IXmlNodeRef
    XML_NODE_REF_DECL
};


#endif // __IECFEATURESCOMOBJ_H_


