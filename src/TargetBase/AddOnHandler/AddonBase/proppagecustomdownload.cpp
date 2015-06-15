
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AddonBaseResource.h"
#include "PropPageCustomDownload.h"
#include "DirectoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadPage property page


CPropPageCustomDownload::CPropPageCustomDownload() : CBasePropertyPage(CPropPageCustomDownload::IDD)
{
	//{{AFX_DATA_INIT(CDownloadPage)
	m_strDir = _T("");
	//}}AFX_DATA_INIT
}

CPropPageCustomDownload::~CPropPageCustomDownload()
{
}

void CPropPageCustomDownload::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadPage)
	DDX_Text(pDX, IDC_PPCD_DIR, m_strDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageCustomDownload, CBasePropertyPage)
	//{{AFX_MSG_MAP(CDownloadPage)
	ON_BN_CLICKED(IDC_PPCD_BTN_BROWSE, OnCdBtnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageCustomDownload message handlers

void CPropPageCustomDownload::OnCdBtnBrowse() 
{
    UpdateData(TRUE);

    CDirectoryDlg   tDlg;
    tDlg.SetInitialDir(m_strDir);

    if(tDlg.DoModal() == IDOK)
    {
        m_strDir = tDlg.GetDir();
        UpdateData(FALSE);
    }	
}


BOOL CPropPageCustomDownload::Validate()
{
    return TRUE;
}


CString CPropPageCustomDownload::GetDir()
{
    return m_strDir;
}

void CPropPageCustomDownload::SetDir(const CString& str)
{
    m_strDir = str;
}

/* ---------------------------------------------------------------------------- */
