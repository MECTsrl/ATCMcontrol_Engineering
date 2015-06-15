
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// CoverPageDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoverPageDataDlg.h"
#include "PrintLayout.h"
#include "ChangeCaptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoverPageDataDlg dialog


CCoverPageDataDlg::CCoverPageDataDlg(CCoverPage* pCoverPage, CWnd* pParent /*=NULL*/)
	: CDialog(CCoverPageDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoverPageDataDlg)
	m_strBitmap = _T("");
	m_strComment = _T("");
	m_strProjectManager = _T("");
	m_strProjectName = _T("");
	m_strProjectNumber = _T("");
	//}}AFX_DATA_INIT
    ASSERT(pCoverPage);
    m_pCoverPage = pCoverPage;
}


void CCoverPageDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoverPageDataDlg)
	DDX_Text(pDX, IDC_COVPAG_BITMAP_EDIT, m_strBitmap);
	DDX_Text(pDX, IDC_COVPAG_COMMENT_EDIT, m_strComment);
	DDX_Text(pDX, IDC_COVPAG_PRJMAN_EDIT, m_strProjectManager);
	DDX_Text(pDX, IDC_COVPAG_PRJNAME_EDIT, m_strProjectName);
	DDX_Text(pDX, IDC_COVPAG_PRJNO_EDIT, m_strProjectNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoverPageDataDlg, CDialog)
	//{{AFX_MSG_MAP(CCoverPageDataDlg)
	ON_BN_CLICKED(IDC_COVPAG_PRJNOCAP_BTN, OnCovpagPrjnocapBtn)
	ON_BN_CLICKED(IDC_COVPAG_BITMAP_SEL_BTN, OnCovpagBitmapSelBtn)
	ON_BN_CLICKED(IDC_COVPAG_COMMENTCAP_BTN, OnCovpagCommentcapBtn)
	ON_BN_CLICKED(IDC_COVPAG_PRJMANCAP_BTN, OnCovpagPrjmancapBtn)
	ON_BN_CLICKED(IDC_COVPAG_PRJNAMCAP_BTN, OnCovpagPrjnamcapBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoverPageDataDlg message handlers

BOOL CCoverPageDataDlg::OnInitDialog() 
{
    // copy data to member variables
    CopyDataToMembers();

    CDialog::OnInitDialog();
    
    // set button texts
    SetButtonTexts();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCoverPageDataDlg::CopyDataToMembers()
{
    ASSERT(m_pCoverPage);
    if(!m_pCoverPage)
    {
        return;
    }

    m_strBitmap = m_pCoverPage->m_strBitmapFile;
    m_strProjectName = m_pCoverPage->m_strPrjName;
    m_strProjectManager = m_pCoverPage->m_strPrjManager;
    m_strProjectNumber = m_pCoverPage->m_strPrjNo;
    m_strComment = m_pCoverPage->m_strComment;
    m_strProjectNameCaption = m_pCoverPage->m_strPrjNameCaption;
    m_strProjectManagerCaption = m_pCoverPage->m_strPrjManagerCaption;
    m_strProjectNumberCaption = m_pCoverPage->m_strPrjNoCaption;
    m_strCommentCaption = m_pCoverPage->m_strCommentCaption;

    m_strComment.Replace(_T("\n"), _T("\r\n"));
}

void CCoverPageDataDlg::CopyDataFromMembers()
{
    ASSERT(m_pCoverPage);
    if(!m_pCoverPage)
    {
        return;
    }

    m_strComment.Replace(_T("\r\n"), _T("\n"));

    m_pCoverPage->m_strBitmapFile = m_strBitmap;
    m_pCoverPage->m_strPrjName = m_strProjectName;
    m_pCoverPage->m_strPrjManager = m_strProjectManager;
    m_pCoverPage->m_strPrjNo = m_strProjectNumber;
    m_pCoverPage->m_strComment = m_strComment;
    m_pCoverPage->m_strPrjNameCaption = m_strProjectNameCaption;
    m_pCoverPage->m_strPrjManagerCaption = m_strProjectManagerCaption;
    m_pCoverPage->m_strPrjNoCaption = m_strProjectNumberCaption;
    m_pCoverPage->m_strCommentCaption = m_strCommentCaption;
}

void CCoverPageDataDlg::SetButtonTexts()
{
    CWnd*   pWnd;
    
    pWnd = GetDlgItem(IDC_COVPAG_PRJNAMCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strProjectNameCaption);
    }

    pWnd = GetDlgItem(IDC_COVPAG_PRJNOCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strProjectNumberCaption);
    }

    pWnd = GetDlgItem(IDC_COVPAG_PRJMANCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strProjectManagerCaption);
    }

    pWnd = GetDlgItem(IDC_COVPAG_COMMENTCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strCommentCaption);
    }
}


void CCoverPageDataDlg::OnOK() 
{
    UpdateData();
    CopyDataFromMembers();
	CDialog::OnOK();
}

void CCoverPageDataDlg::OnCovpagBitmapSelBtn() 
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
        CWnd*   pWnd = GetDlgItem(IDC_COVPAG_BITMAP_EDIT);
        m_strBitmap = dlgFile.m_ofn.lpstrFile;
        pWnd->SetWindowText(m_strBitmap);
    }
}

void CCoverPageDataDlg::OnCovpagPrjnamcapBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strProjectNameCaption;
    if(tDlg.DoModal() == IDOK)
    {
        m_strProjectNameCaption = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_COVPAG_PRJNAMCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strProjectNameCaption);
    }
}

void CCoverPageDataDlg::OnCovpagCommentcapBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strCommentCaption;
    if(tDlg.DoModal() == IDOK)
    {
        m_strCommentCaption = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_COVPAG_COMMENTCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strCommentCaption);
    }
}

void CCoverPageDataDlg::OnCovpagPrjnocapBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strProjectNumberCaption;
    if(tDlg.DoModal() == IDOK)
    {
        m_strProjectNumberCaption = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_COVPAG_PRJNOCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strProjectNumberCaption);
    }
}

void CCoverPageDataDlg::OnCovpagPrjmancapBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strProjectManagerCaption;
    if(tDlg.DoModal() == IDOK)
    {
        m_strProjectManagerCaption = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_COVPAG_PRJMANCAP_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strProjectManagerCaption);
    }
}

