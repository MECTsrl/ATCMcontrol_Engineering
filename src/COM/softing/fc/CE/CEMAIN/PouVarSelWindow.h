#if !defined(AFX_POUVARSELWINDOW_H__FECDBEF1_6486_4158_AC47_107AEDC05929__INCLUDED_)
#define AFX_POUVARSELWINDOW_H__FECDBEF1_6486_4158_AC47_107AEDC05929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CECompMan.h"
#include "CESysDef.h"


#include "SizedDialogBar.h"
#include "PouVarSelCtrlWrapper.h"

#include "PouVarSelection.h"

#include "SizedControlBar.h"



class CPouVarSelWindow;



class CPouVarSelEventSink :
        public IDispatchImpl<_IPouVarSelCtrlEvents, 
            &IID__IPouVarSelCtrlEvents,
            &LIBID_POUVARSELECTIONLib>,
        public CComObjectRoot
{
public:
    CPouVarSelEventSink(CPouVarSelWindow* pPouVarSelWindow = NULL)
    {
        m_pPouVarSelWindow = pPouVarSelWindow;
    }

    STDMETHOD(GetProjectManager)(IUnknown** ppUnk);
    STDMETHOD(GetKadManager)(IUnknown** ppUnk);  
    STDMETHOD(InsertFBCall)(BSTR sFB);  
    STDMETHOD(InsertVariable)(BSTR sVar);	
    STDMETHOD(ShowHelpFile)(BSTR sHelpFile);
    STDMETHOD(ShowHeaderFile)(BSTR sHeaderFile);
    STDMETHOD(SaveEditorFile)(BSTR sEditorFile);
    STDMETHOD(ReloadEditorFile)(BSTR sEditorFile);
    STDMETHOD(ShowPouVarSelection)();

	BEGIN_COM_MAP(CPouVarSelEventSink)
	    COM_INTERFACE_ENTRY(_IPouVarSelCtrlEvents)
   	    COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

    CPouVarSelWindow* m_pPouVarSelWindow;

};



class CPouVarSelWindow  :  public CSizedControlBar //SECDialogBar //public CSizedDialogBar
{
    DECLARE_DYNCREATE(CPouVarSelWindow)

    FC_CALLBACK(CEStateChangeCB, CPouVarSelWindow) m_StateChangeCB;

public:
	CPouVarSelWindow();
	virtual ~CPouVarSelWindow();
    
    CPouVarSelCtrlWrapper* GetWrapper();
    void InitPouVarSelection();

protected:
    CPouVarSelCtrlWrapper m_ControlWrapper;

    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);

public:
     //{{AFX_MSG(CPouVarSelWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
//    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg BOOL OnEraseBkgnd (CDC *pDC);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    HRESULT AdvisePouVarSelEventHandler(); 
    void UnadvisePouVarSelEventHandler();
    
    CComObject<CPouVarSelEventSink> *m_pPouVarSelEventHandler;
	DWORD                            m_dwPouVarSelEventHandlerCookie;
    DWORD                            m_dwMsgEventHandlerCookie;
};












#endif // !defined(AFX_POUVARSELWINDOW_H__FECDBEF1_6486_4158_AC47_107AEDC05929__INCLUDED_)
