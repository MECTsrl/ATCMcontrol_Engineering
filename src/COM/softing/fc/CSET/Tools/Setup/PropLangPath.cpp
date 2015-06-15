/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropLangPath.cpp 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropLangPath.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropLangPath.cpp $
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
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#include "stdafx.h"
#include "setup.h"
#include "PropSheet.h"
#include "PropLangPath.h"
#include "SetupDef.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropLangPath property page

IMPLEMENT_DYNCREATE(CPropLangPath, CPropBase)


/* CPropLangPath
 * ----------------------------------------------------------------------------
 */
CPropLangPath::CPropLangPath() : CPropBase(CPropLangPath::IDD)
{
	//{{AFX_DATA_INIT(CPropLangPath)
	m_sLanguage = _T("");
	m_sInstallPath = _T("");
	//}}AFX_DATA_INIT

	m_dwLanguage = SET_LANGID_EN;
}


/* ~CPropLangPath
 * ----------------------------------------------------------------------------
 */
CPropLangPath::~CPropLangPath()
{

}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropLangPath::DoDataExchange(CDataExchange* pDX)
{
	CPropBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropLangPath)
	DDX_Control(pDX, IDC_BOX_LANGUAGE, m_cBoxLanguage);
	DDX_Control(pDX, IDC_BOX_PATH, m_cBoxPath);
	DDX_Control(pDX, IDC_PATH, m_cInstallPath);
	DDX_Control(pDX, IDC_CHANGEPATH, m_cChangePath);
	DDX_Control(pDX, IDC_LANGUAGE, m_cLanguage);
	DDX_CBString(pDX, IDC_LANGUAGE, m_sLanguage);
	DDX_Text(pDX, IDC_PATH, m_sInstallPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropLangPath, CPropBase)
	//{{AFX_MSG_MAP(CPropLangPath)
	ON_CBN_SELCHANGE(IDC_LANGUAGE, OnSelchangeLanguage)
	ON_BN_CLICKED(IDC_CHANGEPATH, OnChangepath)
	ON_EN_KILLFOCUS(IDC_PATH, OnKillfocusPath)
	ON_EN_CHANGE(IDC_PATH, OnChangePath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropLangPath methods

/* CheckPath
 * ----------------------------------------------------------------------------
 */
BOOL CPropLangPath::CheckPath()
{
	if (m_bCheckPath == TRUE && ! GetInstall()->IsDirectory(m_sInstallPath))
	{
		CString sMessage;
		sMessage.Format(IDS_MSG_NOT_A_DIR, (LPCTSTR)m_sInstallPath);

		if (MessageBox(sMessage, GetInstall()->GetTitle(), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			if (! CreateDirectory(m_sInstallPath, NULL))
			{
				sMessage.Format(IDS_MSG_CREATEDIR_ERR, m_sInstallPath);
				MessageBox(sMessage, GetInstall()->GetTitle(), MB_OK | MB_ICONWARNING);
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	m_bCheckPath = FALSE;
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPropLangPath message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropLangPath::OnInitDialog() 
{
	CPropBase::OnInitDialog();
	
	CInstall *pInstall = GetInstall();

	
	/* Get installation path
	 */
	m_sInstallPath.Empty();
	
	DWORD dwSize = MAX_PATH;
	char szPath[MAX_PATH];

	if (pInstall->TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP)
		&& pInstall->QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_INSTPATH, (LPBYTE)szPath, &dwSize))
	{
		// Found in registry
		m_sInstallPath = szPath;
	}
	else
	{
		if (m_sInstallPathCmdLine.IsEmpty())
		{
			// New, use default
			CString sPath;
			if (pInstall->GetInstallPath(sPath))
			{
				m_sInstallPath = sPath;
			}
		}
		else
		{
			m_sInstallPath = m_sInstallPathCmdLine;
		}
	}

	pInstall->SetInstallPath(m_sInstallPath);


	/* Get installation language
	 */
	for (int i = m_cLanguage.GetCount(); i > 0 ; i--)
	{
		m_cLanguage.DeleteString(i - 1);
	}

// P.E. OSAI - 24/04/09
// Solo lingua inglese
//
//	m_cLanguage.AddString(SET_HELPLANG_EN);
	m_cLanguage.AddString(SET_HELPLANG_DE);

// P.E. OSAI - 24/04/09
// Solo lingua inglese
//
//	dwSize = sizeof(m_dwLanguage);
//
//	if (pInstall->TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP) 
//		&& pInstall->QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_LANGUAGE, (LPBYTE)&m_dwLanguage, &dwSize))
//	{
//		// Already added to registry
//		m_sLanguage = m_dwLanguage == SET_LANGID_DE ? SET_HELPLANG_DE : SET_HELPLANG_EN;
//	}
//	else
//	{
//		// New, use OS language as default
//		if (GetInstall()->GetLanguage() == LANG_GERMAN)
//		{
//			m_sLanguage		= SET_HELPLANG_DE;
//			m_dwLanguage	= SET_LANGID_DE;
//		}
//		else
//		{
//			m_sLanguage		= SET_HELPLANG_EN;
//			m_dwLanguage	= SET_LANGID_EN;
//		}
//	}
// P.E. OSAI - 24/04/09
// Imposta la lingua inglese e disabilita il box di selezione
	m_sLanguage		= SET_HELPLANG_EN;
	m_dwLanguage	= SET_LANGID_EN;
	m_cBoxLanguage.EnableWindow(FALSE);
	m_cLanguage.EnableWindow(FALSE);
//

	pInstall->SetInstallLang(m_dwLanguage);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropLangPath::OnSetActive() 
{
	m_bCheckPath = TRUE;
	m_bSkipTest	 = FALSE;

	BOOL bIsInstSomething = GetInstall()->IsInstSomething();

	// If a 4C component is already installed, disable path selection
	m_cChangePath.EnableWindow(! bIsInstSomething);
	m_cInstallPath.EnableWindow(! bIsInstSomething);
	m_cBoxPath.EnableWindow(! bIsInstSomething);


// P.E. OSAI - 24/04/09
// Box sempre disabilitato. default lingua inglese
//	// If a 4C component is already installed, disable language selection
//	m_cBoxLanguage.EnableWindow(! bIsInstSomething);
//	m_cLanguage.EnableWindow(! bIsInstSomething);
//	
	m_cBoxLanguage.EnableWindow(FALSE);
	m_cLanguage.EnableWindow(FALSE);
//

	GetPropSheet()->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	
	return CPropBase::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropLangPath::OnKillActive() 
{
	UpdateData(TRUE);
	
	GetInstall()->SetInstallPath(m_sInstallPath);
	GetInstall()->SetInstallLang(m_dwLanguage);
	
	return CPropBase::OnKillActive();
}


/* OnWizardNext
 * ----------------------------------------------------------------------------
 */
LRESULT CPropLangPath::OnWizardNext() 
{
	UpdateData(TRUE);
	
	m_bSkipTest = GetFocus()->GetSafeHwnd() == m_cInstallPath.GetSafeHwnd();

	if (! CheckPath())
	{
		return -1;
	}
	
	return CPropBase::OnWizardNext();
}


/* OnSelchangeLanguage
 * ----------------------------------------------------------------------------
 */
void CPropLangPath::OnSelchangeLanguage() 
{
	UpdateData(TRUE);

	m_dwLanguage = m_sLanguage == SET_HELPLANG_DE ? SET_LANGID_DE : SET_LANGID_EN;
	
	GetInstall()->SetInstallLang(m_dwLanguage);
}


/* OnChangepath
 * ----------------------------------------------------------------------------
 */
void CPropLangPath::OnChangepath() 
{
	UpdateData(TRUE);

	CInstall *pInstall = GetInstall();
	CString sInit;

	if (pInstall->IsDirectory(m_sInstallPath))
	{
		sInit = m_sInstallPath;
	}
	else
	{
		char szPath[MAX_PATH];
		DWORD dwSize = MAX_PATH;

		pInstall->QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_CURRVERSION, SET_HVAL_PROGDIR, (BYTE *)szPath, &dwSize);
		sInit = szPath;
	}

	CFolderDialog fd(sInit, pInstall->GetTitle());

	int iRet = fd.DoModal();

	switch(iRet)
	{
	case IDOK:
		pInstall->SetInstallPath(fd.m_sSelPath);
		m_sInstallPath = fd.m_sSelPath;
		UpdateData(FALSE);
		break;
		
	default:
		break;
	}
}


/* OnKillfocusPath
 * ----------------------------------------------------------------------------
 */
void CPropLangPath::OnKillfocusPath() 
{
	UpdateData(TRUE);

	if (! m_bSkipTest)
	{
		; // CheckPath();
	}

	m_bSkipTest = FALSE;
}


/* OnChangePath
 * ----------------------------------------------------------------------------
 */
void CPropLangPath::OnChangePath() 
{
	m_bCheckPath = TRUE;	
}


/* SetInstallPathCmdLine
 * ----------------------------------------------------------------------------
 */
void CPropLangPath::SetInstallPathCmdLine(LPCTSTR szInstPath)
{
	m_sInstallPathCmdLine = szInstPath;
}

/* ---------------------------------------------------------------------------- */
