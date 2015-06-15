

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"
#include "FcHtmlColor.h"

#include "FcFontBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


INT CALLBACK NEnumFontNameProc2(LOGFONT *plf, TEXTMETRIC *ptm, INT nFontType, LPARAM lParam)
{
	CComboBox* comboBox = (CComboBox*) lParam;
	comboBox->AddString(plf->lfFaceName);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFcFontBar

CFcFontBar::CFcFontBar()
{
}


CFcFontBar::~CFcFontBar()
{
}

BEGIN_MESSAGE_MAP(CFcFontBar, CDialogBar)
	//{{AFX_MSG_MAP(CFcFontBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFcFontBar::Init()
{
	//  Fill font combo box
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_FONTNAME);
	ASSERT_VALID(pCb);

	::EnumFontFamilies(GetDC()->m_hDC, (LPTSTR) NULL, (FONTENUMPROC)NEnumFontNameProc2, (LPARAM)pCb);
	pCb->SetCurSel(-1);

	return TRUE;
}

BOOL CFcFontBar::AddHtmlTag(const CString& tag, BOOL bClear)
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_BLOCKFORMAT);
	ASSERT_VALID(pCb);

	if (bClear)
	{
		for (int ii = pCb->GetCount()-1; ii >= 0; ii--)
			pCb->DeleteString(ii);
	}
	pCb->AddString(tag);
	return TRUE;
}

CString CFcFontBar::GetSelectedFontName() const
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_FONTNAME);
	ASSERT_VALID(pCb);

	CString fontName;
	int selIndex = pCb->GetCurSel();
	if (selIndex != CB_ERR)
		pCb->GetLBText(selIndex, fontName);
	return fontName;
}

int CFcFontBar::GetSelectedFontSize() const
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_FONTSIZE);
	ASSERT_VALID(pCb);

	int selIndex = pCb->GetCurSel();
	if (selIndex != CB_ERR)
		return selIndex + 1; 
	else
		return -1;
}

CString CFcFontBar::GetSelectedHtmlTag() const
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_BLOCKFORMAT);
	ASSERT_VALID(pCb);

	CString HtmlTag;
	int selIndex = pCb->GetCurSel();
	if (selIndex != CB_ERR)
		pCb->GetLBText(selIndex, HtmlTag);
	return HtmlTag;
}

void CFcFontBar::SetSelectedHtmlTag(const CString& SelectString, BOOL bEnable) 
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_BLOCKFORMAT);
	ASSERT_VALID(pCb);

	if (SelectString.GetLength() > 0)
		pCb->SelectString(-1, SelectString);
	else
		pCb->SetCurSel(-1);

	pCb->EnableWindow(bEnable);
}

void CFcFontBar::SetSelectedFontName(const CString& SelectString, BOOL bEnable) 
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_FONTNAME);
	ASSERT_VALID(pCb);

	if (SelectString.GetLength() > 0)
		pCb->SelectString(-1, SelectString);
	else
		pCb->SetCurSel(-1);

	pCb->EnableWindow(bEnable);
}

void CFcFontBar::SetSelectedFontSize(int Size, BOOL bEnable) 
{
	CComboBox* pCb = (CComboBox*)GetDlgItem(ID_FORMAT_FONTSIZE);
	ASSERT_VALID(pCb);

	if (Size > 0)
		pCb->SetCurSel(Size -1);
	else
		pCb->SetCurSel(-1);

	pCb->EnableWindow(bEnable);
}


/////////////////////////////////////////////////////////////////////////////
// CFcFontBar message handlers
