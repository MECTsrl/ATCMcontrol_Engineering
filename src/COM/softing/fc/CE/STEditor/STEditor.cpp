
// STEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxconv.h>
#include <malloc.h>
#include <afxpriv.h>
#include "winspool.h"
#include "STEditor.h"
#include "fc_todebug\fc_assert.h"
#include "htmlhelp.h"
#include "utilif.h"
#include "CESysDef.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"
#include "STEditorDoc.h"
#include "STEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTEditorApp

BEGIN_MESSAGE_MAP(CSTEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	//OBJECT_ENTRY(CLSID_VarWatch, CVarWatch)
END_OBJECT_MAP()


class CSTCommandLineInfo : public CCommandLineInfo
{
public:
	CSTCommandLineInfo ();

	virtual void ParseParam (LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);

	bool m_bRegServer;
};


CSTCommandLineInfo::CSTCommandLineInfo ()
{
	m_bRegServer = false;
}


void CSTCommandLineInfo::ParseParam (LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag)
	{
		if (CString (lpszParam).CompareNoCase (_T ("regserver")) == 0)
		{
			m_bRegServer = true;
			return;
		};
	};

	CCommandLineInfo::ParseParam (lpszParam, bFlag, bLast);
}


/////////////////////////////////////////////////////////////////////////////
// CSTEditorApp construction

CSTEditorApp::CSTEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSTEditorApp object

CSTEditorApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {80F9D242-F22A-4771-938B-D8B609E66FBA}
static const CLSID clsid =
{ 0x80F9D242, 0xF22A, 0x4771, { 0x93, 0x8B, 0xD8, 0xB6, 0x09, 0xE6, 0x6F, 0xBA } };

/////////////////////////////////////////////////////////////////////////////
// CSTEditorApp initialization

BOOL CSTEditorApp::InitInstance()
{
	HRESULT hr;
    COleMessageFilter *pFilter;
	USES_CONVERSION;

#ifdef STEDITOR_DEBUG   
    DebugBreak();
#endif

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	hr = ::CoInitializeSecurity (NULL, -1, NULL, NULL,
	                             RPC_C_AUTHN_LEVEL_NONE, 
	                             RPC_C_IMP_LEVEL_IMPERSONATE, 
	                             NULL, EOAC_NONE, NULL);
	assert (SUCCEEDED (hr));    

    pFilter = AfxOleGetMessageFilter ();
    assert (pFilter);
    if (pFilter != NULL)
    {
        pFilter->EnableNotRespondingDialog (FALSE);
        pFilter->EnableBusyDialog (FALSE);
    };

    AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	_Module.Init (ObjectMap, m_hInstance);

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.

	SetRegistryKey (_T("Softing\\4CONTROLV2\\4CONTROL Engineering"));               // keywords are case insensitive
	
	// AUTOFMT 08.08.06 SIS >>
	InitRegistry();
	// AUTOFMT 08.08.06 SIS <<

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplate = new CMultiDocTemplate(
		IDR_STTYPE,
		RUNTIME_CLASS(CSTEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSTEditorView));
	m_pDocTemplate->SetServerInfo(
		IDR_STTYPE_SRVR_EMB, IDR_STTYPE_SRVR_IP,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(m_pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, m_pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CSTCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER);
	COleObjectFactory::UpdateRegistryAll();

	if (cmdInfo.m_bRegServer)
	{
		TCHAR szModule[_MAX_PATH+10];
		LPOLESTR pszModule;
		HRESULT hr;
		LPTYPELIB pTypeLib;
		USES_CONVERSION;

		::GetModuleFileName (m_hInstance, szModule, _MAX_PATH);
		pszModule = T2OLE (szModule);
		hr = LoadTypeLib (pszModule, &pTypeLib);
		if (SUCCEEDED (hr))
		{
			hr = RegisterTypeLib (pTypeLib, pszModule, NULL);
			pTypeLib->Release ();
		};

		return (FALSE);
	};

    // Prevents ProcessShellCommand from displaying
    // a message box when unregistering.
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
    {
        cmdInfo.m_bRunEmbedded = true;
    };

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


// AUTOFMT 09.08.06 SIS >>
// initializes the settings for different file extensions
// without overwriting current values
void CSTEditorApp::InitRegistry()
{
    // ST-Settings:
    SECEditSettings tSettingsST(_T("ST"));

    SetSettingDefaults(tSettingsST);

    tSettingsST.LoadFromRegistry();                 // overwrite defaults by current values
    tSettingsST.SaveToRegistry();                   // set default values, if not yet present

    // IL-Settings:
    SECEditSettings tSettingsIL(_T("IL"));

    SetSettingDefaults(tSettingsIL);

    tSettingsIL.LoadFromRegistry();                 // overwrite defaults by current values
    tSettingsIL.SaveToRegistry();                   // set default values, if not yet present

    // 4CH-Settings:
    SECEditSettings tSettings4CH(_T("4CH"));

    SetSettingDefaults(tSettings4CH);

    tSettings4CH.LoadFromRegistry();                // overwrite defaults by current values
    tSettings4CH.SaveToRegistry();                  // set default values, if not yet present

    // CON-Settings:
    SECEditSettings tSettingsCON(_T("CON"));

    SetSettingDefaults(tSettingsCON);

    tSettingsCON.LoadFromRegistry();                // overwrite defaults by current values
    tSettingsCON.SaveToRegistry();                  // set default values, if not yet present

    // CST-Settings:
    SECEditSettings tSettingsCST(_T("CST"));

    SetSettingDefaults(tSettingsCST);

    tSettingsCST.LoadFromRegistry();                // overwrite defaults by current values
    tSettingsCST.SaveToRegistry();                  // set default values, if not yet present

    // GVL-Settings:
    SECEditSettings tSettingsGVL(_T("GVL"));

    SetSettingDefaults(tSettingsGVL);

    tSettingsGVL.LoadFromRegistry();                // overwrite defaults by current values
    tSettingsGVL.SaveToRegistry();                  // set default values, if not yet present

    // SDT-Settings:
    SECEditSettings tSettingsSDT(_T("SDT"));

    SetSettingDefaults(tSettingsSDT);

    tSettingsSDT.LoadFromRegistry();                // overwrite defaults by current values
    tSettingsSDT.SaveToRegistry();                  // set default values, if not yet present

    // RES-Settings:
    SECEditSettings tSettingsRES(_T("RES"));

    SetSettingDefaults(tSettingsRES);

    tSettingsRES.LoadFromRegistry();                // overwrite defaults by current values
    tSettingsRES.SaveToRegistry();                  // set default values, if not yet present

    // HDR-Settings:
    SECEditSettings tSettingsHDR(_T("HDR"));

    SetSettingDefaults(tSettingsHDR);

    tSettingsHDR.LoadFromRegistry();                // overwrite defaults by current values
    tSettingsHDR.SaveToRegistry();                  // set default values, if not yet present
}


void CSTEditorApp::SetSettingDefaults(SECEditSettings& rtSettings)
{
    CString sExtension = rtSettings.GetFileMappingKey();

    if(sExtension.CompareNoCase(_T("ST")) == 0)
    {
        // sizeof (LOGFONT) gives you the maximum size of a LOGFONT structure.
        // CGdiObject::GetObject possibly returns less because the font name
        // at the end of the LOGFONT structure may not take the whole space
        // reserved for it.
        HFONT       hFont;
        LOGFONT     logFont;
        CGdiObject* pGdiObject;

        hFont = (HFONT)GetStockObject (ANSI_FIXED_FONT);
        pGdiObject = CGdiObject::FromHandle (hFont);
        if (pGdiObject != NULL && pGdiObject->GetObject (sizeof (LOGFONT), &logFont) != 0)
        {
            int iFontSize;
            CWindowDC dc (m_pMainWnd);

            rtSettings.SetFontName(logFont.lfFaceName);
            iFontSize = MulDiv (logFont.lfHeight, 72, dc.GetDeviceCaps (LOGPIXELSY));
            rtSettings.SetFontSize(iFontSize);
            rtSettings.SetFontWeight(logFont.lfWeight);
            rtSettings.SetFontItalic(logFont.lfItalic);
            rtSettings.SetCharSet(logFont.lfCharSet);
        }

        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("IL")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(8);                           // tab is as wide as 8 spaces
        rtSettings.SetKeywordFile(_T("ilkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("4CH")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("4chkeywords.ini"));   // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("CON")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("CST")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("GVL")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("SDT")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("RES")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else if(sExtension.CompareNoCase(_T("HDR")) == 0)
    {
        rtSettings.SetCase(FALSE);                          // keywords are case insensitive
        rtSettings.SetChroma(TRUE);                         // coloring enabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetTextColKeyword(RGB(0,0,192));         // keyword color middle blue
        rtSettings.SetTextColQuote(RGB(128,128,128));       // string color grey
        rtSettings.SetTextColComment(RGB(0,128,0));         // comment color dark green
        rtSettings.SetCommentStart1(_T("(*"));              // comments start with (*
        rtSettings.SetCommentEnd1(_T("*)"));                // comments end with *)
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
        rtSettings.SetKeywordFile(_T("stkeywords.ini"));    // set keyword ini file
    }
    else    // any other extension
    {
        rtSettings.SetChroma(FALSE);                        // coloring disabled
        rtSettings.SetBackgroundCol(RGB(255,255,255));      // background color white
        rtSettings.SetTextColDefault(RGB(0,0,0));           // text color black
        rtSettings.SetKeepTabs(TRUE);                       // tabs are not converted to spaces
        rtSettings.SetShowTabs(FALSE);                      // tabs are not visualized
        rtSettings.SetTabSize(4);                           // tab is as wide as 4 spaces
    }
}
// AUTOFMT 09.08.06 SIS <<



int CSTEditorApp::ExitInstance ()
{
    _Module.Term ();

    IFCSessions *pSessions = NULL;
    if (SUCCEEDED( CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (void**)&pSessions) ))
    {
        pSessions->Shutdown_Gracefully();
        pSessions->Release();
    }

    return (CWinApp::ExitInstance ());
}


int CSTEditorApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
	// disable windows for modal dialog
	EnableModeless(FALSE);
	HWND hWndTop;
	HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);

	// set help context if possible
	DWORD* pdwContext = NULL;
	if (hWnd != NULL)
	{
#ifdef BUGGY_DOMESSAGEBOX
		// use app-level context or frame level context
		LRESULT lResult = ::SendMessage(hWndTop, WM_HELPPROMPTADDR, 0, 0);
#else
		// In cases where the topmost HWND has no active popup child (which
		// is the normal case) GetSafeOwner_ returns NULL in hWndTop and the
		// topmost HWND in its return value. SendMessage does not seem to
		// bother about sending a message to a NULL HWND although I can't be-
		// lieve that it was intended to do so. Probably the return value of
		// GetSafeOwner_ was intended to be used in this case. However if
		// there is an active popup child (e.g. the Find dialog) GetSafeOwner_
		// returns the topmost HWND in hWndTop and the popup childs HWND in
		// its return value. In case of inplace activation of an active docu-
		// ment this topmost HWND belongs to some other process. Sending
		// WM_HELPPROMPTADDR to this window will return a address from the
		// other processes address space. It seems to be a better solution to
		// use AfxGetMainWnd which always returns a CWnd connected to a HWND
		// that belongs to the own process.
		HWND hWndTopLocal = AfxGetMainWnd ()->GetSafeHwnd ();
		LRESULT lResult = ::SendMessage(hWndTopLocal, WM_HELPPROMPTADDR, 0, 0);
#endif
		if (lResult != 0)
			pdwContext = (DWORD*)lResult;
	}

	// for backward compatibility use app context if possible
	if (pdwContext == NULL && this != NULL)
		pdwContext = &m_dwPromptContext;

	DWORD dwOldPromptContext = 0;
	if (pdwContext != NULL)
	{
		// save old prompt context for restoration later
		dwOldPromptContext = *pdwContext;
		if (nIDPrompt != 0)
			*pdwContext = HID_BASE_PROMPT+nIDPrompt;
	}

	// determine icon based on type specified
	if ((nType & MB_ICONMASK) == 0)
	{
		switch (nType & MB_TYPEMASK)
		{
		case MB_OK:
		case MB_OKCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_YESNO:
		case MB_YESNOCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_ABORTRETRYIGNORE:
		case MB_RETRYCANCEL:
			// No default icon for these types, since they are rarely used.
			// The caller should specify the icon.
			break;
		}
	}

#ifdef _DEBUG
	if ((nType & MB_ICONMASK) == 0)
		TRACE0("Warning: no icon specified for message box.\n");
#endif

	TCHAR szAppName[_MAX_PATH];
	LPCTSTR pszAppName;
	if (this != NULL)
		pszAppName = m_pszAppName;
	else
	{
		pszAppName = szAppName;
		GetModuleFileName(NULL, szAppName, _MAX_PATH);
	}

	int nResult =
		::MessageBox(hWnd, lpszPrompt, pszAppName, nType);

	// restore prompt context if possible
	if (pdwContext != NULL)
		*pdwContext = dwOldPromptContext;

	// re-enable windows
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	EnableModeless(TRUE);

	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSTEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSTEditorApp commands

void CSTEditorApp::OnHelpFinder() 
{
   ShowHelp(0, HELP_FINDER);
}


void CSTEditorApp::WinHelp (DWORD dwData, UINT nCmd)
{
   ShowHelp(dwData, nCmd);
}


void CSTEditorApp::ShowHelp(DWORD dwData, UINT nCmd) 
{
    assert(m_pMainWnd->m_hWnd != NULL);
    CString strManualPath;
    UINT    nStrLen = _MAX_PATH;

    HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    if(SUCCEEDED(hr)) 
    {
       strManualPath += CE_MANUAL_FILENAME;
       switch(nCmd) 
       {
          case HELP_CONTEXT:
             if     (dwData >= 0x30000) dwData -= 0x30000;
             else if(dwData >= 0x20000) dwData -= 0x20000;
             else if(dwData >= 0x10000) dwData -= 0x10000;

             // Do not use standard help ids for find/replace dialog
             // because the dialog appearance has been modified by
             // stingray.

             if (dwData == AFX_IDD_FIND)
                 dwData = IDD_FIND_HELP_ID;
             else if (dwData == AFX_IDD_REPLACE)
                 dwData = IDD_REPLACE_HELP_ID;

             HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_HELP_CONTEXT, dwData); 
             break;
          case HELP_INDEX: 
             break;
          case HELP_FINDER:
             HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_HELP_CONTEXT, IDR_STTYPE);
             //HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_DISPLAY_TOPIC, NULL); 
             break;
      }
   }
}
