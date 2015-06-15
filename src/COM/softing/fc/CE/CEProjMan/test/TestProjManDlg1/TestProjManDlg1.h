// TestProjManDlg1.h : main header file for the TESTPROJMANDLG1 application
//

#if !defined(AFX_TESTPROJMANDLG1_H__4D3E5454_2293_42B9_83C1_5B255A56CA63__INCLUDED_)
#define AFX_TESTPROJMANDLG1_H__4D3E5454_2293_42B9_83C1_5B255A56CA63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1App:
// See TestProjManDlg1.cpp for the implementation of this class
//

class CTestProjManDlg1App : public CWinApp
{
public:
	CTestProjManDlg1App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestProjManDlg1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestProjManDlg1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPROJMANDLG1_H__4D3E5454_2293_42B9_83C1_5B255A56CA63__INCLUDED_)
