
// FcHtmlEd.h : main header file for the FCHTMLED application
//

#if !defined(AFX_FCHTMLED_H__A04CED79_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
#define AFX_FCHTMLED_H__A04CED79_B51D_11D2_9FD9_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "FcHtmlEdOptions.h"

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdApp:
// See FcHtmlEd.cpp for the implementation of this class
//

class CFcHtmlEdApp : public CWinApp
{
public:
	CFcHtmlEdApp();

// Attributes
public:
	inline CFcHtmlEdOptions* GetOptions() { return &m_Options; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcHtmlEdApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CFcHtmlEdApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFcHtmlEdOptions m_Options;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCHTMLED_H__A04CED79_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
