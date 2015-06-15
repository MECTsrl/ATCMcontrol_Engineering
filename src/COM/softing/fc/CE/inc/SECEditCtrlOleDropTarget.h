#ifndef __SECEditCtrlOleDropTarget__
#define __SECEditCtrlOleDropTarget__

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL


// SECEditCtrlOleDropTarget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SECEditCtrlOleDropTarget window

class SECEditCtrlOleDropTarget : public COleDropTarget
{
// Construction
public:
	SECEditCtrlOleDropTarget();

// Attributes
public:

// Operations
public:
// Overridables
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);

	// These are just stubs that do nothing right now
	virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState,
		CPoint point);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);

// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECEditCtrlOleDropTarget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SECEditCtrlOleDropTarget();

	// Generated message map functions
protected:
	//{{AFX_MSG(SECEditCtrlOleDropTarget)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA


/////////////////////////////////////////////////////////////////////////////
#endif // __SECEditCtrlOleDropTarget__