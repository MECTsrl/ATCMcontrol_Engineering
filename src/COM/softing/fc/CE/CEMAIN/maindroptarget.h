#ifndef __MAINDROPTARGET_H_
#define __MAINDROPTARGET_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CMainDropTarget document

class CMainDropTarget : public COleDropTarget
{

public:
//	DECLARE_DYNCREATE(CMainDropTarget)

    CMainDropTarget(CMainFrame* pMainFrame = NULL);
	virtual ~CMainDropTarget();

    void SetMainFrame(CMainFrame* pMainFrame);

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDropTarget)
	public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainDropTarget)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    CMainFrame*       m_pMainFrame;     // pointer to main frame
};

#endif // __MAINDROPTARGET_H_

