

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// WatchRenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "watchrc.h"
#include "WatchRenameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWatchRenameDlg dialog


CWatchRenameDlg::CWatchRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWatchRenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWatchRenameDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CWatchRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWatchRenameDlg)
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWatchRenameDlg, CDialog)
	//{{AFX_MSG_MAP(CWatchRenameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWatchRenameDlg message handlers

BOOL CWatchRenameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CWnd*   pWnd = GetDlgItem(IDC_NAME_EDIT);
    if(pWnd)
    {
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWatchRenameDlg::OnOK() 
{
    UpdateData();

    if(m_strName.IsEmpty())
    {
        ::AfxMessageBox(IDS_ERR_TABNAME_INVALID);
        CWnd*   pWndCtrl = GetDlgItem(IDC_NAME_EDIT);
        if(pWndCtrl)
        {
            GotoDlgCtrl(pWndCtrl);
        }
        return;
    }
	
	CDialog::OnOK();
}


