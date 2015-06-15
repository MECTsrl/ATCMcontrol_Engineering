// STEditor.h : main header file for the STEDITOR application
//

#if !defined(AFX_STEDITOR_H__9F79143A_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
#define AFX_STEDITOR_H__9F79143A_09AC_11D2_B9A5_006008749B3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SECEdit.h"        // AUTOFMT 14.08.06 SIS

/////////////////////////////////////////////////////////////////////////////
// CSTEditorApp:
// See STEditor.cpp for the implementation of this class
//

class CSTEditorApp : public CWinApp
{
public:
	CSTEditorApp();

	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);

    void SetSettingDefaults(SECEditSettings& rtSettings);   // AUTOFMT 14.08.06 SIS

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTEditorApp)
	public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();
	virtual void WinHelp (DWORD dwData, UINT nCmd=HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	CMultiDocTemplate *m_pDocTemplate;

	//{{AFX_MSG(CSTEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpFinder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        
// AUTOFMT 09.08.06 SIS >>
protected:
    void InitRegistry();
// AUTOFMT 09.08.06 SIS <<


private:
	void ShowHelp(DWORD dwData, UINT nCmd);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEDITOR_H__9F79143A_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
