
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// MultiSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MultiSelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSelDlg dialog


CMultiSelDlg::CMultiSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bSingleSelect = FALSE;
}


void CMultiSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiSelDlg)
	DDX_Control(pDX, IDC_MULTI_SEL_LIST, m_tListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiSelDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiSelDlg)
	ON_LBN_DBLCLK(IDC_MULTI_SEL_LIST, OnDblclkMultiSelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMultiSelDlg::Initialize(const CString& crstrTitle, const CStringArray& crastrStrings, BOOL bSingleSelect/*=FALSE*/)
{
    m_astrStrings.Copy(crastrStrings);
    m_strTitle = crstrTitle;
	m_bSingleSelect = bSingleSelect;
}

void CMultiSelDlg::GetSelection(CStringArray& rastrSelection)
{
    rastrSelection.Copy(m_astrStringsSel);
}


/////////////////////////////////////////////////////////////////////////////
// CMultiSelDlg message handlers


BOOL CMultiSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_bSingleSelect)
	{
		// unfortunately the LBS_EXTENDEDSEL style cannot be modified
		// on runtime -> destroy window and recreate it.
		RECT	tRect;
		long	lNewStyle = GetWindowLong(m_tListBox.GetSafeHwnd(), GWL_STYLE);
		long	lStyleEx = GetWindowLong(m_tListBox.GetSafeHwnd(), GWL_EXSTYLE);
		CFont*	pFont = m_tListBox.GetFont();

		lNewStyle |= WS_VSCROLL | WS_HSCROLL; ///<-for some reason these bits get lost by GetWindowLong 
		lNewStyle &= ~LBS_EXTENDEDSEL;

		m_tListBox.GetWindowRect(&tRect);
		ScreenToClient(&tRect);
		m_tListBox.DestroyWindow();
		m_tListBox.CreateEx(lStyleEx, _T("LISTBOX"), NULL, lNewStyle, tRect, this, IDD_MULTI_SELECT_DLG);
		m_tListBox.SetFont(pFont);
	}


    int iNumStrings = m_astrStrings.GetSize();
    for(int iString = 0; iString < iNumStrings; ++iString)
    {
        m_tListBox.AddString(m_astrStrings[iString]);
    }
    if(iNumStrings > 0)
    {
		if(m_bSingleSelect)
			m_tListBox.SetCurSel(0);
		else
			m_tListBox.SetSel(0);
    }

    SetWindowText(m_strTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMultiSelDlg::OnOK() 
{
    // now copy selection to the selection strings
    CString         strSel;

	if(m_bSingleSelect)
	{
		int iSel = m_tListBox.GetCurSel();
		if(iSel>=0)
		{
			m_tListBox.GetText(iSel, strSel);
			m_astrStringsSel.Add(strSel);
		}
	}
	else
	{
		CArray<int,int> aiListBoxSel;
		int             iCount = m_tListBox.GetSelCount();
		aiListBoxSel.SetSize(iCount);
		m_tListBox.GetSelItems(iCount, aiListBoxSel.GetData());

		for(int iSel = 0; iSel < iCount; ++iSel)
		{
			m_tListBox.GetText(aiListBoxSel[iSel], strSel);
			m_astrStringsSel.Add(strSel);
		}
	}

	CDialog::OnOK();
}

void CMultiSelDlg::OnDblclkMultiSelList() 
{
    OnOK();
}
