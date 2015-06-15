


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ceaddonhandler.h"
#include "FileSelDlg.h"
//----  Static Initializations:   ----------------------------------*



// FileSelDlg.cpp : implementation file
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSelDlg dialog


CFileSelDlg::CFileSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSelDlg)
	DDX_Control(pDX, IDC_LIST1, m_tListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSelDlg, CDialog)
	//{{AFX_MSG_MAP(CFileSelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSelDlg message handlers

BOOL CFileSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int iNumStrings = m_astrStrings.GetSize();
    for(int iString = 0; iString < iNumStrings; ++iString)
    {
        m_tListBox.AddString(m_astrStrings[iString]);
    }
    if(iNumStrings > 0)
    {
        m_tListBox.SetSel(0);
    }

    SetWindowText(m_strTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileSelDlg::OnOK() 
{
	CString         strSel;
    CArray<int,int> aiListBoxSel;
    int             iCount = m_tListBox.GetSelCount();

    aiListBoxSel.SetSize(iCount);
    m_tListBox.GetSelItems(iCount, aiListBoxSel.GetData());

    for(int iSel = 0; iSel < iCount; ++iSel)
    {
        m_tListBox.GetText(aiListBoxSel[iSel], strSel);
        m_astrStringsSel.Add(strSel);
    }
	
	CDialog::OnOK();
}


void CFileSelDlg::Initialize(const CString& crstrTitle, const CStringArray& crastrStrings)
{
    m_astrStrings.Copy(crastrStrings);
    m_strTitle = crstrTitle;
}

void CFileSelDlg::GetSelection(CStringArray& rastrSelection)
{
    rastrSelection.Copy(m_astrStringsSel);
}

