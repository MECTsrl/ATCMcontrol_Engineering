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
//@mfunc Internal function that breaks pTextPos->pLine at pTextPos->iOffset.  Copy any
//              trailing text to a new line and link it in
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  BOOL bSaveUndoRecord

int SECEdit::SplitLine(PTEXTPOS pTextPos, BOOL bSaveUndoRecord)
   {
   PLINEDESC pLine;
   PLINEDESC pNewLine;
   int iSpot,iLength;
   UNDORECORD UndoRecord;
	ASSERT(pTextPos);

   if (m_bReadOnly)
      return IDS_OE_READONLY;

   pLine = pTextPos->pLine;
   iLength = pLine->iTxtSize;
   if (iLength && (pLine->pText[iLength-m_iLineEndLength] == m_szLineEnd[0]))
      iLength -= m_iLineEndLength;

   // if past the end of the line, just link in a new line
   if (pTextPos->iOffset > iLength)
      {
      pNewLine = MakeNewLine(32);
      strcat(pNewLine->pText,m_szLineEnd);
      pNewLine->iTxtSize = m_iLineEndLength;
      ListInsertAfter(pNewLine,pTextPos->pLine);
      }
   else
      {
      pNewLine = MakeNewLine(pLine->iTxtSize-pTextPos->iOffset+m_iLineEndLength);
      iSpot = pTextPos->iOffset;
      memcpy(pNewLine->pText,&pLine->pText[iSpot],pLine->iTxtSize-pTextPos->iOffset);
      pNewLine->iTxtSize = pLine->iTxtSize-pTextPos->iOffset;
      pLine->pText[iSpot] = 0;
      strcat(pLine->pText,m_szLineEnd);
      pLine->iTxtSize = iSpot+m_iLineEndLength;
      ListInsertAfter(pNewLine,pTextPos->pLine);
		CountLines(pNewLine);
      }

   if (bSaveUndoRecord)
      {
      UndoRecord.iRow = GetLineNo(pLine);
      UndoRecord.iCol = pTextPos->iOffset;
      UndoRecord.iUndoType = OE_UNDO_NEWLINE;
      SaveUndoRecord(&UndoRecord,0L);
      }

   SetModified(TRUE);
   return 0;
   }
