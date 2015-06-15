// TestProjManDlg1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestProjManDlg1.h"
#include "TestProjManDlg1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1App

BEGIN_MESSAGE_MAP(CTestProjManDlg1App, CWinApp)
	//{{AFX_MSG_MAP(CTestProjManDlg1App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1App construction

CTestProjManDlg1App::CTestProjManDlg1App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestProjManDlg1App object

CTestProjManDlg1App theApp;


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1App initialization

BOOL CTestProjManDlg1App::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance);
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    HRESULT hr; 
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        return -1;
    }


	CTestProjManDlg1Dlg* pDlg= new CTestProjManDlg1Dlg;
	m_pMainWnd = pDlg;
	int nResponse = pDlg->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
    
    pDlg->UnadviseProjEventHandler();
    pDlg->UnadviseMsgEventHandler();
    pDlg->UnadviseCompManEventHandler();

    delete pDlg;

    CoUninitialize();

    int *memo;
    FC_NewMemory((void**)&memo, 100);
    FC_FreeMemory(memo);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
