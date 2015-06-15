/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropSelEmb.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropSelEmb.h $
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

#if !defined(AFX_PROPSELEMB_H__17B857BC_13F1_4B4E_861A_2B8E1B63833F__INCLUDED_)
#define AFX_PROPSELEMB_H__17B857BC_13F1_4B4E_861A_2B8E1B63833F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSelEmb.h : header file
//

#include "PropBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPropSelEmb dialog

class CPropSelEmb : public CPropBase
{
	DECLARE_DYNCREATE(CPropSelEmb)

// Construction
public:
	CPropSelEmb();
	~CPropSelEmb();

// Dialog Data
	//{{AFX_DATA(CPropSelEmb)
	enum { IDD = IDD_PP_SEL_EMB };
	CButton	m_cAddOn1;
	CButton	m_cAddOn2;
	CButton	m_cAddOn3;
	CButton	m_cAddOn4;
	CButton	m_cDemo;
	CButton	m_cEngineering;
	BOOL	m_bAddOn1;
	BOOL	m_bAddOn2;
	BOOL	m_bAddOn3;
	BOOL	m_bAddOn4;
	BOOL	m_bDemo;
	BOOL	m_bEngineering;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropSelEmb)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropSelEmb)
	afx_msg void OnCheckEmbEng();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Backup
	protected:
		BOOL	m_bOldAddOn1;
		BOOL	m_bOldAddOn2;
		BOOL	m_bOldAddOn3;
		BOOL	m_bOldAddOn4;
		BOOL	m_bOldDemo;

	protected:
		CString	m_sAddOn1;
		CString	m_sAddOn2;
		CString	m_sAddOn3;
		CString	m_sAddOn4;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSELEMB_H__17B857BC_13F1_4B4E_861A_2B8E1B63833F__INCLUDED_)

/* ---------------------------------------------------------------------------- */
