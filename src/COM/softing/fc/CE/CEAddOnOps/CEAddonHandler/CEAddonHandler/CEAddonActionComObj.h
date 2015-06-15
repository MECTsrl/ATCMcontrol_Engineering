
// CEAddonActionComObj.h : Declaration of the CCEAddonActionComObj

#ifndef __CEADDONACTIONCOMOBJ_H_
#define __CEADDONACTIONCOMOBJ_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCEAddonActionComObj
#include "CEAddOnActionEvents.h"
#include "CEAddonHandler.h"
#include "CEAddonAction.h"
#include "CEVarDecl.h"
#include "CEIECOperation.h"
#include "CEDragDrop.h"

#include "CEAddonHandlerCP.h"
class ATL_NO_VTABLE CCEAddonActionComObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCEAddonActionComObj, &CLSID_CEAddonActionComObj>,
	public ICEAddonAction,
	public ICEVarDecl,
    public ICEIECResourceOp,
    public ICEDragDrop,
    public CProxyICEAddOnActionEvents< CCEAddonActionComObj >,
    public IConnectionPointContainerImpl<CCEAddonActionComObj>
{
public:
	CCEAddonActionComObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CEADDONACTIONCOMOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCEAddonActionComObj)
	COM_INTERFACE_ENTRY(ICEAddonAction)
	COM_INTERFACE_ENTRY(ICEVarDecl)
    COM_INTERFACE_ENTRY(ICEIECResourceOp)
    COM_INTERFACE_ENTRY(ICEDragDrop)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// ICEAddonAction
public:
	STDMETHOD(doAction)(BSTR sMethodName, BSTR sSourceFile, BSTR sId, ICEProjInfo * pICEProjectInfo, BSTR sAdditionalInfo);

// ICEDragDropAction
    STDMETHOD(OnDrop)(IUnknown* pDragNode, BSTR sDropSource, BSTR sDropId, BSTR sDropType, DWORD dwDropEffect, ICEProjInfo* pICEProjectInfo);

// ICEVarDecl
	STDMETHOD(InsertVariable)(BSTR sFileName, ICEProjInfo * pProjInfo, BSTR sName, BSTR sVarClass, BSTR sDataType, BSTR sAddress, BSTR sAlias, BSTR sComment, BSTR sInitValue);
	STDMETHOD(DeleteVariable)(BSTR sFileName, ICEProjInfo * pProjInfo, BSTR sName);
	STDMETHOD(ModifyVariable)(BSTR sFileName, ICEProjInfo * pProjInfo, BSTR sOldName, BSTR sName, BSTR sVarClass, BSTR sDataType, BSTR sAddress, BSTR sAlias, BSTR sComment, BSTR sInitValue);

// ICEIECResourceOp
    STDMETHOD(InsertGVLImport)(ICEProjInfo * pProjInfo, 
                               BSTR sResourceName, 
                               BSTR sGVLName);
    STDMETHOD(RemoveGVLImport)(ICEProjInfo * pProjInfo, 
                               BSTR sResourceName, 
                               BSTR sGVLName);
    STDMETHOD(InsertProgramInstance)(ICEProjInfo* pProjInfo,
                                     BSTR sResourceName,
                                     BSTR sTaskName,
                                     BSTR sProgClass,
                                     BSTR sProgInstName,
                                     BOOL bPosFirst);

    STDMETHOD(RemoveProgramInstance)(ICEProjInfo* pProjInfo,
                                     BSTR sResourceName,
                                     BSTR sProgInstName);

    STDMETHOD(InsertTask)(ICEProjInfo* pProjInfo,
                          BSTR sResourceName,
                          BSTR sTaskName,
                          BSTR sInterval,
                          BSTR sPriority,
                          BOOL bAutostart,
						  BOOL bLoaded,		// NFTASKHANDLING 19.05.05 SIS
                          BSTR sIOSync,
                          BOOL bPosFirst);

    STDMETHOD(RemoveTask)(ICEProjInfo* pProjInfo,
                          BSTR sResourceName,
                          BSTR sTaskName);

    STDMETHOD(InsertTaskWithAttributes)(ICEProjInfo* pProjInfo,
                                        BSTR sResourceName,
                                        BSTR sTaskName,
                                        SAFEARRAY **pparrAttributes,
                                        BOOL bPosFirst);

    STDMETHOD(ModifyTask)(ICEProjInfo* pProjInfo,
                          BSTR sResourceName,
                          BSTR sOldTaskName,
                          BSTR sTaskName,
                          SAFEARRAY **pparrAttributes,
                          BSTR sAlias,
                          BSTR sUserText1,
                          BSTR sUserText2,
                          BSTR sUserText3,
                          BSTR sUserUrl,
                          BSTR sHelpUrl);

    STDMETHOD(ModifyProgramInstance)(ICEProjInfo* pProjInfo,
                                     BSTR sResourceName,
                                     BSTR sOldProgInstName,
                                     BSTR sOldTaskName,
                                     BSTR sTaskName,
                                     BSTR sProgClass,
                                     BSTR sProgInstName);




public :

BEGIN_CONNECTION_POINT_MAP(CCEAddonActionComObj)
	CONNECTION_POINT_ENTRY(IID_ICEAddOnActionEvents)
END_CONNECTION_POINT_MAP()

};

#endif //__CEADDONACTIONCOMOBJ_H_

