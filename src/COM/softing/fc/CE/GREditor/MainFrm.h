#if !defined(AFX_MAINFRM_H__59119A1D_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
#define AFX_MAINFRM_H__59119A1D_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
   public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CSplitterWnd m_wndSplitter;    // to use for test at stand alone
                                  // to check multiview ..
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndToolBarFBD;
	CToolBar    m_wndToolBarSFC;
	CToolBar    m_wndToolBarLD;
	CReBar      m_wndReBar;
//	CDialogBar  m_wndDlgBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewToolbarFbd(void);
	afx_msg void OnViewToolbarSfc(void);
   afx_msg void OnViewToolbarLd (void);
	afx_msg void OnUpdateViewToolbarSfc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbarFbd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbarLd(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_MAINFRM_H__59119A1D_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
