// KadManagerComObj.h : Declaration of the CKadManagerComObj

#ifndef __KADMANAGERCOMOBJ_H_
#define __KADMANAGERCOMOBJ_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKadManagerComObj
class ATL_NO_VTABLE CKadManagerComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CKadManagerComObj, &CLSID_KadManagerComObj>,
	public IClasses,
	public IFiles,
	public IGeneral,
	public IMainMenu,
	public ITargets,
	public IToolbars,
	public ITypes,
    public IDragDrop
{
public:
	CKadManagerComObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_KADMANAGERCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CKadManagerComObj)
COM_INTERFACE_ENTRY(IClasses)
COM_INTERFACE_ENTRY(IFiles)
COM_INTERFACE_ENTRY(IGeneral)
COM_INTERFACE_ENTRY(IMainMenu)
COM_INTERFACE_ENTRY(ITargets)
COM_INTERFACE_ENTRY(IToolbars)
COM_INTERFACE_ENTRY(ITypes)
COM_INTERFACE_ENTRY(IDragDrop)
END_COM_MAP()

// IKadManagerComObj
public:
// IClasses
	STDMETHOD(GetClassesTypeString)(BSTR *psType);
	STDMETHOD(GetClassesType)(IType **ppIType);
	STDMETHOD(GetClassEnumerator)(IEnumClasses **ppEnumClasses);
	STDMETHOD(GetClassByType)(BSTR sType, IClass **ppIClass);
// IFiles
	STDMETHOD(GetFileEnumerator)(IEnumFiles **ppEnumFiles);
	STDMETHOD(GetFileByExtension)(BSTR sExtension, IFile **ppIFile);
// IGeneral
	STDMETHOD(GetCompilerPath)(BSTR *psCompilerPath);
	STDMETHOD(GetSplashScreenFilename)(BSTR *psSplashScreenFilename);
	STDMETHOD(GetDesktopIcon)(BSTR *psDesktopIcon);
	STDMETHOD(GetAboutBoxHandler)(GUID *pHandlerId);
    STDMETHOD(GetApplicationTitle)(BSTR* psApplicationTitle);
    STDMETHOD(GetMaxDocs)(BSTR* psMaxDocs);
    STDMETHOD(GetMaxDocsRec)(BSTR* psMaxDocsRec);
// IMainMenu
	STDMETHOD(GetMenuEnumerator)(IEnumMenus **ppEnumMenus);
	STDMETHOD(GetMenuById)(BSTR sId, IMenu **ppIMenu);
// ITargets
	STDMETHOD(GetTargetsTypeString)(BSTR *psType);
	STDMETHOD(GetTargetsType)(IType **ppIType);
	STDMETHOD(GetTargetEnumerator)(IEnumTargets **ppEnumTargets);
	STDMETHOD(GetTargetByType)(BSTR sType, ITarget **ppITarget);
// IToolbars
	STDMETHOD(GetToolbarEnumerator)(IEnumToolbars **ppEnumToolbars);
	STDMETHOD(GetToolbarById)(BSTR sId, IToolbar **ppIToolbar);
// ITypes
	STDMETHOD(GetTypeEnumerator)(IEnumTypes **ppEnumTypes);
	STDMETHOD(GetTypeById)(BSTR sId, IType **ppIType);
// IDragDrop
    STDMETHOD(GetDragDropMap)(IDragDropMap **ppIDragDropMap);
};

#endif //__KADMANAGERCOMOBJ_H_
