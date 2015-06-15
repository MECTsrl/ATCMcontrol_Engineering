
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// PrintHeaderDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PrintHeaderDataDlg.h"
#include "PrintLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintHeaderDataDlg dialog


CPrintHeaderDataDlg::CPrintHeaderDataDlg(CPrintHeader* pPrintHeader, CWnd* pParent /*=NULL*/)
	: CDialog(CPrintHeaderDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintHeaderDataDlg)
	m_strBitmapLeft = _T("");
	m_strBitmapRight = _T("");
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
    ASSERT(pPrintHeader);
    m_pPrintHeader = pPrintHeader;
}


void CPrintHeaderDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintHeaderDataDlg)
	DDX_Text(pDX, IDC_HEADER_LBITMAP_EDIT, m_strBitmapLeft);
	DDX_Text(pDX, IDC_HEADER_RBITMAP_EDIT, m_strBitmapRight);
	DDX_Text(pDX, IDC_HEADER_TITLE_EDIT, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintHeaderDataDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintHeaderDataDlg)
	ON_BN_CLICKED(IDC_HEADER_LBITMAP_SEL_BTN, OnHeaderLbitmapSelBtn)
	ON_BN_CLICKED(IDC_HEADER_RBITMAP_SEL_BTN, OnHeaderRbitmapSelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintHeaderDataDlg message handlers

BOOL CPrintHeaderDataDlg::OnInitDialog() 
{
    CopyDataToMembers();

	CDialog::OnInitDialog();

    //SetButtonTexts();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintHeaderDataDlg::OnOK() 
{
    UpdateData();
    CopyDataFromMembers();
	CDialog::OnOK();
}

void CPrintHeaderDataDlg::CopyDataToMembers()
{
    ASSERT(m_pPrintHeader);
    if(!m_pPrintHeader)
    {
        return;
    }
    m_strBitmapLeft = m_pPrintHeader->m_strLeftBitmapFile;
    m_strBitmapRight = m_pPrintHeader->m_strRightBitmapFile;
    m_strTitle = m_pPrintHeader->m_strTitle;
}


void CPrintHeaderDataDlg::CopyDataFromMembers()
{
    ASSERT(m_pPrintHeader);
    if(!m_pPrintHeader)
    {
        return;
    }
    m_pPrintHeader->m_strLeftBitmapFile = m_strBitmapLeft;
    m_pPrintHeader->m_strRightBitmapFile = m_strBitmapRight;
    m_pPrintHeader->m_strTitle = m_strTitle;
}


void CPrintHeaderDataDlg::OnHeaderLbitmapSelBtn() 
{
	CFileDialog dlgFile(TRUE);
    CString strFileName;
	CString strFilter;
	CString strTitle;

	VERIFY(strTitle.LoadString(AFX_IDS_OPENFILE));

	// append the "*.*" all files filter
	CString strFilterTmp;
	VERIFY(strFilterTmp.LoadString(IDS_BMPFILTER));
	strFilter += strFilterTmp;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bmp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle = strTitle;

    if(dlgFile.DoModal() == IDOK)
    {
        CWnd*   pWnd = GetDlgItem(IDC_HEADER_LBITMAP_EDIT);
        m_strBitmapLeft = dlgFile.m_ofn.lpstrFile;
        pWnd->SetWindowText(m_strBitmapLeft);
    }
}

void CPrintHeaderDataDlg::OnHeaderRbitmapSelBtn() 
{
	CFileDialog dlgFile(TRUE);
    CString strFileName;
	CString strFilter;
	CString strTitle;

	VERIFY(strTitle.LoadString(AFX_IDS_OPENFILE));

	// append the "*.*" all files filter
	CString strFilterTmp;
	VERIFY(strFilterTmp.LoadString(IDS_BMPFILTER));
	strFilter += strFilterTmp;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.bmp");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle = strTitle;

    if(dlgFile.DoModal() == IDOK)
    {
        CWnd*   pWnd = GetDlgItem(IDC_HEADER_RBITMAP_EDIT);
        m_strBitmapRight = dlgFile.m_ofn.lpstrFile;
        pWnd->SetWindowText(m_strBitmapRight);
    }
}
