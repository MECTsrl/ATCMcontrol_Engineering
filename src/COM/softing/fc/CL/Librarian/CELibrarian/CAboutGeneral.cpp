/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CAboutGeneral.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CAboutGeneral.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CAboutGeneral.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "celibrarian.h"
#include "CAboutGeneral.h"
#include "toolbox.h"
#include "buildnr.h"
#include "product.h"
//----  Static Initializations:   ----------------------------------*



// CAboutGeneral.cpp: Implementierungsdatei
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAboutGeneral 


CAboutGeneral::CAboutGeneral(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutGeneral::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutGeneral)
	m_sAddress = _T("");
	m_sDisclaimer = _T("");
	m_sVersionNumber = _T("");
	m_sVersionTitle = _T("");
	m_sCopyright = _T("");
	//}}AFX_DATA_INIT
}


void CAboutGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutGeneral)
	DDX_Text(pDX, ID_ADDRESS, m_sAddress);
	DDX_Text(pDX, ID_DISCLAIMER, m_sDisclaimer);
	DDX_Text(pDX, ID_VERSION_NUMBER, m_sVersionNumber);
	DDX_Text(pDX, ID_VERSION_TITLE, m_sVersionTitle);
	DDX_Text(pDX, IDC_COPYRIGHT, m_sCopyright);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutGeneral, CDialog)
	//{{AFX_MSG_MAP(CAboutGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CAboutGeneral 

BOOL CAboutGeneral::OnInitDialog() 
{
    LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfHeight = 18;
	logfont.lfWeight = FW_SEMIBOLD;
    logfont.lfPitchAndFamily = FF_SWISS;
	lstrcpy(logfont.lfFaceName, _T("Arial"));
	VERIFY(m_biggerFont.CreateFontIndirect(&logfont));
    GetDlgItem(ID_VERSION_TITLE)->SetFont(&m_biggerFont);
    GetDlgItem(ID_VERSION_NUMBER)->SetFont(&m_biggerFont);

	m_sAddress.LoadString(IDS_ADDRESS);
	m_sCopyright = PROD_COPYRIGHT;
	
	CString strHelp;

	strHelp.LoadString(IDS_DISCLAIMER1);
	m_sDisclaimer = strHelp;
	strHelp.LoadString(IDS_DISCLAIMER2);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER3);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER4);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER5);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER6);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER7);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER8);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER9);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER10);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER11);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER12);
	m_sDisclaimer += strHelp;
	strHelp.LoadString(IDS_DISCLAIMER13);
	m_sDisclaimer += strHelp;

    //CVersionInfo    info;
    //VERIFY(info.ReadInfoString(_T("ProductName"), m_sVersionTitle));
    //VERIFY(info.ReadInfoString(_T("FileVersion"), m_sVersionNumber));
        m_sVersionTitle = _T(PROD_PRODUCTNAME) _T(" ") _T(PROD_SHORT_VERSION_STR);
        m_sVersionNumber = _T(PROD_FULL_VERSION_STR);


    VERIFY(m_4C_Logo.SubclassDlgItem(IDC_LOGO, this));
    m_4C_Logo.LoadBitmaps(MAKEINTRESOURCE(IDB_4CONTROL));
    m_4C_Logo.SizeToContent();

	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CAboutGeneral.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
