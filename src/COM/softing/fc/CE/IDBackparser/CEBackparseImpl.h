#ifndef __CEBACKPARSEIMPL_H_
#define __CEBACKPARSEIMPL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "resource.h"       // main symbols
#include "IDBackparserCP.h"

#include "CEBackparse.h"

/////////////////////////////////////////////////////////////////////////////
// CCEBackparse
class ATL_NO_VTABLE CCEBackparseImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCEBackparseImpl, &CLSID_CEBackparse>,
	public ICEBackparse,
	public CProxy_ICEMessageEvent< CCEBackparseImpl >,
	public IConnectionPointContainerImpl<CCEBackparseImpl>
{
public:
	CCEBackparseImpl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CEBACKPARSE)
DECLARE_NOT_AGGREGATABLE(CCEBackparseImpl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEBackparseImpl)
	COM_INTERFACE_ENTRY(ICEBackparse)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// ICEBackparse
public:
	STDMETHOD(doBackparse)(/*[in,string]*/BSTR sFileName, /*[in]*/ICEProjInfo* pICEProjInfo, /*[out, string]*/ BSTR* psXMLText);
public :

BEGIN_CONNECTION_POINT_MAP(CCEBackparseImpl)
	CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()

};

#endif //__CEBACKPARSEIMPL_H_



