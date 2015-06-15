

#include "stdafx.h"

#include "GrEditor.h"
#include "GridDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog

CGridDlg::CGridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridDlg)
	m_iGrid = -1;
	//}}AFX_DATA_INIT
}

void CGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridDlg)
	DDX_Radio(pDX, IDC_GR_GRID_NO, m_iGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGridDlg, CDialog)
	//{{AFX_MSG_MAP(CGridDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDlg message handlers
