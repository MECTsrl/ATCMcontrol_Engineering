

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "greditor.h"
#include "GrPrintOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrPrintOptionsDlg dialog


CGrPrintOptionsDlg::CGrPrintOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGrPrintOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrPrintOptionsDlg)
	m_bHeader = FALSE;
	//}}AFX_DATA_INIT
    m_pFocusWnd = NULL;
}


void CGrPrintOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrPrintOptionsDlg)
	DDX_Check(pDX, IDC_HEADER_CHECK, m_bHeader);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGrPrintOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CGrPrintOptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrPrintOptionsDlg message handlers

BOOL CGrPrintOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//    SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
//    m_pFocusWnd = SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGrPrintOptionsDlg::OnOK() 
{
	CDialog::OnOK();
//    if(m_pFocusWnd)
//    {
//        m_pFocusWnd->SetFocus();
//    }
}


void CGrPrintOptionsDlg::OnCancel() 
{
	CDialog::OnCancel();
//    if(m_pFocusWnd)
//    {
//        m_pFocusWnd->SetFocus();
//    }
}
