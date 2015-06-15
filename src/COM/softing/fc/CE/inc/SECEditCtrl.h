#ifndef __SECEDITCTRL_H__
#define __SECEDITCTRL_H__

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL



// SECEditCtrl.h : header file
//
//@doc SECEditCtrl
//@mdata SECEdit*  | SECEditCtrl | m_pSECEdit | SECEditCtrl maintains its own
// SECEdit* in this member variable, but you can override GetEdit, and return your own
// SECEdit*, and also override CreateNewEdit, and do nothing to prevent the
// instantiation of m_pSECEdit.

//@doc SECEditCtrl
//@mdata SECEditCtrlOleDropTarget  | SECEditCtrl | m_dt | This is a derivative of
// COleDropTarget that forwards the proper OLE Drag and Drop messages to SECEditCtrl.


/////////////////////////////////////////////////////////////////////////////
// SECEditCtrl window

#include "SECEdit.h"
#include "SECEditCtrlOleDropTarget.h"


//@doc SECEditCtrl 
//@class SECEditCtrl is the CWnd derived Objective Edit window. All of the base functionality
// is encapsulated in the templatized base class, SECEditCore. SECEditCtrl implements only
// the required CWnd functionality that is not provided by SECEditCore.
//@base public | SECEditCore
class SECEditCtrl : public SECEditCore<CWnd>
{
// Construction
public:
	//@cmember Constructor for SECEditCtrl
	SECEditCtrl();

// Attributes
public:
protected:
	//@cmember SECEdit*
	SECEdit* m_pSECEdit;
	DECLARE_DYNAMIC(SECEditCtrl)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECEditCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	//@cmember Registers the custom Drop Target.
	virtual void RegisterDropTarget();
	//@cmember Returns m_pSECEdit. Override to return your own SECEdit*
	virtual SECEdit* GetEdit();
	//@cmember Initializes SECEditCtrl to a control in a dialog.
	virtual BOOL Initialize(UINT nID, CWnd* pParentWnd, DWORD dwExStyle = WS_EX_CLIENTEDGE);

	//@cmember Destructor for SECEditCtrl. Deletes m_pSECEditCtrl if necessary.
	virtual ~SECEditCtrl();

	// Generated message map functions
protected:
	//@cmember Custom Drop Target for SECEditCtrl
	SECEditCtrlOleDropTarget m_dt;
	//@cmember Creates the SECEdit used internally by SECEditCtrl
	virtual void CreateNewEdit();

	//{{AFX_MSG(SECEditCtrl)
	afx_msg UINT OnGetDlgCode();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
#ifndef ORIGINAL_VERSION
   virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
#endif
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



#endif //__SECEDITCTRL_H__