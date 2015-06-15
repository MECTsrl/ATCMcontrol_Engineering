#ifndef __VIEWJOINLINES_INL__
#define __VIEWJOINLINES_INL__
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
//@mfunc Appends the next line onto the end of the current line.
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate
//@parm  BOOL bSaveUndoRecord
template <class BC>
int SECEditCore<BC>::JoinLines(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   int i;
   RECT rect;
   TEXTPOS TextPos;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   TextPos.pLine = m_pCurLine;
   TextPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
   i = pEdit->JoinLines(&TextPos, bSaveUndoRecord);

   if (!i && bUpdate)
      {
      /* fix our updaterect */
      GetClientRect(&rect);
      rect.top = (m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
      InvalidateRect(&rect,FALSE);
      UpdateWindow();
      CountLines();
      MakeCursorVisible();
	  SECEditHint hint;
	  hint.m_pLine = m_pCurLine;
	  UpdateViews(NULL, IDS_OE_DELETELINE, &hint);
      }

   return i;
   }

#endif //__VIEWJOINLINES_INL__