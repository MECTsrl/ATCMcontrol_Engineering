// ProjManListEvents.h: interface for the CProjManListEvents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJMANLISTEVENTS_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_)
#define AFX_PROJMANLISTEVENTS_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEProjMan.h"
#include <atlbase.h>
#include <atlcom.h>

class CCEMAINView;


class CProjManListEvents : public IDispatchImpl<_ICEProjectManagerEvents, 
                                                &IID__ICEProjectManagerEvents, 
                                                &LIBID_CEPROJMANLib>,
						   public CComObjectRoot
{
public:
	CProjManListEvents();
	virtual ~CProjManListEvents();

public:
    void SetListViewPtr(CCEMAINView* pView);

protected:
    CCEMAINView* m_pListView;

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


	BEGIN_COM_MAP(CProjManListEvents)
	    COM_INTERFACE_ENTRY(_ICEProjectManagerEvents)
   	    COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

};

#endif // !defined(AFX_PROJMANLISTEVENTS_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_)
