

#include "stdafx.h"
#include "fchtmled.h"
#include "FcDlgFontBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcDlgFontBar dialog


CFcDlgFontBar::CFcDlgFontBar(CWnd* pParent /*=NULL*/)
	: CDialog(CFcDlgFontBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFcDlgFontBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFcDlgFontBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFcDlgFontBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFcDlgFontBar, CDialog)
	//{{AFX_MSG_MAP(CFcDlgFontBar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcDlgFontBar message handlers
