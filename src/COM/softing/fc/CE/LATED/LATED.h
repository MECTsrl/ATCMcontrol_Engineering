#ifndef __LATED_H_
#define __LATED_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CLATEDApp:
// See LATED.cpp for the implementation of this class
//

class CLATEDApp : public CWinApp
{
public:
	CLATEDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLATEDApp)
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance( );
    virtual void WinHelp (DWORD dwData, UINT nCmd=HELP_CONTEXT);
    //}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CLATEDApp)
	afx_msg void OnAppAbout();
    afx_msg void OnHelpFinder();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    void RegisterComponentCategory(CDocTemplate* pTemplate,bool bRegister = true);
    void ShowHelp(DWORD dwData, UINT nCmd);
    void SetHelpDirectory();
};

class CLATEDCommandLine : public CCommandLineInfo
{
public: 
    CLATEDCommandLine();
    virtual ~CLATEDCommandLine()
    {}

    void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);

    bool CommandRegister() const
    { return m_bRegister; }

    bool CommandUnRegister() const
    { return m_bUnregister; }
private:
    bool    m_bRegister;
    bool    m_bUnregister;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __LATED_H_