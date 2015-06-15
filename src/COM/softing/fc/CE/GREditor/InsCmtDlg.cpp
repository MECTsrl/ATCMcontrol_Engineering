

#include "stdafx.h"
#include "GrEditor.h"
#include "InsCmtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsCmtDlg dialog


CInsCmtDlg::CInsCmtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsCmtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsCmtDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CInsCmtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsCmtDlg)
	DDX_Text(pDX, IDC_GR_CMT_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsCmtDlg, CDialog)
	//{{AFX_MSG_MAP(CInsCmtDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsCmtDlg message handlers
