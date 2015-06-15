// KadManagerComObj.cpp : Implementation of CKadManagerComObj
#include "stdafx.h"

#include "KadManager.h"
#include "CEKadMan.h"
#include "ComObj.h"
#include "KadManagerComObj.h"
#include "ClassComObj.h"
#include "FileComObj.h"
#include "MenuComObj.h"
#include "TargetComObj.h"
#include "ToolbarComObj.h"
#include "TypeComObj.h"
#include "EnumFilesComObj.h"
#include "EnumClassesComObj.h"
#include "EnumMenusComObj.h"
#include "EnumTargetsComObj.h"
#include "EnumToolbarsComObj.h"
#include "EnumTypesComObj.h"
#include "DragDropMapComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CKadManagerComObj

// IClasses

STDMETHODIMP CKadManagerComObj::GetClassesTypeString(BSTR *psType)
{
    return TFGetSubNodeAttribute<BSTR *>(L"CLASSES",L"TYPE",psType);
}

STDMETHODIMP CKadManagerComObj::GetClassesType(IType **ppIType)
{
    return kadAccess.GetSubNodeType(L"CLASSES",ppIType);
}

STDMETHODIMP CKadManagerComObj::GetClassEnumerator(IEnumClasses **ppEnumClasses)
{
    return TFGetSubSubNodeEnum<CEnumClassesComObj,IEnumClasses,CClassComObj,IClass>(
        IID_IEnumClasses,IID_IClass,L"CLASSES",L"CLASS",ppEnumClasses);
}

STDMETHODIMP CKadManagerComObj::GetClassByType(BSTR sType, IClass **ppIClass)
{
    return TFGetSubSubNodeByAttribute<CClassComObj,IClass>(
        IID_IClass,L"CLASSES",L"CLASS",L"TYPE",sType,ppIClass);
}

// IFiles

STDMETHODIMP CKadManagerComObj::GetFileEnumerator(IEnumFiles **ppEnumFiles)
{
    return TFGetSubSubNodeEnum<CEnumFilesComObj,IEnumFiles,CFileComObj,IFile>(
        IID_IEnumFiles,IID_IFile,L"FILES",L"FILE",ppEnumFiles);
}

STDMETHODIMP CKadManagerComObj::GetFileByExtension(BSTR sExtension, IFile **ppIFile)
{
    return TFGetSubSubNodeByAttribute<CFileComObj,IFile>(
        IID_IFile,L"FILES",L"FILE",L"EXT",sExtension,ppIFile);
}

// IGeneral

HRESULT CKadManagerComObj::GetCompilerPath(BSTR *psCompilerPath)
{
    return TFGetSubNodeAttribute<BSTR *>(L"GENERAL",L"COMPILERPATH",psCompilerPath);
}

HRESULT CKadManagerComObj::GetSplashScreenFilename(BSTR *psSplashScreenFilename)
{
    return TFGetSubNodeAttribute<BSTR *>(L"GENERAL",L"SPLASHSCREEN",
        psSplashScreenFilename);
}

HRESULT CKadManagerComObj::GetDesktopIcon(BSTR *psDesktopIcon)
{
    return TFGetSubNodeAttribute<BSTR *>(L"GENERAL",L"ICON",psDesktopIcon);
}

HRESULT CKadManagerComObj::GetAboutBoxHandler(GUID *pHandlerId)
{
    return TFGetSubNodeAttribute<GUID *>(L"GENERAL",L"ABOUTBOX",pHandlerId);
}

HRESULT CKadManagerComObj::GetApplicationTitle(BSTR* psApplicationTitle)
{
    return TFGetSubNodeAttribute<BSTR *>(L"GENERAL",L"APPTITLE", psApplicationTitle);
}

HRESULT CKadManagerComObj::GetMaxDocs(BSTR* psMaxDocs)
{
    return TFGetSubNodeAttribute<BSTR *>(L"GENERAL",L"MAXDOCS", psMaxDocs);
}

HRESULT CKadManagerComObj::GetMaxDocsRec(BSTR* psMaxDocsRec)
{
    return TFGetSubNodeAttribute<BSTR *>(L"GENERAL",L"MAXDOCSREC", psMaxDocsRec);
}

// IMainMenu

STDMETHODIMP CKadManagerComObj::GetMenuEnumerator(IEnumMenus **ppEnumMenus)
{
    return TFGetSubSubNodeEnum<CEnumMenusComObj,IEnumMenus,CMenuComObj,IMenu>(
        IID_IEnumMenus,IID_IMenu,L"MAINMENU",L"MENU",ppEnumMenus);
}

STDMETHODIMP CKadManagerComObj::GetMenuById(BSTR sId, IMenu **ppIMenu)
{
    return TFGetSubSubNodeByAttribute<CMenuComObj,IMenu>(
        IID_IMenu,L"MAINMENU",L"MENU",L"ID",sId,ppIMenu);
}

// ITargets

STDMETHODIMP CKadManagerComObj::GetTargetsTypeString(BSTR *psType)
{
    return TFGetSubNodeAttribute<BSTR *>(L"TARGETS",L"TYPE",psType);
}

STDMETHODIMP CKadManagerComObj::GetTargetsType(IType **ppIType)
{
    return kadAccess.GetSubNodeType(L"TARGETS",ppIType);
}

STDMETHODIMP CKadManagerComObj::GetTargetEnumerator(IEnumTargets **ppEnumTargets)
{
    return TFGetSubSubNodeEnum<CEnumTargetsComObj,IEnumTargets,CTargetComObj,ITarget>(
        IID_IEnumTargets,IID_ITarget,L"TARGETS",L"TARGET",ppEnumTargets);
}

STDMETHODIMP CKadManagerComObj::GetTargetByType(BSTR sType, ITarget **ppITarget)
{
    return TFGetSubSubNodeByAttribute<CTargetComObj,ITarget>(
        IID_ITarget,L"TARGETS",L"TARGET",L"TYPE",sType,ppITarget);
}

// IToolbars

STDMETHODIMP CKadManagerComObj::GetToolbarEnumerator(IEnumToolbars **ppEnumToolbars)
{
    return TFGetSubSubNodeEnum<CEnumToolbarsComObj,IEnumToolbars,CToolbarComObj,IToolbar>(
        IID_IEnumToolbars,IID_IToolbar,L"TOOLBARS",L"TOOLBAR",ppEnumToolbars);
}

STDMETHODIMP CKadManagerComObj::GetToolbarById(BSTR sId, IToolbar **ppIToolbar)
{
    return TFGetSubSubNodeByAttribute<CToolbarComObj,IToolbar>(
        IID_IToolbar,L"TOOLBARS",L"TOOLBAR",L"ID",sId,ppIToolbar);
}

// ITypes

STDMETHODIMP CKadManagerComObj::GetTypeEnumerator(IEnumTypes **ppEnumTypes)
{
    return TFGetSubSubNodeEnum<CEnumTypesComObj,IEnumTypes,CTypeComObj,IType>(
        IID_IEnumTypes,IID_IType,L"TYPES",L"TYPE",ppEnumTypes);
}

STDMETHODIMP CKadManagerComObj::GetTypeById(BSTR sId, IType **ppIType)
{
    return TFGetSubSubNodeByAttribute<CTypeComObj,IType>(
        IID_IType,L"TYPES",L"TYPE",L"ID",sId,ppIType);
}

// IDragDrop

STDMETHODIMP CKadManagerComObj::GetDragDropMap(IDragDropMap **ppIDragDropMap)
{
    return TFGetSubSubNode<CDragDropMapComObj, IDragDropMap>(
        NULL, IID_IDragDropMap,L"DRAGDROP",L"DRAGDROPMAP",ppIDragDropMap);
}


