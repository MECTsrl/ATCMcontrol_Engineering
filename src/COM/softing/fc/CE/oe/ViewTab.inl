#ifndef __VIEWTAB_INL__
#define __VIEWTAB_INL__
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
//@mfunc 
//  If there is no selection insert a tab or the appropriate
//              number of spaces if m_bKeepTabs is FALSE.  If there's a
//              selection, shift the selection one tab stop to the right.
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Indicates if the display should be updated or not.
//@parm  BOOL bSaveUndoRecord Should undo information be saved?
template <class BC>
int SECEditCore<BC>::InsertTab(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
	int iRval;
   int iNextStop;
   int iPos;
	int iNewColNo;
	TEXTPOS TextPos;
	RECT rect;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   
   if (m_bSelecting)
      return ShiftBlock(FALSE, bUpdate, bSaveUndoRecord);

	TextPos.pLine = m_pCurLine;
   TextPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);

   if (pEdit->m_bKeepTabs)
      {
		if ((pEdit->GetTabbedOffset(m_pCurLine,m_pCurLine->iTxtSize) + pEdit->m_iTabSize) >=
			  OE_MAXLINELENGTH)
			  return IDS_OE_LINETOOLONG;
      iRval = pEdit->InsertChar(&TextPos, 9, TRUE, bSaveUndoRecord);
		m_iColNo = ((m_iColNo + pEdit->m_iTabSize) / pEdit->m_iTabSize) * pEdit->m_iTabSize;
      }
	else
		{
		iPos = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
		iNextStop = pEdit->m_iTabSize - (iPos % pEdit->m_iTabSize);
		if (!iNextStop)
			iNextStop = pEdit->m_iTabSize;

		iNewColNo = iNextStop;

		for (;iNextStop-1;iNextStop--)
			{
			iRval = pEdit->InsertChar(&TextPos, ' ', TRUE, bSaveUndoRecord);
			TextPos.iOffset++;
	   	}
      iRval = pEdit->InsertChar(&TextPos, ' ', TRUE, bSaveUndoRecord);
      m_iColNo = pEdit->GetTabbedOffset(TextPos.pLine,TextPos.iOffset+1);
		}

	if (bUpdate)
		{
		rect = m_rect;
      rect.top = (int)(m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
      rect.bottom = rect.top+m_iLineHeight;
   
      InvalidateRect(&rect,FALSE);
      UpdateWindow();
      MakeCursorVisible();
	UpdateViews(this,NULL,NULL);
	}
	return iRval;
   
}


//@doc SECEditCore
//@mfunc  If there is no selection move the caret back one tabstop.
// Otherwise, shift the selection one tab stop to the left.
//
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Indicates that the screen should be updated.
//@parm  BOOL bSaveUndoRecord Should we save Undo information?
template <class BC>
int SECEditCore<BC>::BackTab(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   int iPrevStop;
   int iPos;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (m_bSelecting)
      return ShiftBlock(TRUE, bUpdate, bSaveUndoRecord);

	if (!m_iColNo)
		return 0;

   iPos = m_iColNo;

   iPrevStop =  (iPos / pEdit->m_iTabSize) * pEdit->m_iTabSize;
	if (iPrevStop == iPos)
		iPrevStop -= pEdit->m_iTabSize;

	if (iPrevStop < 0)
		iPrevStop = 0;
	m_iColNo = iPrevStop;
   if (bUpdate)
      MakeCursorVisible();

	return 0;
   }


//@doc SECEditCore
//@mfunc Moves the current selection one tab stop to the left
//              or right.
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bBackTab Move to the left or right?
//@parm  BOOL bUpdate Update the screen?
//@parm  BOOL bSaveUndoRecord Save undo information?
template <class BC>
int SECEditCore<BC>::ShiftBlock(BOOL bBackTab, BOOL bUpdate, BOOL bSaveUndoRecord)
   {
	TEXTPOS TextPos;
	int iRval = 0;
	PLINEDESC pLine,pLastLine;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	pEdit->SetUndoGroup(TRUE);

   if (m_iBlockType == OE_COLUMN)
      return IDS_OE_STREAMONLY;

	// set the block to be whole lines
	m_BlockStart.iOffset = 0;
	if (m_BlockEnd.iOffset != 0)
		{
		if (m_BlockEnd.pLine->pNext)
			{
			m_BlockEnd.pLine = m_BlockEnd.pLine->pNext;
			m_BlockEnd.iOffset = 0;
			}
		else
			m_BlockEnd.iOffset = m_BlockEnd.pLine->iTxtSize;
		}
      
   /* set the current position */
	TextPos.pLine = m_pCurLine = pLine = m_BlockStart.pLine;
	TextPos.iOffset = 0;
	if (m_BlockEnd.iOffset == 0)
	   pLastLine = m_BlockEnd.pLine;
	else
		pLastLine = m_BlockEnd.pLine->pNext;
	m_iColNo = 0;

	m_bSelecting = FALSE;

   while (m_pCurLine != pLastLine)
      {
      if (bBackTab)
         {
         char cChar;
         int i;

         i = 0;
         for (i=0;i<pEdit->m_iTabSize;i++)
            {
            cChar = m_pCurLine->pText[0];
            if (cChar == ' ')
					iRval = pEdit->DeleteChar(&TextPos,bSaveUndoRecord);
            else if (cChar == 9)
               {
					iRval = pEdit->DeleteChar(&TextPos,bSaveUndoRecord);
               break;
               }
				else
					break;
            }
         }
      else
			{
			// InsertTab needs the current position updated
			m_iColNo = 0;
			m_pCurLine = TextPos.pLine;
			InsertTab(FALSE, bSaveUndoRecord);
			//iRval = pEdit->InsertChar(&TextPos, 9, TRUE, bSaveUndoRecord);
			}
      TextPos.pLine = m_pCurLine = m_pCurLine->pNext;
      }

   // reset the block
	m_bSelecting = TRUE;
	if (m_BlockEnd.iOffset != 0)
		m_BlockEnd.iOffset = m_BlockEnd.pLine->iTxtSize;

	m_pCurLine = pLine;
	m_iLineNo = pEdit->GetLineNo(m_pCurLine);
	m_iColNo = 0;

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
      UpdateWindow();
		UpdateViews(this, NULL, NULL);
      }

	pEdit->SetUndoGroup();

   return iRval;
   }
   



#endif //__VIEWTAB_INL__