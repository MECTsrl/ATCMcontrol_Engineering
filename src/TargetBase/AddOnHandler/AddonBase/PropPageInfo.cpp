
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"

#include "PropPageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageInfo dialog


CPropPageInfo::CPropPageInfo(CWnd* pParent /*=NULL*/)
	: CBasePropertyPage(CPropPageInfo::IDD)
{
	//{{AFX_DATA_INIT(CPropPageInfo)
	m_strAlias = _T("");
	m_strURL = _T("");
	m_strText1 = _T("");
	m_strText2 = _T("");
	m_strText3 = _T("");
	//}}AFX_DATA_INIT
}



void CPropPageInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageInfo)
	DDX_Text(pDX, IDC_PROPINFO_ALIAS, m_strAlias);
	DDX_Text(pDX, IDC_PROPINFO_URL, m_strURL);
	DDX_Text(pDX, IDC_PROPINFO_TEXT1, m_strText1);
	DDX_Text(pDX, IDC_PROPINFO_TEXT2, m_strText2);
	DDX_Text(pDX, IDC_PROPINFO_TEXT3, m_strText3);
	//}}AFX_DATA_MAP
}

//------------------------------------------------------------------*
/**
 * initialize.
 *
 * @param           crstrProjectPath: project path
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageInfo::Init(const CString& crstrProjectPath)
{
    m_strProjectPath = crstrProjectPath;
}


BEGIN_MESSAGE_MAP(CPropPageInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageInfo)
	ON_BN_CLICKED(IDC_PROPGEN_BROWSE, OnPropgenBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageInfo message handlers

void CPropPageInfo::OnPropgenBrowse() 
{
    CString sFilter;
    CString sTitle;
    CString sUrl;

    sFilter.LoadString(IDS_STRING_URL_FILTER);
    sTitle.LoadString(IDS_STRING_URL_FILE);
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, sFilter, this);
    dlg.m_ofn.lpstrTitle = sTitle;
    dlg.m_ofn.lpstrInitialDir = m_strProjectPath;
    if (dlg.DoModal() == IDOK) 
    {
        CString sUrl = dlg.GetPathName();
        int iPos = sUrl.ReverseFind(_T('\\'));
        CString strFHeader;
        if (iPos>0)
        {
            CString strPath = sUrl.Left(iPos);
            if (strPath.CompareNoCase(m_strProjectPath)==0)
            {
                sUrl = sUrl.Mid(iPos+1);
                strFHeader.LoadString(IDS_URL_FILEHEADER);
            }
        }
        sUrl = strFHeader + sUrl;
        SetDlgItemText(IDC_PROPINFO_URL, sUrl);
    }
}


BOOL CPropPageInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/* ---------------------------------------------------------------------------- */
