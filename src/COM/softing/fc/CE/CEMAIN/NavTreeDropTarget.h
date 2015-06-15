#if !defined(AFX_NAVTREEDROPTARGET_H__6DB292C0_0B2E_4A7A_8914_0AB75040A06E__INCLUDED_)
#define AFX_NAVTREEDROPTARGET_H__6DB292C0_0B2E_4A7A_8914_0AB75040A06E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NavTreeDropTarget.h : header file
//

class CNavTreeCtrl;

/////////////////////////////////////////////////////////////////////////////
// CNavTreeDropTarget document

class CNavTreeDropTarget : public COleDropTarget
{

public:
//	DECLARE_DYNCREATE(CNavTreeDropTarget)

    CNavTreeDropTarget(CNavTreeCtrl* pNavTreeCtrl = NULL);
	virtual ~CNavTreeDropTarget();

    void SetTreeCtrl(CNavTreeCtrl* pNavTreeCtrl);

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavTreeDropTarget)
	public:
	virtual BOOL OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CNavTreeDropTarget)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    CNavTreeCtrl*       m_pNavTreeCtrl;     // pointer to tree control
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVTREEDROPTARGET_H__6DB292C0_0B2E_4A7A_8914_0AB75040A06E__INCLUDED_)
