#ifndef __SECEDITVIEW_H__
#define __SECEDITVIEW_H__

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL

// SECEditView.h : header file
//

#include "SECEdit.h"

/////////////////////////////////////////////////////////////////////////////
// SECEditView view
//@doc SECEditView 
//@class SECEditView is the CView derived Objective Edit window. All of the base functionality
// is encapsulated in the templatized base class, SECEditCore. SECEditView implements only
// the required CView functionality that is not provided by SECEditCore.
//
// The only functionality added to SECEditView is Printing and Print Preview support,
// and synchronization logic for containment in dynamic splitter windows.
//@base public | SECEditCore
class SECEditView : public SECEditCore<CView>
{
protected:
	//@cmember SECEditView constructor
	SECEditView();
	DECLARE_DYNCREATE(SECEditView)

// Attributes
public:
protected:
	//Internal member used for the OLE drag and drop implementation.
	COleDropTarget m_dt;
	BOOL m_bPreviewSelection;


// Operations
public:
	virtual void MySetScrollPos(int fnBar, int iPos, BOOL bRedraw);
	CDocument* GetActiveDoc();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECEditView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnDragLeave();
	protected:
   virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
   virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
   virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);	
	//}}AFX_VIRTUAL

// Implementation
protected:
	//@cmember SECEditView destructor
	virtual ~SECEditView();
	CUIntArray m_PageList;

	// Generated message map functions
protected:
	virtual void UpdateViews(CWnd* pSender, LPARAM lHint = 0L, CObject* pHint = NULL );
	//{{AFX_MSG(SECEditView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif //__SECEDITVIEW_H__