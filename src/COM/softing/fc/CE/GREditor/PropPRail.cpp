

#include "stdafx.h"
#include "greditor.h"
#include "PropPRail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPRail dialog


CPropPRail::CPropPRail(CWnd* pParent /*=NULL*/)
	: CDialog(CPropPRail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPRail)
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
}


void CPropPRail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPRail)
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPRail, CDialog)
	//{{AFX_MSG_MAP(CPropPRail)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPRail message handlers
