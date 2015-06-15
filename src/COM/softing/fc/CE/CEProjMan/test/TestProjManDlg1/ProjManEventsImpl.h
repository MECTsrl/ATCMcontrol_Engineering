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

class CTestProjManDlg1Dlg;


class CProjManEventsImpl : public IDispatchImpl<_ICEProjectManagerEvents, 
                                                &IID__ICEProjectManagerEvents, 
                                                &LIBID_CEPROJMANLib>,
						   public CComObjectRoot
{
public:
	CProjManEventsImpl();
	virtual ~CProjManEventsImpl();

public:
    void SetDlgWnd (CTestProjManDlg1Dlg* pDlg);

protected:
    CTestProjManDlg1Dlg* m_pDlg;

private:
   // STDMETHOD (InvalidateClassTree)();    
   // STDMETHOD (InvalidateTargetTree)();
    STDMETHOD (InitClassTree)(IUnknown* pXMLDocument);
    STDMETHOD (InitTargetTree)(IUnknown* pXMLDocument );  
    STDMETHOD (UpdateClassTree)(SAFEARRAY** parrChangedIdPaths);	
    STDMETHOD (UpdateTargetTree)(SAFEARRAY** parrChangedIdPaths);
    STDMETHOD (InvalidateFileView)();

	BEGIN_COM_MAP(CProjManEventsImpl)
	    COM_INTERFACE_ENTRY(_ICEProjectManagerEvents)
   	    COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

};

#endif // !defined(AFX_PROJMANEVENTSIMPL_H__D5F5BCF2_A325_4580_ADCB_F24DC8634104__INCLUDED_)
