/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropWelcome.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropWelcome.h $
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

#if !defined(AFX_PROPWELCOME_H__92B5DD9B_9DDD_4C15_836D_50E731437CB0__INCLUDED_)
#define AFX_PROPWELCOME_H__92B5DD9B_9DDD_4C15_836D_50E731437CB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropWelcome.h : header file
//

#include "PropBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPropWelcome dialog

class CPropWelcome : public CPropBase
{
	DECLARE_DYNCREATE(CPropWelcome)

// Construction
public:
	CPropWelcome();
	~CPropWelcome();

// Dialog Data
	//{{AFX_DATA(CPropWelcome)
	enum { IDD = IDD_PP_WELCOME };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropWelcome)
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
	//{{AFX_MSG(CPropWelcome)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPWELCOME_H__92B5DD9B_9DDD_4C15_836D_50E731437CB0__INCLUDED_)

/* ---------------------------------------------------------------------------- */
