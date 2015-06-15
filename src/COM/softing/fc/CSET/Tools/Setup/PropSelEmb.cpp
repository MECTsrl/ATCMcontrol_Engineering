/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropSelEmb.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropSelEmb.cpp $
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
#include "setup.h"
#include "PropSheet.h"
#include "PropSelEmb.h"
#include "SetupDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSelEmb property page

IMPLEMENT_DYNCREATE(CPropSelEmb, CPropBase)


/* CPropSelEmb
 * ----------------------------------------------------------------------------
 */
CPropSelEmb::CPropSelEmb() : CPropBase(CPropSelEmb::IDD)
{
	//{{AFX_DATA_INIT(CPropSelEmb)
	m_bAddOn1 = FALSE;
	m_bAddOn2 = FALSE;
	m_bAddOn3 = FALSE;
	m_bAddOn4 = FALSE;
	m_bDemo = FALSE;
	m_bEngineering = FALSE;
	//}}AFX_DATA_INIT
}


/* ~CPropSelEmb
 * ----------------------------------------------------------------------------
 */
CPropSelEmb::~CPropSelEmb()
{

}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropSelEmb::DoDataExchange(CDataExchange* pDX)
{
	CPropBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropSelEmb)
	DDX_Control(pDX, IDC_CHECK_EMB_AO1, m_cAddOn1);
	DDX_Control(pDX, IDC_CHECK_EMB_AO2, m_cAddOn2);
	DDX_Control(pDX, IDC_CHECK_EMB_AO3, m_cAddOn3);
	DDX_Control(pDX, IDC_CHECK_EMB_AO4, m_cAddOn4);
	DDX_Control(pDX, IDC_CHECK_EMB_DEMO, m_cDemo);
	DDX_Control(pDX, IDC_CHECK_EMB_ENG, m_cEngineering);
	DDX_Check(pDX, IDC_CHECK_EMB_AO1, m_bAddOn1);
	DDX_Check(pDX, IDC_CHECK_EMB_AO2, m_bAddOn2);
	DDX_Check(pDX, IDC_CHECK_EMB_AO3, m_bAddOn3);
	DDX_Check(pDX, IDC_CHECK_EMB_AO4, m_bAddOn4);
	DDX_Check(pDX, IDC_CHECK_EMB_DEMO, m_bDemo);
	DDX_Check(pDX, IDC_CHECK_EMB_ENG, m_bEngineering);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropSelEmb, CPropBase)
	//{{AFX_MSG_MAP(CPropSelEmb)
	ON_BN_CLICKED(IDC_CHECK_EMB_ENG, OnCheckEmbEng)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropSelEmb message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropSelEmb::OnInitDialog() 
{
	CPropBase::OnInitDialog();
	
	CInstall *pInstall = GetInstall();

	pInstall->GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO1, m_sAddOn1);
	pInstall->GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO2, m_sAddOn2);
	pInstall->GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO3, m_sAddOn3);
	pInstall->GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO4, m_sAddOn4);

	/* Get ToDo List from registry
	 */
	DWORD dwToDo = 0;
	DWORD dwSize = sizeof(dwToDo);

	if (pInstall->TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP) 
		&& pInstall->QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, pInstall->GetEmbeddedKey(), (LPBYTE)&dwToDo, &dwSize))
	{
			m_bEngineering	= (dwToDo & SET_FLAG_KERNEL) != 0;
			
			m_bDemo			= (dwToDo & SET_FLAG_DEMO) != 0;
			m_bAddOn1		= (dwToDo & SET_FLAG_AO1) != 0;
			m_bAddOn2		= (dwToDo & SET_FLAG_AO2) != 0;
			m_bAddOn3		= (dwToDo & SET_FLAG_AO3) != 0;
			m_bAddOn4		= (dwToDo & SET_FLAG_AO4) != 0;

			m_cDemo.EnableWindow(m_bEngineering);
			m_cAddOn1.EnableWindow(m_bEngineering);
			m_cAddOn2.EnableWindow(m_bEngineering);
			m_cAddOn3.EnableWindow(m_bEngineering);
			m_cAddOn4.EnableWindow(m_bEngineering);
	}
	else
	{
		// No registry entry, use default.
		if (! pInstall->IsInstSomething())
		{
			m_bEngineering	= TRUE;
			
			m_bDemo			= TRUE;

			m_bAddOn1		= TRUE && ! m_sAddOn1.IsEmpty();
			m_bAddOn2		= TRUE && ! m_sAddOn2.IsEmpty();
			m_bAddOn3		= TRUE && ! m_sAddOn3.IsEmpty();
			m_bAddOn4		= TRUE && ! m_sAddOn4.IsEmpty();
		}
		else
		{
			m_bEngineering	= pInstall->IsInst4C(SET_PRODKEY_KERNEL);

			m_bDemo			= pInstall->IsInst4C(SET_PRODKEY_DEMO);

			m_bAddOn1		= ! m_sAddOn1.IsEmpty() && pInstall->IsInst4C(m_sAddOn1);
			m_bAddOn2		= ! m_sAddOn2.IsEmpty() && pInstall->IsInst4C(m_sAddOn2);
			m_bAddOn3		= ! m_sAddOn3.IsEmpty() && pInstall->IsInst4C(m_sAddOn3);
			m_bAddOn4		= ! m_sAddOn4.IsEmpty() && pInstall->IsInst4C(m_sAddOn4);
		}
	}

	UpdateData(FALSE);

	m_cAddOn1.ShowWindow(m_sAddOn1.IsEmpty() ? SW_HIDE : SW_SHOW);
	m_cAddOn2.ShowWindow(m_sAddOn2.IsEmpty() ? SW_HIDE : SW_SHOW);
	m_cAddOn3.ShowWindow(m_sAddOn3.IsEmpty() ? SW_HIDE : SW_SHOW);
	m_cAddOn4.ShowWindow(m_sAddOn4.IsEmpty() ? SW_HIDE : SW_SHOW);

	CString sDesc;

	if (! m_sAddOn1.IsEmpty())
	{
		pInstall->GetKeyFromIni(SET_SCT_DESCRIPTION, m_sAddOn1, sDesc);
		m_cAddOn1.SetWindowText(sDesc);
	}

	if (! m_sAddOn2.IsEmpty())
	{
		pInstall->GetKeyFromIni(SET_SCT_DESCRIPTION, m_sAddOn2, sDesc);
		m_cAddOn2.SetWindowText(sDesc);
	}

	if (! m_sAddOn3.IsEmpty())
	{
		pInstall->GetKeyFromIni(SET_SCT_DESCRIPTION, m_sAddOn3, sDesc);
		m_cAddOn3.SetWindowText(sDesc);
	}

	if (! m_sAddOn4.IsEmpty())
	{
		pInstall->GetKeyFromIni(SET_SCT_DESCRIPTION, m_sAddOn4, sDesc);
		m_cAddOn4.SetWindowText(sDesc);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropSelEmb::OnSetActive() 
{
	GetPropSheet()->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	
	return CPropBase::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropSelEmb::OnKillActive() 
{
	UpdateData(TRUE);
	
	DWORD dwToDo = 0;
	
	dwToDo |= m_bEngineering	? SET_FLAG_KERNEL	: 0;
	dwToDo |= m_bDemo			? SET_FLAG_DEMO		: 0;
	dwToDo |= m_bAddOn1			? SET_FLAG_AO1		: 0;
	dwToDo |= m_bAddOn2			? SET_FLAG_AO2		: 0;
	dwToDo |= m_bAddOn3			? SET_FLAG_AO3		: 0;
	dwToDo |= m_bAddOn4			? SET_FLAG_AO4		: 0;
	
	GetInstall()->SetTodo(dwToDo);

	return CPropBase::OnKillActive();
}


/* OnCheckEmbEng
 * ----------------------------------------------------------------------------
 */
void CPropSelEmb::OnCheckEmbEng() 
{
	UpdateData(TRUE);
	
	if (m_bEngineering == FALSE)
	{
		m_bOldDemo	 = m_bDemo;
		m_bOldAddOn1 = m_bAddOn1;
		m_bOldAddOn2 = m_bAddOn2;
		m_bOldAddOn3 = m_bAddOn3;
		m_bOldAddOn4 = m_bAddOn4;

		m_bDemo	  = FALSE;
		m_bAddOn1 = FALSE;
		m_bAddOn2 = FALSE;
		m_bAddOn3 = FALSE;
		m_bAddOn4 = FALSE;
	}
	else
	{
		m_bDemo	  = m_bOldDemo;		
		m_bAddOn1 = m_bOldAddOn1;
		m_bAddOn2 = m_bOldAddOn2;
		m_bAddOn3 = m_bOldAddOn3;
		m_bAddOn4 = m_bOldAddOn4;
	}

	m_cDemo.EnableWindow(m_bEngineering);
	m_cAddOn1.EnableWindow(m_bEngineering);
	m_cAddOn2.EnableWindow(m_bEngineering);
	m_cAddOn3.EnableWindow(m_bEngineering);
	m_cAddOn4.EnableWindow(m_bEngineering);

	UpdateData(FALSE);
}

/* ---------------------------------------------------------------------------- */
