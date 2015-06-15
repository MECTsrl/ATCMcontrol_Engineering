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
//@mfunc Internal function. If pTextPos->iOffset is past the end of the line (not
//              counting m_szLineEnd), pad the difference with whitespace
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm BOOL bSaveUndoRecord
int SECEdit::PadWithSpaces(PTEXTPOS pTextPos, BOOL bSaveUndoRecord)
   {
	USES_CONVERSION_T;

   PLINEDESC pLine = pTextPos->pLine;
	int iInsert;
   int iPos = pTextPos->iOffset;
   int iLength = pLine->iTxtSize;
   if (iLength && (pLine->pText[iLength-m_iLineEndLength] == m_szLineEnd[0]))
      iLength -= m_iLineEndLength;
   if (pTextPos->pLine->iTxtSize)
      {
      if (iPos > iLength)
         iInsert = (iPos - iLength);
      else
         iInsert = 0;
      }
   else
      iInsert = iPos;

	if (!iInsert)
		return 0;

   ResizeLine(pTextPos->pLine,pTextPos->iOffset+m_iLineEndLength);

	CString str;
	LPSTR p = T2A(str.GetBuffer(pTextPos->iOffset));
	pTextPos->iOffset -= iInsert;
	if (m_bKeepTabs)
		{
		int iTabbedPos = GetTabbedOffset(pTextPos->pLine,pTextPos->iOffset);
		int iTotal = iTabbedPos + iInsert;
		int iNumTabs = iInsert / m_iTabSize;
		int iNumSpaces;
		if (iNumTabs)
			iNumSpaces = iTotal - (((iTabbedPos / m_iTabSize) + iNumTabs) * m_iTabSize);
		else
			iNumSpaces = iInsert;

		memset(p,9,iNumTabs);
		memset(&p[iNumTabs],' ',iNumSpaces);
		p[iNumTabs+iNumSpaces] = 0;
		iPos = pTextPos->iOffset + iNumTabs + iNumSpaces;
		}
	else
		{
		memset(p,' ',iInsert);
		p[iInsert] = 0;
		iPos = pTextPos->iOffset + iInsert;
		}

   int iMoveSize = iPos - pTextPos->iOffset;

   memmove(&pTextPos->pLine->pText[iPos],						/* to      */
           &pTextPos->pLine->pText[pTextPos->iOffset],   /* from    */
			  pTextPos->pLine->iTxtSize - pTextPos->iOffset);	/* count */
	memcpy(&pTextPos->pLine->pText[pTextPos->iOffset],
			 p,
			 iMoveSize);
   pTextPos->pLine->iTxtSize += iMoveSize;
   if (pTextPos->pLine->pText[pTextPos->pLine->iTxtSize-m_iLineEndLength] != m_szLineEnd[0])
		{
		strcat(pTextPos->pLine->pText,m_szLineEnd);
		pTextPos->pLine->iTxtSize += m_iLineEndLength;
		}
	if (bSaveUndoRecord)
		{
		UNDORECORD UndoRecord;
		UndoRecord.iRow = GetLineNo(pTextPos->pLine);
		UndoRecord.iCol = pTextPos->iOffset;
		UndoRecord.iEndRow = UndoRecord.iRow;
		UndoRecord.iEndCol = iPos;
		UndoRecord.iUndoType = OE_UNDO_PASTE;
		SaveUndoRecord(&UndoRecord, p);
		}
	str.ReleaseBuffer(-1);
	pTextPos->iOffset = iPos;
   return 0;
   }


//@doc SECEdit
//@mfunc Internal function. Append the next line with pTextPos->pLine
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  BOOL bSaveUndoRecord
int SECEdit::JoinLines(PTEXTPOS pTextPos, BOOL bSaveUndoRecord)
   {
   PLINEDESC pLine;
   PLINEDESC pNextLine;
   int iSpot;
   UNDORECORD UndoRecord;

   if (m_bReadOnly)
      return IDS_OE_READONLY;

   pLine = pTextPos->pLine;
   pNextLine = pLine->pNext;

   if (!pNextLine)
      return IDS_OE_EOF;

	if ((GetTabbedOffset(pLine,pLine->iTxtSize) + GetTabbedOffset(pNextLine,pNextLine->iTxtSize)) > OE_MAXLINELENGTH)
		return IDS_OE_LINETOOLONG;

	SetUndoGroup(TRUE);

   PadWithSpaces(pTextPos,bSaveUndoRecord);

   ResizeLine(pLine, pLine->iTxtSize + pNextLine->iTxtSize);
   iSpot = pLine->iTxtSize;
   if (iSpot && (pLine->pText[iSpot-m_iLineEndLength] == m_szLineEnd[0]))
      iSpot -= m_iLineEndLength;
   pLine->pText[iSpot] = 0;
   strcat(pLine->pText,pNextLine->pText);
   pLine->iTxtSize = iSpot+pNextLine->iTxtSize;
   pNextLine = pNextLine->pNext;
   FreeTextLine(pLine->pNext);
   pLine->pNext = pNextLine;
   if (pNextLine)
      pNextLine->pPrev = pLine;
   m_iLinesInFile--;

   SetModified(TRUE);

   if (bSaveUndoRecord)
      {
      UndoRecord.iRow = GetLineNo(pLine);
      UndoRecord.iCol = pTextPos->iOffset;
      UndoRecord.iUndoType = OE_UNDO_JOINLINE;
      SaveUndoRecord(&UndoRecord, 0L);
      }

	SetUndoGroup();

   return 0;
   }
