
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
//#include "BaseTargetData.h"
#include "PropPageNetBUIComm.h"
#include "commdefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageNetBUIComm property page

//IMPLEMENT_DYNCREATE(CPropPageNetBUIComm, CBaseCommPropertyPage)

CPropPageNetBUIComm::CPropPageNetBUIComm() : CBaseCommPropertyPage(CPropPageNetBUIComm::IDD)
{
	//{{AFX_DATA_INIT(CPropPageNetBUIComm)
	m_strCNAddress = _T("");
	//}}AFX_DATA_INIT
}

CPropPageNetBUIComm::~CPropPageNetBUIComm()
{
}

void CPropPageNetBUIComm::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageNetBUIComm)
	DDX_Text(pDX, IDC_EDIT_CN_ADDRES, m_strCNAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageNetBUIComm, CBaseCommPropertyPage)
	//{{AFX_MSG_MAP(CPropPageNetBUIComm)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageNetBUIComm message handlers


CString CPropPageNetBUIComm::GetCommChannel()
{
    return COMM_CHANNEL_NETBUI;
}

CString CPropPageNetBUIComm::GetCommChannelDescription()
{
    CString strDescr;
    strDescr.LoadString(IDS_COMM_CHANNEL_DESC_NETBUI);
    return strDescr;
}

void CPropPageNetBUIComm::SetConnectParameter(const CString& crstrParameter)
{
    m_strCNAddress = crstrParameter;
}

CString CPropPageNetBUIComm::GetConnectParameter()
{
    return m_strCNAddress;
}

/* ---------------------------------------------------------------------------- */
