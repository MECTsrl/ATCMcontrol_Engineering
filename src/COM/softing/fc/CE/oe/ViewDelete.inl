#ifndef __VIEWDELETE_INL__
#define __VIEWDELETE_INL__
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
//@mfunc Deletes the character to the right of the caret position
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?
//@parm  BOOL bSaveUndoRecord Store undo information for this call?

template <class BC>
int SECEditCore<BC>::DeleteChar(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   TEXTPOS TextPos;
   RECT rect;
   int i;
	char cChar;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   
   if (m_BlockStart.pLine)
      return Cut(bUpdate,bSaveUndoRecord);

   if (bUpdate)
      MakeCursorVisible();

   TextPos.pLine = m_pCurLine;
   TextPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);

	cChar = TextPos.pLine->pText[TextPos.iOffset];
   i = pEdit->DeleteChar(&TextPos,bSaveUndoRecord);

   if (i)
      {
      if (i == IDS_OE_JOINLINE)
			{
			i = JoinLines(bUpdate, bSaveUndoRecord);
         return (i == IDS_OE_EOF ? 0 : i);
			}
      else 
         return i;
      }

   if (bUpdate)
      {
      MakeCursorVisible();
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
      MySetCaretPos();
	  SECEditHint hint;
	  hint.m_pLine = m_pCurLine;
	  UpdateViews(this, IDS_OE_DELETELINE, &hint);
      }

   return 0;
   }

#endif //__VIEWDELETE_INL__