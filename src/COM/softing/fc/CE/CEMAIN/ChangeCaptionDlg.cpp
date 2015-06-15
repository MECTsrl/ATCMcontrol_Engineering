
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// ChangeCaptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChangeCaptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeCaptionDlg dialog


CChangeCaptionDlg::CChangeCaptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeCaptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeCaptionDlg)
	m_strCaption = _T("");
	//}}AFX_DATA_INIT
}


void CChangeCaptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeCaptionDlg)
	DDX_Text(pDX, IDC_CAPTION_EDIT, m_strCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeCaptionDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeCaptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeCaptionDlg message handlers

BOOL CChangeCaptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CWnd*   pWnd = GetDlgItem(IDC_CAPTION_EDIT);
    if(pWnd)
    {
        GotoDlgCtrl(pWnd);
        return FALSE;
    }
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

