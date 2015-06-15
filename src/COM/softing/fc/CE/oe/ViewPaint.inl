#ifndef __VIEWPAINT_INL__
#define __VIEWPAINT_INL__
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

/////////////////////////////////////////////////////////////////////
//
// ViewPaint.inl
//
//
// Description: Routines to render the current view on screen
//
//



#include "SECEdit.h"


//@doc SECEditCore
//@mfunc Virtual function called to draw bookmarks, etc.
//              Override to draw custom marks.  The LINEDESC structure
//              #defines eight flags that can be set for your implementation.
//@rdesc void 
//@parm CPaintDC* dc The current paint DC.
//@parm  RECT* rect The rectangle to draw into.
//@parm  PLINEDESC pLine A line description structure, there are several fields you can use here to help you customize your drawing.

#ifdef ORIGINAL_VERSION
template <class BC>
void SECEditCore<BC>::DrawMarks(CPaintDC* dc, RECT* rect, PLINEDESC pLine)
#else
template <class BC>
void SECEditCore<BC>::DrawMarks(CDC* dc, RECT* rect, PLINEDESC pLine)
#endif
{
	CRect temprect;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	if (pLine->fBookMark)
		{
#ifdef ORIGINAL_VERSION
		temprect.top = rect->top;
		temprect.left = 2;
		temprect.right = ((m_iMarkSpace/2)*2)-2;
		temprect.top += (((rect->bottom - rect->top) / 2) - ((temprect.right - temprect.left) / 2));
		temprect.bottom = temprect.top + (temprect.right - temprect.left);
		CBrush Brush(RGB(0,0,255));  // blue
		CBrush* pOldBrush = dc->SelectObject(&Brush);
		dc->Ellipse(&temprect);
		dc->SelectObject(pOldBrush);
		dc->SetBkColor(pEdit->m_crBackground);
#else
		CBrush brush (RGB (240, 240, 255));
		CBrush *pOldBrush;

		temprect.top = rect->top + 1;
		temprect.left = 0;
		temprect.right = m_iMarkSpace - 1;
		temprect.bottom = rect->bottom - 1;

		pOldBrush = dc->SelectObject (&brush);
		dc->RoundRect (&temprect, CPoint (5, 5));
		dc->SelectObject (pOldBrush);
		dc->SetBkColor (pEdit->m_crBackground);
#endif
		}
}


//@doc SECEditCore
//@mfunc Draws the current Objective Edit view on the screen DC
//@rdesc int 0 on success, error code on failure.

#ifdef ORIGINAL_VERSION


template <class BC>
int SECEditCore<BC>::Paint(void)
   {
	USES_CONVERSION_T;

	CPaintDC dc(this); // device context for painting
	// Use GetActiveDoc for SECEditCtrl
	CDocument* pDoc = GetActiveDoc();
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   POINT ptBlock;
   int Y;
   PLINEDESC lpLine;
   PLINEDESC lpNextLine;
   PLINEDESC pstTempLine;
   int iLength;
   BOOL bBlock;
   RECT rect;
   UINT wMaxChars;
   int bottom;
   int top;
   UINT uOptions;
   RECT temprect;
	char TextLine[OE_MAXLINELENGTH+1];

   uOptions = ETO_OPAQUE;

   lpLine = m_pTopLine;
   if (!lpLine)
      return 0;
      
   /* is a block defined?  */   
   if (m_BlockStart.pLine == NULL)
      bBlock = FALSE;
   else
      bBlock = TRUE;

   CFont* oldfont = dc.SelectObject(&m_font);

	dc.SetBkColor(pEdit->m_crBackground);
   dc.SetTextColor(pEdit->m_crText);

   pstTempLine = lpLine;

   Y=0;
   rect = m_rect;
   wMaxChars = (rect.right/m_iCharWidth)-1;

   rect.left += m_iMarkSpace;

   bottom = dc.m_ps.rcPaint.bottom;
   top    = dc.m_ps.rcPaint.top;

   while (Y < bottom)
      {
      if ((Y+m_iLineHeight) > top)
         {
         if (lpLine)
            iLength = PaintExpandTabs(pstTempLine->pText,TextLine,pstTempLine->iTxtSize,pEdit->m_iTabSize,pEdit->m_iLineEndLength);
         else
            iLength = 0;

         if (iLength)
            {
            /* (maybe) adjust for OE_CR/OE_LF */
				char cFirst,cSecond;
				cFirst = TextLine[iLength-pEdit->m_iLineEndLength];
				cSecond = pEdit->m_szLineEnd[0];
            if (cFirst == cSecond)
               iLength -= pEdit->m_iLineEndLength;
            /* adjust for leftedge */
            iLength -= m_iLeftEdge;
            }
         
         rect.top = rect.bottom = Y;
         rect.bottom += m_iLineHeight;

			// do an empty fullwidth rect
         temprect.top = rect.top;
         temprect.bottom = rect.bottom;
			temprect.left = 0;
			temprect.right = m_iMarkSpace;

			// Draw the Gutter Area
			COLORREF clrOldText = dc.GetTextColor();
			COLORREF clrGutter = GetGutterColor(pEdit->m_crBackground);
			
			dc.SetTextColor(clrGutter);
			dc.FillRect(&temprect, &m_brGutter);
			dc.SetTextColor(clrOldText);


			if (pstTempLine)
				DrawMarks(&dc,&temprect,pstTempLine);

			temprect.top = rect.top;
			temprect.bottom = rect.bottom;
			temprect.right = rect.right;

         if (iLength > 0)
            {

				PaintTextOut(pEdit, 
								 lpLine, 				// pointer to the current line
								 TextLine, 				// the full text line, tabs expanded
								 iLength+m_iLeftEdge,// the full length of the text
								 m_iLeftEdge, 			// first character to draw
								 wMaxChars, 			// max chars to draw
								 pEdit->m_bChroma, 	// do it in color?
								 &dc,					 	// screen dc
								 rect.left, 			// textout coordinates
								 rect.top);				//		 "			 "

				// erase the balance of the rectangle, if any
				temprect.left = (m_iCharWidth * iLength) + m_iMarkSpace;
				temprect.right = rect.right;
				if (temprect.right > temprect.left)
					dc.ExtTextOut(temprect.left,temprect.top,uOptions,&temprect,
							        NULL,0,NULL);
            }
         else
				{
				// do an empty fullwidth rect
				temprect.left = m_iMarkSpace;
				temprect.right = rect.right;
				dc.ExtTextOut(temprect.left,temprect.top,uOptions,&temprect,
					        NULL,0,NULL);
				}

         if (lpLine && bBlock)
            {
            if ((pDoc == GetActiveDoc()) && 
					 (pEdit->m_pActiveView == this) && 
					 InBlock(pstTempLine,iLength+m_iLeftEdge,&ptBlock))
               {
					ptBlock.x -= m_iLeftEdge;
					if (ptBlock.x < 0)
						{
						ptBlock.y += ptBlock.x;
						ptBlock.x = 0;
						}
					if (ptBlock.y > 0)
                  dc.PatBlt(ptBlock.x*m_iCharWidth+m_iMarkSpace,Y,
                         ptBlock.y*m_iCharWidth,m_iLineHeight,DSTINVERT);
					//else if (iLength == 0)
               //   dc.PatBlt(m_iMarkSpace,Y,
               //             m_iCharWidth/2,m_iLineHeight,DSTINVERT);
               }
            }
         }
      if (lpLine)
         {
         lpNextLine = pstTempLine->pNext;
         if (lpNextLine)
            lpLine = lpNextLine;
         }
      else
         lpNextLine = NULL;
      Y += m_iLineHeight;
      if (lpLine && !lpNextLine)
         lpLine = lpLine->pNext;
      if (lpLine)
         pstTempLine = lpLine;
		else
			pstTempLine = NULL;
      }

   dc.SelectObject(oldfont);
	return 0;
   }


#else // ORIGINAL_VERSION


template<class BC> int SECEditCore<BC>::Paint ()
{
	CPaintDC dc(this); // device context for painting
	CRect drawArea;

	drawArea = m_rect;
	drawArea.top = dc.m_ps.rcPaint.top;
	drawArea.bottom = dc.m_ps.rcPaint.bottom;

	return (InternalPaint (&dc, drawArea, false));
}


template<class BC> int SECEditCore<BC>::InternalPaint (CDC *pDC, CRect drawArea, bool bIsBlank)
{
	USES_CONVERSION_T;

	// Use GetActiveDoc for SECEditCtrl
	CDocument* pDoc = GetActiveDoc();
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	POINT ptBlock;
	int Y;
	PLINEDESC lpLine;
	PLINEDESC lpNextLine;
	PLINEDESC pstTempLine;
	int iLength;
	BOOL bBlock;
	RECT rect;
	UINT wMaxChars;
	int bottom;
	int top;
	UINT uOptions;
	RECT temprect;
	char TextLine[OE_MAXLINELENGTH+1];

	uOptions = ETO_OPAQUE;

	lpLine = m_pTopLine;
	if (!lpLine)
		return 0;
      
	/* is a block defined?  */   
	if (m_BlockStart.pLine == NULL)
		bBlock = FALSE;
	else
		bBlock = TRUE;

	CFont* oldfont = pDC->SelectObject(&m_font);

	pDC->SetBkColor(pEdit->m_crBackground);
	pDC->SetTextColor(pEdit->m_crText);

	pstTempLine = lpLine;

	Y=0;
	rect = drawArea;
	wMaxChars = (rect.right/m_iCharWidth)-1;

	rect.left += m_iMarkSpace;

	bottom = rect.bottom;
	top    = rect.top;

	while (Y < bottom)
	{
		if ((Y+m_iLineHeight) > top)
		{
            COLORREF crBackground;

            if (!OverrideBackgroundColor (lpLine, crBackground))
            {
                crBackground = pEdit->m_crBackground;
            };
	        pDC->SetBkColor(crBackground);

			if (lpLine)
				iLength = PaintExpandTabs(pstTempLine->pText,TextLine,pstTempLine->iTxtSize,pEdit->m_iTabSize,pEdit->m_iLineEndLength);
			else
				iLength = 0;

			if (iLength)
			{
				/* (maybe) adjust for OE_CR/OE_LF */
				char cFirst,cSecond;
				cFirst = TextLine[iLength-pEdit->m_iLineEndLength];
				cSecond = pEdit->m_szLineEnd[0];
				if (cFirst == cSecond)
					iLength -= pEdit->m_iLineEndLength;
				/* adjust for leftedge */
				iLength -= m_iLeftEdge;
			}
     
			rect.top = rect.bottom = Y;
			rect.bottom += m_iLineHeight;

			// do an empty fullwidth rect
			temprect.top = rect.top;
			temprect.bottom = rect.bottom;
			temprect.left = 0;
			temprect.right = m_iMarkSpace;

			// Draw the Gutter Area
			COLORREF clrOldText = pDC->GetTextColor();
			COLORREF clrGutter = GetGutterColor(pEdit->m_crBackground);
			
			pDC->SetTextColor(clrGutter);
			pDC->FillRect(&temprect, &m_brGutter);
			pDC->SetTextColor(clrOldText);

			if (pstTempLine)
				DrawMarks(pDC,&temprect,pstTempLine);

			temprect.top = rect.top;
			temprect.bottom = rect.bottom;
			temprect.right = rect.right;

			if (iLength > 0)
			{
				PaintTextOut(pEdit, 
				             lpLine, 				// pointer to the current line
				             TextLine, 				// the full text line, tabs expanded
				             iLength+m_iLeftEdge,// the full length of the text
				             m_iLeftEdge, 			// first character to draw
				             wMaxChars, 			// max chars to draw
				             pEdit->m_bChroma, 	// do it in color?
				             pDC,					 	// screen dc
				             rect.left, 			// textout coordinates
				             rect.top);				//		 "			 "

				// erase the balance of the rectangle, if any
				if (!bIsBlank)
				{
					temprect.left = (m_iCharWidth * iLength) + m_iMarkSpace;
					temprect.right = rect.right;
					if (temprect.right > temprect.left)
						pDC->ExtTextOut(temprect.left,temprect.top,uOptions,&temprect,
						                NULL,0,NULL);
				};
			}
			else
			{
				// do an empty fullwidth rect
				if (!bIsBlank)
				{
					temprect.left = m_iMarkSpace;
					temprect.right = rect.right;
					pDC->ExtTextOut(temprect.left,temprect.top,uOptions,&temprect,
					                NULL,0,NULL);
				};
			}

			if (lpLine && bBlock)
			{
				if ((pDoc == GetActiveDoc()) && 
				    (pEdit->m_pActiveView == this) && 
				    InBlock(pstTempLine,iLength+m_iLeftEdge,&ptBlock))
				{
					ptBlock.x -= m_iLeftEdge;
					if (ptBlock.x < 0)
					{
						ptBlock.y += ptBlock.x;
						ptBlock.x = 0;
					}
					if (ptBlock.y > 0)
						pDC->PatBlt(ptBlock.x*m_iCharWidth+m_iMarkSpace,Y,
					                ptBlock.y*m_iCharWidth,m_iLineHeight,DSTINVERT);
					//else if (iLength == 0)
					//   pDC->PatBlt(m_iMarkSpace,Y,
					//               m_iCharWidth/2,m_iLineHeight,DSTINVERT);
				}
			}
		}
		if (lpLine)
		{
			lpNextLine = pstTempLine->pNext;
			if (lpNextLine)
				lpLine = lpNextLine;
		}
		else
			lpNextLine = NULL;
		Y += m_iLineHeight;
		if (lpLine && !lpNextLine)
			lpLine = lpLine->pNext;
		if (lpLine)
			pstTempLine = lpLine;
		else
			pstTempLine = NULL;
	}

	SubClassDraw (pDC, drawArea);

	pDC->SelectObject(oldfont);
	return 0;
}


template<class BC> void SECEditCore<BC>::SubClassDraw (CDC *pDC, CRect drawArea)
{
}

template<class BC> bool SECEditCore<BC>::OverrideBackgroundColor (PLINEDESC pLine, COLORREF &backgroundColor)
{
    return (false);
}

template<class BC> void SECEditCore<BC>::GetSubclassFlags (PLINEDESC pLine, char *pchFlags)
{
}

template<class BC> bool SECEditCore<BC>::OverrideTextBackground (char chFlag, COLORREF &backgroundColor)
{
    return (false);
}

template<class BC> bool SECEditCore<BC>::OverrideTextForeground (char chFlag, COLORREF &foregroundColor)
{
    return (false);
}

#endif // ORIGINAL_VERSION


//@doc SECEditCore
//@mfunc Internal function that determine if pstLine is in a block.  If it is, also
//              update the passed in POINT structure to return the beginning
//              column in ptBlock.x and the width in columns in ptBlock.y
//@rdesc BOOL Is it a block?
//@parm PLINEDESC pstLine The line structure to analyze.
//@parm  int iLength Length of the line.
//@parm  POINT *ptBlock Pointer to the block
template <class BC>
 BOOL SECEditCore<BC>::InBlock(PLINEDESC pstLine, int iLength, POINT *ptBlock)
             				
   {
   PLINEDESC pLine;
   PLINEDESC TheOther;

   if (!m_BlockStart.pLine || !m_BlockEnd.pLine)
      return FALSE;

   pLine = pstLine;

   if (m_iBlockType == OE_COLUMN)
      {
      if ((pLine == m_BlockStart.pLine) ||
          (pLine == m_BlockEnd.pLine))
         {
         ptBlock->x = min(m_BlockStart.iOffset,m_BlockEnd.iOffset);
         ptBlock->y = max(m_BlockStart.iOffset,m_BlockEnd.iOffset) - ptBlock->x;
         return TRUE;
         }
      else
         {
         // look backwards for BlockStart or BlockEnd
         TheOther = NULL;
         while (pLine)
            {
            if (!TheOther)  // haven't found either yet
               {
               if (pLine == m_BlockEnd.pLine)
                  TheOther = m_BlockStart.pLine;
               else if (pLine == m_BlockStart.pLine)
                  TheOther = m_BlockEnd.pLine;
               }
            if (TheOther) // we've found one, look for TheOther
               if (pLine == TheOther)  // found both, we're not in
                  return FALSE;
            pLine = pLine->pPrev;
            }
         }

      if (!pLine && TheOther)  // we were in between BlockStart and BlockEnd
         {
         ptBlock->x = min(m_BlockStart.iOffset,m_BlockEnd.iOffset);
         ptBlock->y = max(m_BlockStart.iOffset,m_BlockEnd.iOffset) - ptBlock->x;
         return TRUE;
         }
      }
   else
      {
      if (pLine == m_BlockStart.pLine)
         {
         ptBlock->x = min(m_BlockStart.iOffset,iLength);
         if (pLine == m_BlockEnd.pLine)
            ptBlock->y = min(m_BlockEnd.iOffset,iLength)  - ptBlock->x;
         else
            ptBlock->y = iLength - ptBlock->x;
         return TRUE;
         }
      else if (pLine == m_BlockEnd.pLine)
         {
         ptBlock->x = 0;
         ptBlock->y = min(m_BlockEnd.iOffset,iLength);
         return TRUE;
         }
      else
         {
         // look backwards for BlockStart or BlockEnd
         while (pLine)
            {
            if (pLine == m_BlockEnd.pLine)
               return FALSE;
            else if (pLine == m_BlockStart.pLine)
               {
               // the whole line is in
               ptBlock->x = 0;
               ptBlock->y = iLength;
               return TRUE;
               }
            pLine = pLine->pPrev;
            }
         return FALSE;
         }
      }
	return FALSE;
   }

//@doc SECEditCore
//@mfunc Internal function used to UnPatBlt the screen's selection
//@rdesc void 
//@parm void
template <class BC>
void SECEditCore<BC>::ClearBlockFromScreen(void)
   {
	CClientDC dc(this); // device context for painting
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   POINT ptBlock;
   int Y;
   PLINEDESC lpLine;
   LPSTR pTextLine;
   int iLength;
   RECT rect;

   if (m_iBlockType == OE_COLUMN)
      {
      Invalidate(FALSE);
      return;
      }

   lpLine = m_pTopLine;
   if (!lpLine)
      return;

   if (m_BlockStart.pLine == NULL)
      return;

   pTextLine = lpLine->pText;
   rect = m_rect;
   Y=0;

   while (Y <= rect.bottom)
      {
      if ((Y+m_iLineHeight) >= rect.top)
         {
         if (pTextLine)
            iLength = lpLine->iTxtSize;
         else
            iLength = 0;
         if (iLength)
            {
            /* (maybe) adjust for OE_CR/OE_LF */
            if (pTextLine[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
               iLength -= pEdit->m_iLineEndLength;
            // adjust for tabs
            iLength = pEdit->GetTabbedOffset(lpLine,iLength);
            if (InBlock(lpLine,iLength,&ptBlock))
               {
	            if (iLength > 0)
                  {
						ptBlock.x -= m_iLeftEdge;
						if (ptBlock.x < 0)
							{
							ptBlock.y += ptBlock.x;
							ptBlock.x = 0;
							}
						if (ptBlock.y > 0)
							dc.PatBlt(ptBlock.x*m_iCharWidth+m_iMarkSpace,Y,
                         ptBlock.y*m_iCharWidth,m_iLineHeight,DSTINVERT);
                  }
					//else
					//	dc.PatBlt(m_iMarkSpace,Y,
               //             m_iCharWidth/2,m_iLineHeight,DSTINVERT);
               }
            }
         }
      Y += m_iLineHeight;
      lpLine = lpLine->pNext;
      if (lpLine)
         {
         pTextLine = lpLine->pText;
         }
      else
         break;
      }

   }

// expands tabbed text in lpText into lpWorkBuffer, returns new expanded length


//@doc SECEditCore
//@mfunc Internal function that expands tabbed text in lpText into lpWorkBuffer
//@rdesc int Length of the lpWorkBuffer
//@parm LPTSTR lpText
//@parm  LPTSTR lpWorkBuffer
//@parm  int iTextSize
//@parm  int iTabSize
//@parm  int iLineEndLength
template <class BC>
int SECEditCore<BC>::PaintExpandTabs(LPSTR lpText, LPSTR lpWorkBuffer, int iTextSize, int iTabSize, int iLineEndLength)
   {
   LPSTR lpNext;
   LPSTR lpStart;
   int iNewTextSize,iInsert,iColNo,iLength;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (!iTextSize)
      return iTextSize;

	iLength = iTextSize;

   /* quick check for tabs, if any */
   lpNext = (LPSTR)memchr(lpText,9,iTextSize);

   if (!lpNext) /* no tabs, no work */
      {
      memcpy(lpWorkBuffer,lpText,iTextSize);
      lpWorkBuffer[iTextSize] = 0;
      return iTextSize;
      }

   lpNext = lpText;
     
   lpStart = lpWorkBuffer;

   iNewTextSize = iColNo = 0;

   while (iTextSize > 0)
      {
      switch (*lpNext)
         {
         case 0x09:
            lpNext++;
            iTextSize--;
            iInsert = iTabSize - (iColNo % iTabSize);
            if (!iInsert)
               iInsert = iTabSize;

				
				if (pEdit->m_bShowTabs)
					{
					// put a funny one in first
					iColNo++;
					iNewTextSize++;
					*lpWorkBuffer++ = '»';
					iInsert--;
					}
            for (;iInsert;iInsert--)
               {
               iColNo++;
               iNewTextSize++;
               *lpWorkBuffer++ = ' ';
               }
            break;

         case 0x0D:
         case 0x0A:
            /* skip OE_CR/LFs and reset our ColNo indicator */
            {
            int i;
            for (i=0;i<iLineEndLength;i++)
               *lpWorkBuffer++ = *lpNext++;
            iColNo = 0;
            iTextSize -= iLineEndLength;
            iNewTextSize += iLineEndLength;
            break;
            }

         default:
            *lpWorkBuffer++ = *lpNext++;
            iNewTextSize++;
            iColNo++;
            iTextSize--;

         }
      }

   lpStart[iNewTextSize] = 0;

   return iNewTextSize;
   }
#if 0
template <class BC>
int SECEditCore<BC>::GetWidth(CDC* dc, LPCTSTR lpText, int nCount)
	{
	CSize size = dc->GetTextExtent(lpText,nCount);
	return size.cx;
	}
#endif
#define OE_REGULAR 2
#define OE_KEYWORD 4	
#define OE_QUOTE   8
#define OE_COMMENT 16
#ifndef ORIGINAL_VERSION
#define OE_SFCFORCED 64
#define OE_SFCBLOCKED 128
#endif

template <class BC>
BOOL SECEditCore<BC>::PaintTextOut( SECEdit* pEdit, 
										   PLINEDESC lpLine, 	// pointer to the current line
											LPSTR str, 			// the full text line, tabs expanded
											int nLength,			// the full length of the text
											int nIndex, 			// first character to draw
											int nCount, 			// max characters to draw
											BOOL bChroma, 			// do it in color
											CDC* dc, 					// screen dc
											int x, 					// textout coordinates
											int y)					//		 "			 "
{
	USES_CONVERSION_T;

#ifdef ORIGINAL_VERSION
	if (!bChroma)
		return dc->TextOut( x, y, A2T(&str[nIndex]), min(nLength,nCount) );
#else
	if (!bChroma)
		return dc->TextOut( x, y, A2T(&str[nIndex]), min(nLength - nIndex,nCount) );
#endif

	int iStart,iEnd;
	char crTemplate[OE_MAXLINELENGTH+1];
	memset(crTemplate,OE_REGULAR,sizeof(crTemplate));
#ifndef ORIGINAL_VERSION
    char subclassFlags[OE_MAXLINELENGTH+1];
    memset(subclassFlags, 0, sizeof (subclassFlags));
    GetSubclassFlags (lpLine, subclassFlags);
#endif

	LPSTR pText = str;

   // keywords
   while (*pText && pEdit->KeyWord(&pText,&iStart,&iEnd))
      {
		// keywords get painted immediately because we
		// might change the value of m_crKeyWord from
		// word to word
      int iWordStart = pText - str;
      int iWordEnd = iWordStart + (iEnd-iStart);
#ifdef ORIGINAL_VERSION
		dc->SetTextColor(pEdit->m_crKeyWord);
#endif
		// find the starting point for the TextOut()
		if (iWordStart < nIndex)
			iWordStart = nIndex;
		if (iWordEnd > iWordStart)
			{
#ifdef ORIGINAL_VERSION
			int n = x + GetWidth(dc, &str[nIndex],iWordStart-nIndex);
			dc->TextOut( n, y, A2T(&str[iWordStart]), iWordEnd-iWordStart );
#endif
			for (;iWordStart!=iWordEnd;iWordStart++)
				crTemplate[iWordStart] |= OE_KEYWORD;
			}
		pText = &(str[iWordEnd]);
      }

   // comments
   int i = iStart = iEnd = 0;
   pText = str;
	int iTempLength = nLength;
	int iType;
	int iNext;
	BOOL bInComment = pEdit->InComment(lpLine, &iType);
   while (iTempLength > 0)
      {
      if (bInComment)
			{
         iNext = 0;
			bInComment = FALSE;
			}
      else
			iNext = pEdit->CommentStart(&pText[iStart],iTempLength,&iType);
                                                                        
      if (iNext >= 0)                                                  
         {
			iStart += iNext;
         iEnd = pEdit->CommentEnd(&pText[iStart],iTempLength-iNext,iType);
			int iCommentStart, iCommentEnd;
			iCommentStart = &pText[iStart] - pText;
			iCommentEnd = iStart+iEnd;
			for (;iCommentStart!=iCommentEnd;iCommentStart++)
				crTemplate[iCommentStart] |= OE_COMMENT;
                                                                        
         iStart += iEnd;
         iTempLength -= (iNext + iEnd);
         }
      else
         iTempLength = 0;
      }

   // quotes
   pText = str;
   iStart = -1;
   i = iEnd = 0;
   char quote;

	while (i < nLength)
		{
		// find the start
		while (nLength > i)
			{
			if ((pText[i] == '"') || (pText[i] == '\''))
				{
				// ignore if in comment
				if (!(crTemplate[i] & OE_COMMENT))
					{
					quote = pText[i];
					iStart = i+1;
					}
				}
			i++;

			if (iStart >= 0)                                                  
				{

				// find the matching end
				while (nLength > i)
					{
					if ((pText[i] == quote) && !(crTemplate[i] & OE_COMMENT))
						{
						iEnd = i++;
						break;
						}
               else
                  // Escape character in ST is '$'
#ifdef ORIGINAL_VERSION
                  if ((pText[i] == '\\') && (!(crTemplate[i] & OE_COMMENT)))
#else
                  if ((pText[i] == '$') && (!(crTemplate[i] & OE_COMMENT)))
#endif
                     i++; //skip next char as treated literally
					i++;
					}

				if (iEnd > iStart)
					{
					for (;iStart!=iEnd;iStart++)
						crTemplate[iStart] |= OE_QUOTE;
					}
				iStart = iEnd = -1;
				}
			}
		}

	// now we're ready to paint
	char szOutText[OE_MAXLINELENGTH+1];
	COLORREF cr;
	pText = str;
	int j = 0;
	int iLast = crTemplate[nIndex];
#ifndef ORIGINAL_VERSION
    char chLastSubclassFlags = subclassFlags[nIndex];
    int iKeywordPos = 0;
    COLORREF crBackground;
    if (!OverrideBackgroundColor (lpLine, crBackground))
    {
        crBackground = pEdit->m_crBackground;
    };
#endif
	int iOutMax = nCount;
	int iOutLength;
	szOutText[0] = 0;
	for (i = nIndex; pText[i] && i < nLength; i++)
		{
		// everytime our color value changes, do the textout
#ifdef ORIGINAL_VERSION
		if (crTemplate[i] != iLast)
#else
        if (crTemplate[i] != iLast || subclassFlags[i] != chLastSubclassFlags)
#endif
			{
			iOutLength = min(j,iOutMax);
#ifndef ORIGINAL_VERSION
            if (!OverrideTextForeground (chLastSubclassFlags, cr))
            {
#endif
			if (iLast & OE_COMMENT)
				cr = pEdit->m_crComment;
			else if (iLast & OE_QUOTE)
				cr = pEdit->m_crQuote;
			else if (iLast & OE_KEYWORD)
				{
#ifdef ORIGINAL_VERSION
				x += GetWidth(dc,szOutText,j);
				iLast = crTemplate[i];
				j = 0;
				// keep building our output string
				szOutText[j++] = pText[i];
				szOutText[j] = 0;
				continue;
#else
                LPSTR pszTemp;
                int iStart;
                int iEnd;

                pszTemp = &pText[iKeywordPos];

                pEdit->KeyWord (&pszTemp,&iStart,&iEnd);
                cr = pEdit->m_crKeyWord;
#endif
				}
			else
				cr = pEdit->m_crText;
#ifndef ORIGINAL_VERSION
            };
#endif

		    dc->SetTextColor(cr);
#ifndef ORIGINAL_VERSION
            {
                COLORREF crBack;
                if (OverrideTextBackground (chLastSubclassFlags, crBack))
                {
                    dc->SetBkColor (crBack);
                }
                else
                {
                    dc->SetBkColor (crBackground);
                };
            };
#endif
			dc->TextOut( x, y, A2T(szOutText), iOutLength );
			x += GetWidth(dc,szOutText,j);
			iLast = crTemplate[i];
#ifndef ORIGINAL_VERSION
            chLastSubclassFlags = subclassFlags[i];
            if (crTemplate[i] != iLast &&
                !(crTemplate[i] & OE_COMMENT) &&
                !(crTemplate[i] & OE_QUOTE) &&
                crTemplate[i] & OE_KEYWORD)
            {
                iKeywordPos = i;
            };
#endif
			j = 0;
			}

		// keep building our output string
		szOutText[j++] = pText[i];
		szOutText[j] = 0;
		}

	// do the final piece
	if (szOutText[0])
		{
#ifndef ORIGINAL_VERSION
        if (!OverrideTextForeground (chLastSubclassFlags, cr))
        {
#endif
		iOutLength = min(j,iOutMax);
		if (iLast & OE_COMMENT)
			cr = pEdit->m_crComment;
		else if (iLast & OE_QUOTE)
			cr = pEdit->m_crQuote;
		else if (iLast & OE_KEYWORD)
			cr = pEdit->m_crKeyWord;
		else
			cr = pEdit->m_crText;
#ifndef ORIGINAL_VERSION
            };
#endif

		dc->SetTextColor(cr);
#ifndef ORIGINAL_VERSION
        {
            COLORREF crBack;

            if (OverrideTextBackground (chLastSubclassFlags, crBack))
            {
                dc->SetBkColor (cr);
            }
            else
            {
                dc->SetBkColor (crBackground);
            };
        };
#endif
		dc->TextOut( x, y, A2T(szOutText), iOutLength );
		}

	// reset to default text color
	dc->SetTextColor(pEdit->m_crText);
#ifndef ORIGINAL_VERSION
    dc->SetBkColor (crBackground);
#endif

	return TRUE;
}

#endif //__VIEWPAINT_INL__