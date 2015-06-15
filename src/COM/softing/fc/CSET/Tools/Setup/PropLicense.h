/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropLicense.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropLicense.h $
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
 
#if !defined(AFX_PROPLICENSE_H__E0DEDB85_D6D9_4C24_9FD4_FB2CC2EEFDB9__INCLUDED_)
#define AFX_PROPLICENSE_H__E0DEDB85_D6D9_4C24_9FD4_FB2CC2EEFDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropLicense.h : header file
//

#include "PropBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPropLicense dialog

class CPropLicense : public CPropBase
{
	DECLARE_DYNCREATE(CPropLicense)

// Construction
public:
	CPropLicense();
	~CPropLicense();

// Dialog Data
	//{{AFX_DATA(CPropLicense)
	enum { IDD = IDD_PP_LICENSE };
	CRichEditCtrl	m_cRichEdit;
	int		m_iAccept;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropLicense)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropLicense)
	afx_msg void OnRadioAccept();
	afx_msg void OnRadioNoAccept();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	protected:
		BOOL	m_bInitialized;

	protected:
		void SetWizardButtons();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPLICENSE_H__E0DEDB85_D6D9_4C24_9FD4_FB2CC2EEFDB9__INCLUDED_)

/* ---------------------------------------------------------------------------- */
