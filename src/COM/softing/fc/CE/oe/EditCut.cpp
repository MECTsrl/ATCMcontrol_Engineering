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
// EditCut.cpp
//
//
// Description: Low level support for cutting a stream of text from
//              the linked list.
//

#include "stdafx.h"
#include "secedit.h"


//
// cuts from pStartPos to pEndPos.  Never deletes pStartPos->pLine
//
// Returns:      0 on success
//               errorcode on failure


//@doc SECEdit
//@mfunc Internal function. Cuts the text from pStartPos to pEndPos.  Never deletes
//              pStartPos->pLine.
//@rdesc int 
//@parm PTEXTPOS pStartPos
//@parm  PTEXTPOS pEndPos
//@parm  BOOL bSaveUndoRecord
int SECEdit::Cut(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord)
   {
   int iRval;
   UNDORECORD UndoRecord;
   LPSTR lpText;
   PLINEDESC pFirstDeadLine,pNextDeadLine,pLastDeadLine;
   TEXTPOS TempPos;
	ASSERT(pStartPos);
	ASSERT(pEndPos);

   iRval = 0;

   if (m_bReadOnly)
      return IDS_OE_READONLY;

   if ((pStartPos->pLine == pEndPos->pLine) && 
		 (pStartPos->iOffset == pEndPos->iOffset))
      return IDS_OE_NOSELECTION;

   if ((pStartPos->pLine == NULL) || (pEndPos->pLine == NULL))
      return IDS_OE_NOSELECTION;

   // save the undo record
   if (bSaveUndoRecord)
      {
      UndoRecord.iRow    = GetLineNo(pStartPos->pLine);
		if (pStartPos->pLine == pEndPos->pLine)
			UndoRecord.iEndRow = UndoRecord.iRow;
		else
			UndoRecord.iEndRow = GetLineNo(pEndPos->pLine);
      UndoRecord.iCol    = pStartPos->iOffset;
      UndoRecord.iEndCol = pEndPos->iOffset;
      UndoRecord.iUndoType = OE_UNDO_CUT;
      lpText = Copy(pStartPos, pEndPos, FALSE);
      }

   if (pStartPos->pLine == pEndPos->pLine)
		{
      iRval = DeleteString(pStartPos, pEndPos->iOffset-pStartPos->iOffset, FALSE);
		}
   else
      {
      // lop the starting line off at iOffset
      if (pStartPos->iOffset < (pStartPos->pLine->iTxtSize-m_iLineEndLength))
         {
         pStartPos->pLine->pText[pStartPos->iOffset] = 0;
         pStartPos->pLine->iTxtSize = pStartPos->iOffset;
         }

      // delete the marked part of the last line
      if (pEndPos->iOffset > pEndPos->pLine->iTxtSize)
			{
			pEndPos->iOffset = pEndPos->pLine->iTxtSize;
         /* (maybe) adjust for OE_CR/OE_LF */
			char cFirst,cSecond;
			cFirst = pEndPos->pLine->pText[pEndPos->pLine->iTxtSize-m_iLineEndLength];
			cSecond = m_szLineEnd[0];
         if (cFirst == cSecond)
				{
	         pEndPos->iOffset -= m_iLineEndLength;
		      UndoRecord.iEndCol -= m_iLineEndLength;
				}
			}

      TempPos.pLine = pEndPos->pLine;
      TempPos.iOffset = 0;
      iRval = DeleteString(&TempPos, pEndPos->iOffset, FALSE);

      // link the two together, and unlink the ones in between
      if ((!iRval) || (iRval == IDS_OE_JOINLINE))
         {
         if (pStartPos->pLine->pNext != pEndPos->pLine)
            {
            pFirstDeadLine = pStartPos->pLine->pNext;
				if (pEndPos->pLine)
					{
               pLastDeadLine = pEndPos->pLine->pPrev;
	            pEndPos->pLine->pPrev = pStartPos->pLine;
					}
				else
					pLastDeadLine = NULL;
            pStartPos->pLine->pNext = pEndPos->pLine;
				if (pLastDeadLine)
               pLastDeadLine->pNext = NULL;
            while (pFirstDeadLine)
               {
               pNextDeadLine = pFirstDeadLine->pNext;
               FreeTextLine(pFirstDeadLine);
               pFirstDeadLine = pNextDeadLine;
               }
            }
         // join the chopped start with the chopped end
         iRval = JoinLines(pStartPos, FALSE);
			if (iRval == IDS_OE_EOF)
				iRval = 0;
         }
      }

   if (!iRval)
      {
      if (bSaveUndoRecord)
			{
         SaveUndoRecord(&UndoRecord,lpText);
		   free(lpText);
			}

      // set the changed flag
      SetModified(TRUE);
      }

   return iRval;
   }

//@doc SECEdit
//@mfunc Internal function.  Cuts the column of text from pStartPos to pEndPos, 
//              from column pStartPos->iOffset to pEndPos->iOffset.
//@rdesc int 
//@parm PTEXTPOS pStartPos
//@parm  PTEXTPOS pEndPos
//@parm  BOOL bSaveUndoRecord
int SECEdit::ColumnCut(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord)
   {
   int iRval;
   UNDORECORD UndoRecord;
   LPSTR lpText;
   PLINEDESC pLine;
   TEXTPOS TextPos;
   TEXTPOS StartPos,EndPos;

   if (m_bReadOnly)
      return IDS_OE_READONLY;

   // column blocks may not be in order...
   if (!InOrder(pStartPos,pEndPos))
      {
      StartPos = *pEndPos;
      EndPos = *pStartPos;
      }
   else
      {
      StartPos = *pStartPos;
      EndPos = *pEndPos;
      }

   iRval = 0;

	UndoRecord.iCol    = StartPos.iOffset;
	UndoRecord.iEndCol = EndPos.iOffset;

	if (bSaveUndoRecord)
		{
		UndoRecord.iRow    = GetLineNo(StartPos.pLine);
		UndoRecord.iEndRow = GetLineNo(EndPos.pLine);
		UndoRecord.iUndoType = OE_UNDO_COLUMNCUT;
		lpText = ColumnCopy(&StartPos, &EndPos, FALSE);
		}

	int iTabbedOffset = GetTabbedOffset(StartPos.pLine,StartPos.iOffset);
	int iCount = UndoRecord.iEndCol - UndoRecord.iCol;
   pLine = StartPos.pLine;
   while (pLine)
      {
      TextPos.pLine = pLine;
		TextPos.iOffset = GetRealOffset(TextPos.pLine,iTabbedOffset);
      iRval = DeleteString(&TextPos, iCount, FALSE);
		if (pLine == EndPos.pLine)
			break;
		pLine = pLine->pNext;
      }

   if (!iRval && bSaveUndoRecord)
		{
      SaveUndoRecord(&UndoRecord, lpText);
		free(lpText);
		}

   return iRval;
   }
