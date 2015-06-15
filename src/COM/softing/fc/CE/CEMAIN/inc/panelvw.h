
#ifndef __PANELVW_H_
#define __PANELVW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <afxhtml.h>
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


// PanelVw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelView view
// #include "webbrowser.h"

class CPanelDoc;
class CPanelView : public CHtmlView
{
protected:
	CPanelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPanelView)

// Attributes
public:

// Operations
public:
    CPanelDoc* GetDocument();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
    void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
        LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
        LPCTSTR lpszHeaders, BOOL* pbCancel);

// Implementation
protected:
	virtual ~CPanelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanelView)
    afx_msg void OnGoBack();
    afx_msg void OnGoForward();
    afx_msg void OnGoSearchTheWeb();
    afx_msg void OnGoStartPage();
    afx_msg void OnViewStop();
    afx_msg void OnViewRefresh();
	afx_msg void OnUpdateViewStop(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void NavigateTo4C (CString strPath);
    void NavigateTo4CFile (CString strPath);
    void NavigateTo4CHttp (CString strAddress, CString strPath);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.



#endif // __PANELVW_H_

