/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropLicense.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropLicense.cpp $
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
#include "SetupDef.h"
#include "PropSheet.h"
#include "PropLicense.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropLicense property page

IMPLEMENT_DYNCREATE(CPropLicense, CPropBase)

/* CPropLicense
 * ----------------------------------------------------------------------------
 */
CPropLicense::CPropLicense() : CPropBase(CPropLicense::IDD)
{
	//{{AFX_DATA_INIT(CPropLicense)
	m_iAccept = 0;
	//}}AFX_DATA_INIT

	m_bInitialized = FALSE;
}


/* ~CPropLicense
 * ----------------------------------------------------------------------------
 */
CPropLicense::~CPropLicense()
{

}


/* DoDataExchange
 * ----------------------------------------------------------------------------
 */
void CPropLicense::DoDataExchange(CDataExchange* pDX)
{
	CPropBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropLicense)
	DDX_Control(pDX, IDC_RICHEDIT, m_cRichEdit);
	DDX_Radio(pDX, IDC_RADIO_NO_ACCEPT, m_iAccept);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropLicense, CPropBase)
	//{{AFX_MSG_MAP(CPropLicense)
	ON_BN_CLICKED(IDC_RADIO_ACCEPT, OnRadioAccept)
	ON_BN_CLICKED(IDC_RADIO_NO_ACCEPT, OnRadioNoAccept)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropLicense methods

/* MyStreamInCallback
 * ----------------------------------------------------------------------------
 */
static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*) dwCookie;

   *pcb = pFile->Read(pbBuff, cb);

   return 0;
}


/* SetWizardButtons
 * ----------------------------------------------------------------------------
 */
void CPropLicense::SetWizardButtons() 
{
	UpdateData(TRUE);

	if (! m_bInitialized)
	{
		GetPropSheet()->SetWizardButtons(0);
	}
	else
	{
		GetPropSheet()->SetWizardButtons((m_iAccept == 1 ? PSWIZB_NEXT : 0) | PSWIZB_BACK);	
	}
}


/////////////////////////////////////////////////////////////////////////////
// CPropLicense message handlers

/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CPropLicense::OnInitDialog() 
{
	CPropBase::OnInitDialog();
	
	/* Load license RTF-File
	 */
	CString sLicFile;
	GetPropSheet()->GetInstall()->GetExeDir(sLicFile);

	sLicFile += SET_LIC_FILE;

	CFile *pLicFile = NULL;

	TRY
	{
		pLicFile = new CFile (sLicFile, CFile::modeRead);
	}
	CATCH_ALL( e )
	{
		m_bInitialized = FALSE;
		
		THROW_LAST();
	}
	END_CATCH_ALL

	m_bInitialized = TRUE;

	EDITSTREAM es;
	
	es.dwCookie = (DWORD)pLicFile;
	es.pfnCallback = MyStreamInCallback; 
	m_cRichEdit.StreamIn(SF_RTF, es);

	delete pLicFile;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* OnSetActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropLicense::OnSetActive() 
{
	SetWizardButtons();

	return CPropBase::OnSetActive();
}


/* OnKillActive
 * ----------------------------------------------------------------------------
 */
BOOL CPropLicense::OnKillActive() 
{
	UpdateData(FALSE);
	
	return CPropBase::OnKillActive();
}


/* OnRadioAccept
 * ----------------------------------------------------------------------------
 */
void CPropLicense::OnRadioAccept() 
{
	SetWizardButtons();
}


/* OnRadioNoAccept
 * ----------------------------------------------------------------------------
 */
void CPropLicense::OnRadioNoAccept() 
{
	SetWizardButtons();
}

/* ---------------------------------------------------------------------------- */
