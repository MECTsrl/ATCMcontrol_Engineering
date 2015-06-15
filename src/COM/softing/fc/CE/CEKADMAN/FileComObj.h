

// FileComObj.h : Declaration of the CFileComObj

#ifndef __FILECOMOBJ_H_
#define __FILECOMOBJ_H_

#include "resource.h"       // main symbols

#include "XmlNodeRef.h"

/////////////////////////////////////////////////////////////////////////////
// CFileComObj
class ATL_NO_VTABLE CFileComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IFile,
	public IXmlNodeRef
{
    ALLOC_CHECK
public:
	CFileComObj()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFileComObj)
COM_INTERFACE_ENTRY(IFile)
COM_INTERFACE_ENTRY(IXmlNodeRef)
END_COM_MAP()

// IFileComObj
public:
// IFile
	STDMETHOD(GetExtension)(BSTR *psExtension);
	STDMETHOD(GetIcon)(BSTR *psIcon);
	STDMETHOD(GetParser)(GUID *pParserId);
	STDMETHOD(GetStandardEditor)(GUID *pEditorId);
	STDMETHOD(GetProgramEditor)(GUID *pEditorId);
	STDMETHOD(GetExeEditor)(BSTR *psEditor);
	STDMETHOD(GetContainer)(GUID *pContainerId);
    STDMETHOD(GetFilterText)(BSTR *psFilterText);
    STDMETHOD(GetHandler)(GUID *pHandler);
	STDMETHOD(GetDispatchId)(BSTR *psDispatchId);
    STDMETHOD(GetConfigurator)(GUID *pConfigurator);
    STDMETHOD(GetVisibilityFlag)(BOOL *pbVisible);
    STDMETHOD(GetFindInFilesFlag)(BOOL* pbFindInFiles);
    STDMETHOD(GetOrderKey)(BSTR *psOrderKey);
    STDMETHOD(GetPrintFlag)(BOOL *pbPrint);
// IXmlNodeRef
    XML_NODE_REF_DECL
};

#endif //__FILECOMOBJ_H_

