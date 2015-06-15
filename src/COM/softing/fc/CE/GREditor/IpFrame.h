#if !defined(AFX_IPFRAME_H__59119A28_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
#define AFX_IPFRAME_H__59119A28_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInPlaceFrame : public COleDocIPFrameWnd
{
	DECLARE_DYNCREATE(CInPlaceFrame)
public:
	CInPlaceFrame();

// Attributes
public:

// Operations
public:
	void SetStatusText (const CString &strText);

	LPOLEINPLACEUIWINDOW GetOleDocFrame ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceFrame)
	public:
	virtual BOOL OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void ViewToolbarFbd(bool bShow);
	void ViewToolbarSfc(bool bShow);
   void ViewToolbarLd (bool bShow);

protected:
	CToolBar       m_wndToolBar;
	CToolBar       m_wndToolBarFBD;
	CToolBar       m_wndToolBarSFC;
	CToolBar       m_wndToolBarLD;
//	CDialogBar     m_wndDlgBar;
	CReBar         m_wndReBar;
	COleResizeBar  m_wndResizeBar;

	COleDropTarget	m_dropTarget;

// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
   afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPFRAME_H__59119A28_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
