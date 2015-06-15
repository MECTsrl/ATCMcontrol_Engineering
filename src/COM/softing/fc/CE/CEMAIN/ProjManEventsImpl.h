// ProjManEventsImpl.h: interface for the CProjManEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJMANEVENTSIMPL_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_)
#define AFX_PROJMANEVENTSIMPL_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEProjMan.h"
#include <atlbase.h>
#include <atlcom.h>

class CNavigator;


class CProjManEventsImpl : public IDispatchImpl<_ICEProjectManagerEvents, 
                                                &IID__ICEProjectManagerEvents, 
                                                &LIBID_CEPROJMANLib>,
						   public CComObjectRoot,
						   public IDispatchImpl<_ICEProjManInstanceEvents, &IID__ICEProjManInstanceEvents, &LIBID_CEPROJMANLib>
{
public:
	CProjManEventsImpl();
	virtual ~CProjManEventsImpl();

public:
    void SetNavigatorPtr(CNavigator* pDlg);

protected:
    CNavigator* m_pNavigator;

private:
    STDMETHOD (InitClassTree)(IUnknown* pXMLDocument);
    STDMETHOD (InitTargetTree)(IUnknown* pXMLDocument );  
    STDMETHOD (InitInstanceTree)(IUnknown* pXMLDocument );  
    STDMETHOD (UpdateClassTree)(BSTR sChangedIdPath, E_UPDATE_MODE eMode);	
    STDMETHOD (UpdateTargetTree)(BSTR sChangedIdPath, E_UPDATE_MODE eMode);
    STDMETHOD (UpdateInstanceTree)(BSTR sChangedIdPath, E_UPDATE_MODE eMode);
    STDMETHOD (OnFileAdded)(BSTR sFilePath, enum E_FV_FileCategory eFileCat);
    STDMETHOD (OnFileRemoved)(BSTR sFilePath, enum E_FV_FileCategory eFileCat);
    STDMETHOD (ShowContextHelp)(DWORD dwContextId);
    STDMETHOD (ProjectFileChanged)();

	BEGIN_COM_MAP(CProjManEventsImpl)
	    COM_INTERFACE_ENTRY(_ICEProjectManagerEvents)
//DEL    	    COM_INTERFACE_ENTRY(IDispatch)
   	    COM_INTERFACE_ENTRY2(IDispatch, _ICEProjectManagerEvents)
   	    COM_INTERFACE_ENTRY(_ICEProjManInstanceEvents)
	END_COM_MAP()

// _ICEProjManInstanceEvents
	STDMETHOD(TargetInstanceAdded)(BSTR sId, BSTR sIdPath);
	STDMETHOD(TargetInstanceRemoved)(BSTR sId);
};

#endif // !defined(AFX_PROJMANEVENTSIMPL_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_)
