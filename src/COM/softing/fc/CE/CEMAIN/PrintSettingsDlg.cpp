
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// PrintSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cemain.h"
#include "PrintSettingsDlg.h"
#include "CoverPageDataDlg.h"
#include "PrintHeaderDataDlg.h"
#include "PrintFooterDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintSettingsDlg dialog


CPrintSettingsDlg::CPrintSettingsDlg(CPrintSettings* pPrintSettings, CWnd* pParent /*=NULL*/)
	: CDialog(CPrintSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintSettingsDlg)
	m_bCoverPage = FALSE;
	m_bContents = FALSE;
	m_bCrossRef = FALSE;
	m_bFileView = FALSE;
	m_bClassView = FALSE;
	m_bInstView = FALSE;
	m_bHWView = FALSE;
	m_bGrHeader = FALSE;
	//}}AFX_DATA_INIT
    ASSERT(pPrintSettings);
    m_pPrintSettings = pPrintSettings;
}


void CPrintSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintSettingsDlg)
	DDX_Check(pDX, IDC_COVER_PAGE_CHECK, m_bCoverPage);
	DDX_Check(pDX, IDC_CONTENTS_CHECK, m_bContents);
	DDX_Check(pDX, IDC_CROSS_REF_CHECK, m_bCrossRef);
	DDX_Check(pDX, IDC_PS_FILE_VIEW_CHECK, m_bFileView);
	DDX_Check(pDX, IDC_PS_CLASS_VIEW_CHECK, m_bClassView);
	DDX_Check(pDX, IDC_PS_INST_VIEW_CHECK, m_bInstView);
	DDX_Check(pDX, IDC_PS_HW_VIEW_CHECK, m_bHWView);
	DDX_Check(pDX, IDC_PS_GRED_HEADER_CHECK, m_bGrHeader);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintSettingsDlg)
	ON_BN_CLICKED(IDC_COVPAGE_BTN, OnCovpageBtn)
	ON_BN_CLICKED(IDC_FOOTER_BTN, OnFooterBtn)
	ON_BN_CLICKED(IDC_HEADER_BTN, OnHeaderBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintSettingsDlg message handlers


BOOL CPrintSettingsDlg::OnInitDialog() 
{
    CopyDataToMembers();

	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPrintSettingsDlg::OnOK() 
{
    UpdateData();
    CopyDataFromMembers();
	CDialog::OnOK();
}


//------------------------------------------------------------------*
/**
 * copy data to members.
 *
 *  initialization of dialog members from data members
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             OnInitDialog(), CopyDataFromMembers()
*/
void CPrintSettingsDlg::CopyDataToMembers()
{
    UINT    uiPrintFlags = m_pPrintSettings->m_uiPrintFlags;
    m_bCoverPage = (uiPrintFlags & ePF_coverpage) ? TRUE : FALSE;
    m_bContents = (uiPrintFlags & ePF_contents) ? TRUE : FALSE;
	m_bCrossRef = (uiPrintFlags & ePF_crossref) ? TRUE : FALSE;
    m_bFileView = (uiPrintFlags & ePF_fileview) ? TRUE : FALSE;
    m_bClassView = (uiPrintFlags & ePF_classview) ? TRUE : FALSE;
    m_bInstView = (uiPrintFlags & ePF_instview) ? TRUE : FALSE;
    m_bHWView = (uiPrintFlags & ePF_hwview) ? TRUE : FALSE;
    m_bGrHeader = (uiPrintFlags & ePF_grheader) ? TRUE : FALSE;
}


//------------------------------------------------------------------*
/**
 * copy data from members
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             OnOK(), CopyDataToMembers()
*/
void CPrintSettingsDlg::CopyDataFromMembers()
{
    UINT&    ruiPrintFlags = m_pPrintSettings->m_uiPrintFlags;
    
    if(m_bCoverPage)
    {
        ruiPrintFlags |= ePF_coverpage;
    }
    else
    {
        ruiPrintFlags &= ~ePF_coverpage;
    }

    if(m_bContents)
    {
        ruiPrintFlags |= ePF_contents;
    }
    else
    {
        ruiPrintFlags &= ~ePF_contents;
    }

    if(m_bCrossRef)
    {
        ruiPrintFlags |= ePF_crossref;
    }
    else
    {
        ruiPrintFlags &= ~ePF_crossref;
    }

    if(m_bFileView)
    {
        ruiPrintFlags |= ePF_fileview;
    }
    else
    {
        ruiPrintFlags &= ~ePF_fileview;
    }

    if(m_bClassView)
    {
        ruiPrintFlags |= ePF_classview;
    }
    else
    {
        ruiPrintFlags &= ~ePF_classview;
    }

    if(m_bInstView)
    {
        ruiPrintFlags |= ePF_instview;
    }
    else
    {
        ruiPrintFlags &= ~ePF_instview;
    }

    if(m_bHWView)
    {
        ruiPrintFlags |= ePF_hwview;
    }
    else
    {
        ruiPrintFlags &= ~ePF_hwview;
    }

    if(m_bGrHeader)
    {
        ruiPrintFlags |= ePF_grheader;
    }
    else
    {
        ruiPrintFlags &= ~ePF_grheader;
    }
}


void CPrintSettingsDlg::OnCovpageBtn() 
{
    ASSERT(m_pPrintSettings);
    if(!m_pPrintSettings)
    {
        return;
    }

    CCoverPageDataDlg   tDlg(&m_pPrintSettings->m_tCoverPage, this);

    tDlg.DoModal();
}

void CPrintSettingsDlg::OnFooterBtn() 
{
    ASSERT(m_pPrintSettings);
    if(!m_pPrintSettings)
    {
        return;
    }

    CPrintFooterDataDlg   tDlg(&m_pPrintSettings->m_tPrintFooter, this);

    tDlg.DoModal();
}

void CPrintSettingsDlg::OnHeaderBtn() 
{
    ASSERT(m_pPrintSettings);
    if(!m_pPrintSettings)
    {
        return;
    }

    CPrintHeaderDataDlg   tDlg(&m_pPrintSettings->m_tPrintHeader, this);

    tDlg.DoModal();
}

