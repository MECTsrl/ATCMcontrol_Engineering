
// AboutDlg.h : Declaration of the CAboutDlg

#ifndef __ABOUTDLG_H_
#define __ABOUTDLG_H_

#include "resource.h"       // main symbols
#include "AboutBox.h"

#include "CEAddonAction.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
class ATL_NO_VTABLE CAboutDlg : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAboutDlg, &CLSID_AboutDlg>,
	public ICEAddonAction
{
public:
	CAboutDlg()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ABOUTDLG)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAboutDlg)
	COM_INTERFACE_ENTRY(ICEAddonAction)
END_COM_MAP()

// IAboutDlg
public:
// ICEAddonAction
	STDMETHOD(doAction)(BSTR sMethodName, BSTR sSourceFile, BSTR sId, ICEProjInfo * pICEProjectInfo, BSTR sAdditionalInfo)
	{
      if(wcscmp(sMethodName, CComBSTR(_T("showAboutBox"))) == 0) {
          CAboutBox* pDlg = new CAboutBox();

          pDlg->DoModal();

          delete pDlg;
          return(S_OK);
      }
		return(S_FALSE);
	}
};

#endif //__ABOUTDLG_H_

