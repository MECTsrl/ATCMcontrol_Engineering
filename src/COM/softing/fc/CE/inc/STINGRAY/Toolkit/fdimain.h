// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			Dean Hallman
//  Description:	Declarations for SECFDIFrameWnd
//

#ifndef __FDIMAIN_H__
#define __FDIMAIN_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SWINFRM_H__
#include "toolkit\swinfrm.h"
#endif

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL






// adinsert AutoDuck insertion point for SECFDIFrameWnd 
//@doc SECFDIFrameWnd 
//@class SECFDIFrameWnd is derived from SECFrameWnd. The SECFDIFrameWnd class is the base class for the main frame of an 
// FDI-based application.  If you are converting an MDI application to FDI, change the base class of your main frame 
// from CMDIFrameWnd to SECFDIFrameWnd.  SECFDIFrameWnd adds support for a window recently used list on the "Window" menu
// as well as a "Windows..." dialog.  This class introduces no member functions or variables which are meant to be 
// called or overridden.
//@comm See the FDI sample in the \SAMPLES\TOOLKIT\STANDARD\MDI directory for a demonstration of this class.
//@xref<c SECFDIChildWnd>
//@BASE public|SECFrameWnd
class SECFDIFrameWnd : public SECFrameWnd
{
    DECLARE_DYNCREATE(SECFDIFrameWnd);

// Construction


public:

	CPtrList m_children;

protected:
	 
	CWnd* m_pActiveWnd;


public:
    SECFDIFrameWnd();

	
	void AddWindow(CWnd* pWnd);
	
	void RemoveWindow(CWnd* pWnd);
	
	void SetActiveWindow(CWnd* pWnd);
	
	CWnd* GetActiveWindow() const;

	~SECFDIFrameWnd();

// Attributes


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECFDIFrameWnd)
	public:
	virtual CFrameWnd* GetActiveFrame();
	//}}AFX_VIRTUAL
    
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

// Operations

	
// Generated message map functions
protected:
    //{{AFX_MSG(SECFDIFrameWnd)
	afx_msg BOOL OnActivateWindow(UINT nID);
	afx_msg void OnUpdateWindowMenu(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

/////////////////////////////////////////////////////////////////////////////
// SECWindowsDialog dialog

/////////////////////////////
// AutoDuck tag block block for SECWindowsDialog 
// adinsert AutoDuck insertion point for SECWindowsDialog 
//@doc SECWindowsDialog 
//@class SECWindowsDialog is a utility class that is used by SECFDIFrameWnd to display and maintain the windows' list.
//@xref<c SECFDIFrameWnd> 
//@BASE public|CDialog
class SECWindowsDialog : public CDialog
{
// Construction
public:
	SECWindowsDialog(CWnd* pParent = NULL, CPtrList* pChildren = NULL);
	
// Attributes
public:
	CPtrList* m_pChildren;

// Dialog Data
	//{{AFX_DATA(SECWindowsDialog)
	enum { IDD = IDD_WINDOWS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECWindowsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CObList* m_pWindowList;

	// Generated message map functions
	//{{AFX_MSG(SECWindowsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseWindow();
	virtual void OnOK();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __FDIMAIN_H__
