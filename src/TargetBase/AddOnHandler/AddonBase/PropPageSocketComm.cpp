
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "BaseTargetData.h"
#include "PropPageSocketComm.h"
#include "CommDefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageSocketComm property page

//IMPLEMENT_DYNCREATE(CPropPageSocketComm, CBaseCommPropertyPage)

CPropPageSocketComm::CPropPageSocketComm(CWnd* pParent /*= NULL*/) 
  : CBaseCommPropertyPage(CPropPageSocketComm::IDD)
{
	//{{AFX_DATA_INIT(CPropPageSocketComm)
	m_strIPAddress = _T("");
	//}}AFX_DATA_INIT
}

CPropPageSocketComm::~CPropPageSocketComm()
{
}

void CPropPageSocketComm::DoDataExchange(CDataExchange* pDX)
{
	CBaseCommPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSocketComm)
	DDX_Text(pDX, IDC_PROPGEN_IPADDR_EDIT, m_strIPAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageSocketComm, CBaseCommPropertyPage)
	//{{AFX_MSG_MAP(CPropPageSocketComm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageSocketComm message handlers

BOOL CPropPageSocketComm::OnInitDialog() 
{
	CBaseCommPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageSocketComm::OnOK() 
{
	CBaseCommPropertyPage::OnOK();
}


//------------------------------------------------------------------*
/**
 * check IP-Address.
 *  
 *  only '.' and '0' .. '9' allowed
 *
 * @param           -
 * @return          valid IP-Address?
 * @exception       -
 * @see             -
*/
BOOL CPropPageSocketComm::CheckIPAddress()
{
    // Check IP-Address
    int iDotCount = 0;
    int iStrLen = m_strIPAddress.GetLength();
    if(iStrLen == 0)
    {
        return TRUE;
    }

    for(int iChar = 0; iChar < iStrLen; ++iChar)
    {
        switch(m_strIPAddress[iChar])
        {
        case _T('.'):
            ++iDotCount;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        default:
            return FALSE;
            break;
        }
    }
    if(iDotCount == 3)
    {
        return TRUE;
    }
    return FALSE;
}

void CPropPageSocketComm::SetConnectParameter(const CString& crstrParameter)
{
    m_strIPAddress = crstrParameter;
}

CString CPropPageSocketComm::GetConnectParameter()
{
    return m_strIPAddress;
}

CString CPropPageSocketComm::GetCommChannel()
{
    return COMM_CHANNEL_TCP;
}

CString CPropPageSocketComm::GetCommChannelDescription()
{
    CString strDescription;
    strDescription.LoadString(IDS_COMM_CHANNEL_DESC_TCP);
    return strDescription;
}

/* ---------------------------------------------------------------------------- */
