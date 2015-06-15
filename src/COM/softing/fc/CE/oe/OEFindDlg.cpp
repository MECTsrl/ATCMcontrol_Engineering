// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------


// OEFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SECEdit.h"
#include "OEFindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OEFindDlg dialog


//@doc OEFindDlg
//@mfunc Constructor for the OEFindDlg.

OEFindDlg::OEFindDlg()
	: CFindReplaceDialog()
{
	//{{AFX_DATA_INIT(OEFindDlg)
	m_bRegular = FALSE;
	m_bMatchCase = FALSE;
	m_FindWhat = _T("");
	m_iUp = -1;
	m_ReplaceWith = _T("");
	m_bWrapSearch = FALSE;
   m_bMarkAll = FALSE;
	//}}AFX_DATA_INIT
}


//@doc OEFindDlg
//@mfunc Internal OEFindDlg method.
//@rdesc void 
//@parm CDataExchange* pDX
void OEFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OEFindDlg)
	DDX_Check(pDX, IDC_REGULAR, m_bRegular);
	DDX_Check(pDX, IDC_OE_CHX2, m_bMatchCase);
	DDX_Text(pDX, IDC_OE_EDT1, m_FindWhat);
	DDX_Radio(pDX, IDC_OE_RAD1, m_iUp);
	DDX_Text(pDX, IDC_OE_EDT2, m_ReplaceWith);
	DDX_Check(pDX, IDC_FINDCHECK2, m_bWrapSearch);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		CComboBox* combo = (CComboBox*)GetDlgItem(IDC_FINDCOMBO1);
		ASSERT(combo != NULL);
		combo->GetWindowText(m_FindWhat);
		if (m_bReplacing)
		{
			combo = (CComboBox*)GetDlgItem(IDC_FINDCOMBO2);
			ASSERT(combo != NULL);
			combo->GetWindowText(m_ReplaceWith);
		}
	}
}


BEGIN_MESSAGE_MAP(OEFindDlg, CDialog)
	//{{AFX_MSG_MAP(OEFindDlg)
	ON_BN_CLICKED(IDC_MARKALL, OnMarkAll)
	//}}AFX_MSG_MAP
#ifndef ORIGINAL_VERSION
    ON_MESSAGE (WM_HELP, OnHelp)
#endif
END_MESSAGE_MAP()


//@doc OEFindDlg
//@mfunc Internal OEFindDlg method. Implements the MarkAll behavior.
//@rdesc void 
void OEFindDlg::OnMarkAll() 
{
	m_bMarkAll = TRUE;
	SendMessage(WM_COMMAND,IDOK,0);
}

//@doc OEFindDlg
//@mfunc Internal OEFindDlg method. Picks a good spot for the dialog that doesn't obscure
//              the current line of text.  Fill the combo boxes with
//              the search history.
//@rdesc BOOL 
BOOL OEFindDlg::OnInitDialog()
   {
	// Could be SECEditCtrl or SECEditView
	// I'm not calling any virtual functions so i can do this
	SECEditCore<CWnd>* pView = (SECEditCore<CWnd>*)GetActiveView();

	if (pView != NULL)
		{
		CRect apprect;
		CRect rect;
		CRect thisrect;
		AfxGetApp()->m_pMainWnd->GetWindowRect(&apprect);
		pView->GetWindowRect(&rect);
		GetWindowRect(&thisrect);
		int height = thisrect.bottom - thisrect.top;
		int linetop = ((pView->m_iLineNo - pView->m_iFirstLineNo)*pView->m_iLineHeight) + rect.top;
		if ((height+apprect.top) < linetop)
			{
			thisrect.top = apprect.top;
			thisrect.bottom = thisrect.top + height;
			}
		else
			{
			thisrect.top = apprect.bottom - (height + (pView->m_iLineHeight*2));
			thisrect.bottom = thisrect.top + height;
			int cy = GetSystemMetrics(SM_CYMAXIMIZED);
			if (thisrect.bottom > cy)
				{
				thisrect.bottom = cy;
				thisrect.top = thisrect.bottom - height;
				}
			}
		MoveWindow(&thisrect,FALSE);
		}
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	InitCombo();
	if (m_bReplacing)
		{
		GetDlgItem(IDC_OE_PSH1)->EnableWindow(TRUE);
		GetDlgItem(IDC_OE_PSH2)->EnableWindow(TRUE);
		InitCombo2();
		}

	return CFindReplaceDialog::OnInitDialog();
	}


//@doc OEFindDlg
//@mfunc Internal OEFindDlg method. Reads the search history from the Registry and fill
//              the combo box
//@rdesc void 

void OEFindDlg::InitCombo()
   {

	USES_CONVERSION_T;

	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_FINDCOMBO1);
	ASSERT(combo != NULL);

	// get up to 10 search strings
	CString str;
	CString temp;  temp.LoadString(IDS_OE_FINDPERCENT);
	CString findstring;
	CString section;	section.LoadString(IDS_OE_SETTINGSFIND);
	for (int i = 1; i < 11; i++)
		{
		str.Format(temp,i);
		findstring =  AfxGetApp()->GetProfileString(section,str,afxEmptyString);
		if (!findstring.IsEmpty())
			{
		   combo->InsertString(i-1,findstring);
			if (m_FindWhat.IsEmpty())
				{
   			combo->GetLBText(0,m_FindWhat);
				combo->SetCurSel(0);
				}
			}
		else
			break;
		}
	if (!m_FindWhat.IsEmpty())
		{
		//char str[OE_MAXLINELENGTH+1];
		//strcpy(str,T2CA(m_FindWhat));
		//::SendMessage(combo->m_hWnd,WM_SETTEXT,0,(LPARAM)str);
		combo->SetWindowText(m_FindWhat);
		}
	}



//@doc OEFindDlg
//@mfunc Internal OEFindDlg method. Reads the Replace history from the Registry and
//              fill the combo box.
//@rdesc void 
void OEFindDlg::InitCombo2()
   {
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_FINDCOMBO2);
	// get up to 10 replace strings
	CString str;
	CString temp;  temp.LoadString(IDS_OE_REPLACEPERCENT);
	CString findstring;
	CString section;	section.LoadString(IDS_OE_SETTINGSFIND);
	for (int i = 1; i < 11; i++)
		{
		str.Format(temp,i);
		findstring =  AfxGetApp()->GetProfileString(section,str,afxEmptyString);
		BOOL bEmpty = (findstring == _T("<OE empty string>"));
		if (!findstring.IsEmpty() || bEmpty)
			{
			if (bEmpty)
				findstring.Empty();
		   combo->InsertString(i-1,findstring);
			if (m_ReplaceWith.IsEmpty())
				{
   			combo->GetLBText(0,m_ReplaceWith);
				combo->SetCurSel(0);
				}
			}
		else
			break;
		}
	}

//@doc OEFindDlg
//@mfunc Internal OEFindDlg method. Finds the active view so we can position the dlg
//              accordingly
//@rdesc CView* 
CView* OEFindDlg::GetActiveView()
{
	CMDIChildWnd* pChild = ((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	if (!pChild)
	   return ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	else
		return pChild->GetActiveView();
}


#ifndef ORIGINAL_VERSION
LRESULT OEFindDlg::OnHelp (WPARAM wParam, LPARAM lParam)
{
    CWnd *pOwner;

    pOwner = GetOwner ();
    if (pOwner != NULL)
    {
        pOwner->SendMessage (WM_HELP, wParam, lParam);
    };

    return (TRUE);
}
#endif
