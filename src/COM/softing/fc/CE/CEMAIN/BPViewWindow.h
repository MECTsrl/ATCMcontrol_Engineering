#if !defined(AFX_BPVIEWWINDOW_H__0548200D_B2E5_416E_B8A7_D4EEFEB6F003__INCLUDED_)
#define AFX_BPVIEWWINDOW_H__0548200D_B2E5_416E_B8A7_D4EEFEB6F003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "stdafx.h"
#include "CECompMan.h"
#include "CESysDef.h"

#include "BPViewWrapper.h"
#include "SizedControlBar.h"


class CBPViewWindow  :  public CSizedControlBar //SECDialogBar //public CSizedDialogBar
{
    DECLARE_DYNCREATE(CBPViewWindow)

    FC_CALLBACK(CEStateChangeCB, CBPViewWindow) m_StateChangeCB;

public:
	CBPViewWindow();
	virtual ~CBPViewWindow();
    
    CBPViewWrapper* GetWrapper();

protected:
    CBPViewWrapper m_ControlWrapper;


public:
     //{{AFX_MSG(CBPViewWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg BOOL OnEraseBkgnd (CDC *pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void AttachToBreakpointManager ();
    void DetachFromBreakpointManager ();
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
  
    bool m_bAttached;

};

#endif // !defined(AFX_BPVIEWWINDOW_H__0548200D_B2E5_416E_B8A7_D4EEFEB6F003__INCLUDED_)