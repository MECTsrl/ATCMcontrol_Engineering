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
//  Author:			John Williston
//  Description:	Declaration for SECUserToolsDlg
//  Created:		August 1996
//

#ifndef __USRTLDLG_H__
#define __USRTLDLG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
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

class SECBrowseFileEdit;
class SECBrowseDirEdit;
class SECUserTool;

/////////////////////////////////////////////////////////////////////////////
// SECUserToolsDlg dialog

/////////////////////////////
// AutoDuck tag block block for SECUserToolsDlg 

//@doc SECUserToolsDlg
//@mfunc void  | SECUserToolsDlg | SetToolsArrayPtr | To allow the user to edit a list of user-tools, the user-tools dialog
// must have an array of user-tools to operate on.  Since the lifetime of this array is typically greater than that of the
// user-tools dialog, the user-tools dialog cannot own the array.  The class which constructs the dialog must own the
// user-tools array and pass a pointer to it, using this method, just after constructing the dialog.  
//@parm CObArray*  | p | Pointer to the user tools array.
//@xref<mf SECUserToolsDlg::SECUserToolsDlg>
//@xref<c SECUserToolsDlg>


// adinsert AutoDuck insertion point for SECUserToolsDlg 
//@doc SECUserToolsDlg 
//@class The SECUserToolsDlg class implements a user-tools dialog.  A user-tools dialog allows a user to edit a list of 
// user-tools, where each user-tool is represented by one SECUserTool object. Through this dialog, the user can create 
// new user-tools, edit and delete existing user-tools and reorder the list of user-tools. Refer to the documentation for
// SECUserTool for a complete explanation of a user-tool.
//@comm See the USERTOOL sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\USERTOOL directory for a demonstration of this class.
//@xref<c SECUserTool>
//@BASE public|CDialog
class SECUserToolsDlg : public CDialog
{
// Construction
public:
	// standard constructor
	//@Access Public Members
	//@cmember
	/* Constructs an SECUserToolsDlg object.*/
	SECUserToolsDlg(CWnd* pParent = NULL); 
	
	//@cmember
	/* Sets the array of user-tools the dialog is to edit.*/
	void SetToolsArrayPtr(CObArray* p)
		{ m_pTools = p; }

// Dialog Data
	//{{AFX_DATA(SECUserToolsDlg)
	enum { IDD = IDD_UTM_USER_TOOLS };
	CListBox	m_lbTools;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECUserToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SECUserToolsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnSelchangeList();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	afx_msg void OnRemove();
	afx_msg void OnChangeMenuText();
	afx_msg void OnKillfocusMenuText();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
//
// customizations
//
////////////////////////////////////////////////////////////////////////////////

protected:
	CObArray* m_pTools;

	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFields(CCmdUI* pCmdUI);

	SECBrowseFileEdit* m_peditCmd;
	SECBrowseDirEdit* m_peditDir;
	SECUserTool* m_pLastTool;

	void UpdateLastTool( SECUserTool* pNewTool );

public:
	~SECUserToolsDlg();
	
	SECBrowseFileEdit* GetBrowseFileEdit()
		{return m_peditCmd;}

	SECBrowseDirEdit* GetBrowseDirEdit()
		{return m_peditDir;}

	
	
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// __USRTLDLG_H__

