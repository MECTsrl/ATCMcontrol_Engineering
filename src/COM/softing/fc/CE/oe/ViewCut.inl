#ifndef __VIEWCUT_INL__
#define __VIEWCUT_INL__
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
//@mfunc Cuts the current selection to the clipboard
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?
//@parm  BOOL bSaveUndoRecord Save this information in the undo buffer?

template <class BC>
int SECEditCore<BC>::Cut(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   int iRval;
   TEXTPOS Start,End;

   if (!m_BlockStart.pLine || !m_BlockEnd.pLine)
      return IDS_OE_NOTEXT;

   if ((m_BlockStart.pLine == m_BlockEnd.pLine) && 
		 (m_BlockStart.iOffset == m_BlockEnd.iOffset))
      return IDS_OE_NOTEXT;

   if (m_iBlockType == OE_COLUMN)
      return ColumnCut(bUpdate,bSaveUndoRecord);

	PLINEDESC pSave = m_pCurLine;

   // BlockStart and BlockEnd positions are tabbed,
   // SECEdit.Copy gets the real offset
   Start = m_BlockStart;
   End = m_BlockEnd;
   Start.iOffset = pEdit->GetRealOffset(Start.pLine,Start.iOffset);
   End.iOffset = pEdit->GetRealOffset(End.pLine,End.iOffset);
	BOOL bMultiLine = Start.pLine != End.pLine;
   iRval = pEdit->Cut(&Start,&End,bSaveUndoRecord);

   if (iRval)
      return iRval;

   m_pCurLine = Start.pLine;
   m_iColNo = pEdit->GetTabbedOffset(Start.pLine,Start.iOffset);

	if (m_pCurLine != pSave)
		m_iLineNo = pEdit->GetLineNo(m_pCurLine);

   ClearSelection(FALSE);

   // normally ViewClearSelection does this...
   if (bUpdate)
      ShowCaret();

	if (bMultiLine)
		{
		CheckLines(m_pCurLine);  // might have toasted m_pTopLine
		CountLines();
		}

   if (bUpdate)
      {
		SECEditHint hint;
		hint.m_pLine = m_pCurLine;
		UpdateViews(NULL, IDS_OE_DELETELINE, &hint);
      CountLines();
      MakeCursorVisible();
      InvalidateRect(NULL,FALSE);
      UpdateWindow();
      }
   return iRval;
   }


//@doc SECEditCore
//@mfunc Cuts a column selection to the clipboard
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?
//@parm  BOOL bSaveUndoRecord Save this information in the undo buffer?

template <class BC>
int SECEditCore<BC>::ColumnCut(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   int iRval;

   if (!m_BlockStart.pLine || !m_BlockEnd.pLine)
      return IDS_OE_NOTEXT;

	{
	TEXTPOS Start = m_BlockStart;
	TEXTPOS End = m_BlockEnd;
   Start.iOffset = pEdit->GetRealOffset(Start.pLine,Start.iOffset);
   End.iOffset = pEdit->GetRealOffset(Start.pLine,End.iOffset);
	iRval = pEdit->ColumnCut(&Start,&End,bSaveUndoRecord);
	}

   if (iRval)
      return iRval;

   m_pCurLine = m_BlockStart.pLine;
   m_iColNo = m_BlockStart.iOffset;
   m_iLineNo = pEdit->GetLineNo(m_pCurLine);

   ClearSelection(FALSE);

   // normally ViewClearSelection does this...
   if (bUpdate)
      ShowCaret();

   if (bUpdate)
      {
      CountLines();
      MakeCursorVisible();
      InvalidateRect(NULL,FALSE);
      UpdateWindow();
	  SECEditHint hint;
	  hint.m_pLine = m_pCurLine;
	  UpdateViews(NULL, IDS_OE_DELETELINE, &hint);
      }
   return iRval;
   }

#endif //__VIEWCUT_INL__