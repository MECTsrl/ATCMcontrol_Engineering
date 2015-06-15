

#include "stdafx.h"
#include "greditor.h"
#include "PropLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropLineDlg dialog


CPropLineDlg::CPropLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropLineDlg)
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
}


void CPropLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropLineDlg)
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropLineDlg, CDialog)
	//{{AFX_MSG_MAP(CPropLineDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropLineDlg message handlers
