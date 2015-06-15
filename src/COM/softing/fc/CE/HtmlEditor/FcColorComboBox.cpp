

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlColor.h"
#include "FcColorComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcColorComboBox

CFcColorComboBox::CFcColorComboBox()
{

}

CFcColorComboBox::~CFcColorComboBox()
{
}

void CFcColorComboBox::Init()
{
	ResetContent();	
	for (unsigned ii = 0; ii < FcGetNamedColorMax(); ii++)
	{
		AddString(FcGetHtmlColorNameAndValue(ii));
	}
}

BEGIN_MESSAGE_MAP(CFcColorComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFcColorComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcColorComboBox message handlers


void CFcColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	if (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))
	{
		COLORREF TextColor, BgColor, FieldColor;
		if (lpDIS->itemState & ODS_SELECTED)
		{
			TextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
			BgColor = GetSysColor(COLOR_HIGHLIGHT);
		}
		else
		{
			TextColor = GetSysColor(COLOR_WINDOWTEXT);
			BgColor = GetSysColor(COLOR_WINDOW);
		}

		CString ItemText;
		GetLBText(lpDIS->itemID, ItemText);
		if (ItemText.GetLength() > 0)
		{
			CString HtmlColor = FcGetHtmlColorValueFromString(ItemText);
			FieldColor =  FcHtmlColor2ColorRef(HtmlColor);
		}
		else
			FieldColor =  0;

		// draw background
		CBrush BgBrush(BgColor);
		pDC->FillRect(&lpDIS->rcItem, &BgBrush);

		// draw coloured field
		CRect ColorRect = lpDIS->rcItem;
		ColorRect.left    += 2;
		ColorRect.top     += 2;
		ColorRect.bottom  -= 2 * 2;
		ColorRect.right   = ColorRect.left + 2 * ColorRect.Height();

		CBrush FieldBrush(FieldColor);
		CBrush* pOldBr  = pDC->SelectObject(&FieldBrush);
		pDC->Rectangle(ColorRect);
		pDC->SelectObject(pOldBr);

		// draw text
		COLORREF OldBgColor   = pDC->SetBkColor(BgColor);
		COLORREF OldTextColor = pDC->SetTextColor(TextColor);
		pDC->ExtTextOut(
			ColorRect.right + 4, lpDIS->rcItem.top, 
			ETO_OPAQUE, NULL,
			ItemText, NULL);
		pDC->SetTextColor(OldTextColor);
		pDC->SetBkColor(OldBgColor);
	}
}
