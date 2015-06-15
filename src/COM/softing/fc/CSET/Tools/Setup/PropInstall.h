/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropInstall.h 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropInstall.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropInstall.h $
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

#if !defined(AFX_PROPINSTALL_H__48C48A6E_F7E0_4622_96DF_AEA7C30D0E75__INCLUDED_)
#define AFX_PROPINSTALL_H__48C48A6E_F7E0_4622_96DF_AEA7C30D0E75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropInstall.h : header file
//

#include "PropBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPropInstall dialog

class CPropInstall : public CPropBase
{
	DECLARE_DYNCREATE(CPropInstall)

// Construction
public:
	CPropInstall();
	~CPropInstall();

// Dialog Data
	//{{AFX_DATA(CPropInstall)
	enum { IDD = IDD_PP_INSTALL };
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropInstall)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropInstall)
	afx_msg void OnItemchangedListinfobox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInstall();
	afx_msg void OnInstallAll();
	afx_msg void OnUninstall();
	afx_msg void OnUninstallAll();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	public:
		CListCtrl *GetListCtrl();
		void SetWizardButtons();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPINSTALL_H__48C48A6E_F7E0_4622_96DF_AEA7C30D0E75__INCLUDED_)

/* ---------------------------------------------------------------------------- */
