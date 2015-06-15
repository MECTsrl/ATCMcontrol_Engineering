


#include "stdafx.h"
#include "GrEditor.h"
#include "InsFbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsFbDlg dialog

CInsFbDlg::CInsFbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsFbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsFbDlg)
	m_strName = _T("");
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
   m_bEnableInst = false;
}

void CInsFbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsFbDlg)
	DDX_Text(pDX, IDC_GR_FB_NAME, m_strName);
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsFbDlg, CDialog)
	//{{AFX_MSG_MAP(CInsFbDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsFbDlg message handlers

BOOL CInsFbDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_GR_FB_NAME)->EnableWindow( m_bEnableInst );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CInsFbDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
