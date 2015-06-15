// 
// Stingray Software Extension Classes
// Copyright (C) 1996 Jim Beveridge
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			Jim Beveridge
//  Description:	Macro dialog - Definitions
//  Created:		Sun Jan 28 10:10:29 1996
//

#ifndef __SHORTCUTDLG_H__
#define __SHORTCUTDLG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32
#ifndef __SCUTCMDS_H__
#include "toolkit\SCutCmds.h"
#endif // __SCUTCMDS_H__

#ifndef __SCUTACCL_H__
#include "toolkit\SCutAccl.h"
#endif // __SCUTACCL_H__

//AAB #include "resource.h"
#ifdef _SECDLL
        #undef AFXAPP_DATA
        #define AFXAPP_DATA AFXAPI_DATA
        #undef AFX_DATA
        #define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

/////////////////////////////////////////////////////////////////////////////
// SECShortcutDlg dialog

/////////////////////////////
// AutoDuck tag block block for SECShortcutDlg 


// adinsert AutoDuck insertion point for SECShortcutDlg 
//@doc SECShortcutDlg 
//@class The SECShortcutDlg is a dialog class that provides a front end for entering application macros. It presents a standard
// dialog for accelerator key entry to the application.
//@comm See the SHORTCUT sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\SHORTCUT directory for a demonstration of this class.
//@BASE public|CDialog
//@xref<c SECShortcutTable> <c SECCommandList>
class SECShortcutDlg : public CDialog
{
// Construction
//@access Public Members
public:
	// standard constructor
	//@cmember
	/* The standard constructor creates an SECShortcutDlg instance.*/
	SECShortcutDlg(SECCommandList& commandList, SECShortcutTable& shortcuts, CWnd* pParent = NULL);

	//@cmember
	/* The SECCommandList object associated with the shortcut dialog. Initiated in the constructor.*/
	SECCommandList&		m_Commands;
	//@cmember
	/* The SECShortcutTable object associated with the shortcut dialog. Initiated in the constructor.*/
	SECShortcutTable&	m_Shortcuts;

	BOOL				m_bDirty;

// Dialog Data
	//{{AFX_DATA(SECShortcutDlg)
	enum { IDD = IDD_SHORTCUT_KEYS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void UpdateKeys();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECShortcutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CListBox*		m_pctlNames;

		// Generated message map functions
	//{{AFX_MSG(SECShortcutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeShortcutNameList();
	afx_msg void OnShortcutRemove();
	afx_msg void OnShortcutCreate();
	afx_msg void OnShortcutReset();
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

#endif //WIN32

#endif // __SCUTDIALOG_H__

