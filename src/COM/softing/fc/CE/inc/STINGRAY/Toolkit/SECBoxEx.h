// Stingray Software Extension Classes
// Copyright (C) 1996-1999 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//		Author:		Brad		January 1999
//

#ifndef __SECCOMBOBOXEX_H__
#define __SECCOMBOBOXEX_H__

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

/////////////////////////////////////////////////////////////////////////////
// SECComboBoxEx window
//

const UINT SECCOMBOEX_NEWITEM = RegisterWindowMessage( _T("secComboExNewItem") );
const UINT SECCOMBOEX_APPLY   = RegisterWindowMessage( _T("secComboExApply") );

/////////////////////////////
// AutoDuck tag block block for SECComboBoxEx 

//@doc SECComboBoxEx
//@mdata UINT  | SECComboBoxEx | m_vkDelimiterKey | The delimiter key for the control.
//@xref <c SECComboBoxEx> <mf SECComboBoxEx::SetDelimiterKey>

//@doc SECComboBoxEx
//@mdata CString  | SECComboBoxEx | m_strDelimiter | The delimiter string for the control.
//@xref <c SECComboBoxEx> <mf SECComboBoxEx::SetDelimiterString>

//@doc SECComboBoxEx
//@mdata UINT  | SECComboBoxEx | m_nLastKey | The last key pressed in the control.
//@xref <c SECComboBoxEx>

//@doc SECComboBoxEx
//@mdata BOOL  | SECComboBoxEx | m_bCaseSensitive | Determines if control searches in 
// a case sensitive manner.
//@xref <c SECComboBoxEx> <mf SECComboBoxEx::SetCaseSensitive>

//@doc SECComboBoxEx
//@mfunc void  | SECComboBoxEx | SetCaseSensitive | Sets the case sensitivity of the control.
//@parm BOOL | bEnable | If TRUE, user entries must match case of combobox items in order for
// auto-complete to work.
//@xref <c SECComboBoxEx>

// adinsert AutoDuck insertion point for SECComboBoxEx 
//@doc SECComboBoxEx 
//@class SECComboBoxEx implements an enhanced CComboBoxEx, implementing
// auto-completion features. 
//@comm For the parent window to receive notification of the user pressing the Enter key, 
// the parent must handle the SECCOMBOEX_NEWITEM message.  The SECCOMBOEX_APPLY 
// message is sent after the SECCOMBOEX_NEWITEM message is handled successfully and 
// the new string has been added, or after an existing entry has been selected and the 
// Enter key pressed.
//@base public | CComboBoxEx
class SECComboBoxEx : public CComboBoxEx
{

	//@access Creation/Initialization

public:
	SECComboBoxEx();

	//@access Attributes
public:

	//@cmember
	/* Sets the case sensitivity of the control for selecting items.*/
	void SetCaseSensitive(BOOL bEnable) { m_bCaseSensitive=bEnable; }
	
	//@cmember
	/* Sets the delimiter key for the control.*/
	void SetDelimiterKey(UINT vkDelim);

	//@cmember
	/* Sets the delimiter string for the control.*/
	void SetDelimiterString(LPCTSTR strDelim);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECComboBoxEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

	//@access Overridable

	//@cmember
	/* Called when the delimiter key is pressed.*/
	virtual void OnDelimiterKey(UINT nKey);
	
	//@cmember
	/* Called when the enter key is pressed.*/
	virtual void OnEnterKey();
	
	//@cmember
	/* Searches for a matching string and selects it into the edit control.*/
	virtual BOOL SelectTheString(int nStartIndex, CString& strFind);
	
	//@cmember
	/* Override to add image, or to control insertion point in the list.*/
	virtual BOOL OnInsertNewEntry(COMBOBOXEXITEM* pItem);

	//@cmember
	/* Override to provide any validation.*/
	virtual BOOL PreProcessNewEntry(CString& strText);

	virtual ~SECComboBoxEx();

	// Generated message map functions
protected:

	//@access Protected data members

	//@cmember
	/* The delimiter key for the control.*/
	UINT m_vkDelimiterKey;

	//@cmember
	/* The delimiter string for the control.*/
	CString m_strDelimiter;
	
	//@cmember
	/* the last key pressed in the control.*/
	UINT m_nLastKey;
	
	//@cmember
	/* Determines if control searches in a case sensitive manner.*/
	BOOL m_bCaseSensitive;

	//{{AFX_MSG(SECComboBoxEx)
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnEditchange();
	//}}AFX_MSG

	void NotifyApply();

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

#endif // __SECCOMBOBOXEX_H__
