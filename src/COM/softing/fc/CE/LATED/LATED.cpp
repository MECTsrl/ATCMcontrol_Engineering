//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "LATED.h"
#include "mainfrm.h"
#include "childfrm.h"
#include "ipframe.h"
#include "lateddoc.h"
#include "latedview.h"
#include "winsock.h"
#include "toolbox.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLATEDApp

BEGIN_MESSAGE_MAP(CLATEDApp, CWinApp)
	//{{AFX_MSG_MAP(CLATEDApp)
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
// CLATEDApp construction

CLATEDApp::CLATEDApp()
{
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLATEDApp object

CLATEDApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {86046CB4-294D-4EB0-9411-13EB84F3888F}
static const CLSID clsid =
{ 0x86046cb4, 0x294d, 0x4eb0, { 0x94, 0x11, 0x13, 0xeb, 0x84, 0xf3, 0x88, 0x8f } };

/////////////////////////////////////////////////////////////////////////////
// CLATEDApp initialization

BOOL CLATEDApp::InitInstance()
{
    HRESULT hr = S_OK;
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
    // prevent the Server-Busy dialog
    COleMessageFilter *pFilter = AfxOleGetMessageFilter();
    assert (pFilter);
    if (pFilter != NULL)
    {
      pFilter->EnableNotRespondingDialog(FALSE);
      pFilter->EnableBusyDialog(FALSE);
    }

	AfxEnableControlContainer();


	WSADATA wsd;
	//Do not want to initalize and clean up sockets too often
	WSAStartup(0x0101,&wsd);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

     // set path to help/ATCM manual file
    SetHelpDirectory();
	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Softing\\4CONTROLV2\\4CONTROL Engineering"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_4CLCTYPE,
		RUNTIME_CLASS(CLATEDDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLATEDView));
	pDocTemplate->SetServerInfo(
		IDR_4CLCTYPE_SRVR_EMB, IDR_4CLCTYPE_SRVR_IP,
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
	CLATEDCommandLine cmdInfo;
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
    
    //Type library is registered but not unregistered
	if (cmdInfo.CommandRegister())
	{
		TCHAR szModule[_MAX_PATH+10];
		LPOLESTR pszModule;
		HRESULT hr;
		ITypeLib* pTypeLib;
		USES_CONVERSION;

		::GetModuleFileName (m_hInstance, szModule, _MAX_PATH);
		pszModule = T2OLE (szModule);
		hr = LoadTypeLib (pszModule, &pTypeLib);
		
        if (SUCCEEDED (hr) && cmdInfo.CommandRegister())
		{
			hr = RegisterTypeLib (pTypeLib, pszModule, NULL);
			pTypeLib->Release ();
		}
        
        RegisterComponentCategory(pDocTemplate);
        return FALSE;
	}

    
    // Prevents ProcessShellCommand from displaying
    // a message box when unregistering.
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
    {
        RegisterComponentCategory(pDocTemplate,false);
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

int CLATEDApp::ExitInstance()
{
    WSACleanup();
    return CWinApp::ExitInstance();
}

void CLATEDApp::RegisterComponentCategory(CDocTemplate* pTemplate, bool bRegister)
{
    CString str;
    CGuid guid;
    CRegKey key,keyCAT;
    long lRet;

    assert(pTemplate);
    //use the document string to determine the ProgId
    pTemplate->GetDocString(str,CDocTemplate::regFileTypeId);
    //this should be the ProgId;
    BSTR sProgId = str.AllocSysString();
    //then get CLSID from the ProgId
    ::CLSIDFromProgID(sProgId,guid);

    if(guid.IsEmpty()) { //not there
        return;
    }

    str = _T("CLSID\\");
    str += (LPTSTR) guid;

    /* open HKEY_CLASSES_ROOT\CLSID\clsid */
    lRet = key.Open(HKEY_CLASSES_ROOT,str);
    if(lRet != ERROR_SUCCESS) {
        return;        
    }

    str = IMPLEMENTED_CAT;

    if(bRegister) {
        //open implemented categories - create if not exists
        lRet = keyCAT.Open(key,str);
        if(lRet != ERROR_SUCCESS) {
            lRet = keyCAT.Create(key,str);
            if(lRet != ERROR_SUCCESS) {
                return;
            }

        }

        key.Close(); //reuse key
        guid.Empty();
        guid = CATID_4CActiveDocSrv;
        str = guid;
        //create component category.
        lRet = key.Create(keyCAT,str);
    }
    else { //unregister
        lRet = key.RecurseDeleteKey(str);    
    }
}

//------------------------------------------------------------------*
/**
 * Set path to help/ATCM manual file.
 * @return			void 
*/
void CLATEDApp::SetHelpDirectory()
{
    CString strManualPath;
    UINT    nStrLen = _MAX_PATH;
    UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    strManualPath.TrimRight(_T('\\'));
    strManualPath += CE_MANUAL_FILENAME;
    if (m_pszHelpFilePath != NULL) {
        free((void*) m_pszHelpFilePath);
    }
    m_pszHelpFilePath = _tcsdup(strManualPath);
}



void CLATEDApp::OnHelpFinder() 
{
   ShowHelp(0, HELP_FINDER);
}


void CLATEDApp::WinHelp (DWORD dwData, UINT nCmd)
{
   ShowHelp(dwData, nCmd);
}


void CLATEDApp::ShowHelp(DWORD dwData, UINT nCmd) 
{
    HWND hWnd = NULL;

    assert(m_pMainWnd->m_hWnd != NULL);
    
    switch(nCmd)  {
        case HELP_CONTEXT:
            if     (dwData >= 0x30000) dwData -= 0x30000;
            else if(dwData >= 0x20000) dwData -= 0x20000;
            else if(dwData >= 0x10000) dwData -= 0x10000;

            hWnd = HtmlHelp(m_pMainWnd->m_hWnd, m_pszHelpFilePath, 
                HH_HELP_CONTEXT, dwData); 

            if(!hWnd) {
                //error try with display topic
                hWnd = HtmlHelp(m_pMainWnd->m_hWnd, m_pszHelpFilePath, 
                            HH_DISPLAY_TOPIC, NULL); 
            }
            break;
        case HELP_INDEX: 
            break;
        case HELP_FINDER:
            HtmlHelp(m_pMainWnd->m_hWnd, m_pszHelpFilePath, HH_DISPLAY_TOPIC, NULL); 
        break;
    }


}

CLATEDCommandLine::CLATEDCommandLine() : CCommandLineInfo()
{
    m_bRegister = false;
    m_bUnregister = false;
    //trick disable file new initially.
    //usually it is set to FileNew.
    m_nShellCommand = FileNothing; 
}

void CLATEDCommandLine::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
    if(_tcsicmp(pszParam, _T("RegServer"))==0) {
        m_bRegister = true;        
    }

    if(_tcsicmp(pszParam, _T("UnregServer"))==0) {
        m_bUnregister = true;        
    }

    CCommandLineInfo::ParseParam(pszParam,bFlag,bLast);
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
	CString	m_strBuildVersion;
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
	m_strBuildVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_LATED_BUILDSTR, m_strBuildVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CString str;
    CVersionInfo    info;

    VERIFY(info.ReadInfoString(_T("ProductName"), str));
    m_strBuildVersion = str;
    VERIFY(info.ReadInfoString(_T("FileVersion"), str));
    m_strBuildVersion += _T(" (");
    m_strBuildVersion += str;
    m_strBuildVersion += _T(")");

    UpdateData(FALSE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



// App command to run the dialog
void CLATEDApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDApp message handlers

