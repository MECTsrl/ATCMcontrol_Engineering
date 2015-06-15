// ProjInfoDummyComObj.h : Declaration of the CProjInfoDummyComObj

#ifndef __PROJINFODUMMYCOMOBJ_H_
#define __PROJINFODUMMYCOMOBJ_H_

#include "resource.h"       // main symbols
#include "CEContainer.h"

/////////////////////////////////////////////////////////////////////////////
// CProjInfoDummyComObj
class ATL_NO_VTABLE CProjInfoDummyComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProjInfoDummyComObj, &CLSID_ProjInfoDummyComObj>,
	public ICEProjInfo
{
public:
	CProjInfoDummyComObj();
	~CProjInfoDummyComObj();

DECLARE_REGISTRY_RESOURCEID(IDR_PROJINFODUMMYCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProjInfoDummyComObj)
	COM_INTERFACE_ENTRY(ICEProjInfo)
END_COM_MAP()

public:
// ICEProjInfo
	STDMETHOD(getProjectPath)(BSTR * psProjectPath)
	{
        SysFreeString(*psProjectPath);
        *psProjectPath = SysAllocString(L".");
        return S_OK;
/*
		if (psProjectPath == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
*/
	}
	STDMETHOD(getProjectName)(BSTR * psProjectName)
	{
		if (psProjectName == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getProjectGenPath)(BSTR * psGenPath)
	{
		if (psGenPath == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getLibraryPath)(BSTR * psPath)
	{
		if (psPath == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getInstallationPath)(BSTR * psPath)
	{
		if (psPath == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getXMLNodePtr)(BSTR sId, IUnknown * * ppXMLPtr)
	{
		if (ppXMLPtr == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getContainerIF)(BSTR sSourceFile, BSTR sId, IUnknown * * ppICEContainer, BSTR * psParentSource, BSTR * psParentId);
	STDMETHOD(getContainerIFforFile)(BSTR sContainerFile, IUnknown * * ppICEContainer);
	STDMETHOD(getAllTargetTypes)(SAFEARRAY * * parrNames)
	{
		if (parrNames == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getAllTargets)(SAFEARRAY * * parrTargets)
	{
		if (parrTargets == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
    STDMETHOD(getAllTargetsAsIdPaths) (SAFEARRAY * * parrTargets)
    {
		if (parrTargets == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
    }
	STDMETHOD(getAllClassTypes)(SAFEARRAY * * parrNames)
	{
		if (parrNames == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getTextForClassType)(BSTR sType, BSTR * psText)
	{
		if (psText == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getAllClasses)(SAFEARRAY * * parrFileNames)
	{
		if (parrFileNames == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
    STDMETHOD(getAllClassesAsIdPaths)(SAFEARRAY * * parrClasses)
    {
		if (parrClasses == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
    }
	STDMETHOD(getClassesOfType)(BSTR sClassType, SAFEARRAY * * parrFileNames)
	{
		if (parrFileNames == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
    STDMETHOD(getClassesOfTypeAsIdPaths)(BSTR sClassType, SAFEARRAY * * parrClasses)
    {
		if (parrClasses == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
    }
	STDMETHOD(getAllLibraries)(SAFEARRAY * * parrLibraries)
	{
		if (parrLibraries == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getClassesOfTypeFromLib)(BSTR sLibraryName, BSTR sClassType, SAFEARRAY * * parrFileNames)
	{
		if (parrFileNames == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getTypeOfTarget)(BSTR sTargetName, BSTR * psTargetType)
	{
		if (psTargetType == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getAddressStringForTarget)(BSTR sTargetName, BSTR * psAddrString)
	{
		if (psAddrString == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getUserAddressStringForTarget)(BSTR sTargetName, BSTR * psAddrString)
	{
		if (psAddrString == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getTargetNameForAddr)(BSTR sAddr, BSTR * psTargetName)
	{
		if (psTargetName == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getResourceNameForTarget)(BSTR sTarget, BSTR * psResourceName)
	{
		if (psResourceName == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
    STDMETHOD(getFreeChannelNrForPC)(BSTR sPCName, int * piChannelNr)
	{
		if (piChannelNr == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getGlobalNameProposal)(BSTR sBasis, BSTR * psName)
	{
		if (psName == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(getPathForNewSourceFile)(BSTR sSourceFile, BSTR * psPath)
	{
		if (psPath == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(fileChangedHint)(BSTR sSourceFile);
	STDMETHOD(fileParseNow)(BSTR sSourceFile)
	{
		return E_NOTIMPL;
	}
    STDMETHOD(getFileNameFromIdPath)(BSTR sIdPath, BSTR * psFileName)
    {
		if (psFileName == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
    }
    STDMETHOD(getIdPathFromFileName)(BSTR sFileName, BSTR * psIdPath)
    {
		if (psIdPath == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
    }
	STDMETHOD(CPInsertToSection)(BSTR sSection, BSTR sKey, BSTR sValue)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(CPDeleteFromSection)(BSTR sSection, BSTR sKey)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(CPDeleteKeyWithValueFromSection)(BSTR sSection, BSTR sKey, BSTR sValue)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(CPGetValueForKeyFromSection)(BSTR sSection, BSTR sKey, BSTR * psValue)
	{
		if (psValue == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(CPGetAllValuesForKeyFromSection)(BSTR sSection, BSTR sKey, SAFEARRAY * * parrFileNames)
	{
		if (parrFileNames == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
private:
	//ICEContainer * m_pICEContainer;
	IUnknown * m_pIUnkCEContainer;
	BOOL disconnectCEContainer();
	BOOL connectCEContainer();
};

#endif //__PROJINFODUMMYCOMOBJ_H_
