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
// EditDelete.cpp
//
//
// Description: low level code to delete a character from a line
//

#include "stdafx.h"
#include "secedit.h"


//@doc SECEdit
//@mfunc Internal function. Deletes the character at pTextPos->iOffset
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  BOOL bSaveUndoRecord
int SECEdit::DeleteChar(PTEXTPOS pTextPos, BOOL bSaveUndoRecord)
   {
   PLINEDESC pLine;
   int iPos,iLength;
   LPSTR szText;
   UNDORECORD UndoRecord;
   ASSERT(pTextPos);

   if (m_bReadOnly)
      return IDS_OE_READONLY;

   pLine = pTextPos->pLine;
   if (!pLine->pText)
      return 0;  // no error, just ignore it

   szText = pLine->pText;
   iLength = pLine->iTxtSize;

   /* (maybe) adjust for OE_CR/OE_LF */
   if (szText[iLength-m_iLineEndLength] == m_szLineEnd[0])
      iLength -= m_iLineEndLength;

   // if past the end of the line, there's nothing to delete
   if (pTextPos->iOffset >= iLength)
      return IDS_OE_JOINLINE;  // but it's not an error

   iPos = pTextPos->iOffset;
   if (bSaveUndoRecord)
      {
      UndoRecord.iRow = GetLineNo(pTextPos->pLine);
      UndoRecord.iCol = iPos;
      UndoRecord.iUndoType = OE_UNDO_DELETE;
		UndoRecord.cInsertChar = szText[iPos];
      SaveUndoRecord(&UndoRecord,NULL);
      }

   SetModified(TRUE);

   memmove(&szText[iPos],
             &szText[iPos+1],
             pLine->iTxtSize-iPos);
   pLine->iTxtSize--;
   szText[pLine->iTxtSize] = 0;
   return 0;
   }


//@doc SECEdit
//@mfunc Internal function.  Deletes iCount characters from the text, starting at
//              pTextPos->iOffset.
//
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  int iCount
//@parm  BOOL bSaveUndoRecord
int SECEdit::DeleteString(PTEXTPOS pTextPos, int iCount, BOOL bSaveUndoRecord)
   {
   int i;
   TEXTPOS TextPos;
   UNDORECORD UndoRecord;
	ASSERT(pTextPos);

   TextPos = *pTextPos;

	// it's confusing to SaveUndoRecord if the OE_UNDO_CUT is just one character...
	if (iCount == 1)
		return (DeleteChar(&TextPos, bSaveUndoRecord));

	LPSTR lpString;
   if (bSaveUndoRecord)
      {
      lpString = (LPSTR)SECEditAlloc(iCount+1);
      memcpy(lpString,&pTextPos->pLine->pText[pTextPos->iOffset],iCount);
      }

   int iRval = 0;
   for (i=0;i<iCount;i++)
      {
      iRval = DeleteChar(&TextPos, FALSE);
      if (iRval && (iRval != IDS_OE_JOINLINE))
         break;
		else
			iRval = 0;
      }

   if (bSaveUndoRecord)
   	{
      if (!iRval)
      	{
	      UndoRecord.iRow = GetLineNo(pTextPos->pLine);
	      UndoRecord.iCol = pTextPos->iOffset;
	      UndoRecord.iEndRow = UndoRecord.iRow;
	      UndoRecord.iEndCol = TextPos.iOffset+iCount;
	      UndoRecord.iUndoType = OE_UNDO_CUT;
	      SaveUndoRecord(&UndoRecord, lpString);
      	}
      free(lpString);
      }

   return iRval;
   }
