
// 
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			Mark Isham
//  Description:	

#ifndef __WKSEXDLG_H__
#define __WKSEXDLG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#ifndef __LSTBXEDT_H__
#include "toolkit\lstbxedt.h"
#endif

class SECToolBarManager;

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

class SECWsMgrDialogEx;

/////////////////////////////////////////////////////////////////////////////
// SECWSLBEditorEx

class SECWSLBEditorEx : public SECListBoxEditor
{
public:
	SECWSLBEditorEx(CStringList& listOrigWorkspaces,
					CStringList& listNewWorkspaces,	
					CStringList& listDeletedWorkspaces,
					SECWsMgrDialogEx* pParent);
					
protected:
	// virtual overrides
	virtual void OnEditingJustStarted();
	virtual void OnEditingStopped();
	virtual void OnItemRenamed(int nIndexRenamed,CString strOldName,CString NewName);
	virtual void OnItemAdded(int nIndexNewItem,CString strTextInserted);
	virtual void OnItemMoved(int nOldIndex,int nNewIndex,CString strTextMoved,BOOL WasDragged);
	virtual BOOL OnItemDelete(int nIndexToBeDeleted,CString strTextDeleted);
	
	// implementation
	BOOL ReplaceListBoxItem(CListBox* pLB,int iIndex,CString strNewText);
	BOOL MoveStrings(int nOldIndex,int nNewIndex,CStringList& strList);
	
	CStringList& m_listOrigWorkspaces;			// list with original workspace names
	CStringList& m_listNewWorkspaces;			// list with latest workspaces names
	CStringList& m_listDeletedWorkspaces;		// list of all removed workspaces	
	SECWsMgrDialogEx* m_pParent;	
};


/////////////////////////////////////////////////////////////////////////////
// SECWorkspaceManagerDialog

class SECWsMgrDialogEx : public CDialog
{
// Construction
public:
	SECWsMgrDialogEx(CStringList& listOrigWorkspaces,
					 CStringList& listNewWorkspaces,		
					 CStringList& listDeletedWorkspaces,
					 CString& strActionWorkspace,
					 BOOL bAllowOpen,BOOL bAllowSave,
					 CWnd* pParent = NULL);
	~SECWsMgrDialogEx();
	
	void SetOpenSaveButtons(BOOL bEnableOpen,BOOL bEnableSave);

// Dialog Data
	//{{AFX_DATA(SECWsMgrDialogEx)
	enum { IDD = IDD_OPEN_WORKSPACE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{Afx_VIRTUAL(SECWsMgrDialogEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	SECWSLBEditorEx* m_pLBEditor;
	CStringList& m_listOrigWorkspaces;
	CString& m_strActionWorkspace;				// name of workspace receiving the action
	BOOL m_bNT351;
	BOOL m_bAllowOpen,m_bAllowSave;

	// Generated message map functions
	//{{AFX_MSG(SECWsMgrDialogEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnApplyWorkspace();
	afx_msg void OnDone();
	afx_msg void OnPaint();
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

#endif // WIN32

#endif // __WKSEXDLG_H__