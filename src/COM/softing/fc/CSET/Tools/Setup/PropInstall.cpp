/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropInstall.cpp 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropInstall.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropInstall.cpp $
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
#include "PropInstall.h"
#include "SetupDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropInstall property page

IMPLEMENT_DYNCREATE(CPropInstall, CPropBase)

/* CPropInstall
 * ----------------------------------------------------------------------------
 */
CPropInstall::CPropInstall() : CPropBase(CPropInstall::IDD)
{
	//{{AFX_DATA_INIT(CPropInstall)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


/* ~CPropInstall
 * ----------------------------------------------------------------------------
 */
CPropInstall::~CPropInstall()
{

}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropInstall::DoDataExchange(CDataExchange* pDX)
{
	CPropBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropInstall)
	DDX_Control(pDX, IDC_LISTINFOBOX, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropInstall, CPropBase)
	//{{AFX_MSG_MAP(CPropInstall)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTINFOBOX, OnItemchangedListinfobox)
	ON_BN_CLICKED(IDC_INSTALL, OnInstall)
	ON_BN_CLICKED(IDC_INSTALL_ALL, OnInstallAll)
	ON_BN_CLICKED(IDC_UNINSTALL, OnUninstall)
	ON_BN_CLICKED(IDC_UNINSTALL_ALL, OnUninstallAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* GetListCtrl
 * ----------------------------------------------------------------------------
 */
CListCtrl *CPropInstall::GetListCtrl()
{
	return &m_ListCtrl;
}


/////////////////////////////////////////////////////////////////////////////
// CPropInstall message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropInstall::OnInitDialog() 
{
	CPropBase::OnInitDialog();
	
	GetInstall()->SetListCtrl(&m_ListCtrl);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropInstall::OnSetActive() 
{
	SetWizardButtons();

	GetInstall()->ActivateList(this);
	
	/* Continue install all after reboot or handle install all from command line
	 */
	if (GetInstall()->GetInstallAll() == TRUE && GetDlgItem(IDC_INSTALL_ALL)->IsWindowEnabled())
	{
		Sleep(1000);
		PostMessage(WM_COMMAND, MAKEWPARAM(IDC_INSTALL_ALL, BN_CLICKED),(LPARAM)GetDlgItem(IDC_INSTALL_ALL)->m_hWnd);
	}

	/* Handle uninstall all from command line
	 */
	if (GetInstall()->GetUnInstallAll() == TRUE && GetDlgItem(IDC_UNINSTALL_ALL)->IsWindowEnabled())
	{
		PostMessage(WM_COMMAND, MAKEWPARAM(IDC_UNINSTALL_ALL, BN_CLICKED),(LPARAM)GetDlgItem(IDC_UNINSTALL_ALL)->m_hWnd);
	}

	return CPropBase::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropInstall::OnKillActive() 
{
	UpdateData(TRUE);
	
	return CPropBase::OnKillActive();
}


/* OnItemchangedListinfobox
 * ----------------------------------------------------------------------------
 */
void CPropInstall::OnItemchangedListinfobox(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	GetInstall()->OnItemChangedListCtrl(this, pNMListView);
	
	*pResult = 0;
}


/* OnInstall
 * ----------------------------------------------------------------------------
 */
void CPropInstall::OnInstall() 
{
	GetInstall()->OnInstall(this);
}


/* OnInstallAll
 * ----------------------------------------------------------------------------
 */
void CPropInstall::OnInstallAll() 
{
	GetInstall()->OnInstallAll(this);
}


/* OnUninstall
 * ----------------------------------------------------------------------------
 */
void CPropInstall::OnUninstall() 
{
	GetInstall()->OnUninstall(this);
}


/* OnUninstallAll
 * ----------------------------------------------------------------------------
 */
void CPropInstall::OnUninstallAll() 
{
	GetInstall()->OnUninstallAll(this);
}


/* SetWizardButtons
 * ----------------------------------------------------------------------------
 */
void CPropInstall::SetWizardButtons()
{
	GetInstall()->SetWizardButtons(this);
}

/* ---------------------------------------------------------------------------- */
