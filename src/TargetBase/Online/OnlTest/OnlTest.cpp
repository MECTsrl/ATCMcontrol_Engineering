/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTest.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlTest.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTest.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "OnlTest.h"
#include "OnlTestDlg.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/////////////////////////////////////////////////////////////////////////////
// COnlTestApp

BEGIN_MESSAGE_MAP(COnlTestApp, CWinApp)
	//{{AFX_MSG_MAP(COnlTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnlTestApp construction

COnlTestApp::COnlTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COnlTestApp object

COnlTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COnlTestApp initialization

BOOL COnlTestApp::InitInstance()
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

	COnlTestDlg dlg;
	m_pMainWnd = &dlg;
	
	dlg.DoModal();

	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/* ---------------------------------------------------------------------------- */
