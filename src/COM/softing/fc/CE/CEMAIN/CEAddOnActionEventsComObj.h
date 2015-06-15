#ifndef __CEADDONACTIONEVENTSCOMOBJ_H_
#define __CEADDONACTIONEVENTSCOMOBJ_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CEAddonActionEvents.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CCEAddOnActionEvents - implements interface ICEAddOnActionEvents.
 *  Allows COM components to query for other COM components maintained
 *  by the component manager residing in 4CE. The corresponding comp-
 *  onents will be created on demand if they are not active.
 *
*/
class CCEAddOnActionEvents : public CComObjectRoot,
                         public ICEAddOnActionEvents,
                         public ICEAddOnActionEvents2
{
public:
    CCEAddOnActionEvents() {}
    virtual ~CCEAddOnActionEvents() {}

    //------------------------------------------------------------------*
    /**
     * Will be called during final construction of the object. Overwritten
     * to provide an initial reference count of one for the object. Hence
     * calls AddRef() to increment reference count.
     * @return          S_OK;
     * 
    */
    HRESULT FinalConstruct()
    { 
        AddRef(); 
        return S_OK;
    }
public:

protected:
    DECLARE_PROTECT_FINAL_CONSTRUCT();
private:
	BEGIN_COM_MAP(CCEAddOnActionEvents)
   	    COM_INTERFACE_ENTRY(ICEAddOnActionEvents)
        COM_INTERFACE_ENTRY(ICEAddOnActionEvents2)
        COM_INTERFACE_ENTRY2(IUnknown, ICEAddOnActionEvents)
	END_COM_MAP()


// interface ICEAddOnActionEvents

	STDMETHOD(OnEnsureTargetDisconnected)(BSTR sTargetId, BOOL bRebuildHint);
	STDMETHOD(OnFileRemoved)(BSTR sFilePath);
    STDMETHOD(OnFileRenamed)(BSTR sOldFilePath, BSTR sNewFilePath);  


// interface ICEAddOnActionEvents2

    STDMETHOD(BuildAll)();
    STDMETHOD(BuildTarget)(BSTR sTargetId);
    STDMETHOD(ShowTargetMsgTab)(BSTR sTargetId);
    STDMETHOD(ShowMsgTab)(enum E_FC_MsgTab msgTab);
    STDMETHOD(IsConnected)(BSTR sTargetId, BOOL* pbStatus);
    STDMETHOD(IsExclusiveConnected)(BSTR sTargetId, BOOL* pbStatus);
    STDMETHOD(GetResourceState)(BSTR sTargetId, LONG* plStatus);
    STDMETHOD(Connect)(BSTR sTargetId);
    STDMETHOD(Disconnect)(BSTR sTargetId);
    STDMETHOD(ConnectExclusive)(BSTR sTargetId);
    STDMETHOD(DisconnectExclusive)(BSTR sTargetId);
    STDMETHOD(Download)(BSTR sTargetId);
    STDMETHOD(Flash)(BSTR sTargetId);
    STDMETHOD(StartControl)(BSTR sTargetId);
    STDMETHOD(StopControl)(BSTR sTargetId);
    STDMETHOD(StartAllTasks)(BSTR sTargetId);
    STDMETHOD(StopAllTasks)(BSTR sTargetId);
    STDMETHOD(ColdStart)(BSTR sTargetId);
    STDMETHOD(WarmStart)(BSTR sTargetId);
    STDMETHOD(InitClear)(BSTR sTargetId);
    STDMETHOD(CustomOnlCmd)(BSTR sTargetId, SAFEARRAY** ppDataDown, SAFEARRAY** ppDataUp);  
    STDMETHOD(ProjectInfoUpload)(BSTR sTargetId, 
                                 BSTR* psProjVers,
	                             BSTR* psProjName,
	                             BSTR* psModified,
	                             BSTR* psCreated,
                                 BSTR* psOwner,
	                             BSTR* psComment1,
	                             BSTR* psComment2,
	                             BSTR* psComment3);
    STDMETHOD(ProjectDownload)(BSTR sTargetId, 
                              SAFEARRAY **ppDataDown);
    STDMETHOD(ProjectUpload)(BSTR sTargetId, 
                             SAFEARRAY **ppDataUp);
    STDMETHOD(CustCmd1)(BSTR sTargetId, 
                        SAFEARRAY **ppDataDown, 
                        SAFEARRAY **ppDataUp);  
    STDMETHOD(CustCmd2)(BSTR sTargetId, 
                        SAFEARRAY **ppDataDown, 
                        SAFEARRAY **ppDataUp); 
    STDMETHOD(CustCmd3)(BSTR sTargetId, 
                        SAFEARRAY **ppDataDown, 
                        SAFEARRAY **ppDataUp);  
    STDMETHOD(CustCmd4)(BSTR sTargetId, 
                        SAFEARRAY **ppDataDown, 
                        SAFEARRAY **ppDataUp); 
    STDMETHOD(CustCmd5)(BSTR sTargetId, 
                        SAFEARRAY **ppDataDown, 
                        SAFEARRAY **ppDataUp); 
    STDMETHOD(LoadFile)(BSTR sTargetId,
                        BSTR sName, 
                        SAFEARRAY **ppData); 
    STDMETHOD(SaveFile)(BSTR sTargetId,
                        BSTR sName, 
                        SAFEARRAY **ppData);   
    STDMETHOD(Dir)(BSTR sTargetId,
                   SAFEARRAY **ppFileNames);
};

#endif // __CEADDONACTIONEVENTSCOMOBJ_H_

