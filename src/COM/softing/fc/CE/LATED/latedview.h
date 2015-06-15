#ifndef __LATEDVIEW_H_
#define __LATEDVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "stdedattr.h"
#include "lcconfig.h"
//----  Forward Class Definitions:   -------------------------------*
class CLATEDDoc;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CLATEDView : public CTreeView, public CLCConfigListener
{
protected: // create from serialization only
	CLATEDView();
	DECLARE_DYNCREATE(CLATEDView)
public:
	//{{AFX_DATA(CLATEDView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CLATEDDoc* GetDocument();
    void DrawInto (CDC *pDC, CSize drawAreaSize);

// Overrides of config listener
    void OnDataType(int, CLCDataType *);
    void OnMember(int, CLCDataType *, CLCMember *);
    void OnLink(int, CLCLink *);
    void OnDestLink(int, CLCLink *, CLCDestLink *);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLATEDView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void OnDraw (CDC *pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLATEDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void GetPosition (CStdEditorPosition &);
    void GotoPosition (CStdEditorPosition &);
// Generated message map functions
protected:
	//{{AFX_MSG(CLATEDView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult); 
    afx_msg void OnToolTipNotify(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddDataType();
	afx_msg void OnDelete();
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRename();
	afx_msg void OnProperty();
	afx_msg void OnAddlink();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnAddDestLink();
	afx_msg void OnAssignDatatype();
	afx_msg void OnUpdateProp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssignDatatype(CCmdUI* pCmdUI);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int  OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//data
    /**Image list */
    CImageList m_ctlImage;
    CString    m_strLinkGroupLabel;
    CString    m_strLinkGroupTT;
    CString    m_strDataGroupLabel;
    CString    m_strDataGroupTT;
    HTREEITEM  m_hDataGroup;

    HACCEL      m_hLatAccel;    // special accelerators for LATED
//methods
    void OnUpdateAll(CObject*);
    void OnUpdateNew();
    CLCBase* GetNode(HTREEITEM);
    CLCConfig* GetConfig();

    void Expand(HTREEITEM hParent,bool bSelf=true);

    HTREEITEM InsertItem(DWORD dwData, bool bChildren = true,
                         HTREEITEM hParent = TVI_ROOT,
		                 HTREEITEM hInsertAfter = TVI_SORT);
    void InvalidateItem(HTREEITEM hItem);

    void OnProperty(CLCDataType *pDataType);
    void OnProperty(CLCLink* pLink);
    void OnProperty(CLCLink* pLink, CLCDestLink* pDestLink);

    void SetModified();
};

#ifndef _DEBUG  // debug version in latedview.cpp
inline CLATEDDoc* CLATEDView::GetDocument()
   { return (CLATEDDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __LATEDVIEW_H_