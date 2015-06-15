
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// PrintFooterDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PrintFooterDataDlg.h"
#include "PrintLayout.h"
#include "ChangeCaptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintFooterDataDlg dialog


CPrintFooterDataDlg::CPrintFooterDataDlg(CPrintFooter* pPrintFooter, CWnd* pParent /*=NULL*/)
	: CDialog(CPrintFooterDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintFooterDataDlg)
	m_strRevNo1 = _T("");
	m_strRevNo2 = _T("");
	m_strRevNo3 = _T("");
	m_strChangeDesc1 = _T("");
	m_strChangeDesc2 = _T("");
	m_strChangeDesc3 = _T("");
	m_strChangeName1 = _T("");
	m_strChangeName2 = _T("");
	m_strChangeName3 = _T("");
	m_strChangeDate1 = _T("");
	m_strChangeDate2 = _T("");
	m_strChangeDate3 = _T("");
	m_strDate = _T("");
	m_strDrawn = _T("");
	m_strApproved = _T("");
	m_strReleased = _T("");
	m_strDateTime = _T("");
	m_strProjPath = _T("");
	m_strObjName = _T("");
	m_strPageCount = _T("");
	m_strInfo1 = _T("");
	m_strInfo2 = _T("");
	m_strInfo3 = _T("");
	m_strInfo4 = _T("");
	m_strBitmap = _T("");
	//}}AFX_DATA_INIT
    ASSERT(pPrintFooter);
    m_pPrintFooter = pPrintFooter;
}


void CPrintFooterDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintFooterDataDlg)
	DDX_Text(pDX, IDC_FOOTER_REVNO1_EDIT, m_strRevNo1);
	DDX_Text(pDX, IDC_FOOTER_REVNO2_EDIT, m_strRevNo2);
	DDX_Text(pDX, IDC_FOOTER_REVNO3_EDIT, m_strRevNo3);
	DDX_Text(pDX, IDC_FOOTER_CHDESCR1_EDIT, m_strChangeDesc1);
	DDX_Text(pDX, IDC_FOOTER_CHDESCR2_EDIT, m_strChangeDesc2);
	DDX_Text(pDX, IDC_FOOTER_CHDESCR3_EDIT, m_strChangeDesc3);
	DDX_Text(pDX, IDC_FOOTER_CHNAME1_EDIT, m_strChangeName1);
	DDX_Text(pDX, IDC_FOOTER_CHNAME2_EDIT, m_strChangeName2);
	DDX_Text(pDX, IDC_FOOTER_CHNAME3_EDIT, m_strChangeName3);
	DDX_Text(pDX, IDC_FOOTER_CHDATE1_EDIT, m_strChangeDate1);
	DDX_Text(pDX, IDC_FOOTER_CHDATE2_EDIT, m_strChangeDate2);
	DDX_Text(pDX, IDC_FOOTER_CHDATE3_EDIT, m_strChangeDate3);
	DDX_Text(pDX, IDC_FOOTER_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FOOTER_DRAWN_EDIT, m_strDrawn);
	DDX_Text(pDX, IDC_FOOTER_APPROVED_EDIT, m_strApproved);
	DDX_Text(pDX, IDC_FOOTER_RELEASED_EDIT, m_strReleased);
	DDX_Text(pDX, IDC_FOOTER_DATETIME_EDIT, m_strDateTime);
	DDX_Text(pDX, IDC_FOOTER_PRJPATH_EDIT, m_strProjPath);
	DDX_Text(pDX, IDC_FOOTER_OBJNAME_EDIT, m_strObjName);
	DDX_Text(pDX, IDC_FOOTER_PAGECNT_EDIT, m_strPageCount);
	DDX_Text(pDX, IDC_FOOTER_INFO1_EDIT, m_strInfo1);
	DDX_Text(pDX, IDC_FOOTER_INFO2_EDIT, m_strInfo2);
	DDX_Text(pDX, IDC_FOOTER_INFO3_EDIT, m_strInfo3);
	DDX_Text(pDX, IDC_FOOTER_INFO4_EDIT, m_strInfo4);
	DDX_Text(pDX, IDC_FOOTER_CBITMAP_EDIT, m_strBitmap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintFooterDataDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintFooterDataDlg)
	ON_BN_CLICKED(IDC_FOOTER_APPR_BTN, OnFooterApprBtn)
	ON_BN_CLICKED(IDC_FOOTER_CBITMAP_SEL_BTN, OnFooterCbitmapSelBtn)
	ON_BN_CLICKED(IDC_FOOTER_CHDATE_BTN, OnFooterChdateBtn)
	ON_BN_CLICKED(IDC_FOOTER_CHDESCR_BTN, OnFooterChdescrBtn)
	ON_BN_CLICKED(IDC_FOOTER_CHNAME_BTN, OnFooterChnameBtn)
	ON_BN_CLICKED(IDC_FOOTER_DATE_BTN, OnFooterDateBtn)
	ON_BN_CLICKED(IDC_FOOTER_DRAWN_BTN, OnFooterDrawnBtn)
	ON_BN_CLICKED(IDC_FOOTER_REL_BTN, OnFooterRelBtn)
	ON_BN_CLICKED(IDC_FOOTER_REVNO_BTN, OnFooterRevnoBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintFooterDataDlg message handlers

BOOL CPrintFooterDataDlg::OnInitDialog() 
{
    CopyDataToMembers();

	CDialog::OnInitDialog();

    SetButtonTexts();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintFooterDataDlg::OnOK() 
{
    UpdateData();
    CopyDataFromMembers();	
	CDialog::OnOK();
}


void CPrintFooterDataDlg::CopyDataToMembers()
{
    ASSERT(m_pPrintFooter);
    if(!m_pPrintFooter)
    {
        return;
    }
	m_strRevNo1         = m_pPrintFooter->m_strRevNo[0];
	m_strRevNo2         = m_pPrintFooter->m_strRevNo[1];
	m_strRevNo3         = m_pPrintFooter->m_strRevNo[2];
	m_strChangeDesc1    = m_pPrintFooter->m_strChangeDescr[0];
	m_strChangeDesc2    = m_pPrintFooter->m_strChangeDescr[1];
	m_strChangeDesc3    = m_pPrintFooter->m_strChangeDescr[2];
	m_strChangeName1    = m_pPrintFooter->m_strChangeName[0];
	m_strChangeName2    = m_pPrintFooter->m_strChangeName[1];
	m_strChangeName3    = m_pPrintFooter->m_strChangeName[2];
	m_strChangeDate1    = m_pPrintFooter->m_strChangeDate[0];
	m_strChangeDate2    = m_pPrintFooter->m_strChangeDate[1];
	m_strChangeDate3    = m_pPrintFooter->m_strChangeDate[2];
	m_strDate           = m_pPrintFooter->m_strRevDate;
	m_strDrawn          = m_pPrintFooter->m_strRevDrawn;
	m_strApproved       = m_pPrintFooter->m_strRevApproved;
	m_strReleased       = m_pPrintFooter->m_strRevReleased;
	m_strDateTime       = m_pPrintFooter->m_strDateAndTime;
	m_strProjPath       = m_pPrintFooter->m_strPrjPath;
	m_strObjName        = m_pPrintFooter->m_strObjName;
	m_strPageCount      = m_pPrintFooter->m_strPageCount;
	m_strInfo1          = m_pPrintFooter->m_strInfo[0];
	m_strInfo2          = m_pPrintFooter->m_strInfo[1];
	m_strInfo3          = m_pPrintFooter->m_strInfo[2];
	m_strInfo4          = m_pPrintFooter->m_strInfo[3];
 
    m_strRevNoCap       = m_pPrintFooter->m_strRevNoCaption;
    m_strChangeDescCap  = m_pPrintFooter->m_strChangeDescrCaption;
    m_strChangeDateCap  = m_pPrintFooter->m_strChangeDateCaption;
    m_strChangeNameCap  = m_pPrintFooter->m_strChangeNameCaption;
    m_strDateCap        = m_pPrintFooter->m_strRevDateCaption;
    m_strDrawnCap       = m_pPrintFooter->m_strRevDrawnCaption;
    m_strApprovedCap    = m_pPrintFooter->m_strRevApprovedCaption;
    m_strReleasedCap    = m_pPrintFooter->m_strRevReleasedCaption;

    m_strBitmap         = m_pPrintFooter->m_strCenterBitmap;

    m_strDateTime.Replace(_T("\n"), _T("\r\n"));
    m_strInfo3.Replace(_T("\n"), _T("\r\n"));
}

void CPrintFooterDataDlg::CopyDataFromMembers()
{
    ASSERT(m_pPrintFooter);
    if(!m_pPrintFooter)
    {
        return;
    }

    m_strDateTime.Replace(_T("\r\n"), _T("\n"));
    m_strInfo3.Replace(_T("\r\n"), _T("\n"));

	m_pPrintFooter->m_strRevNo[0]           = m_strRevNo1;
	m_pPrintFooter->m_strRevNo[1]           = m_strRevNo2;
	m_pPrintFooter->m_strRevNo[2]           = m_strRevNo3;
	m_pPrintFooter->m_strChangeDescr[0]     = m_strChangeDesc1;
	m_pPrintFooter->m_strChangeDescr[1]     = m_strChangeDesc2;
	m_pPrintFooter->m_strChangeDescr[2]     = m_strChangeDesc3;
	m_pPrintFooter->m_strChangeName[0]      = m_strChangeName1;
	m_pPrintFooter->m_strChangeName[1]      = m_strChangeName2;
	m_pPrintFooter->m_strChangeName[2]      = m_strChangeName3;
	m_pPrintFooter->m_strChangeDate[0]      = m_strChangeDate1;
	m_pPrintFooter->m_strChangeDate[1]      = m_strChangeDate2;
	m_pPrintFooter->m_strChangeDate[2]      = m_strChangeDate3;
	m_pPrintFooter->m_strRevDate            = m_strDate;
	m_pPrintFooter->m_strRevDrawn           = m_strDrawn;
	m_pPrintFooter->m_strRevApproved        = m_strApproved;
	m_pPrintFooter->m_strRevReleased        = m_strReleased;
	m_pPrintFooter->m_strDateAndTime        = m_strDateTime;
	m_pPrintFooter->m_strPrjPath            = m_strProjPath;
	m_pPrintFooter->m_strObjName            = m_strObjName;
	m_pPrintFooter->m_strPageCount          = m_strPageCount;
	m_pPrintFooter->m_strInfo[0]            = m_strInfo1;
	m_pPrintFooter->m_strInfo[1]            = m_strInfo2;
	m_pPrintFooter->m_strInfo[2]            = m_strInfo3;
	m_pPrintFooter->m_strInfo[3]            = m_strInfo4;

    m_pPrintFooter->m_strRevNoCaption       = m_strRevNoCap;
    m_pPrintFooter->m_strChangeDescrCaption = m_strChangeDescCap;
    m_pPrintFooter->m_strChangeDateCaption  = m_strChangeDateCap;
    m_pPrintFooter->m_strChangeNameCaption  = m_strChangeNameCap;
    m_pPrintFooter->m_strRevDateCaption     = m_strDateCap;
    m_pPrintFooter->m_strRevDrawnCaption    = m_strDrawnCap;
    m_pPrintFooter->m_strRevApprovedCaption = m_strApprovedCap;
    m_pPrintFooter->m_strRevReleasedCaption = m_strReleasedCap;

    m_pPrintFooter->m_strCenterBitmap       = m_strBitmap;
}


void CPrintFooterDataDlg::SetButtonTexts()
{
    CWnd*   pWnd;
    
    pWnd = GetDlgItem(IDC_FOOTER_REVNO_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strRevNoCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_CHDESCR_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strChangeDescCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_CHDATE_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strChangeDateCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_CHNAME_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strChangeNameCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_DATE_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strDateCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_DRAWN_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strDrawnCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_APPR_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strApprovedCap);
    }

    pWnd = GetDlgItem(IDC_FOOTER_REL_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strReleasedCap);
    }
}

void CPrintFooterDataDlg::OnFooterCbitmapSelBtn() 
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
        CWnd*   pWnd = GetDlgItem(IDC_FOOTER_CBITMAP_EDIT);
        m_strBitmap = dlgFile.m_ofn.lpstrFile;
        pWnd->SetWindowText(m_strBitmap);
    }
}

void CPrintFooterDataDlg::OnFooterApprBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strApprovedCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strApprovedCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_APPR_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strApprovedCap);
    }
}

void CPrintFooterDataDlg::OnFooterChdateBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strChangeDateCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strChangeDateCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_CHDATE_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strChangeDateCap);
    }
}

void CPrintFooterDataDlg::OnFooterChdescrBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strChangeDescCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strChangeDescCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_CHDESCR_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strChangeDescCap);
    }
}

void CPrintFooterDataDlg::OnFooterChnameBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strChangeNameCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strChangeNameCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_CHNAME_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strChangeNameCap);
    }
}

void CPrintFooterDataDlg::OnFooterDateBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strDateCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strDateCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_DATE_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strDateCap);
    }
}

void CPrintFooterDataDlg::OnFooterDrawnBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strDrawnCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strDrawnCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_DRAWN_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strDrawnCap);
    }
}

void CPrintFooterDataDlg::OnFooterRelBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strReleasedCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strReleasedCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_REL_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strReleasedCap);
    }
}

void CPrintFooterDataDlg::OnFooterRevnoBtn() 
{
    CChangeCaptionDlg tDlg(this);
    tDlg.m_strCaption = m_strRevNoCap;
    if(tDlg.DoModal() == IDOK)
    {
        m_strRevNoCap = tDlg.m_strCaption;
    }

    CWnd* pWnd = GetDlgItem(IDC_FOOTER_REVNO_BTN);
    if(pWnd)
    {
        pWnd->SetWindowText(m_strRevNoCap);
    }
}
