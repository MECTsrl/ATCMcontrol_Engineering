#ifndef __CEMAIN_H_
#define __CEMAIN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CEMAIN.h : main header file for the CEMAIN application
//

#if !defined(AFX_CEMAIN_H__E1198A0E_2ECC_432B_8D44_8CA02513B495__INCLUDED_)
#define AFX_CEMAIN_H__E1198A0E_2ECC_432B_8D44_8CA02513B495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "msgdoc.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols

//----  Forward Class Definitions:   -------------------------------*
class CCEStateManager;
class CCEComponentManager;
class CGenEdCont;
class CGenEdDoc;
class CCEFileHandler;
class CCEMAINDoc;
class CKADInfo;
class CSettings;
class CCEMAINView;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CCEMAINApp:
// See CEMAIN.cpp for the implementation of this class
//

class CCEMAINApp : public CWinApp
{
public:
	CCEMAINApp();
    CMsgDoc* GetMsgDoc();

    // SIS, 15.03.01: copied from V1.3
    CCEMAINDoc* GetProjectDoc();

    // SIS, 28.02.01
	CGenEdCont* GetContainer();

    // SIS, 12.03.01
	CCEFileHandler* GetFileHandler();

    // SIS, 23.04.01:
    CKADInfo* GetKADInfoObject();

    CSettings* GetSettings();

    void DeleteSettings();  // call this when project is closed
    void LoadSettings(const CString& crstrFilePath);

    // SIS, 21.11.01: close all panel views
    void CloseAllPanelViews();

    CCEMAINView* GetPLView();

    // SIS, 20.02.01: maybe this is needed later
//    inline CString & GetWndClassName() {return m_strWndClassName;};

private: //private methods
    BOOL RegisterWindowClass();
    BOOL RegisterSystemComponents();
    void RegisterFrameComponents();
    void RegisterProjectComponents();
    void InitializeCOM() throw();
    void ShowHelp(DWORD dwData, UINT nCmd);
    // SIS, 23.04.01
    BOOL LoadKADInfo();
    CString GetInitialBrowseProjectPath();
    CString GetStandardProjectPath();
    

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEMAINApp)
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	virtual BOOL OnIdle(LONG lCount);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL
	virtual BOOL OnDDECommand (LPTSTR lpszCommand);

// Implementation
	//{{AFX_MSG(CCEMAINApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
    afx_msg void OnFileOpen();
	afx_msg void OnProjectOpen();							// 13.12.06 SIS
    afx_msg void OnProjectClose();
    afx_msg void OnProjectSave();
    afx_msg void OnProjectSaveAs();
    afx_msg void OnUpdateIsProjectOpen(CCmdUI* pCmdUI);
    afx_msg void OnUpdateIsProjectNotOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnableAlways(CCmdUI* pCmdUI);		// 13.12.06 SIS
	afx_msg void OnAppExit();
    afx_msg void OnHelpFinder();
    afx_msg void OnFilePrintSetup(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
    void SetPrintInfo();

    virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
    
    void SetShuttingDown (bool bShuttingDown);
    bool IsShuttingDown ();
	
	// 09.11.04 SIS >>
	BOOL IsOpeningProject();
	// 09.11.04 SIS <<

// public members
public:
    CCEStateManager*     m_pStateManager;
    CCEComponentManager* m_pComponentManager;

    CMultiDocTemplate* m_pPanelTemplate;

    // SIS, 06.02.01
    BOOL    m_bPrjOpen;     // project open?
	BOOL    m_bPrjLocked;   // project locked?

    // SIS, 28.02.01
	CGenEdCont*         m_pContainer;

    // SIS, 12.03.01
    CCEFileHandler*     m_pFileHandler;     // file handler

    // SIS, 13.08.01
    bool                m_bShuttingDown;    // shut down flag

    // SIS, 23.04.01: KAD info object for navigator and proj list view
    CKADInfo*           m_pKADInfo;     
    
    CMultiDocTemplate*  m_pMsgTemplate;
// protected members
protected:
    CSettings*          m_pSettings;    // use GetSettings() to get access to settings object

    // SIS, 06.02.01: doc templates
    CMultiDocTemplate* m_pPrjTemplate;
    BOOL    m_bLaunch;      //avoid processing in OnFileNew during start up
	
	// 09.11.04 SIS >>
	// avoid pou var selection processing opening document on opening project
	BOOL	m_bOpeningProject;
	// 09.11.04 SIS <<

    // SIS: 20.02.01: unique window class name
    CString m_strWndClassName;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEMAIN_H__E1198A0E_2ECC_432B_8D44_8CA02513B495__INCLUDED_)

#endif // __CEMAIN_H_
