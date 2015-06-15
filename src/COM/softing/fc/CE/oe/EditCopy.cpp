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
// EditCopy.cpp
//
//
// Description: Low level support for copying selected text from
//              one position to another
//

#include "stdafx.h"
#include "secedit.h"


//@doc SECEdit
//@mfunc Internal function - Copies a stream of text from the linked list to a buffer
//@rdesc LPTSTR pointer to buffer
//@parm PTEXTPOS pStartPos
//@parm  PTEXTPOS pEndPos
//@parm  BOOL bSaveUndoRecord
LPSTR SECEdit::Copy(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord)
   {
   int iMemSize;
   int iPos;
   int iMax;
   LPSTR lpData;
   PLINEDESC pLine;
	ASSERT(pStartPos);
	ASSERT(pEndPos);
	bSaveUndoRecord; //UNUSEd

   lpData = NULL;

   if ((pStartPos == NULL) || (pEndPos == NULL))
      return NULL;

   // get a count of the bytes of text to be copied 
   if (pStartPos->pLine == pEndPos->pLine)
      iMemSize = pEndPos->iOffset - pStartPos->iOffset;
   else
      {
      // count it line by line 
      pLine = pStartPos->pLine;
      iMemSize = pLine->iTxtSize - pStartPos->iOffset;

      while (pLine != pEndPos->pLine)
         {
         pLine = pLine->pNext;
         if (pLine == pEndPos->pLine)
            iMemSize += pEndPos->iOffset;
         else
            iMemSize += pLine->iTxtSize;
         }
      }

   // allocate memory for the clipboard text 
   lpData = (LPSTR)SECEditAlloc(iMemSize+16);
   if (!lpData)
      return NULL;

   pLine = pStartPos->pLine;
   iPos = 0;
   if (pStartPos->pLine == pEndPos->pLine)
      {
      iMax = pLine->iTxtSize;
      if (pLine->pText[iMax-m_iLineEndLength] == m_szLineEnd[0])
         iMax -= m_iLineEndLength;
      if (pEndPos->iOffset < iMax)
         iMax = pEndPos->iOffset;
		int iCopyLength = max(0,iMax-pStartPos->iOffset);
      memcpy(lpData,&pLine->pText[pStartPos->iOffset],iCopyLength);
      }
   else
      {
      // first line
      if ((pLine->iTxtSize - pStartPos->iOffset) > 0)
         {
         memcpy(lpData,&pLine->pText[pStartPos->iOffset],pLine->iTxtSize-pStartPos->iOffset);
         iPos = pLine->iTxtSize-pStartPos->iOffset;
         }
      // copy the rest of the block text to lpData line by line
      pLine = pLine->pNext;
      while (pLine != pEndPos->pLine)
         {
         memcpy(&lpData[iPos],pLine->pText,pLine->iTxtSize);
         iPos += pLine->iTxtSize;
         pLine = pLine->pNext;
         }
      // copy the last line
      iMax = pLine->iTxtSize;
      if (pLine->pText[iMax-m_iLineEndLength] == m_szLineEnd[0])
         iMax -= m_iLineEndLength;
      if (pEndPos->iOffset < iMax)
         iMax = pEndPos->iOffset;
      memcpy(&lpData[iPos],pLine->pText,iMax);
      }

   // outgoing text always gets set to OE_CRLF
   iMemSize = strlen(lpData);
   if (FindLineEnd(lpData, iMemSize) != OE_CRLF)
      ConvertLineEnds(&lpData,iMemSize,OE_CRLF);
   return lpData;
   }

//@doc SECEdit
//@mfunc Internal function. Copies a column of text from the linked list to a
//              buffer.  
//@rdesc LPTSTR Pointer to buffer.
//@parm PTEXTPOS pStartPos
//@parm  PTEXTPOS pEndPos
//@parm  BOOL bSaveUndoRecord
LPSTR SECEdit::ColumnCopy(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord)
   {
	bSaveUndoRecord;//UNUSED
   int iMemSize;
   int iPos;
   LPSTR lpData;
   LPSTR lpTemp;
   PLINEDESC pLine;
   int iStart,iEnd,iMoveSize,iLength;
   BOOL bDone;
   TEXTPOS StartPos,EndPos;

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

	if (StartPos.iOffset > EndPos.iOffset)
		{
		int iStart = EndPos.iOffset;
		EndPos.iOffset = StartPos.iOffset;
		StartPos.iOffset = iStart;
		}
   lpData = NULL;
	// convert the offsets back to tabbed offsets
	StartPos.iOffset = GetTabbedOffset(StartPos.pLine,StartPos.iOffset);
	EndPos.iOffset = GetTabbedOffset(StartPos.pLine,EndPos.iOffset);
   iStart = StartPos.iOffset;
   iEnd = EndPos.iOffset;
   iMoveSize = iEnd-iStart;
   // get a count of the bytes of text to be copied 
   if (StartPos.pLine == EndPos.pLine)
      iMemSize = iMoveSize+m_iLineEndLength;
   else
      {
      // count it line by line 
      pLine = StartPos.pLine;
      iMemSize = iMoveSize+m_iLineEndLength;

      while (pLine && (pLine != EndPos.pLine))
         {
         pLine = pLine->pNext;
         iMemSize += iMoveSize+m_iLineEndLength;
         }
      }

   // allocate memory for the clipboard text 
   lpData = (LPSTR)SECEditAlloc(iMemSize+1);
   if (!lpData)
      return NULL;

   iPos = 0;
   pLine = StartPos.pLine;
   bDone = FALSE;
   while (!bDone)
      {
      bDone = (pLine == EndPos.pLine);
      lpTemp = ExpandTabs(pLine->pText,pLine->iTxtSize,m_iTabSize, m_iLineEndLength,&iLength);
      if (iLength && (lpTemp[iLength-m_iLineEndLength] == m_szLineEnd[0]))
         iLength -= m_iLineEndLength;
      if (iLength < iEnd)
         {
         // copy to the end of the line
         if (iLength > iStart)
            {
            memcpy(&lpData[iPos],&lpTemp[iStart],abs(iLength-iStart));
            iPos += iLength-iStart;
            // pad with spaces
            memset(&lpData[iPos],' ',iEnd-iLength);
            iPos += iEnd-iLength;
            }
         else
            {
            // pad iMoveSize spaces
            memset(&lpData[iPos],' ',iMoveSize);
            iPos += iMoveSize;
            }
         }
      else
         {
         memcpy(&lpData[iPos],&lpTemp[iStart],iMoveSize);
         iPos += iMoveSize;
         }
      // append the lineend except for the last line
      if (!bDone)
         {
         memcpy(&lpData[iPos],m_szLineEnd,m_iLineEndLength);
         iPos += m_iLineEndLength;
         }
      free(lpTemp);
      pLine = pLine->pNext;
      }

   // outgoing text always gets set to OE_CRLF
   if (FindLineEnd(lpData, iPos) != OE_CRLF)
      ConvertLineEnds(&lpData,iMemSize,OE_CRLF);

   return lpData;
   }
