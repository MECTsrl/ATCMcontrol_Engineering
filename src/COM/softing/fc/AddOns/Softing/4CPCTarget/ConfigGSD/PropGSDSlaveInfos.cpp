// PropGSDSlaveInfos.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigGSD.h"
#include "PropGSDSlaveInfos.h"

#include "resource.h"
#include "defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined _UNICODE | defined UNICODE
	static const char pszEmpty[] = "- - -";
#else
	static const char pszEmpty[] = _T("- - -");
#endif

/////////////////////////////////////////////////////////////////////////////
// PropGSDSlaveInfos property page

IMPLEMENT_DYNCREATE(PropGSDSlaveInfos, CPropertyPage)

PropGSDSlaveInfos::PropGSDSlaveInfos() : CPropertyPage(PropGSDSlaveInfos::IDD)
{
	//{{AFX_DATA_INIT(PropGSDSlaveInfos)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_szFiledate = pszEmpty;
	m_szFilename = pszEmpty;
	m_szGSDRevision = pszEmpty;
	m_szVendorName = pszEmpty;
	m_szModelName = pszEmpty;
	m_szRevision = pszEmpty;
	m_szIdentNmb = pszEmpty;
	m_szFMSSupport = pszEmpty;
	m_szHWRelease = pszEmpty;
	m_szSWRelease = pszEmpty;
	m_bSetSlaveAddrSupp = FALSE;
	m_bAutoBaud = FALSE;
	m_bFreezeRequest = FALSE;
	m_bSyncRequest = FALSE;
	m_bFailSafe = FALSE;
	m_bRedundancy = FALSE;

	m_wStationType = 0; // Init as Slave
	m_wSuppBaudRates = 0;
	m_wRepeater = 0;	// Init as NC (Not Connected)
	m_wPin_24V = 0;	// Init as NC (Not Connected)
	m_wIdent = 0;
}

PropGSDSlaveInfos::~PropGSDSlaveInfos()
{
}

void PropGSDSlaveInfos::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropGSDSlaveInfos)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropGSDSlaveInfos, CPropertyPage)
	//{{AFX_MSG_MAP(PropGSDSlaveInfos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropGSDSlaveInfos message handlers

//-------------------------------- Funktion -----------------------------------
BOOL PropGSDSlaveInfos::OnInitDialog()
/**
 * OnInitDialog()
 * Initialization of the Dialog: Fill Controlls and Editboxes
 *
 * Returnvalue: TRUE
*/
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_FILE_DATE)->SetWindowText(m_szFiledate);
	GetDlgItem(IDC_FILE_NAME)->SetWindowText(m_szFilename);
	GetDlgItem(IDC_GSD_REVISION)->SetWindowText(m_szGSDRevision);
	GetDlgItem(IDC_VENDOR_NAME)->SetWindowText(m_szVendorName);
	GetDlgItem(IDC_MODEL_NAME)->SetWindowText(m_szModelName);
	GetDlgItem(IDC_REVISION_STRING)->SetWindowText(m_szRevision);
	GetDlgItem(IDC_IDENT_NUMBER)->SetWindowText(m_szIdentNmb);
	GetDlgItem(IDC_FMS_SUPPORT)->SetWindowText(m_szFMSSupport);
	GetDlgItem(IDC_HARDWARE_RELEASE)->SetWindowText(m_szHWRelease);
	GetDlgItem(IDC_SOFTWARE_RELEASE)->SetWindowText(m_szSWRelease);

	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST_SUPP_BAUDRATES);
	SetUsableBaudrates( pListBox );

	((CButton*)GetDlgItem(IDC_SYNC_REQUEST))->SetCheck(m_bSyncRequest);
	((CButton*)GetDlgItem(IDC_FREEZE_REQUEST))->SetCheck(m_bFreezeRequest);
	((CButton*)GetDlgItem(IDC_SUPPORT_AUTOBAUD))->SetCheck(m_bAutoBaud);
	((CButton*)GetDlgItem(IDC_FAIL_SAVE))->SetCheck(m_bFailSafe);
	((CButton*)GetDlgItem(IDC_SET_SLAVE_ADDR_SUP))->SetCheck(m_bSetSlaveAddrSupp);
	((CButton*)GetDlgItem(IDC_REDUNDANCY))->SetCheck(m_bRedundancy);

	switch (m_wStationType)	{
		case 0:
			((CButton*)GetDlgItem(IDC_RADIO_SLAVE))->SetCheck(1);
			break;
		case 1:
			((CButton*)GetDlgItem(IDC_RADIO_MASTER))->SetCheck(1);
			break;
		default:
			break;
	}
	switch (m_wRepeater)	{
		case 1:
			GetDlgItem(IDC_REPEATER_CTRL_SIG)->SetWindowText(_T("RS485"));
			break;
		case 2:
			GetDlgItem(IDC_REPEATER_CTRL_SIG)->SetWindowText(_T("TTL"));
			break;
		case 0:
		default:
			GetDlgItem(IDC_REPEATER_CTRL_SIG)->SetWindowText(_T("NC"));
			break;
	}
	switch (m_wPin_24V)	{
		case 1:
			GetDlgItem(IDC_PINS_24V)->SetWindowText(_T("Input/Senke"));
			break;
		case 2:
			GetDlgItem(IDC_PINS_24V)->SetWindowText(_T("Output/Quelle"));
			break;
		case 0:
		default:
			GetDlgItem(IDC_PINS_24V)->SetWindowText(_T("NC"));
			break;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------- Funktion -----------------------------------
BOOL PropGSDSlaveInfos::SetUsableBaudrates(CListBox* pLBox)
{
	if (m_wSuppBaudRates & _9_6kBaud)
		pLBox->AddString(_T("9.6 kBaud"));
	if (m_wSuppBaudRates & _19_2kBaud)
		pLBox->AddString(_T("19.2 kBaud"));
	if (m_wSuppBaudRates & _31_25kBaud)
		pLBox->AddString(_T("31.25 kBaud"));
	if (m_wSuppBaudRates & _45_45kBaud)
		pLBox->AddString(_T("45.45 kBaud"));
	if (m_wSuppBaudRates & _93_75kBaud)
		pLBox->AddString(_T("93.75 kBaud"));
	if (m_wSuppBaudRates & _187_5kBaud)
		pLBox->AddString(_T("187.5 kBaud"));
	if (m_wSuppBaudRates & _500kBaud)
		pLBox->AddString(_T("500 kBaud"));
	if (m_wSuppBaudRates & _1_5MBaud)
		pLBox->AddString(_T("1.5 MBaud"));
	if (m_wSuppBaudRates & _3MBaud)
		pLBox->AddString(_T("3 MBaud"));
	if (m_wSuppBaudRates & _6MBaud)
		pLBox->AddString(_T("6 MBaud"));
	if (m_wSuppBaudRates & _12MBaud)
		pLBox->AddString(_T("12 MBaud"));

	return TRUE;
}

BOOL PropGSDSlaveInfos::SetFileName(CString& szFilename)
{
	m_szFilename = szFilename;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetFileDate(DATE DateGSD)
{
    COleDateTime    date(DateGSD);
    m_szFiledate = date.Format();
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetSupportedBaudRates(WORD wBRates)
{
	m_wSuppBaudRates = wBRates;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetVendorName(CString szName)
{
	m_szVendorName = szName;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetModelName(CString szModelName)
{
	m_szModelName = szModelName;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetIdentNmb(LONG lIdent)
{
	m_wIdent = (WORD)lIdent;
	m_szIdentNmb.Format(_T("0x%04X"), lIdent);
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetStationType(WORD wStationType)
{
	m_wStationType = wStationType;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetHWRelease(CString szHWRelease)
{
	m_szHWRelease = szHWRelease;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetSWRelease(CString szSWRelease)
{
	m_szSWRelease = szSWRelease;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetGSDRevision(LONG lGSDRev)
{
	if (lGSDRev != -1)	{
		m_szGSDRevision.Format(_T("%d"), lGSDRev);
		return TRUE;
	}
	return FALSE;
}

BOOL PropGSDSlaveInfos::SetRevisionString(CString szRevision)
{
	m_szRevision = szRevision;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetFMSSupported(BOOL bFMSSupport)
{
	if (bFMSSupport != FALSE)
		m_szFMSSupport = _T("Yes");
	else
		m_szFMSSupport = _T("No");
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetSyncRequest(BOOL bSync)
{
	m_bSyncRequest = bSync;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetFreezeRequest(BOOL bFreeze)
{
	m_bFreezeRequest = bFreeze;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetAutoBaudSupp(BOOL bAuto)
{
	m_bAutoBaud = bAuto;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetFailSafe(BOOL bFailSafe)
{
	m_bFailSafe = bFailSafe;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetRedundancy(BOOL bVal)
{
	m_bRedundancy = bVal;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetRepeaterSignal(LONG lVal)
{
	m_wRepeater = (WORD)lVal;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetPin24V(LONG lVal)
{
	m_wPin_24V = (WORD)lVal;
	return TRUE;
}

BOOL PropGSDSlaveInfos::SetSlaveAddrSetSupported(BOOL bSetAddr)
{
	m_bSetSlaveAddrSupp = bSetAddr;
	return TRUE;
}

LONG PropGSDSlaveInfos::GetIdentNumber()
{
	return m_wIdent;
}
