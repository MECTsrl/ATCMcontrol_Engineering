// ProjectWizardComObj.h : Declaration of the CProjectWizardComObj

#ifndef __PROJECTWIZARDCOMOBJ_H_
#define __PROJECTWIZARDCOMOBJ_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardComObj
class ATL_NO_VTABLE CProjectWizardComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProjectWizardComObj, &CLSID_ProjectWizardComObj>,
	public ICEProjectWizard
{
public:
	CProjectWizardComObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROJECTWIZARDCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProjectWizardComObj)
	COM_INTERFACE_ENTRY(ICEProjectWizard)
END_COM_MAP()

// IProjectWizardComObj
public:
// ICEProjectWizard
	STDMETHOD(ExecuteProjectWizard)(BSTR sPath, BSTR sProjectName, BSTR * psFileName);
	STDMETHOD(GetDescription)(LONG iLanguageId, BSTR * psName, BSTR * psDescription);
};

#endif //__PROJECTWIZARDCOMOBJ_H_

