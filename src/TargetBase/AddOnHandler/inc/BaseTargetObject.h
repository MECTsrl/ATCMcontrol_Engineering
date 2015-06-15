#ifndef _BASETARGETOBJECT_H_
#define _BASETARGETOBJECT_H_

#include "CEProjInfo.h"
#include "CEAddonAction.h"
#include "CEBackparse.h"
#include "CEContainer.h"
#include "CEMsg.h"
#include "CEAddonActionEvents.h"
#include "cecompmanev.h"
#include "CEProjWiz.h"
#include "BaseProjectData.h"
#include "ExeDelegator.h"

#include "AddonBaseCP.h"
#include "XMLHelper\XMLNode.h"

class CBaseTargetData;


//------------------------------------------------------------------*
/**
 *  class CBaseTargetObject
 *
 *  - COM class for target object.
 *  - implements ICEAddonAction, ICEBackparse, ICEProjectWizard
 *  - connection points _ICEMessageEvent, ICEAddOnActionEvents
 *  - creates CBaseTargetData object (CreateTargetData())
 *  - delegates target specific operations to CBaseTargetData
 *
 *  @see    CBaseTargetData
 */
class CBaseTargetObject  :
    public CComObjectRootEx<CComMultiThreadModel>,
    public ICEAddonAction,
    public ICEBackparse,
    public ICEProjectWizard,
	public CProxy_ICEMessageEvent< CBaseTargetObject >,
	public CProxyICEAddOnActionEvents< CBaseTargetObject >,
	public CProxyICECompManEvents< CBaseTargetObject >,
  	public IConnectionPointContainerImpl<CBaseTargetObject>,
  	public CProxyICEAddOnActionEvents2< CBaseTargetObject >
{
public:
	CBaseTargetObject(const CString& crstrKADFileName);
	virtual ~CBaseTargetObject();


BEGIN_COM_MAP(CBaseTargetObject)
    COM_INTERFACE_ENTRY(ICEAddonAction)
    COM_INTERFACE_ENTRY(ICEBackparse)
    COM_INTERFACE_ENTRY(ICEProjectWizard)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CBaseTargetObject)
    CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
	CONNECTION_POINT_ENTRY(IID_ICEAddOnActionEvents)
    CONNECTION_POINT_ENTRY(IID_ICECompManEvents)
    CONNECTION_POINT_ENTRY(IID_ICEAddOnActionEvents2)
END_CONNECTION_POINT_MAP()



    // ICEAddonAction
    STDMETHOD(doAction)(BSTR sMethodName, BSTR sSourceFile, BSTR sId, ICEProjInfo * pICEProjectInfo, BSTR sAdditionalInfo);

    // ICEBackparse 
	STDMETHOD(doBackparse)(BSTR sFileName, ICEProjInfo * pICEProjInfo, BSTR * psXmlText);

    // ICEProjectWizard
    STDMETHOD(ExecuteProjectWizard)(BSTR sPath, BSTR sProjectName, BSTR *psFileName);
    STDMETHOD(GetDescription)(long iLanguageId, BSTR *psName, BSTR *psDescription);

    // override this method to implement more addon functions
    virtual HRESULT ActionDispatcher(CBaseTargetData* ptBaseTargetData,
                                     const CString& crstrMethodName,
                                     const CString& crstrSourceFile,
                                     const CString& crstrSourceId,
                                     const CString& crstrAddInfo,
                                     BOOL& bHandled);
    HRESULT GetExeDelegator(CComPtr<ICEExecute>& pExeDelegator);
    CString GetInstallPath();

protected:

    virtual CBaseTargetData* CreateTargetData(ICEProjInfo* pICEProjInfo);


    // help functions for backparsing
    virtual HRESULT CreateBackparseOutput(  CBaseTargetData* ptBaseTargetData,
                                            const CString& crstrFileName, 
                                            CString& rstrXmlText);

    virtual void SetBackparseRootNodeAttributes(CBaseTargetData* ptBaseTargetData, const CString& crstrFileName, CXMLNode& rtRootNode);
    virtual BOOL AddBackparseSubNodes(CXMLNode& rtRootNode, CXMLDocument& rtXMLDoc);

    // functions for project wizard
    virtual CBaseProjectData* CreateProjectData();
    virtual HRESULT ShowProjectWizardDialog(CBaseProjectData* pProjectData);
    HRESULT ExecuteProjectWizard(const CString& crstrPath, const CString& crstrName);
    HRESULT GetProjectWizardDescription(int iLanguageId, CString& rstrName, CString& rstrDescription);
    HRESULT DoProjectInfoUpload(const CString strSourceId);


protected:
    CString   m_strKADFileName;
};

#endif
