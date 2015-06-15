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


#include "stdafx.h"
#include "secedit.h"


//@doc SECEdit
//@mfunc Internal function that pastes lpText into the linked list at pTextPos
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  LPTSTR lpText
//@parm  int iSize
//@parm  BOOL bSaveUndoRecord

int SECEdit::Paste(PTEXTPOS pTextPos, LPSTR lpText, int iSize, BOOL bSaveUndoRecord)
   {
   int iRval,iLineEnd;
   TEXTPOS Bottom;
   PLINEDESC pNewLine, pLastNewLine;
   UNDORECORD UndoRecord;
	LPSTR lpNewText;
	BOOL bAllocated = FALSE;
	ASSERT(pTextPos);
	ASSERT(lpText);

   iRval = 0;

   if (m_bReadOnly)
      return IDS_OE_READONLY;

	// make sure our line end types match
   iLineEnd = FindLineEnd(lpText, iSize);
   if (iLineEnd != (int)m_iLineEndType)
		{
	   lpNewText = (LPSTR)SECEditAlloc(iSize+1);
		strcpy(lpNewText,lpText);
      iSize = ConvertLineEnds(&lpNewText,iSize,m_iLineEndType);
		bAllocated = TRUE;
		}
	else
		lpNewText = lpText;

   // make a linked list of lines from lpNewText
   pNewLine = LineFromTextBuffer(lpNewText);
	CountLines(pNewLine);

	// get all of the undo information
	if (bSaveUndoRecord)
	{
		
		UndoRecord.iRow = GetLineNo(pTextPos->pLine);
		UndoRecord.iCol = pTextPos->iOffset;
		
		// the last line info depends on whether or not there's a linefeed
		PLINEDESC pLastLine = ListGetLast(pNewLine);
		UndoRecord.iEndRow =  GetLineNo(pLastLine) + UndoRecord.iRow - 1;
		int iLength = pLastLine->iTxtSize;
		if (iLength && (pLastLine->pText[iLength-m_iLineEndLength] == m_szLineEnd[0]))
			{
			// record our spot as the beginning of the next line
			UndoRecord.iEndRow++;
			UndoRecord.iEndCol = 0;
			}
		else
			UndoRecord.iEndCol = iLength;
      UndoRecord.iUndoType = OE_UNDO_PASTE;
      }

   pNewLine = ListGetFirst(pNewLine, NULL);

   
	// split the old line at pTextPos->iOffset
   iRval = SplitLine(pTextPos, FALSE);
	if (iRval)
		{
		if (bAllocated)
			free(lpNewText);
		return iRval;
		}

   // save the trailing end
   pLastNewLine = ListGetLast(pNewLine);
   Bottom.pLine = pTextPos->pLine->pNext;
   Bottom.iOffset = pLastNewLine->iTxtSize;
   pNewLine = ListGetFirst(pNewLine, NULL);

   // link in the new text after pTextPos->pLine
	Bottom.pLine->pPrev = pLastNewLine;
	pLastNewLine->pNext = Bottom.pLine;
	pTextPos->pLine->pNext = pNewLine;
	pNewLine->pPrev = pTextPos->pLine;
   //ListInsertAfter(pNewLine,pTextPos->pLine);

   // join the beginning and end
   iRval = JoinLines(pTextPos,FALSE);
	if (iRval)
		{
		if (bAllocated)
			free(lpNewText);
		return iRval;
		}

	Bottom.pLine = Bottom.pLine->pPrev;
	Bottom.iOffset = Bottom.pLine->iTxtSize;
	if (Bottom.iOffset && 
		 (Bottom.pLine->pText[Bottom.iOffset -m_iLineEndLength] != m_szLineEnd[0]))
		{
      iRval = JoinLines(&Bottom,FALSE);
      if (iRval && iRval != IDS_OE_EOF)
			{
			if (bAllocated)
				free(lpNewText);
			return iRval;
			}
		}

	CountLines(Bottom.pLine);

   // save the undo record
   if (bSaveUndoRecord)
		{
// #326 08.12.06 SIS >>
// UndoRecord.iCol and iEndCol are already real offsets
#ifdef _ORIGINAL_VERSION
		UndoRecord.iCol    = GetRealOffset(GoToLineNo(UndoRecord.iRow),UndoRecord.iCol);
		UndoRecord.iEndCol = GetRealOffset(GoToLineNo(UndoRecord.iRow),UndoRecord.iEndCol);
#endif
// #326 08.12.06 SIS <<
      SaveUndoRecord(&UndoRecord, lpNewText);
		}

	if (bAllocated)
		free(lpNewText);
   return 0;
   }

//@doc SECEdit
//@mfunc Internal function that inserts a column block into the linked list.
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  LPTSTR lpText
//@parm  int iSize
//@parm  BOOL bSaveUndoRecord
int SECEdit::ColumnPaste(PTEXTPOS pTextPos, LPSTR lpText, int iSize, BOOL bSaveUndoRecord)
   {
   int iRval,iCount,iLineEnd;
   UNDORECORD UndoRecord;
   LPSTR lpString;
	LPSTR lpNewText;
	BOOL bAllocated = FALSE;
   PLINEDESC pLine;
   PLINEDESC pNewLine;
   PLINEDESC pSaveLine;
   TEXTPOS TextPos;
	ASSERT(pTextPos);
	ASSERT(lpText);

   iRval = 0;

   if (m_bReadOnly)
      return IDS_OE_READONLY;

   SetModified(TRUE);

   // make sure our line end types match
   iLineEnd = FindLineEnd(lpText, iSize);
   if (iLineEnd != (int)m_iLineEndType)
		{
	   lpNewText = (LPSTR)SECEditAlloc(iSize+1);
		strcpy(lpNewText,lpText);
      iSize = ConvertLineEnds(&lpNewText,iSize,m_iLineEndType);
		bAllocated = TRUE;
		}
	else
		lpNewText = lpText;

   // make a linked list of lines from lpNewText
   pNewLine = LineFromTextBuffer(lpNewText);

   iCount = strlen(pNewLine->pText);
   if (pNewLine->pText[iCount-m_iLineEndLength] == m_szLineEnd[0])
         iCount -= m_iLineEndLength;

	if (bSaveUndoRecord)
		{
		UndoRecord.iRow    = GetLineNo(pTextPos->pLine);
		UndoRecord.iEndRow = UndoRecord.iRow + CountLines(pNewLine);
		UndoRecord.iUndoType = OE_UNDO_COLUMNPASTE;
		UndoRecord.iCol    = pTextPos->iOffset;
		UndoRecord.iEndCol = UndoRecord.iCol + iCount;
		}

   lpString = (LPSTR)SECEditAlloc(iCount+1);

	int iTabbedOffset = GetTabbedOffset(pTextPos->pLine,pTextPos->iOffset);
   pLine = pTextPos->pLine;
	PLINEDESC pFirstLine = pLine;
   pSaveLine = pNewLine;
   while (!iRval && pNewLine)
      {
      TextPos.pLine = pLine;
      TextPos.iOffset = GetRealOffset(TextPos.pLine, iTabbedOffset);
		PadWithSpaces(&TextPos,bSaveUndoRecord);
      memcpy(lpString,pNewLine->pText,iCount);
      lpString[iCount] = 0;
      iRval = InsertString(&TextPos, lpString, TRUE, FALSE);
		if (pLine->pNext)
         pLine = pLine->pNext;
		else
			{
			// link in a new line
			TEXTPOS TempPos;
		   TempPos.pLine = pLine;
			TempPos.iOffset = pLine->iTxtSize;
			SplitLine(&TempPos,FALSE);
			pLine = pLine->pNext;
			}
      pNewLine = pNewLine->pNext;
      }

	CountLines(TextPos.pLine);

   // save the undo record
   if (!iRval && bSaveUndoRecord)
		{
// #326 08.12.06 SIS >>
// UndoRecord.iCol is already real offset
#ifdef _ORIGINAL_VERSION
		UndoRecord.iCol    = GetRealOffset(pFirstLine,UndoRecord.iCol);
#endif
// #326 08.12.06 SIS <<
		UndoRecord.iEndCol = UndoRecord.iCol + iCount;
      SaveUndoRecord(&UndoRecord, lpNewText);
		}

   while (pSaveLine)
      {
      pNewLine = pSaveLine->pNext;
      FreeTextLine(pSaveLine);
      pSaveLine = pNewLine;
      }

	free(lpString);
	if (bAllocated)
		free(lpNewText);

   return iRval;
   }
