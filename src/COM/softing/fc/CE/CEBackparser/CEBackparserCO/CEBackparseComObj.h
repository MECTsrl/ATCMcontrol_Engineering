// CEBackparseComObj.h : Declaration of the CCEBackparseComObj

#ifndef __CEBACKPARSECOMOBJ_H_
#define __CEBACKPARSECOMOBJ_H_

#include "resource.h"       // main symbols
#include "CEBackparserCO.h"
#include "CEBackparse.h"

/////////////////////////////////////////////////////////////////////////////
// CCEBackparseComObj
class ATL_NO_VTABLE CCEBackparseComObj : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCEBackparseComObj, &CLSID_CEBackparseComObj>,
	public ICEBackparse
{
public:
	CCEBackparseComObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CEBACKPARSECOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEBackparseComObj)
COM_INTERFACE_ENTRY(ICEBackparse)
END_COM_MAP()

// ICEBackparseComObj
public:
// ICEBackparse
	STDMETHOD(doBackparse)(BSTR sFileName, ICEProjInfo * pICEProjInfo, BSTR * psXmlText);
};

#endif //__CEBACKPARSECOMOBJ_H_
