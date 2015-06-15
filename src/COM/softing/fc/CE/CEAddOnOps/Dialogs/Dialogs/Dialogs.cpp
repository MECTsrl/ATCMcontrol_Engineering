
// Dialogs.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dialogs.h"
#include "GlobConst.h"
#include "utilif.h"
#include "CeSysdef.h"
#include "htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CDialogsApp

BEGIN_MESSAGE_MAP(CDialogsApp, CWinApp)
	//{{AFX_MSG_MAP(CDialogsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
        ON_COMMAND(ID_HELP, CWinApp::OnHelp) 
        ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





/////////////////////////////////////////////////////////////////////////////
// CDialogsApp construction

CDialogsApp::CDialogsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

BOOL CDialogsApp::InitInstance()
{
    return CWinApp::InitInstance();
}


void CDialogsApp::WinHelp(DWORD dwData, UINT nCmd) 
{
    CString strManualPath;
    UINT    nStrLen = _MAX_PATH;

    HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    if(SUCCEEDED(hr)) {
        strManualPath += CE_MANUAL_FILENAME;
        switch(nCmd) {
            case HELP_CONTEXT:
                if(dwData >= 0x30000) {
                    dwData -= 0x30000;
                } else if(dwData >= 0x20000) {
                    dwData -= 0x20000;
                } else if(dwData >= 0x10000) {
                    dwData -= 0x10000;
                }
                TRACE("DIALOGS> HelpID: %i\n", dwData);
                HtmlHelp(0, strManualPath, HH_HELP_CONTEXT, dwData);
                break;
            case HELP_INDEX:
                break;
            case HELP_FINDER:
                HtmlHelp(0, strManualPath, HH_DISPLAY_TOPIC, NULL);
                break;
        }
    }
    return;}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDialogsApp object

CDialogsApp theApp;

