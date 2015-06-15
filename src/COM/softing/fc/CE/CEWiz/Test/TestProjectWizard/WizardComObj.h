// WizardComObj.h : Declaration of the CWizardComObj

#ifndef __WIZARDCOMOBJ_H_
#define __WIZARDCOMOBJ_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWizardComObj
class ATL_NO_VTABLE CWizardComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWizardComObj, &CLSID_WizardComObj>,
	public ICEProjectWizard
{
public:
	CWizardComObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WIZARDCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWizardComObj)
COM_INTERFACE_ENTRY(ICEProjectWizard)
END_COM_MAP()

// IWizardComObj
public:
// ICEProjectWizard
	STDMETHOD(ExecuteProjectWizard)(BSTR sPath, BSTR sProjectName, BSTR * psFileName)
	{
		if (psFileName == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(GetDescription)(INT iLanguageId, BSTR * psName, BSTR * psDescription)
	{
	    AFX_MANAGE_STATE(AfxGetStaticModuleState())

        HRESULT hrResult;
        CString sName,sDescription;

        if ((psName == NULL) || (psDescription == NULL)) {
		    hrResult = E_POINTER;
        } else {
            sName = "Test Project Wizard";
            sDescription = "Test Wizard Description";
            *psName = sName.AllocSysString();
            *psDescription = sDescription.AllocSysString();
            hrResult = S_OK;	
        }		
	    return hrResult;
	}
};

#endif //__WIZARDCOMOBJ_H_
