// SelectTargetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "SelectTargetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectTargetDlg dialog


CSelectTargetDlg::CSelectTargetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectTargetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectTargetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pSelTarget = NULL;
}


void CSelectTargetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectTargetDlg)
	DDX_Control(pDX, IDC_TARGET_LIST, m_tTargetListCtrl);
	//}}AFX_DATA_MAP
}

void CSelectTargetDlg::Initialize(TTIList* pTargetInfoList)
{
    m_pTargetInfoList = pTargetInfoList;
}

BEGIN_MESSAGE_MAP(CSelectTargetDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectTargetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectTargetDlg message handlers

void CSelectTargetDlg::OnOK() 
{
    int iCurSel = m_tTargetListCtrl.GetNextItem(0, LVNI_SELECTED);
    if(iCurSel < 0 && m_tTargetListCtrl.GetItemCount() > 1)
    {
        AfxMessageBox(_T("No target selected"));
        CWnd*   pWnd = GetDlgItem(IDC_TARGET_LIST);
        if(pWnd)
        {
            GotoDlgCtrl(pWnd);
        }
        return;
    }
    m_pSelTarget = (TTargetInfo*) m_tTargetListCtrl.GetItemData(iCurSel);
	
	CDialog::OnOK();
}

TTargetInfo* CSelectTargetDlg::GetSelectedTarget()
{
    return m_pSelTarget;
}

BOOL CSelectTargetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CRect   tRect;
    m_tTargetListCtrl.GetClientRect(&tRect);
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = "Name";
	lvc.cx = tRect.Width()/2;
    m_tTargetListCtrl.InsertColumn(COL_TARGET_NAME, &lvc);

	lvc.pszText = "Address";
    m_tTargetListCtrl.InsertColumn(COL_ADDRESS_OLD, &lvc);

    FillTargetListControl(m_pTargetInfoList);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectTargetDlg::FillTargetListControl(TTIList* pTargetInfoList)
{
    m_tTargetListCtrl.DeleteAllItems();

    POSITION    pos = pTargetInfoList->GetHeadPosition();
    TTargetInfo*    pTargetInfo;
    int             iItem = 0;
    while(pos)
    {
        pTargetInfo = pTargetInfoList->GetNext(pos);
        m_tTargetListCtrl.InsertItem(iItem, pTargetInfo->strTargetName);
        m_tTargetListCtrl.SetItemText(iItem, COL_ADDRESS_OLD, pTargetInfo->strTargetAddress);
        m_tTargetListCtrl.SetItemData(iItem, (DWORD)pTargetInfo);
        ++iItem;
    }
}
