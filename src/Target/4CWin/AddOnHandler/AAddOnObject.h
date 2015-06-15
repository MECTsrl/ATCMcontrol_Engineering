/* $Header: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/AAddOnObject.h 1     28.02.07 19:09 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AAddOnObject.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/AAddOnObject.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 19:09 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _AADDONOBJECT_H_
#define _AADDONOBJECT_H_

#include "resource.h"       // main symbols
#include "BaseTargetObject.h"
#include "BaseTargetData.h"


class CATargetData;

/* ---------------------------------------------------------------------------- */
/**
 * class CAAddOnObject
 *
 */
class ATL_NO_VTABLE CAAddOnObject : 
	public CComCoClass<CAAddOnObject, &CLSID_WinAddonObject>,
    public ISupportErrorInfo,
	public ICEContainer,
    public CBaseTargetObject
{
public:
	CAAddOnObject();
    virtual ~CAAddOnObject();

DECLARE_REGISTRY_RESOURCEID(IDR_AADDONOBJECT)
DECLARE_NOT_AGGREGATABLE(CAAddOnObject)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAAddOnObject)
   	COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(ICEContainer)
    COM_INTERFACE_ENTRY_CHAIN(CBaseTargetObject)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CAAddOnObject)
END_CONNECTION_POINT_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ICEContainer
    STDMETHOD(createLink)(BSTR sFileName, BSTR sLocation, BSTR sLinkName);
    STDMETHOD(renameLink)(BSTR sFileName, BSTR sLocation, BSTR sOldLinkName, BSTR sNewLinkName);
    STDMETHOD(deleteLink)(BSTR sFileName, BSTR sLocation, BSTR sLinkName);
    STDMETHOD(setProjectInfo)(IUnknown* pICEProjectInfo);
protected:
//    virtual CBaseProjectData* CreateProjectData();
    virtual HRESULT ShowProjectWizardDialog(CBaseProjectData* pProjectData);

    virtual CBaseTargetData* CreateTargetData(ICEProjInfo* pICEProjInfo);

    virtual CBaseProjectData* CreateProjectData();

    virtual HRESULT ActionDispatcher(CBaseTargetData* ptBaseTargetData,
                                     const CString& crstrMethodName,
                                     const CString& crstrSourceFile,
                                     const CString& crstrSourceId,
                                     const CString& crstrAddInfo,
                                     BOOL& bHandled);

    HRESULT OnPouProtection(CATargetData* pTargetData, 
							const CString& crstrFileName, 
                            const CString& crstrIdPath, 
                            ICEProjInfo* pICEProjInfo, 
                            const CString& crstrAddInfo);


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */
    virtual HRESULT CreateBackparseOutput(  CBaseTargetData* ptBaseTargetData,
                                            const CString& crstrFileName, 
                                            CString& rstrXmlText);

    void OutputErrorMessage(const CString& crstrTargetId, 
                            HRESULT hError,
                            LPCTSTR pszArg1 = NULL, 
                            LPCTSTR pszArg2 = NULL,
                            LPCTSTR pszArg3 = NULL, 
                            LPCTSTR pszArg4 = NULL);

    CString BuildErrorMsg(HRESULT hError, 
                          LPCTSTR pszArg1 = NULL, 
                          LPCTSTR pszArg2 = NULL,
                          LPCTSTR pszArg3 = NULL, 
                          LPCTSTR pszArg4 = NULL);

protected:
    CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> m_pProjInfo;

};

#endif

/* ---------------------------------------------------------------------------- */
