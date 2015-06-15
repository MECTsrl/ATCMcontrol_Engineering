// ContainerDummyComObj.h : Declaration of the CContainerDummyComObj

#ifndef __CONTAINERDUMMYCOMOBJ_H_
#define __CONTAINERDUMMYCOMOBJ_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CContainerDummyComObj
class ATL_NO_VTABLE CContainerDummyComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CContainerDummyComObj, &CLSID_ContainerDummyComObj>,
	public ICEContainer
{
public:
	CContainerDummyComObj();

DECLARE_REGISTRY_RESOURCEID(IDR_CONTAINERDUMMYCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CContainerDummyComObj)
	COM_INTERFACE_ENTRY(ICEContainer)
END_COM_MAP()

public:
// ICEContainer
	STDMETHOD(createLink)(BSTR sFileName, BSTR sLocation, BSTR sLinkName);
	STDMETHOD(renameLink)(BSTR sFileName, BSTR sLocation, BSTR sOldLinkName, BSTR sNewLinkName);
	STDMETHOD(deleteLink)(BSTR sFileName, BSTR sLocation, BSTR sLinkName);
};

#endif //__CONTAINERDUMMYCOMOBJ_H_
