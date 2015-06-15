

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"

#include "afxconv.h"
#include "malloc.h"
#include "GrEditor.h"
#include "4CGr_Version.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"

#include "htmlhelp.h"
#include "utilif.h"
#include "CESysDef.h"

#include <winspool.h>

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*proto*=====================================================================*/

// mfc winutil ...
void AFXAPI AfxGlobalFree(HGLOBAL hGlobal);

/*func*======================================================================*/

/////////////////////////////////////////////////////////////////////////////
// CGrEditorApp

BEGIN_MESSAGE_MAP(CGrEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CGrEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
   ON_COMMAND(ID_HELP, CWinApp::OnHelp) 
   ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp) 
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
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

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CSTCommandLineInfo::CSTCommandLineInfo ()
{
	m_bRegServer = false;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
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
// CGrEditorApp construction

CGrEditorApp::CGrEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

//   afxMemDF |= checkAlwaysMemDF;
}

CGrEditorApp::~CGrEditorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGrEditorApp object

CGrEditorApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {59119A13-6422-11D2-9A3D-0000C0D26FBC}  wizard generated - old
// {E46EFC83-FAF7-11D1-8E7B-444553540000}  V1.0 - V1.3
// {F8F838B1-096A-11d5-94E1-00E02939A881}  V2.0 ..
static const CLSID clsid =
{ 0xf8f838b1, 0x96a, 0x11d5, { 0x94, 0xe1, 0x0, 0xe0, 0x29, 0x39, 0xa8, 0x81 } };

/////////////////////////////////////////////////////////////////////////////
// CGrEditorApp initialization

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorApp::InitInstance()
{
	HRESULT hr;
	HFONT hFont;
	LOGFONT logFont;
	CGdiObject *pGdiObject;
	USES_CONVERSION;

   // Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_GR_OLE_INIT_FAILED);
		return FALSE;
	}
	hr = ::CoInitializeSecurity (NULL, -1, NULL, NULL,
	                             RPC_C_AUTHN_LEVEL_NONE, 
	                             RPC_C_IMP_LEVEL_IMPERSONATE, 
	                             NULL, EOAC_NONE, NULL);
	assert (SUCCEEDED (hr));    

   // prevent the Server-Busy dialog
   COleMessageFilter *pFilter = AfxOleGetMessageFilter();
   assert (pFilter);
   if (pFilter != NULL)
   {
      pFilter->EnableNotRespondingDialog(FALSE);
      pFilter->EnableBusyDialog(FALSE);
   }

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
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	SetRegistryKey (_T ("Softing\\4CONTROLV2\\4CONTROL Engineering"));

   int     iValue;
   CString strKey, strSec;
   strSec.LoadString(IDS_GR_REG_SECTION);

   strKey.LoadString(IDS_GR_REG_RUBBLINE);
	iValue = GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 0);
          WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, iValue );   // set Line draw mode to rubberlines

   strKey.LoadString(IDS_GR_REG_RUBB_RECON);
	iValue = GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 1);
          WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, iValue );   // set Line draw mode to rubberlines

   strKey.LoadString(IDS_GR_REG_ZOOMVALUE);
	iValue = GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 100);
          WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, iValue );   // set zoom factor for 4gr

   // default sizes for element insertion
   strKey.LoadString(IDS_GR_REG_STEPSIZE);
	iValue = GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, (4<<16)+4); // <0: AutoSize:on; HiWord:X; LoWord:Y
          WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, iValue );

    // ST actions we yes the STEditor settings ..
	WriteProfileInt    (_T ("ST"), _T ("Case"           ), 0                    );   // keywords are case insensitive
	WriteProfileInt    (_T ("ST"), _T ("Chroma"         ), 1                    );   // coloring enabled
	WriteProfileString (_T ("ST"), _T ("Background"     ), _T ("255,255,255"   ));   // background color white
	WriteProfileString (_T ("ST"), _T ("Default Text"   ), _T ("0,0,0"         ));   // text color black
	WriteProfileString (_T ("ST"), _T ("Quote"          ), _T ("128,128,128"   ));   // string color grey
	WriteProfileString (_T ("ST"), _T ("Comment"        ), _T ("0,128,0"       ));   // comment color red
	WriteProfileString (_T ("ST"), _T ("Comment Start 1"), _T ("(*"            ));   // comments start with (*
	WriteProfileString (_T ("ST"), _T ("Comment End 1"  ), _T ("*)"            ));   // comments end with *)
	WriteProfileInt    (_T ("ST"), _T ("Keep Tabs"      ), 0                    );   // tabs are not converted to spaces
	WriteProfileInt    (_T ("ST"), _T ("Show Tabs"      ), 0                    );   // tabs are not visualized
	WriteProfileInt    (_T ("ST"), _T ("Tab Size"       ), 4                    );   // tab is as wide as 4 spaces
	WriteProfileString (_T ("ST"), _T ("Keywords"       ), _T ("stkeywords.ini"));   //

	// sizeof (LOGFONT) gives you the maximum size of a LOGFONT structure.
	// CGdiObject::GetObject possibly returns less because the font name
	// at the end of the LOGFONT structure may not take the whole space
	// reserved for it.
	hFont = (HFONT)GetStockObject (ANSI_FIXED_FONT);
	pGdiObject = CGdiObject::FromHandle (hFont);
	if (pGdiObject != NULL && pGdiObject->GetObject (sizeof (LOGFONT), &logFont) != 0)
	{
		int iFontSize;
		CWindowDC dc (m_pMainWnd);

		WriteProfileString (_T ("ST"), _T ("Font Name"), A2T (logFont.lfFaceName));
		iFontSize = MulDiv (logFont.lfHeight, 72, dc.GetDeviceCaps (LOGPIXELSY));
		WriteProfileInt (_T ("ST"), _T ("Font Size"), iFontSize);
		WriteProfileInt (_T ("ST"), _T ("Font Weight"), logFont.lfWeight);
		WriteProfileInt (_T ("ST"), _T ("Font Italic"), logFont.lfItalic);
		WriteProfileInt (_T ("ST"), _T ("CharSet"), logFont.lfCharSet);
	};

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplate = new CMultiDocTemplate(
		IDR_GR_TYPE,
		RUNTIME_CLASS(CGrEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGrEditorView));
	m_pDocTemplate->SetServerInfo(
		IDR_GR_TYPE_SRVR_EMB, IDR_GR_TYPE_SRVR_IP,
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
	if (!pMainFrame->LoadFrame(IDR_GR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

   // create a dummy image .. to solve the problem with DeleteTempMap in OnIdle
   m_pDummyImage = new CImageList();
   VERIFY (m_pDummyImage->Create (16, 15, TRUE, 0, 1));      
   CBitmap bm;
   bm.LoadBitmap (IDR_GR_MAINFRAME);
   m_pDummyImage->Add (&bm, RGB (192, 192, 192));

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

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
int CGrEditorApp::ExitInstance()
{
   if (m_pDummyImage) { delete m_pDummyImage; m_pDummyImage = NULL; }

   _Module.Term();

    IFCSessions *pSessions = NULL;
    if (SUCCEEDED( CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (void**)&pSessions) ))
    {
        pSessions->Shutdown_Gracefully();
        pSessions->Release();
    }

   return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_GR_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual BOOL OnInitDialog();
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

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog()
{
   CString text;
   GetDlgItemText(IDC_GR_VERSIONINFO,text);
   text += "\t[";
   text += GREDITOR_VERSION_STR;
#ifdef _DEBUG
   text += "d";
#endif
   text += "]";
   SetDlgItemText(IDC_GR_VERSIONINFO,text);
   return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CGrEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorApp message handlers

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorApp::OnHelpFinder() 
{
   ShowHelp(0, HELP_FINDER);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorApp::WinHelp (DWORD dwData, UINT nCmd)
{
   ShowHelp(dwData, nCmd);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorApp::ShowHelp(DWORD dwData, UINT nCmd) 
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
             HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_HELP_CONTEXT, dwData); 
             break;
          case HELP_INDEX: 
             break;
          case HELP_FINDER:
             HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_HELP_CONTEXT, IDR_GR_TYPE);
           //HtmlHelp(m_pMainWnd->m_hWnd, strManualPath, HH_DISPLAY_TOPIC, NULL);
             break;
       }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorApp::OnFilePrintSetup()
{
   CWinApp::OnFilePrintSetup();

	POSITION pos = NULL;
	if (m_pDocManager != NULL)
   {
		pos = m_pDocManager->GetFirstDocTemplatePosition();
   }

	while (pos != NULL)
	{
		CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		ASSERT_KINDOF(CDocTemplate, pTemplate);

		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2 != NULL)
		{
			CDocument* pDocument = pTemplate->GetNextDoc(pos2);
         CGrEditorDoc *pDoc = dynamic_cast<CGrEditorDoc *>(pDocument);
         if( pDoc != NULL )
         {
            pDoc->InitPageSize();
            pDoc->SetMarginRect(TRUE);
            pDoc->ComputePageSize();
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);
         }
      }
	}
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
int CGrEditorApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
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
      nType |= MB_SETFOREGROUND;

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


   CString strTitle;
   strTitle.LoadString(IDS_GR_MSGBOX_TITLE);

	int nResult =
		::MessageBox(hWnd, lpszPrompt, (LPCTSTR)strTitle, nType);

	// restore prompt context if possible
	if (pdwContext != NULL)
		*pdwContext = dwOldPromptContext;

	// re-enable windows
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	EnableModeless(TRUE);

	return nResult;
}
