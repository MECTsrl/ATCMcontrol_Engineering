

#include "stdafx.h"
#include "fchtmled.h"
#include "FcDlgInsertHtml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcDlgInsertHtml dialog


CFcDlgInsertHtml::CFcDlgInsertHtml(CWnd* pParent /*=NULL*/)
	: CDialog(CFcDlgInsertHtml::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFcDlgInsertHtml)
	m_HtmlText = _T("");
	//}}AFX_DATA_INIT
}


void CFcDlgInsertHtml::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFcDlgInsertHtml)
	DDX_Text(pDX, IDC_INSERTHTML_EDIT, m_HtmlText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFcDlgInsertHtml, CDialog)
	//{{AFX_MSG_MAP(CFcDlgInsertHtml)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcDlgInsertHtml message handlers
