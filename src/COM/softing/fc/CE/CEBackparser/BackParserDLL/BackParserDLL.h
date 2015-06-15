
// BackParserDLL.h : main header file for the BACKPARSERDLL DLL
//

#if !defined(AFX_BACKPARSERDLL_H__83816576_F84C_11D4_A09E_006008736ABF__INCLUDED_)
#define AFX_BACKPARSERDLL_H__83816576_F84C_11D4_A09E_006008736ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBackParserDLLApp
// See BackParserDLL.cpp for the implementation of this class
//

class CBackParserDLLApp : public CWinApp
{
public:
	CBackParserDLLApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackParserDLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBackParserDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKPARSERDLL_H__83816576_F84C_11D4_A09E_006008736ABF__INCLUDED_)


