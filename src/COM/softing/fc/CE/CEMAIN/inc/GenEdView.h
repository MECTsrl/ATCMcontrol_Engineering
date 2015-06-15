
#ifndef __GENEDVIEW_H_
#define __GENEDVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// GenEdView.h : interface of the CGenEdView class
//
/////////////////////////////////////////////////////////////////////////////


class CGenEdDoc;
class CGenEdCntrItem;


class CGenEdView : public CView
{
public:
	virtual ~CGenEdView ();

	CGenEdDoc *GetDocument ();

	CGenEdCntrItem *GetServer ();
	void ShowServer (CGenEdCntrItem *pServer);
	void ActivateServer ();
	void DeactivateServer ();
	void SetFocusToServer ();

	void OnUpdate (CView *pSender, LPARAM lHint, CObject *pHint);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGenEdCntrItem* m_pServer;

	CGenEdView ();

private:
	afx_msg void OnActivateEditor (LPARAM, WPARAM);

//wizard:
	DECLARE_DYNCREATE(CGenEdView)

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenEdView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CGenEdView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
};

#ifndef _DEBUG  // debug version in GenEdView.cpp
inline CGenEdDoc* CGenEdView::GetDocument()
   { return (CGenEdDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __GENEDVIEW_H_


