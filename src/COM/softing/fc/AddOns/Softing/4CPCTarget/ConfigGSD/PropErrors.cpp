// PropErrors.cpp : implementation file
//

#include "stdafx.h"
#include "configgsd.h"
#include "PropErrors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropErrors property page

IMPLEMENT_DYNCREATE(PropErrors, CPropertyPage)

PropErrors::PropErrors() : CPropertyPage(PropErrors::IDD)
{
	//{{AFX_DATA_INIT(PropErrors)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

PropErrors::~PropErrors()
{
}

void PropErrors::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropErrors)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropErrors, CPropertyPage)
	//{{AFX_MSG_MAP(PropErrors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropErrors message handlers

//----------------------------- FUNCTION -------------------------------------*
BOOL PropErrors::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CListBox* pList = (CListBox*)GetDlgItem(IDC_ERROR_MESSAGES);
	CListCtrl* pCtrl = (CListCtrl*)GetDlgItem(IDC_ERROR_MESSAGES);
	if (pCtrl == NULL)
		return FALSE;

    CRect   rect;
	int iWidth;
	if (pCtrl)	{
		pCtrl->GetClientRect(&rect);
		iWidth = rect.Width();
		CString sTitle(_T("Error Messages:"));//.LoadString(IDS_EXTUSERPARAM_DESCRIPTION);
		pCtrl->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth, 0 );	// Create 1st Column
	}

	DWORD  count = m_StringArray.GetSize();
	for (DWORD i = 0; i < count; i++)	{
		CString szError = m_StringArray.GetAt(i);
	
		pCtrl->InsertItem(i, szError);
	}

	CString szCount;
	szCount.Format(_T("%d"), m_dwErrorCount);
	CStatic* pEdit = (CStatic*)GetDlgItem(IDC_ERROR_COUNT);
	if (pEdit != NULL)
		pEdit->SetWindowText(szCount);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//----------------------------- FUNCTION -------------------------------------*
void PropErrors::AddErrorString(CString szError)
{
	m_StringArray.Add((LPCTSTR)szError);
}
