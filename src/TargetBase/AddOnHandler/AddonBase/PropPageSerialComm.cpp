
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "PropPageSerialComm.h"
#include "BaseTargetData.h"
#include "CommDefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageSerialComm property page

//IMPLEMENT_DYNCREATE(CPropPageSerialComm, CBaseCommPropertyPage)

CPropPageSerialComm::CPropPageSerialComm(CWnd* pParent /*= NULL*/) 
  : CBaseCommPropertyPage(CPropPageSerialComm::IDD)
{
	//{{AFX_DATA_INIT(CPropPageSerialComm)
	//}}AFX_DATA_INIT
}

CPropPageSerialComm::~CPropPageSerialComm()
{
}

void CPropPageSerialComm::DoDataExchange(CDataExchange* pDX)
{
	CBaseCommPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSerialComm)
	DDX_Control(pDX, IDC_PROPGEN_STOPBITS_CB, m_tStopBitsCombo);
	DDX_Control(pDX, IDC_PROPGEN_PARITY_CB, m_tParityCombo);
	DDX_Control(pDX, IDC_PROPGEN_DATABITS_CB, m_tDataBitsCombo);
	DDX_Control(pDX, IDC_PROPGEN_SERIALPORT_CB, m_tSerialPortCombo);
	DDX_Control(pDX, IDC_PROPGEN_BAUDRATE_CB, m_tBaudRateCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageSerialComm, CBaseCommPropertyPage)
	//{{AFX_MSG_MAP(CPropPageSerialComm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageSerialComm message handlers

BOOL CPropPageSerialComm::OnInitDialog() 
{
	CBaseCommPropertyPage::OnInitDialog();

    ReadTargetData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageSerialComm::OnOK() 
{
    UpdatePageData();
    
	CBaseCommPropertyPage::OnOK();
}

//------------------------------------------------------------------*
/**
 * read target data.
 *
 *  parse serial string: <COM-Port>:<Baud-Rate>,<Parity>,<Data-Bits>,<Stop-Bits>"
 *  e.g. "COM1:19200,n,8,0"
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             WriteTargetData()
*/
void CPropPageSerialComm::ReadTargetData()
{
    CString strPort, strBaudRate, strParity, strDataBits, strStopBits;

    int     iSel;
    int     iStart = 0;
    int     iEnd = m_strSerialPar.Find(_T(':'));

    if(iEnd > 0)
    {
        strPort = m_strSerialPar.Left(iEnd);
        iStart = iEnd + 1;
        iEnd = m_strSerialPar.Find(_T(','), iStart);
        if(iEnd > 0)
        {
            strBaudRate = m_strSerialPar.Mid(iStart, iEnd - iStart);
            iStart = iEnd + 1;
            iEnd = m_strSerialPar.Find(_T(','), iStart);
            if(iEnd > 0)
            {
                strParity = m_strSerialPar.Mid(iStart, iEnd - iStart);
                iStart = iEnd + 1;
                iEnd = m_strSerialPar.Find(_T(','), iStart);
                if(iEnd > 0)
                {
                    strDataBits = m_strSerialPar.Mid(iStart, iEnd - iStart);
                    iStart = iEnd + 1;
                    iEnd = m_strSerialPar.Find(_T(','), iStart);
                    if(iEnd > 0)
                    {
                        strStopBits = m_strSerialPar.Mid(iStart, iEnd - iStart);
                    }
                    else
                    {
                        strStopBits = m_strSerialPar.Mid(iStart);
                    }
                }
            }
        }
    }

    strParity.MakeLower();
    if(strParity.IsEmpty())
    {
        strParity = _T("n");
    }

    TCHAR   cParity = strParity[0];
    switch(cParity)
    {
    case _T('n'):
        strParity = _T("None");
        break;
    case _T('o'):
        strParity = _T("Odd");
        break;
    case _T('e'):
        strParity = _T("Even");
        break;
    case _T('m'):
        strParity = _T("Mark");
        break;
    case _T('s'):
        strParity = _T("Space");
        break;
    }

    iSel = m_tSerialPortCombo.FindStringExact(0, strPort);
    if(iSel < 0)
    {
        iSel = 0;
    }
    m_tSerialPortCombo.SetCurSel(iSel);

    iSel = m_tBaudRateCombo.FindStringExact(0, strBaudRate);
    if(iSel < 0)
    {
        iSel = m_tBaudRateCombo.FindStringExact(0, _T("19200"));
        ASSERT(iSel >= 0);
    }
    m_tBaudRateCombo.SetCurSel(iSel);

    iSel = m_tParityCombo.FindStringExact(0, strParity);
    if(iSel < 0)
    {
        iSel = 0;
    }
    m_tParityCombo.SetCurSel(iSel);

    iSel = m_tDataBitsCombo.FindStringExact(0, strDataBits);
    if(iSel < 0)
    {
        iSel = m_tDataBitsCombo.FindStringExact(0, _T("8"));
        ASSERT(iSel >= 0);
    }
    m_tDataBitsCombo.SetCurSel(iSel);

    iSel = m_tStopBitsCombo.FindStringExact(0, strStopBits);
    if(iSel < 0)
    {
        iSel = 0;
    }
    m_tStopBitsCombo.SetCurSel(iSel);
}


//------------------------------------------------------------------*
/**
 * write target data.
 *
 *  generate serial string: <COM-Port>:<Baud-Rate>,<Parity>,<Data-Bits>,<Stop-Bits>"
 *  e.g. "COM1:19200,n,8,0"
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             ReadTargetData()
*/
void CPropPageSerialComm::WriteTargetData()
{
    CString strPort, strBaudRate, strParity, strDataBits, strStopBits;

    m_tSerialPortCombo.GetWindowText(strPort);
    m_tBaudRateCombo.GetWindowText(strBaudRate);
    m_tParityCombo.GetWindowText(strParity);
    m_tDataBitsCombo.GetWindowText(strDataBits);
    m_tStopBitsCombo.GetWindowText(strStopBits);

    //n=None, o=Odd, e=Even, m=Mark, s=Space
    strParity = strParity.Left(1);
    strParity.MakeLower();

    m_strSerialPar.Format("%s:%s,%s,%s,%s", strPort, strBaudRate, strParity, strDataBits, strStopBits);
}

void CPropPageSerialComm::SetConnectParameter(const CString& crstrParameter)
{
    m_strSerialPar = crstrParameter;
}

CString CPropPageSerialComm::GetConnectParameter()
{
    return m_strSerialPar;
}

CString CPropPageSerialComm::GetCommChannel()
{
    return COMM_CHANNEL_RS232;
}

CString CPropPageSerialComm::GetCommChannelDescription()
{
    CString strDescription;
    strDescription.LoadString(IDS_COMM_CHANNEL_DESC_RS232);
    return strDescription;
}

//------------------------------------------------------------------*
/**
 * update page data.
 *
 *  call WriteTargetData() to update m_strSerialPar member.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             CBasePropertyPage::UpdatePageData()
*/
void CPropPageSerialComm::UpdatePageData()
{
    if(GetSafeHwnd())
    {
        UpdateData();
        WriteTargetData();
    }
}

/* ---------------------------------------------------------------------------- */
