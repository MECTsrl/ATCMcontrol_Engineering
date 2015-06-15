
#ifndef __CONFIGGSDBASIC_H_
#define __CONFIGGSDBASIC_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "SheetConfigGSD.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CConfigGSDBasic
class ATL_NO_VTABLE CConfigGSDBasic : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CConfigGSDBasic, &CLSID_ConfigGSDBasic>,
	public IDispatchImpl<IConfigGSDBasic, &IID_IConfigGSDBasic, &LIBID_CONFIGGSDLib,2,0>
{
public:
	// Constructor
	CConfigGSDBasic();
	// Destructor
	~CConfigGSDBasic();

    HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_CONFIGGSDBASIC)

BEGIN_COM_MAP(CConfigGSDBasic)
	COM_INTERFACE_ENTRY(IConfigGSDBasic)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IConfigGSDBasic
public:
	STDMETHOD(get_GSDObject)(/*[out, retval]*/ LPUNKNOWN *pVal);
	STDMETHOD(SetHelpConnection)(IConfigGSDHelp* pHelp);
	STDMETHOD(get_UserPrmData)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_ConfigBytes)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Ident_Number)(/*[out, retval]*/ long *pVal);
	STDMETHOD(StartConfigGSD)(/*[in,optional]*/ BSTR sFileGSD);
	STDMETHOD(SetGSDFileName)(BSTR sFileGSD = NULL);
	STDMETHOD(get_DialogState)(BSTR *pVal);
	STDMETHOD(StartConfigGSDEx)(/* [optional][in] */ BSTR sFileGSD,
                                /* [optional][in] */ BSTR sDialogState);

private:
private:
	HRESULT GetGSDFile(CString& szFileName);
	CSheetConfigGSD Sheet;		// Sheet which contains the Pages
	CString			m_szFileName;	// Filename of GSD
};

#endif // __CONFIGGSDBASIC_H_

