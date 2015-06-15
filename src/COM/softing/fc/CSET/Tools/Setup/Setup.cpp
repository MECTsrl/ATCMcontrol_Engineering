/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Setup.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/Setup.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#include "stdafx.h"
#include "Setup.h"
#include "SetupDef.h"

#include "PropSheet.h"

#include "PropInstall.h"
#include "PropLangPath.h"
#include "PropLicense.h"
#include "PropSelEmb.h"
#include "PropSelPC.h"
#include "PropWelcome.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupApp

BEGIN_MESSAGE_MAP(CSetupApp, CWinApp)
	//{{AFX_MSG_MAP(CSetupApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupApp construction

CSetupApp::CSetupApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSetupApp object

CSetupApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSetupApp initialization

BOOL CSetupApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	AfxInitRichEdit();
	

	/* Create Property Sheet
	 * ------------------------------------------------------------------------
	 */
	CPropSheet PropSheet("");
	CInstall *pInstall = PropSheet.GetInstall();
	

	/* Command Line Handling (I)
	 * ------------------------------------------------------------------------
	 */
    CString sCmdParam(m_lpCmdLine);
	sCmdParam.MakeLower();


	CString sIniSpecial;

	int iIni = sCmdParam.Find(_T("/ini")) == -1 ? sCmdParam.Find(_T("-ini")) : sCmdParam.Find(_T("/ini")); 

    if (iIni != -1)
    {
        int iS1 = sCmdParam.Find('"', iIni);
		int iS2 = iS1 == -1 ? -1 : sCmdParam.Find('"', iS1 + 1);

		if (iS1 != -1 && iS2 != -1)
		{
			sIniSpecial = sCmdParam.Mid(iS1 + 1, iS2 - iS1 - 1);
		}

    }
	else
	{
// P.E. OSAI - 24/04/09
// Vedi oltre, sIniSpecial. Il nuovo default è ora OSAI.INI
//
		sIniSpecial = "OSAI.INI";
	}	


	CString sInstPathCmdLine;

	int iDir = sCmdParam.Find(_T("/dir")) == -1 ? sCmdParam.Find(_T("-dir")) : sCmdParam.Find(_T("/dir")); 

    if (iDir != -1)
    {
        int iS1 = sCmdParam.Find('"', iDir);
		int iS2 = iS1 == -1 ? -1 : sCmdParam.Find('"', iS1 + 1);

		if (iS1 != -1 && iS2 != -1)
		{
			sInstPathCmdLine = sCmdParam.Mid(iS1 + 1, iS2 - iS1 - 1);
		}

    }


	/* Set ini files
	 * ------------------------------------------------------------------------
	 */
	CString sIni;
	BOOL	bIniFileFound = FALSE;
	

// P.E. OSAI - 24/04/09
// Modificato per usare come default il file OSAI.INI
//
	if (sIniSpecial.IsEmpty())
		sIniSpecial = "OSAI.INI";

    if (! sIniSpecial.IsEmpty())
	{
		/* Use command line ini file first
		 */
		pInstall->GetExeDir(sIni);
		sIni += sIniSpecial;

		CFileFind FF;
		bIniFileFound = FF.FindFile(sIni);

		if (bIniFileFound == TRUE)
		{
			pInstall->SetIniFile(SET_INI_FILE_COMMON, sIniSpecial, SET_INI_FILE_LANGUAGE);
		}
		else
		{
// P.E. OSAI - 24/04/09
// Aggiunto messaggio di errore nel caso manchi il file .INI
			sIniSpecial += " file not found.";
			AfxMessageBox(sIniSpecial, MB_OK | MB_ICONEXCLAMATION);
		}
	}

	if (! bIniFileFound)
	{
		/* Try PC-Based ini file next
		 */
		pInstall->GetExeDir(sIni);
		sIni += SET_INI_FILE_PC;
		
		CFileFind FF;
		bIniFileFound = FF.FindFile(sIni);

		if (bIniFileFound == TRUE)
		{
			pInstall->SetIniFile(SET_INI_FILE_COMMON, SET_INI_FILE_PC, SET_INI_FILE_LANGUAGE);
		}
	}

	if (! bIniFileFound)
	{
		/* If no PC-Based ini file is present, use first ini file found.
		 */
		pInstall->GetExeDir(sIni);
		sIni += _T("*.ini");
		
		CFileFind FF;
		bIniFileFound = FF.FindFile(sIni);

		while (bIniFileFound == TRUE)
		{
			bIniFileFound = FF.FindNextFile();

			if (FF.IsDots() || FF.IsDirectory())
			{
				continue;
			}

			CString sIniFile = FF.GetFileName();
			if (sIniFile.CompareNoCase(SET_INI_FILE_COMMON) == 0 || sIniFile.CompareNoCase(SET_INI_FILE_PC) == 0
			||  sIniFile.CompareNoCase(SET_INI_FILE_LANGUAGE) == 0)
			{
				continue;
			}

			pInstall->SetIniFile(SET_INI_FILE_COMMON, FF.GetFileName(), SET_INI_FILE_LANGUAGE);

			break;
		}
	}

	
	/* Set Setup Type
	 * ------------------------------------------------------------------------
	 */
	CString sType;
	pInstall->GetKeyFromIni(SET_SCT_GENERAL, SET_KEY_TYPE, sType);
	sType.MakeUpper();
	
	if (sType != SET_TYPE_PC && sType != SET_TYPE_EMBEDDED)
	{
		AfxMessageBox("Invalid Installation Type.", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}


	/* Create Property Pages
	 * ------------------------------------------------------------------------
	 */
	CPropInstall		*pInst	= (CPropInstall *)	new CPropInstall;
	CPropLangPath		*pLang	= (CPropLangPath *) new CPropLangPath;
	CPropLicense		*pLic	= (CPropLicense *)	new CPropLicense;
// P.E. OSAI - 24/04/09
// Eliminate pagine di selezione dei tipi di target ammessi
	CPropSelEmb			*pEmb	= NULL;
	CPropSelPC			*pPC	= NULL;
//	CPropSelEmb			*pEmb	= (CPropSelEmb *) (sType == SET_TYPE_EMBEDDED ? new CPropSelEmb : NULL);
//	CPropSelPC			*pPC	= (CPropSelPC *)  (sType == SET_TYPE_PC		  ? new CPropSelPC  : NULL);
//	
	CPropWelcome		*pWel	= (CPropWelcome *)	new CPropWelcome;

	PropSheet.AddPage(pWel);
	PropSheet.AddPage(pLic);
	PropSheet.AddPage(pLang);
	if (pEmb != NULL)
	{
		PropSheet.AddPage(pEmb);
	}
	if (pPC != NULL)
	{
		PropSheet.AddPage(pPC);
	}
	PropSheet.AddPage(pInst);


	/* Command Line Handling (II)
	 * ------------------------------------------------------------------------
	 */
    if (sCmdParam.Find(_T("/log")) != -1 || 
        sCmdParam.Find(_T("-log")) != -1)
    {
        PropSheet.GetInstall()->SetDebugLog(TRUE);
    }

	if (sCmdParam.Find(_T("/trc")) != -1 ||
		sCmdParam.Find(_T("-trc")) != -1)
	{
		PropSheet.GetInstall()->SetTrace(TRUE);
	}

	if (sCmdParam.Find(_T("/all")) != -1 ||
		sCmdParam.Find(_T("-all")) != -1)
	{
		PropSheet.GetInstall()->SetInstallAll(TRUE);

		PropSheet.SetActivePage(pInst);
	}

	if (sCmdParam.Find(_T("/uni_all")) != -1 ||
		sCmdParam.Find(_T("-uni_all")) != -1)
	{
		PropSheet.GetInstall()->SetUnInstallAll(TRUE);

		PropSheet.SetActivePage(pInst);
	}

	if (sCmdParam.Find(_T("/auto")) != -1 ||
		sCmdParam.Find(_T("-auto")) != -1)
	{
		PropSheet.SetActivePage(pInst);
	}

	if (sCmdParam.Find(_T("/close")) != -1 ||
		sCmdParam.Find(_T("-close")) != -1)
	{
		PropSheet.GetInstall()->SetAutoClose(TRUE);
	}

	if (sCmdParam.Find(_T("/uni_only")) != -1 ||
		sCmdParam.Find(_T("-uni_only")) != -1)
	{
		pInstall->SetUnInstallOnly(TRUE);

		PropSheet.SetActivePage(pInst);
	}

	if (sCmdParam.Find(_T("/dir")) != -1 ||
		sCmdParam.Find(_T("-dir")) != -1)
	{
		pInstall->SetInstallPathCmdLine(sInstPathCmdLine);
		pLang->SetInstallPathCmdLine(sInstPathCmdLine);
	}
	else
	{
// P.E. OSAI - 24/04/09
// Il nuovo default è quello P.E. OSAI che una volta era passato come parametro dal file SETUP_OSAI.BAT 
		pInstall->SetInstallPathCmdLine("C:\\ATCMControl");
		pLang->SetInstallPathCmdLine("C:\\ATCMControl");
	}


	PropSheet.GetInstall()->DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_RUNONCE, SET_HVAL_RUNONCE);

	PropSheet.SetWizardMode();


	
	/* --> GO!
	 * ------------------------------------------------------------------------
	 */
	PropSheet.DoModal();

	delete	pInst;
	delete	pLang;
	delete	pLic;
	delete	pEmb;
	delete	pPC;
	delete	pWel;
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


/* ---------------------------------------------------------------------------- */
