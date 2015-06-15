#ifndef __VIEWBACKSPACE_INL__
#define __VIEWBACKSPACE_INL__
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


#include "SECEdit.h"


//@doc SECEditCore
//@mfunc Deletes the character preceding the caret position
//@rdesc int 
//@parm BOOL bUpdate
//@parm  BOOL bSaveUndoRecord
template <class BC>
int SECEditCore<BC>::BackSpace(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   TEXTPOS TextPos;
   int i;
   int iPos;
   RECT rect;
   int iLength;
	char cChar;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (m_BlockStart.pLine)
      return Cut(bUpdate,bSaveUndoRecord);

   if (m_iColNo == 0)
      i = IDS_OE_JOINLINE;
   else
      {
      iPos = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
      // if past the end of the line, just move the cursor
      iLength = m_pCurLine->iTxtSize;
      if (m_pCurLine->pText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
         iLength -= pEdit->m_iLineEndLength;
      
		if (iPos > iLength)
			return CursorLeft(bUpdate);

      TextPos.pLine = m_pCurLine;
      TextPos.iOffset = iPos-1;  // delete to the left of the cursor
		cChar = TextPos.pLine->pText[TextPos.iOffset];
      i = pEdit->DeleteChar(&TextPos,bSaveUndoRecord);
      }

   if (i)
      {
      if (i == IDS_OE_JOINLINE)
         {
         if (!UpLine(bUpdate))
				{
				EndOfLine(bUpdate);
				return JoinLines(bUpdate, bSaveUndoRecord);
				}
			else
				return 0;
         }
      else 
         return i;
      }

   m_iColNo = pEdit->GetTabbedOffset(m_pCurLine,iPos-1);

   if (bUpdate)
      {
		if (pEdit->m_bChroma && 
			 ((cChar == '"') || (cChar == '\'') || EndedAComment(pEdit,cChar)))
			{
			// if the char we deleted may have eliminated the beginning or end
			// of a multiline comment or a quote, do a complete redraw
			InvalidateRect(NULL,FALSE);
			}
		else
			{
			GetClientRect(&rect);
			rect.top = (m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
			rect.bottom = rect.top+m_iLineHeight;
			InvalidateRect(&rect,FALSE);
			}
      UpdateWindow();
      MakeCursorVisible();
	  UpdateViews(this, NULL, NULL);
      }

   return 0;
   }


#endif //__VIEWBACKSPACE_INL__