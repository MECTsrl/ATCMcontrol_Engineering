#if !defined(AFX_GREDITOR_H__59119A18_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
#define AFX_GREDITOR_H__59119A18_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "GrResource.h"       // main symbols
#include "STResource.h"
/////////////////////////////////////////////////////////////////////////////
// CGrEditorApp:
// See GrEditor.cpp for the implementation of this class
//

class CGrEditorApp : public CWinApp
{
public:
	CGrEditorApp();

   virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);

private:
   void ShowHelp(DWORD dwData, UINT nCmd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrEditorApp)
	public:
	virtual ~CGrEditorApp();
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	virtual void OnFilePrintSetup();
	virtual void WinHelp (DWORD dwData, UINT nCmd=HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	CMultiDocTemplate *m_pDocTemplate;

   CImageList *m_pDummyImage;    // solve's the problem with DeleteTempMap in OnIdle

	//{{AFX_MSG(CGrEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpFinder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GREDITOR_H__59119A18_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
