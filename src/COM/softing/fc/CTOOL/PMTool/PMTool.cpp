// PMTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PMTool.h"
#include "PMToolDlg.h"
#include "MainPropertySheet.h"
#include "PropPageBackup.h"
#include "PropPageRestore.h"
#include "PropPageDeploy.h"
#include "PropPageFileTransfer.h"

#define FLAG_BACKUP_PAGE        (0x01)
#define FLAG_RESTORE_PAGE       (0x02)
#define FLAG_DEPLOY_PAGE        (0x04)
#define FLAG_FILETRANSFER_PAGE  (0x08)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPMToolApp

BEGIN_MESSAGE_MAP(CPMToolApp, CWinApp)
	//{{AFX_MSG_MAP(CPMToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPMToolApp construction

CPMToolApp::CPMToolApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPMToolApp object

CPMToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPMToolApp initialization

BOOL CPMToolApp::InitInstance()
{
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

    HRESULT hr = CoInitialize(NULL);
    if(FAILED(hr))
    {
        CString strMessage;
        strMessage.Format(_T("Fatal Error: CoInitialize failed: hr=0x%X\n"), hr);
        ::AfxMessageBox(strMessage);
        return FALSE;
    }

    SetRegistryKey("Softing");

    CCommandLineInfo    tCmdInfo;
    ParseCommandLine(tCmdInfo);

    // if file is given in command line
    // this is interpreted as project path
    // and stored in settings for later use
    // in property sheet / pages
    CString         strProjectPath;
    if(!tCmdInfo.m_strFileName.IsEmpty())
    {
        CFileStatus tStatus;
        if(CFile::GetStatus(tCmdInfo.m_strFileName, tStatus) && (tStatus.m_attribute & CFile::directory))
        {
            strProjectPath = tCmdInfo.m_strFileName;
        }
    }

    CMainPropertySheet      tMainDlg(" PMTool");

    CPropPageBackup         tPPBackup(&tMainDlg, strProjectPath);
    CPropPageRestore        tPPRestore(&tMainDlg, strProjectPath);
    CPropPageDeploy         tPPDeploy(&tMainDlg, strProjectPath);
    CPropPageFileTransfer   tPPFileTransfer(&tMainDlg);

    UINT    uiPages = GetPagesToShow();
    
    if(uiPages & FLAG_BACKUP_PAGE)
    {
        tMainDlg.AddPage(&tPPBackup);
    }
    if(uiPages & FLAG_RESTORE_PAGE)
    {
        tMainDlg.AddPage(&tPPRestore);
    }
    if(uiPages & FLAG_DEPLOY_PAGE)
    {
        tMainDlg.AddPage(&tPPDeploy);
    }
    if(uiPages & FLAG_FILETRANSFER_PAGE)
    {
        tMainDlg.AddPage(&tPPFileTransfer);
    }

    tMainDlg.DoModal();

    CoUninitialize();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


UINT CPMToolApp::GetPagesToShow()
{
    UINT    uiPages(0xFFFFFFFF);
    HKEY    hKeyPMTool;
    LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Softing\\PMTool" ,0 , KEY_READ, &hKeyPMTool);
    if(lResult == ERROR_SUCCESS)
    {
        CString strTmp;
        UINT    uiTmp;
        DWORD   dwSize = sizeof(uiTmp);
        DWORD   dwType;
        lResult = RegQueryValueEx(hKeyPMTool, "Pages", NULL, &dwType, (unsigned char*)&uiTmp, &dwSize);
        if(lResult == ERROR_SUCCESS)
        {
            uiPages = uiTmp;
        }
    }
    return uiPages;
}
