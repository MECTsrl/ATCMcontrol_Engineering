// PMTool.h : main header file for the PMTOOL application
//

#if !defined(AFX_PMTOOL_H__B6949A9E_B1EF_43B1_8094_F1F86FAEF770__INCLUDED_)
#define AFX_PMTOOL_H__B6949A9E_B1EF_43B1_8094_F1F86FAEF770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPMToolApp:
// See PMTool.cpp for the implementation of this class
//

class CPMToolApp : public CWinApp
{
public:
	CPMToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPMToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPMToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        
    UINT GetPagesToShow();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMTOOL_H__B6949A9E_B1EF_43B1_8094_F1F86FAEF770__INCLUDED_)
