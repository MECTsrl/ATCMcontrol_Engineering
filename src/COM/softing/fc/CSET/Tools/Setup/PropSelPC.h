/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropSelPC.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropSelPC.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 11.02.08 11:46 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_PROPSELPC_H__D4A13FBC_BF11_462C_986D_9B37B259956C__INCLUDED_)
#define AFX_PROPSELPC_H__D4A13FBC_BF11_462C_986D_9B37B259956C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSelPC.h : header file
//

#include "PropBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPropSelPC dialog

class CPropSelPC : public CPropBase
{
	DECLARE_DYNCREATE(CPropSelPC)

// Construction
public:
	CPropSelPC();
	~CPropSelPC();

// Dialog Data
	//{{AFX_DATA(CPropSelPC)
	enum { IDD = IDD_PP_SEL_PC };
	CButton	m_cControlLoop;
	CButton	m_cConsole;
	CButton	m_cControl;
	CButton	m_cDatabase;
	CButton	m_cEngineering;
	CButton	m_cRTCE;
	CButton	m_cEc;
	CButton	m_cFc;
	CButton	m_cBc;
	CButton	m_cXc;
	CButton	m_cDemo;
	BOOL	m_bControlLoop;
	BOOL	m_bRTCE;
	BOOL	m_bEngineering;
	BOOL	m_bDatabase;
	BOOL	m_bControl;
	BOOL	m_bConsole;
	BOOL	m_bFc;
	BOOL	m_bEc;
	BOOL	m_bBc;
	BOOL	m_bXc;
	BOOL	m_bDemo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropSelPC)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropSelPC)
	afx_msg void OnCheckPcEng();
	afx_msg void OnCheckPcCtl();
	afx_msg void OnCheckPcEc();
	afx_msg void OnCheckPcFc();
	afx_msg void OnCheckPcBc();
	afx_msg void OnCheckPcXc();
	afx_msg void OnCheckDemo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	protected:
		BOOL m_bOldDatabase;
		BOOL m_bOldControlLoop;
		BOOL m_bOldConsole;
		BOOL m_bOldRTCE;
		BOOL m_bOldEc;
		BOOL m_bOldFc;
		BOOL m_bOldBc;
		BOOL m_bOldXc;
		BOOL m_bOldDemo;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSELPC_H__D4A13FBC_BF11_462C_986D_9B37B259956C__INCLUDED_)

/* ---------------------------------------------------------------------------- */
