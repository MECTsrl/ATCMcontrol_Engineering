// MsgCompile.h : interface of the CCompileView and CCompileDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGVIEW_H__02DC42F0_8E66_11D1_AE09_02608C7C2DDE__INCLUDED_)
#define AFX_MSGVIEW_H__02DC42F0_8E66_11D1_AE09_02608C7C2DDE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//HASSEN !!!
class CMsgRegex;
class CMsgDoc;
//struct MsgPCDialogItemInfo;


/////////////////////////////////////////////////////////////////////////////
// local defines

// maximum number of lines in a MessageViewer
#define MSGWND_MAXLINES     1000
// number of lines to delete if an overflow is detected
#define MSGWND_DELLINES     100

// maximum text length (limitations of Windows 95/98
#define MSGWND_MAXTEXTLEN   0xffff


class CMsgView : public CEditView
{
private:

protected: // create from serialization only
	DECLARE_DYNCREATE(CMsgView)
    CBrush * m_phbr;
    CMsgRegex* m_pRegex;

// Attributes
   

public:
    CMsgView();

// Operations
public:
    CMsgDoc* GetDocument();
    int AppendText(LPCTSTR szMsgText);
    void Clear(int iIndex = -1); 

    void ShowCurrentError();
    void ShowNextError();
    void ShowPreviousError();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgView)
	protected:
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMsgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    virtual void OnInitialUpdate ();
    void OnUpdate (CView *pSender, LPARAM lHint, CObject *pHint);

// Generated message map functions
protected:
	//{{AFX_MSG(CMsgView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnEditCopy ();
    afx_msg void OnEditClear ();
    afx_msg void OnContextMenuCommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
    void UpdateEditText (bool bFocus = false);

    BOOL SelectLine(int iLine);
    BOOL GetLine(int iLine, CString& rstrLine);
    BOOL EvaluateErrorMessage(const CString& crstrMessage, CString& rstrSourceFile, CString& rstrLocation);
    BOOL ShowSourceLocation(const CString& crstrSourceFile, const CString& crstrLocation);

};

#ifndef _DEBUG  // debug version in msgView.cpp
inline CMsgDoc* CMsgView::GetDocument()
  { return (CMsgDoc *)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGVIEW_H__02DC42F0_8E66_11D1_AE09_02608C7C2DDE__INCLUDED_)

