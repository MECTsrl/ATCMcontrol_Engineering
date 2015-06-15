// ExpandArrayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cewatchview.h"
#include "ExpandArrayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpandArrayDlg dialog


CExpandArrayDlg::CExpandArrayDlg(int iOffset /*=0*/, int iMaxCount /*=100*/, CWnd* pParent /*=NULL*/)
	: CDialog(CExpandArrayDlg::IDD, pParent)
{
	m_iMaxCount = iMaxCount;
	//{{AFX_DATA_INIT(CExpandArrayDlg)
	m_iCount = iMaxCount;
	m_iOffset = iOffset;
	//}}AFX_DATA_INIT
}


void CExpandArrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpandArrayDlg)
	DDX_Text(pDX, IDC_COUNT_EDT, m_iCount);
	DDV_MinMaxInt(pDX, m_iCount, 1, 100000000);
	DDX_Text(pDX, IDC_OFFSET_EDT, m_iOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpandArrayDlg, CDialog)
	//{{AFX_MSG_MAP(CExpandArrayDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpandArrayDlg message handlers

void CExpandArrayDlg::OnOK() 
{
	//UpdateData();

	if(m_iOffset < 0)
	{
		CWnd*	pWnd = GetDlgItem(IDC_OFFSET_EDT);
		GotoDlgCtrl(pWnd);

		CString		strMessage;
		strMessage.Format(_T("Invalid value: minimum value is 0"));
		AfxMessageBox(strMessage);
		return;
	}
	if(m_iOffset > m_iMaxCount)
	{
		CWnd*	pWnd = GetDlgItem(IDC_OFFSET_EDT);
		GotoDlgCtrl(pWnd);

		CString		strMessage;
		strMessage.Format(_T("Invalid value: maximum value is %d"), m_iMaxCount-1);
		AfxMessageBox(strMessage);
		return;
	}
	if(m_iCount < 1)
	{
		CWnd*	pWnd = GetDlgItem(IDC_COUNT_EDT);
		GotoDlgCtrl(pWnd);

		CString		strMessage;
		strMessage.Format(_T("Invalid value: minimum value is 1"));
		AfxMessageBox(strMessage);
		return;
	}
	if((m_iCount + m_iOffset) > m_iMaxCount)
	{
		CWnd*	pWnd = GetDlgItem(IDC_COUNT_EDT);
		GotoDlgCtrl(pWnd);

		CString		strMessage;
		strMessage.Format(_T("Invalid value: maximum value is %d"), m_iMaxCount-m_iOffset-1);
		AfxMessageBox(strMessage);
		return;
	}

	CDialog::OnOK();
}
