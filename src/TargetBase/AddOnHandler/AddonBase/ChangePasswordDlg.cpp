
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AddonBaseResource.h"
#include "ChangePasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg dialog


CChangePasswordDlg::CChangePasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangePasswordDlg)
	m_strPassword1 = _T("");
	m_strPassword2 = _T("");
	//}}AFX_DATA_INIT
}


void CChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangePasswordDlg)
	DDX_Text(pDX, IDC_PASSWORD_CH1_EDIT, m_strPassword1);
	DDX_Text(pDX, IDC_PASSWORD_CH2_EDIT, m_strPassword2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangePasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CChangePasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg message handlers

void CChangePasswordDlg::OnOK() 
{
    UpdateData();

    if(m_strPassword1.Compare(m_strPassword2) != 0)
    {
        ::AfxMessageBox(IDS_PASSWORD_MISSTYPED);
        m_strPassword1.Empty();
        m_strPassword2.Empty();
        UpdateData(FALSE);
        return;
    }
    CDialog::OnOK();
}

/* ---------------------------------------------------------------------------- */
