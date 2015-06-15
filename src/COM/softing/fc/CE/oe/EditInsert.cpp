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
// EditInsert.cpp
//
//
// Description: low level routines to insert a character or string
//

#include "stdafx.h"
#include "secedit.h"

//@doc SECEdit
//@mfunc Internal function. Inserts iChar at pTextPos->iOffset
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  int iChar
//@parm  BOOL bInsert
//@parm  BOOL bSaveUndoRecord
int SECEdit::InsertChar(PTEXTPOS pTextPos, int iChar, BOOL bInsert, BOOL bSaveUndoRecord)
   {
	TEXTPOS TextPos = *pTextPos;
   PLINEDESC pLine;
   int iLength;
   int iBufSize;
   int iMoveSize;
   int i;
   UNDORECORD UndoRecord;

   if (m_bReadOnly)
      return IDS_OE_READONLY;
      
   pLine = TextPos.pLine;

   PadWithSpaces(&TextPos, bSaveUndoRecord);

   iLength = pLine->iTxtSize;
   if (iLength && (pLine->pText[iLength-m_iLineEndLength] == m_szLineEnd[0]))
      iLength -= m_iLineEndLength;

   iBufSize = pLine->iTxtSize+1+m_iLineEndLength;

   if (!ResizeLine(pLine,iBufSize))
      return IDS_OE_NOMEMORY2;

   if (bSaveUndoRecord)
      {
      UndoRecord.iRow = GetLineNo(TextPos.pLine);
      UndoRecord.iCol = TextPos.iOffset;
		UndoRecord.cInsertChar = (char)iChar;
      if (bInsert)
         UndoRecord.iUndoType = OE_UNDO_INSERT;
      else
         {
         // save what used to be there, or a space if past end of line
         if (TextPos.iOffset > iLength)
            UndoRecord.cOvertypeChar = ' ';
         else
            UndoRecord.cOvertypeChar = pLine->pText[TextPos.iOffset];

         UndoRecord.iUndoType = OE_UNDO_INS_OVER;
         }
		SaveUndoRecord(&UndoRecord, NULL);
      }

   if (bInsert)  // insert mode or past end of line
      {
      iMoveSize = max(m_iLineEndLength,iLength-TextPos.iOffset+m_iLineEndLength);

      memmove(&pLine->pText[TextPos.iOffset+1],              /* to      */
              &pLine->pText[min(TextPos.iOffset,iLength)],   /* from    */
              iMoveSize);                         /* count   */

      pLine->iTxtSize++;
      }
   else if (!bInsert && (TextPos.iOffset == iLength))  // overwriting m_szLineEnd[0]
      {
      pLine->iTxtSize++;  
      for (i=0;i<m_iLineEndLength;i++)
         pLine->pText[TextPos.iOffset+i+1] = m_szLineEnd[i];
      }
   pLine->pText[TextPos.iOffset] = (char)iChar;
   pLine->pText[pLine->iTxtSize] = 0;
   SetModified(TRUE);
   if (pLine->iTxtSize < m_iLineEndLength)
      {
      strcat(pLine->pText,m_szLineEnd);
      pLine->iTxtSize += m_iLineEndLength;
      }
   else if (pLine->pText[pLine->iTxtSize - m_iLineEndLength] != m_szLineEnd[0])
      {
      for (i=0;i<m_iLineEndLength;i++)
			{
         pLine->pText[pLine->iTxtSize] = m_szLineEnd[i];
			pLine->iTxtSize++;
			}
      }

   return 0;
   }


//@doc SECEdit
//@mfunc Internal function that inserts lpString at pTextPos->iOffset
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  LPTSTR lpString
//@parm  BOOL bInsert
//@parm  BOOL bSaveUndoRecord
int SECEdit::InsertString(PTEXTPOS pTextPos, LPSTR lpString, BOOL bInsert, BOOL bSaveUndoRecord)
   {
   int iRval;
   TEXTPOS TextPos;
	PLINEDESC pLine;
   LPSTR lpTemp;
   UNDORECORD UndoRecord;

   if (m_bReadOnly)
      return IDS_OE_READONLY;
      
   TextPos = *pTextPos;

   pLine = TextPos.pLine;

	PadWithSpaces(&TextPos,bSaveUndoRecord);

   iRval = 0;
   lpTemp = lpString;

   UndoRecord.iCol = TextPos.iOffset;

   while (!iRval && *lpTemp)
      {
      iRval = InsertChar(&TextPos, (int)*lpTemp, bInsert, FALSE);
      lpTemp++;
		TextPos.iOffset++;
      }

   if (!iRval && bSaveUndoRecord)
      {
      UndoRecord.iRow = GetLineNo(pTextPos->pLine);
      UndoRecord.iEndRow = UndoRecord.iRow;
      UndoRecord.iEndCol = TextPos.iOffset;
      UndoRecord.iUndoType = OE_UNDO_PASTE;
      SaveUndoRecord(&UndoRecord, lpString);
      }

   return iRval;
   }
