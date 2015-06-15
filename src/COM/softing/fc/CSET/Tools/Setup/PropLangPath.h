/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropLangPath.h 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropLangPath.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropLangPath.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_PROPLANGPATH_H__61083ED3_5E56_456B_A919_F43875EEF56A__INCLUDED_)
#define AFX_PROPLANGPATH_H__61083ED3_5E56_456B_A919_F43875EEF56A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropLangPath.h : header file
//

#include "PropBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPropLangPath dialog

class CPropLangPath : public CPropBase
{
	DECLARE_DYNCREATE(CPropLangPath)

// Construction
public:
	CPropLangPath();
	~CPropLangPath();

// Dialog Data
	//{{AFX_DATA(CPropLangPath)
	enum { IDD = IDD_PP_LANGPATH };
	CButton	m_cBoxLanguage;
	CButton	m_cBoxPath;
	CEdit	m_cInstallPath;
	CButton	m_cChangePath;
	CComboBox	m_cLanguage;
	CString	m_sLanguage;
	CString	m_sInstallPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropLangPath)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropLangPath)
	afx_msg void OnSelchangeLanguage();
	afx_msg void OnChangepath();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusPath();
	afx_msg void OnChangePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	protected:
		DWORD m_dwLanguage;
		BOOL  m_bCheckPath;
		BOOL  m_bSkipTest;
		
		CString m_sInstallPathCmdLine;

	protected:
		BOOL CheckPath();

	public:
		void SetInstallPathCmdLine(LPCTSTR szInstPath);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPLANGPATH_H__61083ED3_5E56_456B_A919_F43875EEF56A__INCLUDED_)

/* ---------------------------------------------------------------------------- */
