

#include "stdafx.h"
#include "GrEditor.h"
#include "InsDivDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsDivDlg dialog


CInsDivDlg::CInsDivDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsDivDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsDivDlg)
	m_iSelection = -1;
	//}}AFX_DATA_INIT
}


void CInsDivDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsDivDlg)
	DDX_Radio(pDX, IDC_GR_DIV_ALT, m_iSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsDivDlg, CDialog)
	//{{AFX_MSG_MAP(CInsDivDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsDivDlg message handlers

BOOL CInsDivDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
