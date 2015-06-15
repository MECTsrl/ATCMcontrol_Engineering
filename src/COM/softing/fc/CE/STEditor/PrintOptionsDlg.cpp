// PrintOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "steditor.h"
#include "PrintOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg dialog


CPrintOptionsDlg::CPrintOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrintOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintOptionsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintOptionsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg message handlers
