

#include "stdafx.h"
#include "resource.h"
#include "CeWatchList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*****************************************************************************
void CCeWatchList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
//*****************************************************************************
{
	int iItem = lpDrawItemStruct->itemID;
	if (iItem >= 0 && iItem < GetItemCount())
	{
		CCeDisplayElement* pDisplayElement = GetDisplayElement(iItem);
		if (pDisplayElement != NULL && pDisplayElement->GetWatchElement() != NULL)
		{
			ASSERT(IsValidDisplayElement(pDisplayElement));
			
			CCeWatchElement* pVar = pDisplayElement->GetWatchElement();
			CCeWatchElement* pVarParent = pVar->GetParent();
			int iLevel = pVar->GetLevel();

	//		some abreviations
			CDC *pDC       = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect ItemRect = lpDrawItemStruct->rcItem;
			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm); 
			pDC->SetBkMode(TRANSPARENT);
			
			COLORREF BgColor;
			COLORREF TextColor;
			COLORREF TreeColor = RGB(128, 128, 128);		
			COLORREF GridColor = RGB(196, 196, 196);

			if (lpDrawItemStruct->itemState & ODS_SELECTED)
			{
				if (GetFocus() == this)
				{
					BgColor = GetSysColor(COLOR_HIGHLIGHT);
					TextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
				}
				else
				{
					BgColor = GetSysColor(COLOR_BTNFACE);
					TextColor = GetSysColor(COLOR_WINDOWTEXT);
				}
			}
			else
			{
				BgColor = GetSysColor(COLOR_WINDOW);
				TextColor = GetSysColor(COLOR_WINDOWTEXT);
			}

	//		draw background
			CBrush BrushBg(BgColor);
			pDC->FillRect(&lpDrawItemStruct->rcItem, &BrushBg);

	//		draw grid lines
			CPen PenGrid(PS_SOLID, 1, GridColor);
			CPen* pOldPen = pDC->SelectObject(&PenGrid);
			pDC->MoveTo(ItemRect.left, ItemRect.bottom-1);
			pDC->LineTo(ItemRect.right-1, ItemRect.bottom-1);

			int x = ItemRect.left - 1;
			for (int ii = 0; ii < GetColumnCount(); ii++)
			{
				x += GetColumnWidth(ii);		
				pDC->MoveTo(x, ItemRect.top);			
				pDC->LineTo(x, ItemRect.bottom);			
			}

			CRect LabelRect;
			GetItemRect(iItem, LabelRect, LVIR_LABEL); 

			// set clipping region for graphic output
			CRgn Rgn;
			Rgn.CreateRectRgn(0, LabelRect.top, LabelRect.right, LabelRect.bottom );
			pDC->SelectClipRgn(&Rgn);

			// draw LEDs
			if (pVar->GetChildrenCount() == 0 && pVar->GetType().IsSimpleType())
			{
				CPoint p(ItemRect.left, ItemRect.top);
				if (pVar->GetQuality() & OPC_QUALITY_GOOD)
					m_ImageList.Draw(pDC, 0, p, ILD_TRANSPARENT);		
				else if (pVar->GetQuality() & OPC_QUALITY_UNCERTAIN)
					m_ImageList.Draw(pDC, 2, p, ILD_TRANSPARENT);		
				else
					m_ImageList.Draw(pDC, 1, p, ILD_TRANSPARENT);		
			}

			// calculate start position (x) for output and
			// indentation per level
			m_xIndent = tm.tmHeight;

			// size of rectangle containing the '+' or '-' sign
			int rWidth = tm.tmHeight - 2;
			rWidth -= (rWidth + 1) % 2;

			int x1 = LabelRect.left + (iLevel - 1) * m_xIndent;
			int y1 = ItemRect.top + (ItemRect.bottom - ItemRect.top) / 2 - tm.tmHeight / 2;
			int x2 = x1 + rWidth - 1;
			int y2 = y1 + rWidth - 1;

			int xCenter = x1 + rWidth / 2;
			int yCenter = y1 + rWidth / 2;
 
	//		draw tree lines
			CPen PenTree(PS_SOLID, 1, TreeColor);
			pDC->SelectObject(&PenTree);
			if (iLevel > 1) // top level objects have level 1 !
			{
				int xo = xCenter - m_xIndent;
				// horizontal
				pDC->MoveTo(xo, yCenter);			
				if (pVar->GetChildrenCount() > 0)
					pDC->LineTo(x1, yCenter);							
				else
					pDC->LineTo(x2, yCenter);							

				// vertical
				pDC->MoveTo(xo, ItemRect.top);
				pDC->LineTo(xo, yCenter);			

				// draw vertical lines of parent elements
				unsigned long iMask = GetVertLineMask(iItem);
				int x = xo;
				for (unsigned ui = iLevel; ui >= 1; ui--)
				{
					if (((1UL << (ui-1)) & iMask) != 0)
					{
						pDC->MoveTo(x, ItemRect.top);
						pDC->LineTo(x, ItemRect.bottom);
					}
					x -= m_xIndent;
				}
			}

			// draw small line below the '-' sign to the next item
			if (pVar->GetChildrenCount() > 0 && IsExpanded(iItem))
			{
				pDC->MoveTo(xCenter, y2 + 1);				
				pDC->LineTo(xCenter, ItemRect.bottom);				
			}
			
	//		draw '+' or '-' character
			if (pVar->GetChildrenCount() > 0 && GetColumnWidth(0) > x1)
			{
				pDC->SelectStockObject(BLACK_PEN);
				pDC->Rectangle(x1, y1, x2+1, y2+1);

				pDC->MoveTo(x1 + 2    , y1 + rWidth/2);			
				pDC->LineTo(x2 - 1, y1 + rWidth/2);
				if (!IsExpanded(iItem))
				{
					pDC->MoveTo(x1 + rWidth/2, y1 + 2);			
					pDC->LineTo(x1 + rWidth/2, y2 - 1);				
				}
			}

			int yo = ItemRect.top + 1;
			int xo = ItemRect.left + 2;

	//		draw name
			pDC->SetTextColor(TextColor);
			CString str;
			int iColumn = 0;

			if (iLevel == 1) // output of full name (including path)
				str = pVar->GetName();
			else
				str = pVar->GetShortName();
			
			if (pVar->GetType().IsArray())
				str += ", [" + GetWatchIndices(iItem)->AsString() + "]";

			// clipping is only neccessary for graphic output
			pDC->SelectClipRgn(NULL);

	//		CString str2;
	//		str2.Format("  %lu", GetVertLineMask(iItem));
	//		str += str2;

			if (str.GetLength() > 0)
			{
				int xOff = iLevel * m_xIndent + 2;
				int x = LabelRect.left + iLevel * m_xIndent + 2;
				pDC->TextOut(LabelRect.left + xOff, yo, FormatText(pDC, str, LabelRect.Width() - xOff));
			}

	//		draw type
			iColumn++;
			xo += GetColumnWidth(iColumn-1);
			str = pVar->GetType().GetName();
			if (str.GetLength() > 0)
				pDC->TextOut(xo, yo, FormatText(pDC, str, GetColumnWidth(iColumn) - 4));

	//		container do not have a value,time and quality
			if (pVar->GetChildrenCount() == 0 && pVar->GetType().IsSimpleType())
			{
	//			draw value
				iColumn++;
				xo += GetColumnWidth(iColumn-1);
				str = pVar->GetValue();
				if (str.GetLength() > 0)
					pDC->TextOut(xo, yo, FormatText(pDC, str, GetColumnWidth(iColumn) - 4));

	//			draw time
				iColumn++;
				xo += GetColumnWidth(iColumn-1);
				str = pVar->GetTime();
				if (str.GetLength() > 0)
					pDC->TextOut(xo, yo, FormatText(pDC, str, GetColumnWidth(iColumn) - 4));

	//			draw quality
				iColumn++;
				xo += GetColumnWidth(iColumn-1);
				str = pVar->GetQualityText();
				if (str.GetLength() > 0)
					pDC->TextOut(xo, yo, FormatText(pDC, str, GetColumnWidth(iColumn) - 4));

	//			draw format
				iColumn++;
				xo += GetColumnWidth(iColumn-1);
				if (pVar->GetFormat().GetLength() > 0)
					pDC->TextOut(xo, yo, FormatText(pDC, pVar->GetFormat(), GetColumnWidth(iColumn) - 4));
			}

			// draw focus rectangle
			if (GetFocus() == this)
			{
				if (lpDrawItemStruct->itemState & ODS_FOCUS)
				{
					CRect r(lpDrawItemStruct->rcItem);
					r.bottom -= 1;
					pDC->DrawFocusRect(r);
				}
			}

			pDC->SelectObject(pOldPen);
		}
	}
}


//*****************************************************************************
unsigned long CCeWatchList::CalcVertLineMask(int iItem) 
//*****************************************************************************
{
	unsigned iMask = 0;
	int iItemMax = GetItemCount() - 1;
	if (iItem == iItemMax)
		return iMask;

	unsigned iLevel = GetVar(iItem)->GetLevel();
	unsigned iNextLevel;
	for (int ii = iItem+1; ii <= iItemMax; ii++)
	{
		iNextLevel = GetVar(ii)->GetLevel();
		if (iNextLevel <= iLevel)
		{
			if (iNextLevel <= 1)
				return iMask;
			iMask |= (1UL << iNextLevel);
		}
	}
	return iMask;
}

//*****************************************************************************
void CCeWatchList::UpdateVertLineMasks(int iStartItem) 
//*****************************************************************************
{
	unsigned iLevel = GetVar(iStartItem)->GetLevel();
	if (iLevel > 1 && iLevel < 32)
	{
		unsigned long iMask = (1UL << (iLevel-1));
		CCeDisplayElement* pDisplayElement = NULL;
		for (int ii = iStartItem-1; ii >= 0; ii--)
		{
			pDisplayElement = (CCeDisplayElement*)GetItemData(ii);
			if (pDisplayElement->GetWatchElement()->GetLevel() >= iLevel)
			{
				if (pDisplayElement->GetVerticalLineMask() & iMask)
					return;
				pDisplayElement->SetVerticalLineMask(pDisplayElement->GetVerticalLineMask() | iMask);
			}
			else
				return;
		}
	}
}

//*****************************************************************************
CString CCeWatchList::FormatText(CDC* pDC, const CString& Text, int MaxWidth)
//*****************************************************************************
{
	int dx = pDC->GetTextExtent(Text).cx;
	if (dx < MaxWidth) // do nothing if there is enough space
		return Text;
	
	int dxMin = pDC->GetTextExtent(".", 1).cx;

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	MaxWidth -= 3 * dxMin;
	int nMinChars = MaxWidth / tm.tmMaxCharWidth;
	int nMaxChars = MaxWidth / dxMin;
	nMaxChars = min(nMaxChars, Text.GetLength());
	if (nMaxChars > 0) 
	{
		int w = pDC->GetTextExtent(Text.Left(nMinChars)).cx;
		LPCSTR pText = (LPCSTR)Text;
		for (int ii = nMinChars; ii < nMaxChars; ii++)
		{
			w += pDC->GetTextExtent(pText+ii, 1).cx;
			if (w > MaxWidth)
			{
				return Text.Left(ii-1) + "...";
			}
		}
	}
	return CString("");
}