/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropSelPC.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropSelPC.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 11.02.08 11:46 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 */

#include "stdafx.h"
#include "setup.h"
#include "PropSheet.h"
#include "PropSelPC.h"
#include "SetupDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSelPC property page

IMPLEMENT_DYNCREATE(CPropSelPC, CPropBase)


/* CPropSelPC
 * ----------------------------------------------------------------------------
 */
CPropSelPC::CPropSelPC() : CPropBase(CPropSelPC::IDD)
{
	//{{AFX_DATA_INIT(CPropSelPC)
	m_bControlLoop = FALSE;
	m_bRTCE = FALSE;
	m_bEngineering = FALSE;
	m_bDatabase = FALSE;
	m_bControl = FALSE;
	m_bConsole = FALSE;
	m_bFc = FALSE;
	m_bEc = FALSE;
	m_bBc = FALSE;
	m_bXc = FALSE;
	m_bDemo = FALSE;
	//}}AFX_DATA_INIT
}


/* ~CPropSelPC
 * ----------------------------------------------------------------------------
 */
CPropSelPC::~CPropSelPC()
{

}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::DoDataExchange(CDataExchange* pDX)
{
	CPropBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropSelPC)
	DDX_Control(pDX, IDC_CHECK_PC_CLL, m_cControlLoop);
	DDX_Control(pDX, IDC_CHECK_PC_CONS, m_cConsole);
	DDX_Control(pDX, IDC_CHECK_PC_CTL, m_cControl);
	DDX_Control(pDX, IDC_CHECK_PC_DBL, m_cDatabase);
	DDX_Control(pDX, IDC_CHECK_PC_ENG, m_cEngineering);
	DDX_Control(pDX, IDC_CHECK_PC_RTCE, m_cRTCE);
	DDX_Control(pDX, IDC_CHECK_PC_EC, m_cEc);
	DDX_Control(pDX, IDC_CHECK_PC_FC, m_cFc);
	DDX_Control(pDX, IDC_CHECK_PC_BC, m_cBc);
	DDX_Control(pDX, IDC_CHECK_PC_XC, m_cXc);
	DDX_Control(pDX, IDC_CHECK_PC_DEMO, m_cDemo);	
	DDX_Check(pDX, IDC_CHECK_PC_CLL, m_bControlLoop);
	DDX_Check(pDX, IDC_CHECK_PC_RTCE, m_bRTCE);
	DDX_Check(pDX, IDC_CHECK_PC_ENG, m_bEngineering);
	DDX_Check(pDX, IDC_CHECK_PC_DBL, m_bDatabase);
	DDX_Check(pDX, IDC_CHECK_PC_CTL, m_bControl);
	DDX_Check(pDX, IDC_CHECK_PC_CONS, m_bConsole);
	DDX_Check(pDX, IDC_CHECK_PC_EC, m_bEc);
	DDX_Check(pDX, IDC_CHECK_PC_FC, m_bFc);
	DDX_Check(pDX, IDC_CHECK_PC_BC, m_bBc);
	DDX_Check(pDX, IDC_CHECK_PC_XC, m_bXc);
	DDX_Check(pDX, IDC_CHECK_PC_DEMO, m_bDemo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropSelPC, CPropBase)
	//{{AFX_MSG_MAP(CPropSelPC)
	ON_BN_CLICKED(IDC_CHECK_PC_ENG, OnCheckPcEng)
	ON_BN_CLICKED(IDC_CHECK_PC_CTL, OnCheckPcCtl)
	ON_BN_CLICKED(IDC_CHECK_PC_EC, OnCheckPcEc)
	ON_BN_CLICKED(IDC_CHECK_PC_FC, OnCheckPcFc)
	ON_BN_CLICKED(IDC_CHECK_PC_BC, OnCheckPcBc)
	ON_BN_CLICKED(IDC_CHECK_PC_XC, OnCheckPcXc)
	ON_BN_CLICKED(IDC_CHECK_PC_DEMO, OnCheckDemo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropSelPC message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropSelPC::OnInitDialog() 
{
	CPropBase::OnInitDialog();
	
	CInstall *pInstall = GetInstall();

	/* Get ToDo List from registry
	 */
	DWORD dwToDo = 0;
	DWORD dwSize = sizeof(dwToDo);

	if (pInstall->TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP) 
		&& pInstall->QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_4C_COMPONENTS, (LPBYTE)&dwToDo, &dwSize))
	{
			m_bEngineering	= (dwToDo & (SET_FLAG_KERNEL | SET_FLAG_PCAO)) != 0;
			m_bControl		= (dwToDo & SET_FLAG_CTRL) != 0;
			m_bConsole		= (dwToDo & SET_FLAG_CONS) != 0;
			m_bRTCE			= (dwToDo & SET_FLAG_RTCE) != 0;
			m_bControlLoop	= (dwToDo & SET_FLAG_PCCL) != 0;
			m_bDatabase		= (dwToDo & SET_FLAG_PCDB) != 0;
			m_bFc			= (dwToDo & SET_FLAG_FCAO) != 0;
			m_bEc			= (dwToDo & SET_FLAG_ECAO) != 0;
			m_bBc			= (dwToDo & SET_FLAG_BCAO) != 0;
			m_bXc			= (dwToDo & SET_FLAG_XCAO) != 0;
			m_bDemo			= (dwToDo & SET_FLAG_DEMO) != 0;

			UpdateData(FALSE);

			m_cDatabase.EnableWindow(m_bEngineering);
			m_cControlLoop.EnableWindow(m_bEngineering);
			m_cEc.EnableWindow(m_bEngineering);
			m_cFc.EnableWindow(m_bEngineering);
			m_cBc.EnableWindow(m_bEngineering);
			m_cXc.EnableWindow(m_bEngineering);
//			m_cGa.EnableWindow(m_bEngineering);
			m_cDemo.EnableWindow(m_bEngineering);

			m_cConsole.EnableWindow((pInstall->IsWindowsVista() || pInstall->IsWindows2K3()) ? FALSE : m_bControl);
			m_cRTCE.EnableWindow(m_bControl);

	}
	else
	{
		// No registry entry, use default.
		if (! pInstall->IsInstSomething())
		{
			m_bEngineering	= TRUE;
			m_bControl		= TRUE;
			m_bConsole		= (pInstall->IsWindowsVista() || pInstall->IsWindows2K3()) ? FALSE : TRUE;
			m_bControlLoop	= TRUE;
			m_bDatabase		= TRUE;
			m_bFc			= TRUE;
			m_bEc			= TRUE;
			m_bBc			= TRUE;
			m_bXc			= TRUE;
			m_bDemo			= TRUE;
		}
		else
		{
			m_bEngineering	= pInstall->IsInst4C(SET_PRODKEY_PCAO) || pInstall->IsInst4C(SET_PRODKEY_KERNEL);
			m_bControl		= pInstall->IsInst4C(SET_PRODKEY_CTRL);
			m_bConsole		= (pInstall->IsWindowsVista() || pInstall->IsWindows2K3()) ? FALSE : m_bControl;
			m_bControlLoop	= pInstall->IsInst4C(SET_PRODKEY_PCCL);
			m_bDatabase		= pInstall->IsInst4C(SET_PRODKEY_PCDB);
			m_bFc			= pInstall->IsInst4C(SET_PRODKEY_FCAO);
			m_bEc			= pInstall->IsInst4C(SET_PRODKEY_ECAO);		
			m_bBc			= pInstall->IsInst4C(SET_PRODKEY_BCAO);
			m_bXc			= pInstall->IsInst4C(SET_PRODKEY_XCAO);		
			m_bDemo			= pInstall->IsInst4C(SET_PRODKEY_DEMO);		
		}

		m_cConsole.EnableWindow((pInstall->IsWindowsVista() || pInstall->IsWindows2K3()) ? FALSE : TRUE);

		UpdateData(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropSelPC::OnSetActive() 
{
	GetPropSheet()->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	
	return CPropBase::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropSelPC::OnKillActive() 
{
	UpdateData(TRUE);

	DWORD dwToDo = 0;
	
	dwToDo |= m_bEngineering	? SET_FLAG_KERNEL | SET_FLAG_PCAO	: 0;
	dwToDo |= m_bConsole		? SET_FLAG_CONS						: 0;
	dwToDo |= m_bRTCE			? SET_FLAG_RTCE						: 0;
	dwToDo |= m_bControl		? SET_FLAG_CTRL						: 0;
	dwToDo |= m_bDatabase		? SET_FLAG_PCDB						: 0;
	dwToDo |= m_bControlLoop	? SET_FLAG_PCCL						: 0;
	dwToDo |= m_bEc				? SET_FLAG_ECAO						: 0;
	dwToDo |= m_bFc				? SET_FLAG_FCAO						: 0;
	dwToDo |= m_bBc				? SET_FLAG_BCAO						: 0;
	dwToDo |= m_bXc				? SET_FLAG_XCAO						: 0;
	dwToDo |= m_bDemo			? SET_FLAG_DEMO						: 0;
	
	GetInstall()->SetTodo(dwToDo);

	return CPropBase::OnKillActive();
}


/* OnCheckPcEng
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckPcEng() 
{
	UpdateData(TRUE);

	if(m_bEngineering == FALSE)
	{
		m_bOldDatabase		= m_bDatabase;
		m_bOldControlLoop	= m_bControlLoop;
		m_bOldEc			= m_bEc;
		m_bOldFc			= m_bFc;
		m_bOldBc			= m_bBc;
		m_bOldXc			= m_bXc;
		m_bOldDemo			= m_bDemo;

		m_bDatabase		= FALSE;
		m_bControlLoop	= FALSE;
		m_bEc			= FALSE;
		m_bFc			= FALSE;
		m_bBc			= FALSE;
		m_bXc			= FALSE;
		m_bDemo			= FALSE;
	}
	else
	{
		m_bDatabase		= m_bOldDatabase;
		m_bControlLoop	= m_bOldControlLoop;
		m_bEc			= m_bOldEc;
		m_bFc			= m_bOldFc;
		m_bBc			= m_bOldBc;
		m_bXc			= m_bOldXc;
		m_bDemo			= m_bOldDemo;
	}

	UpdateData(FALSE);

	m_cDatabase.EnableWindow(m_bEngineering);
	m_cControlLoop.EnableWindow(m_bEngineering);
	m_cEc.EnableWindow(m_bEngineering);
	m_cFc.EnableWindow(m_bEngineering);
	m_cBc.EnableWindow(m_bEngineering);
	m_cXc.EnableWindow(m_bEngineering);
//	m_cGa.EnableWindow(m_bEngineering);
	m_cDemo.EnableWindow(m_bEngineering);
}


/* OnCheckPcCtl
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckPcCtl() 
{
	UpdateData(TRUE);

	if(m_bControl == FALSE)
	{
		m_bOldConsole = m_bConsole;
		m_bOldRTCE	  = m_bRTCE;

		m_bConsole	= FALSE;
		m_bRTCE		= FALSE;
	}
	else
	{
		m_bConsole	= m_bOldConsole;
		m_bRTCE		= m_bOldRTCE;
	}

	UpdateData(FALSE);

	m_cConsole.EnableWindow((GetInstall()->IsWindowsVista() || GetInstall()->IsWindows2K3()) ? FALSE : m_bControl);
	m_cRTCE.EnableWindow(m_bControl);
}


/* OnCheckPcEc
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckPcEc() 
{
	UpdateData(TRUE);
	
	if (m_bEngineering == TRUE && m_bEc == TRUE)
	{
		UpdateData(FALSE);
	}
}


/* OnCheckPcFc
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckPcFc() 
{
	UpdateData(TRUE);
	
	if (m_bEngineering == TRUE && m_bFc == TRUE)
	{
		UpdateData(FALSE);
	}
}


/* OnCheckPcBc
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckPcBc() 
{
	UpdateData(TRUE);
	
	if (m_bEngineering == TRUE && m_bBc == TRUE)
	{
		UpdateData(FALSE);
	}
}


/* OnCheckPcXc
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckPcXc() 
{
	UpdateData(TRUE);
	
	if (m_bEngineering == TRUE && m_bXc == TRUE)
	{
		UpdateData(FALSE);
	}
}


/* OnCheckDemo
 * ----------------------------------------------------------------------------
 */
void CPropSelPC::OnCheckDemo() 
{
	UpdateData(TRUE);
	
	if (m_bEngineering == TRUE && m_bDemo == TRUE)
	{
		UpdateData(FALSE);
	}
}


/* ---------------------------------------------------------------------------- */
