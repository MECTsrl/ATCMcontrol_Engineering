// LibAddOnHandler.h : Declaration of the CLibAddOnHandler

#ifndef __LIBADDONHANDLER_H_
#define __LIBADDONHANDLER_H_

#define DISP_ADDLIB     _T("AddLibrary")
#define DISP_REMOVELIB  _T("RemoveLibrary")
#define LIB_XMLTYPE     _T("LIBRARY")

#define LIB_CPLIB       _T("Libs")


#include "resource.h"       // main symbols
#include "CeProjInfo.h"

#include "CEAddonAction.h"
#include "CEBackparse.h"

/////////////////////////////////////////////////////////////////////////////
// CLibAddOnHandler
class ATL_NO_VTABLE CLibAddOnHandler : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLibAddOnHandler, &CLSID_LibAddOnHandler>,
	public ICEAddonAction,
    public ICEBackparse
{
public:
	CLibAddOnHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LIBADDONHANDLER)
DECLARE_NOT_AGGREGATABLE(CLibAddOnHandler)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLibAddOnHandler)
	COM_INTERFACE_ENTRY(ICEAddonAction)
    COM_INTERFACE_ENTRY(ICEBackparse)
END_COM_MAP()

// ICEAddonAction
public:
	STDMETHOD(doAction)(/*[in, string]*/ BSTR sMethodName,  /*[in, string]*/ BSTR sSourceFile, /*[in, string]*/ BSTR sId, /*[in]*/ ICEProjInfo* pICEProjectInfo, /*[in, string]*/ BSTR sAdditionalInfo);

    // ICEBackparse 
	STDMETHOD(doBackparse)(BSTR sFileName, ICEProjInfo * pICEProjInfo, BSTR * psXmlText);

private:
	HRESULT RemoveLib(CString strSourceFile, CString strSourceId, ICEProjInfo *pICEProjectInfo, CString strAddInfo);
	HRESULT AddLib(CString strSourceFile, CString strSourceId, ICEProjInfo* pICEProjectInfo, CString strAddInfo);
    HRESULT GetAllLibs(CList<CString, CString&>& listLibs, ICEProjInfo *pICEProjectInfo);

    BOOL IsLibPresent(const CString& crstrShortLib, CList<CString, CString&>& listLibs, ICEProjInfo* pICEProjInfo);

};

#endif //__LIBADDONHANDLER_H_
