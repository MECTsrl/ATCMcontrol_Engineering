
#ifndef __OEFINDDLG_H__
#define __OEFINDDLG_H__

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL

// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------


// OEFindDlg.h : header file
//

#include "editres.h"

/////////////////////////////////////////////////////////////////////////////
// OEFindDlg dialog

/////////////////////////////////////////////////////////////////////////////
// 
//@doc OEFindDlg
//@class OEFindDlg derives from CFindReplaceDialog.  This class implements
// a custom find dialog that helps the end user search their source code
// and do find/replace operations.  It has been customized to add case
// sensitivity and "memory".

class OEFindDlg : public CFindReplaceDialog
{
// Construction
public:
	//@cmember OEFindDlg constructor.
	OEFindDlg();   // standard constructor

	//@cmember  Initialize the dialog.
   virtual BOOL OnInitDialog();

   	//@cmember  Initialize the first combo.
   void InitCombo(); 

   	//@cmember Initialize the second combo.
   void InitCombo2();

   	//@cmember Are we in the middle of a replace operation?
	BOOL m_bReplacing;

// Dialog Data
	//{{AFX_DATA(OEFindDlg)
	enum { IDD = IDD_OE_FINDDLGORD };

	//@cmember Pointer to the combobox.
	CComboBox*	m_Combo;

	//@cmember Use a regular expression?
	BOOL	m_bRegular;

	//@cmember Match case?
	BOOL	m_bMatchCase;

	//@cmember String to find.
   BOOL	m_bMarkAll;

	//@cmember String to find.
	CString	m_FindWhat;

	//@cmember Up or down direction?
	int		m_iUp;
	
	//@cmember Replacement string.
	CString	m_ReplaceWith;
	
	//@cmember Turn on or off word wrap?
	BOOL	m_bWrapSearch;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OEFindDlg)
	protected:
		//@cmember DoDataExchange handler.
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//@cmember Retrieve the active view.
	CView* GetActiveView();

	// Generated message map functions
	//{{AFX_MSG(OEFindDlg)
	//@cmember OnMarkAll handler.
	afx_msg void OnMarkAll();
	//}}AFX_MSG
#ifndef ORIGINAL_VERSION
    afx_msg LRESULT OnHelp (WPARAM, LPARAM lParam);
#endif
	DECLARE_MESSAGE_MAP()
};

// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA


#endif //__OEFINDDLG_H__
