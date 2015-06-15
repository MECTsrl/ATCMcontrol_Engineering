// ProjectWizardTest.h : main header file for the PROJECTWIZARDTEST application
//

#if !defined(AFX_PROJECTWIZARDTEST_H__46A1F885_CB4E_11D4_B353_0008C77981F8__INCLUDED_)
#define AFX_PROJECTWIZARDTEST_H__46A1F885_CB4E_11D4_B353_0008C77981F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardTestApp:
// See ProjectWizardTest.cpp for the implementation of this class
//

class CProjectWizardTestApp : public CWinApp
{
public:
	CProjectWizardTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectWizardTestApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CProjectWizardTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTWIZARDTEST_H__46A1F885_CB4E_11D4_B353_0008C77981F8__INCLUDED_)
