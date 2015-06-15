// CpLayout.h : Deklaration von CCpLayout

#ifndef __CPLAYOUT_H_
#define __CPLAYOUT_H_

#include "resource.h"       // Hauptsymbole
#include <asptlb.h>         // Active Server Pages Definitionen

/////////////////////////////////////////////////////////////////////////////
// CCpLayout
class ATL_NO_VTABLE CCpLayout : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCpLayout, &CLSID_CpLayout>,
	public IDispatchImpl<ICpLayout, &IID_ICpLayout, &LIBID_CPSVRTOOLSLib>
{
public:
	CCpLayout()
	{ 
		m_bOnStartPageCalled = FALSE;
		m_Path = "";
	}

public:

DECLARE_REGISTRY_RESOURCEID(IDR_CPLAYOUT)

BEGIN_COM_MAP(CCpLayout)
	COM_INTERFACE_ENTRY(ICpLayout)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICpLayout
public:
	//Active Server Pages Methoden
	STDMETHOD(OnStartPage)(IUnknown* IUnk);
	STDMETHOD(OnEndPage)();
	STDMETHOD(Delete)(BSTR bstrName);
	STDMETHOD(GetNameList)(BSTR* pbstrList);
	STDMETHOD(SetDefaultName)(BSTR bstrName);
	STDMETHOD(GetDefaultName)(BSTR *pbstrName);
	STDMETHOD(Init)(BSTR Path, BSTR bstrUser);
	STDMETHOD(Save)(BSTR Name, BSTR bstrData);
	STDMETHOD(Read)(BSTR Name, BSTR *pbstrData);
private:
	BOOL m_bOnStartPageCalled;						//OnStartPage erfolgreich?

private :
	CString m_Path;

};

#endif //__CPLAYOUT_H_
