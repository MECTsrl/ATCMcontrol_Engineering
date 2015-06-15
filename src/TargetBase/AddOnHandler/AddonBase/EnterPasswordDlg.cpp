/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AddonBaseResource.h"
#include "EnterPasswordDlg.h"
#include "ChangePasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnterPasswordDlg dialog


CEnterPasswordDlg::CEnterPasswordDlg(const CString& crstrComparePwd, CWnd* pParent /*=NULL*/)
  : CDialog(CEnterPasswordDlg::IDD, pParent),
    m_strComparePwd(crstrComparePwd)
{
	//{{AFX_DATA_INIT(CEnterPasswordDlg)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
    m_bPasswdChanged = FALSE;
}


void CEnterPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnterPasswordDlg)
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnterPasswordDlg, CDialog)
	//{{AFX_MSG_MAP(CEnterPasswordDlg)
	ON_BN_CLICKED(IDC_CHANGE_PASSWD, OnChangePasswd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnterPasswordDlg message handlers

void CEnterPasswordDlg::OnOK() 
{
    UpdateData();

    if(m_strComparePwd.Compare(m_strPassword) != 0)
    {
        ::AfxMessageBox(IDS_WRONG_PASSWORD);
        return;
    }
	CDialog::OnOK();
}

void CEnterPasswordDlg::OnChangePasswd() 
{
    UpdateData();

    if(m_strComparePwd.Compare(m_strPassword) != 0)
    {
        ::AfxMessageBox(IDS_WRONG_PASSWORD);
        return;
    }
    CChangePasswordDlg  tChangeDlg;
    if(tChangeDlg.DoModal() == IDOK)
    {
        m_strPassword = tChangeDlg.m_strPassword1;
        m_strComparePwd = m_strPassword;
        m_bPasswdChanged = TRUE;
        UpdateData(FALSE);
    }
}

/* ---------------------------------------------------------------------------- */
