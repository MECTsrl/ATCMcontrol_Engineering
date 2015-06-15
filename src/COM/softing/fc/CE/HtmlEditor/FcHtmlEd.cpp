

#include "stdafx.h"
#include "buildnr.h"
#include "product.h"
#include "FcHtmlEd.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CHtmlCommandLineInfo : public CCommandLineInfo
{
public:
	CHtmlCommandLineInfo ();
	virtual void ParseParam (LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
	bool m_bRegServer;
};


CHtmlCommandLineInfo::CHtmlCommandLineInfo ()
{
	m_bRegServer = false;
}


void CHtmlCommandLineInfo::ParseParam (LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag)
	{
		if (CString (lpszParam).CompareNoCase (_T ("regserver")) == 0)
		{
			m_bRegServer = true;
			return;
		}
	}
	CCommandLineInfo::ParseParam (lpszParam, bFlag, bLast);
}


/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdApp

BEGIN_MESSAGE_MAP(CFcHtmlEdApp, CWinApp)
	//{{AFX_MSG_MAP(CFcHtmlEdApp)
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

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdApp construction

CFcHtmlEdApp::CFcHtmlEdApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFcHtmlEdApp object

CFcHtmlEdApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// old 1.3
// {30E37DE9-B1E6-11D2-9FD8-00A024363DFC}
//static const CLSID clsid =
//{ 0x30e37de9, 0xb1e6, 0x11d2, { 0x9f, 0xd8, 0x0, 0xa0, 0x24, 0x36, 0x3d, 0xfc } };

// new 2.0
// {FC41A9DA-8EA2-423e-A6C7-0075880EB896}
static const CLSID clsid =
{ 0xFC41A9DA, 0x8EA2, 0x423e, { 0xA6, 0xC7, 0x0, 0x75, 0x88, 0x0E, 0xB8, 0x96 } };


/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdApp initialization

BOOL CFcHtmlEdApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	
	// ????

	HRESULT hr = ::CoInitializeSecurity (
		NULL, -1, NULL, NULL,
	    RPC_C_AUTHN_LEVEL_NONE, 
	    RPC_C_IMP_LEVEL_IMPERSONATE, 
	    NULL, EOAC_NONE, NULL);
	ASSERT(SUCCEEDED (hr));		


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

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Softing\\4CONTROLV2\\4CONTROL Engineering"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	m_Options.LoadOptions();   // Load options from registry

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DOCUMENT,
		RUNTIME_CLASS(CFcHtmlEdDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFcHtmlEdView));
	pDocTemplate->SetServerInfo(
		IDR_SRVR_EMB, IDR_SRVR_IP,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

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
	CHtmlCommandLineInfo cmdInfo;
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
		LPTYPELIB pTypeLib;
		USES_CONVERSION;

		::GetModuleFileName (m_hInstance, szModule, _MAX_PATH);
		LPOLESTR pszModule = T2OLE (szModule);
		if (SUCCEEDED(LoadTypeLib (pszModule, &pTypeLib)))
		{
			RegisterTypeLib (pTypeLib, pszModule, NULL);
			pTypeLib->Release ();
		}
		return FALSE;
	}
	
    // Prevents ProcessShellCommand from displaying
    // a message box when unregistering.
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
        cmdInfo.m_bRunEmbedded = true;
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CFcHtmlEdApp::ExitInstance() 
{
	// save changed options
	m_Options.SaveOptions();	

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
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFcHtmlEdApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdApp message handlers
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strAboutText;
	GetDlgItemText(IDC_APP_VERSION, strAboutText);
	strAboutText += _T(PROD_FULL_VERSION_STR);
	SetDlgItemText(IDC_APP_VERSION, strAboutText);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
