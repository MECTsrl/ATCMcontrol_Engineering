/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropWelcome.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropWelcome.cpp $
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
#include "PropWelcome.h"
#include "SetupDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropWelcome property page

IMPLEMENT_DYNCREATE(CPropWelcome, CPropBase)

/* CPropWelcome
 * ----------------------------------------------------------------------------
 */
CPropWelcome::CPropWelcome() : CPropBase(CPropWelcome::IDD)
{
	//{{AFX_DATA_INIT(CPropWelcome)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/* ~CPropWelcome
 * ----------------------------------------------------------------------------
 */
CPropWelcome::~CPropWelcome()
{
}

/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropWelcome::DoDataExchange(CDataExchange* pDX)
{
	CPropBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropWelcome)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropWelcome, CPropBase)
	//{{AFX_MSG_MAP(CPropWelcome)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropWelcome message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropWelcome::OnInitDialog() 
{
	CPropBase::OnInitDialog();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropWelcome::OnSetActive() 
{
	GetPropSheet()->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropBase::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropWelcome::OnKillActive() 
{
	
	return CPropBase::OnKillActive();
}

/* OnWizardNext
 * ----------------------------------------------------------------------------
 */
LRESULT CPropWelcome::OnWizardNext() 
{
	UpdateData(TRUE);

	CInstall *pInstall = GetInstall();

	/* Check if operating system version is valid
	 */
	CString sOsCur;
	CString sOsMin;
	BOOL	bNeedSP;
	
	UINT uValidOS = pInstall->IsOSValid(sOsCur, sOsMin, &bNeedSP);

	pInstall->Trc("CPropWelcome::OnWizardNext", "", "OS valid = %d", uValidOS);
	pInstall->Trc("CPropWelcome::OnWizardNext", "", "Admin = %d", pInstall->IsAdmin());

	if (uValidOS == SET_OS_VALID_WARN)
	{
		CString sMsg;
		sMsg.Format(IDS_MSG_OSVERSION, sOsCur);

		if (MessageBox(sMsg, pInstall->GetTitle(), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) != IDYES)
		{
			EndDialog(IDCANCEL);
			return -1;
		}
	}
	else if (uValidOS == SET_OS_VALID_NOK)
	{
		CString sMsg;
		sMsg.Format(IDS_MSG_OSVERSION2, sOsCur, sOsMin);

		MessageBox(sMsg, pInstall->GetTitle(), MB_OK | MB_ICONHAND);
		EndDialog(IDCANCEL);
		return -1;
	}


	/* Check for administrator rights if OS is Windows NT based.
	 */
	if (pInstall->IsWindowsNTBased() && ! pInstall->IsAdmin())
	{
		CString sMsg;
		sMsg.Format(IDS_MSG_NO_ADMIN);

		MessageBox(sMsg, pInstall->GetTitle(), MB_OK | MB_ICONHAND);
		EndDialog(IDCANCEL);
		return -1;
	}
	
	
	return CPropBase::OnWizardNext();
}

/* ---------------------------------------------------------------------------- */
