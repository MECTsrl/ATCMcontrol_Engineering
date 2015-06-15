/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropSheet.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropSheet.h $
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
 */

#if !defined(AFX_PROPSHEET_H__C160705E_91A7_4FC4_A996_954DD6B9B702__INCLUDED_)
#define AFX_PROPSHEET_H__C160705E_91A7_4FC4_A996_954DD6B9B702__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheet.h : header file
//

#include "Install.h"

/////////////////////////////////////////////////////////////////////////////
// CPropSheet

class CPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet)

// Construction
public:
	CPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	protected:
		CInstall	m_Install;

	public:
		CInstall	*GetInstall();


	protected:
		HICON m_hIcon;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEET_H__C160705E_91A7_4FC4_A996_954DD6B9B702__INCLUDED_)

/* ---------------------------------------------------------------------------- */
