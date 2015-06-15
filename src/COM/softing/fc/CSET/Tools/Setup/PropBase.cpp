/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropBase.cpp 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropBase.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropBase.cpp $
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
#include "PropBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropBase property page

BOOL CPropBase::m_bInitialized = FALSE;

IMPLEMENT_DYNAMIC(CPropBase, CPropertyPage)

/* CPropBase
 * ----------------------------------------------------------------------------
 */
CPropBase::CPropBase(UINT nIDTemplate, UINT nIDCaption) : CPropertyPage(nIDTemplate, nIDCaption)
{
	//{{AFX_DATA_INIT(CPropBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
    m_psp.dwFlags &= ~PSP_HASHELP;
}


/* ~CPropBase
 * ----------------------------------------------------------------------------
 */
CPropBase::~CPropBase()
{

}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropBase::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropBase, CPropertyPage)
	//{{AFX_MSG_MAP(CPropBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropBase methods

/* GetPropSheet
 * ----------------------------------------------------------------------------
 */
CPropSheet *CPropBase::GetPropSheet()
{
	CPropSheet *pPropSheet = (CPropSheet *)GetParent();

	ASSERT(pPropSheet);

	return pPropSheet;
}


/* GetInstall
 * ----------------------------------------------------------------------------
 */
CInstall *CPropBase::GetInstall()
{
	CInstall *pInstall = GetPropSheet()->GetInstall();

	ASSERT(pInstall);

	return pInstall;
}


/* EnableClose
 * ----------------------------------------------------------------------------
 */
void CPropBase::EnableClose(BOOL bEnable)
{
	CMenu *pSysMenu = GetPropSheet()->GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);

	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | (bEnable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
}


/////////////////////////////////////////////////////////////////////////////
// CPropBase message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropBase::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (m_bInitialized == FALSE)
	{
		/* Initialize Installer
		 */
		GetInstall()->InitializeInstaller();
		GetInstall()->UpdateListbox(TRUE);

		m_bInitialized = TRUE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropBase::OnSetActive() 
{

	return CPropertyPage::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropBase::OnKillActive() 
{
	UpdateData(TRUE);
	
	return CPropertyPage::OnKillActive();
}

/* ---------------------------------------------------------------------------- */
