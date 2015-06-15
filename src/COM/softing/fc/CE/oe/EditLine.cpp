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
// EditLine.cpp
//
//
// Description: LINEDESC routines
//

#include "stdafx.h"
#include "secedit.h"


//@doc SECEdit
//@mfunc Internal function that Counts the lines in the linked list
//
//@rdesc int 
//@parm PLINEDESC pLine
int SECEdit::CountLines(PLINEDESC pLine)
   {
   PLINEDESC pTempLine;
   int i;
	ASSERT(pLine);

   pTempLine = ListGetFirst(pLine, NULL);
   for (i = 0;pTempLine;pTempLine = pTempLine->pNext)
		{
       i++;
		 pTempLine->iLineNo = i;
		}
   return i;
   }

//@doc SECEdit
//@mfunc Internal function.  Frees the text and the LINEDESC
//@rdesc int 
//@parm PLINEDESC pLine
int SECEdit::FreeTextLine(PLINEDESC pLine)
   {
   if (!pLine)
      return -1;
   if (pLine->pText)
      free(pLine->pText);
   free(pLine);
	return 0;
   }


//@doc SECEdit
//@mfunc Internal function that frees the entire linked list.
//@rdesc int 
//@parm PLINEDESC pLine
int SECEdit::FreeLineList(PLINEDESC pLine)
   {
   PLINEDESC pTempLine;
	ASSERT(pLine);

   pLine = ListGetFirst(pLine, NULL);
   while (pLine)
      {
      pTempLine = pLine->pNext;
      FreeTextLine(pLine);
      pLine = pTempLine;
      }
	return 0;
   }


//@doc SECEdit
//@mfunc Internal method that Constructs a LINEDESC and initializes it to the 
//              default settings
//@rdesc PLINEDESC 
//@parm int iTextSize
PLINEDESC SECEdit::MakeNewLine(int iTextSize)
   {
   PLINEDESC pNewLine;
   int iRequest;

   pNewLine = (PLINEDESC)SECEditAlloc(sizeof(LINEDESC));
   if (pNewLine && iTextSize)
      {
      /* round up to the next highest multiple of 32 */
      iRequest = ((iTextSize+31) >> 4) << 4;
      pNewLine->pText = (LPSTR)SECEditAlloc(iRequest);
      pNewLine->iBuffLen = iRequest;
		pNewLine->iLineNo = 1;
      }
   return pNewLine;
   }

//@doc SECEdit
//@mfunc Internal function. Returns the expanded text offset of an actual position
//@rdesc int The expanded text offset (e.g. the column position)
//@parm LPTSTR pTextBuffer
//@parm  int iPos
int SECEdit::GetTabbedLength(LPSTR pTextBuffer, int iPos)
   {
	LPSTR lpText;
	int i;       // the real offset count
	int iColNo;  // the apparent offset count
   int iAdd;

	i = iColNo = 0;
	lpText = pTextBuffer;
   if (!lpText)
      return iPos;
	while (i < iPos)
      {
      if (*lpText)
         {
         if (*lpText == 0x09)
            {
            iAdd = m_iTabSize - (iColNo % m_iTabSize);
            if (!iAdd)
               iAdd = m_iTabSize;
            }
         else
            iAdd = 1;
         lpText++;
         }
      else
         iAdd = 1;
      iColNo += iAdd;
      i++;
      }
   return iColNo;
	}


//@doc SECEdit
//@mfunc Internal function that computes the maximum length of a tabbed string that won't exceed OE_MAXLINELENGTH
//@rdesc int maximum length that won't exceed OE_MAXLINELENGTH after it is tabbed
//@parm LPTSTR pTextBuffer
int SECEdit::GetMaxTabbedLength(LPSTR pTextBuffer)
{
	LPSTR lpText;
	int i;       // the real offset count
	int iColNo;  // the apparent offset count
   int iAdd;

	i = iColNo = 0;
	lpText = pTextBuffer;
   if (!lpText)
      return 0;
	while (*lpText && (iColNo < (OE_MAXLINELENGTH-m_iLineEndLength)))
      {
      if (*lpText == 0x09)
         {
         iAdd = m_iTabSize - (iColNo % m_iTabSize);
         if (!iAdd)
            iAdd = m_iTabSize;
         }
      else
         iAdd = 1;
      lpText++;
      iColNo += iAdd;
      i++;
      }
	if (iColNo > (OE_MAXLINELENGTH-m_iLineEndLength))
		i--;
   return i;
}


//@doc SECEdit
//@mfunc Internal function that constructs a linked list of LINEDESC from lpTextBuffer
//@rdesc PLINEDESC pointer to the first line
//@parm LPTSTR lpTextBuffer
PLINEDESC SECEdit::LineFromTextBuffer(LPSTR lpTextBuffer)
   {
   PLINEDESC lpNewLine;
   PLINEDESC lpFirstLine;
   PLINEDESC lpPrevLine;
   LPSTR pNextBreak;
   LPSTR pTextBuffer;
   int iNumberOfChars;
   int iLineSize;
	int i;
   char search[4] = {0x0D,0x0A,0,0};
	BOOL bWarnedOnce = FALSE;
	ASSERT(lpTextBuffer);

   /* can't do it without text */
   if (lpTextBuffer == NULL)
      return NULL;

   pTextBuffer = lpTextBuffer;

   if (!pTextBuffer)
      return NULL;

   iNumberOfChars = strlen(pTextBuffer);

   lpFirstLine = lpNewLine = lpPrevLine = NULL;

   while (iNumberOfChars > 0)
      {
	   pNextBreak = strpbrk(pTextBuffer,search);
      //pNextBreak = (LPTSTR)memchr(pTextBuffer,m_szLineEnd[0],iNumberOfChars);
      if (!pNextBreak)
         iLineSize = iNumberOfChars;
      else
			{
         iLineSize = pNextBreak - pTextBuffer;
			// Special case: splitting a cr/lf combination
			// across fileread buffers.  Finish this line 
			// next time through.
			if ((m_iLineEndLength == 2) && 
				 ((iNumberOfChars - iLineSize) == 1))
				{
				iLineSize = iNumberOfChars;
				pNextBreak = NULL;
				}
			}

		if ((iLineSize * m_iTabSize) > OE_MAXLINELENGTH)
			{
			int iTabbedLength = GetTabbedLength(pTextBuffer, iLineSize);

			if (iTabbedLength > (OE_MAXLINELENGTH-m_iLineEndLength))
				{
				iLineSize = GetMaxTabbedLength(pTextBuffer);
				if (!bWarnedOnce)
					{
					bWarnedOnce = TRUE;
					CString strTemp; 
					strTemp.LoadString(IDS_OE_WRAPPINGLINES);
					CString str;
					str.Format(strTemp,OE_MAXLINELENGTH);
					AfxMessageBox(str);
					pNextBreak = NULL;
					}
				}
			}

      lpNewLine = MakeNewLine(iLineSize+m_iLineEndLength);
      memcpy(lpNewLine->pText,pTextBuffer,iLineSize);
      lpNewLine->iTxtSize = iLineSize;
      iNumberOfChars -= iLineSize;

		// append the proper line end to our new line
		i = 0;
		if (pNextBreak)
			{
			for (;i<m_iLineEndLength;i++)
				lpNewLine->pText[iLineSize+i] = m_szLineEnd[i];
			}
		lpNewLine->iTxtSize += i;

      pTextBuffer += iLineSize;

		// advance the pointer to pTextBuffer. 
		// Note:  the reason for this code is that we could
		// get a file with an inconsistent line end style,
		// and SECEdit needs to massage that out.  Now we
		// have to make assumptions about what combinations
		// might constitute weirdness at the end of this
		// line, and what could instead be just blank lines
		// consisting of line ends only.  Our assumption will
		// be that if m_iLineEndLength is 1, we'll move past
		// just one line end character of any type.  If it's 2,
		// we'll move past up to two line end characters of any
		// type.

		if (pNextBreak)
			{
			// we got at least one of them
			pTextBuffer++;
			iNumberOfChars--;

			if (m_iLineEndLength == 2)
				{
				if ((*pTextBuffer == 0x0D) || (*pTextBuffer == 0x0A))
					{
					pTextBuffer++;
					iNumberOfChars--;
					}
				}
			}

      if (!lpFirstLine)
         lpFirstLine = lpPrevLine = lpNewLine;
      else
         {
         ListInsertAfter(lpNewLine,lpPrevLine);
         lpPrevLine = lpNewLine;
         }
      }  /* end while (iNumberOfChars > 0) */

   return(lpFirstLine);
   } /** End LineFromTextBuffer() **/



//@doc SECEdit
//@mfunc Internal function that reallocates pText to accomodate iSize characters, if
//              necessary
//@rdesc BOOL 
//@parm PLINEDESC pLine
//@parm  int iSize
BOOL SECEdit::ResizeLine(PLINEDESC pLine, int iSize)
   {
   LPSTR lpTemp;
	ASSERT(pLine);

   if (pLine->iBuffLen > iSize)
      return TRUE;
   /* round up to the next highest multiple of 16 */
   iSize = ((iSize+31) >> 4) << 4;
   lpTemp = (LPSTR)SECEditAlloc(iSize);
   if (!lpTemp)
      return FALSE;
   strcpy(lpTemp,pLine->pText);
   free(pLine->pText);
   pLine->pText = lpTemp;
   pLine->iBuffLen = iSize;
   return TRUE;
   }


//@doc SECEdit
//@mfunc Internal function that gets the 1-based line number of pLine
//@rdesc int The line number.
//@parm PLINEDESC pLine
int SECEdit::GetLineNo(PLINEDESC pLine)
   {
	ASSERT(pLine);
	return pLine->iLineNo;
	int i;
   ListGetFirst(pLine,&i);
   return i;
   }

//@doc SECEdit
//@mfunc Internal function to find the line at iLineNo
//@rdesc PLINEDESC pointer to the line
//@parm int iLineNo
PLINEDESC SECEdit::GoToLineNo(int iLineNo)
   {
	ASSERT(iLineNo);
   PLINEDESC pTempLine;
	PLINEDESC pSaveLine;
   int i;

   i = 1;
   pSaveLine = pTempLine = ListGetFirst(m_pLine, NULL);
   while (pTempLine && (i != iLineNo))
      {
      i++;
      pTempLine = pTempLine->pNext;
      }
	if (!pTempLine)
		return ListGetLast(pSaveLine);
   return pTempLine;
   }


//@doc SECEdit
//@mfunc Internal function that links the two parameters into a doubly linked list.  
//@rdesc int 0 on success, -1 on failure.
//@parm PLINEDESC lpOne
//@parm  PLINEDESC lpTwo
int SECEdit::ListInsertBefore(PLINEDESC lpOne, PLINEDESC lpTwo)
   {
   PLINEDESC lpNew;
   PLINEDESC lpSpot;
	ASSERT(lpOne);
	ASSERT(lpTwo);

   lpNew = lpOne;
   lpSpot = lpTwo;

   if (!lpNew || !lpSpot)  /* link nothing to nothing? */
      return -1;

   lpNew->pPrev = lpSpot->pPrev;
   if (lpSpot->pPrev)
      lpSpot->pPrev->pNext = lpNew;
   lpNew->pNext = lpSpot;
   lpSpot->pPrev = lpNew;
	return 0;
   }


//@doc SECEdit
//@mfunc Internal function that links the two parameters into a doubly linked list.
//@rdesc int 0 on success, -1 on failure.
//@parm PLINEDESC lpOne
//@parm  PLINEDESC lpTwo
int SECEdit::ListInsertAfter(PLINEDESC lpOne, PLINEDESC lpTwo)
   {
   PLINEDESC lpNew;
   PLINEDESC lpSpot;
	ASSERT(lpOne);
	ASSERT(lpTwo);

   lpNew = lpOne;
   lpSpot = lpTwo;

   if (!lpNew || !lpSpot)  /* link nothing to nothing? */
      return -1;

   if (lpSpot->pNext)
      lpSpot->pNext->pPrev = lpNew;
   lpNew = ListGetLast(lpNew);   
   lpNew->pNext = lpSpot->pNext;
   lpNew->pPrev = lpSpot;
   lpSpot->pNext = lpNew;
	return 0;
   }


//@doc SECEdit
//@mfunc Internal function that travels the linked list up and returns the first member 
// 				  Counts lines from lpSpot as a bonus	
//@rdesc PLINEDESC pointer to the first member of the linked list 
//@parm PLINEDESC lpSpot
//@parm  int* i
PLINEDESC SECEdit::ListGetFirst(PLINEDESC lpSpot, int* i)
   {
   PLINEDESC lp;
	ASSERT(lpSpot);

   lp = lpSpot;

   if (!lp)
      return NULL;

	int iCount = 1;
   while (lp->pPrev)
		{
      lp = lp->pPrev;
		iCount++;
		}

	if (i)
		*i = iCount;

   return lp;
   }

//@doc SECEdit
//@mfunc Internal function that travels the linked list down and returns the last member   
//@rdesc PLINEDESC pointer to the last member of the linked list
//@parm PLINEDESC lpSpot
PLINEDESC SECEdit::ListGetLast(PLINEDESC lpSpot)
   {
   PLINEDESC lp;
	ASSERT(lpSpot);

   lp = lpSpot;

   if (!lp)
      return NULL;

   while (lp->pNext)
      lp = lp->pNext;

   return lp;
   }



//@doc SECEdit
//@mfunc Internal function that removes lp from the doubly linked list.     
//@rdesc int 
//@parm PLINEDESC lpSpot
int SECEdit::ListUnlink(PLINEDESC lpSpot)
   {
   PLINEDESC lp;
	ASSERT(lpSpot);

   lp = lpSpot;

   if (!lp)
      return -1;
   if (lp->pPrev)
      lp->pPrev->pNext = lp->pNext;
   if (lp->pNext)
      lp->pNext->pPrev = lp->pPrev;
	return 0;
   }

//@doc SECEdit
//@mfunc Frees the linked list of lines.
//@rdesc int 0 on success, -1 on failure.
//@parm PLINEDESC pLine
int SECEdit::LineListFree(PLINEDESC pLine)
   {
   PLINEDESC lpFirst;
   PLINEDESC lpSecond;
	ASSERT(pLine);

   if (!pLine)
      return -1;

   lpFirst = lpSecond = ListGetFirst(pLine, NULL);
   while (lpFirst)
      {
      lpSecond = lpFirst->pNext;
      FreeTextLine(lpFirst);
      lpFirst = lpSecond;
      }
	return 0;
   }


//@doc SECEdit
//@mfunc Internal function that finds out what kind of line ends are in lpString
//@rdesc int line end type
//@parm LPTSTR lpString
//@parm  int iLength
int SECEdit::FindLineEnd(LPSTR lpString, int iLength)
   {
   LPSTR result;
   char search[4] = {0x0D,0x0A,0,0};
   int iSaveChar;
   int iRval;
	ASSERT(lpString);

   iSaveChar = lpString[iLength];
   lpString[iLength] = 0;
   result = strpbrk(lpString,search);
   if (!result)
      {
      iRval = -1;
      lpString[iLength] = (char)iSaveChar;
      return iRval;
      }
      
   switch (*result)
      {
      case 0x0D:
         switch (*(result+1))
            {
            default:
            case 0:
               iRval = OE_CR;
               break;
            case 0x0A:
               iRval = OE_CRLF;
               break;
            }
         break;

      case 0x0A:
         switch (*(result+1))
            {
            default:
            case 0:
               iRval = OE_LF;
               break;

            case 0x0D:
               iRval = OE_LFCR;
               break;
            }
         break;

      default:
         iRval = OE_CRLF;
         break;
      }

   lpString[iLength] = (char)iSaveChar;
   return iRval;
   }

//@doc SECEdit
//@mfunc Internal function that converts the line ends in lpText from m_iLineEndType
//              to iLineEndType
//@rdesc int new size of lpText
//@parm LPTSTR* lpText
//@parm  int iBufferSize
//@parm  int iLineEndType
int SECEdit::ConvertLineEnds(LPSTR* lpText, int iBufferSize, int iLineEndType)
   {
   LPSTR lpWorkBuffer;
   LPSTR lpNext;
   LPSTR lpStart;
   LPSTR lpTemp;
   char szOldLineEnd[4];
	char szNewLineEnd[4];
   int iNewTextSize,iRequest;
   int iOldEndLength,iNewEndLength;
   int iLength,iCount;
   char search[4] = {0x0D,0x0A,0,0};
	ASSERT(*lpText);

   /* quick check for linefeeds, if any */
   lpNext = strpbrk(*lpText,search);

   if (!lpNext) /* no linefeeds, no work */
      return iBufferSize;

	switch(iLineEndType)
		{
      case OE_CR:
         iNewEndLength = 1;
         szNewLineEnd[0] = 0x0D;
         szNewLineEnd[1] = 0;
         break;

      case OE_LF:
         iNewEndLength = 1;
         szNewLineEnd[0] = 0x0A;
         szNewLineEnd[1] = 0;
         break;

      case OE_LFCR:
         iNewEndLength = 2;
         szNewLineEnd[0] = 0x0A;
         szNewLineEnd[1] = 0x0D;
         szNewLineEnd[2] = 0;
         break;

      default:
      case OE_CRLF:
         iNewEndLength = 2;
         szNewLineEnd[0] = 0x0D;
         szNewLineEnd[1] = 0x0A;
         szNewLineEnd[2] = 0;
         break;

		}

   switch (FindLineEnd(*lpText,iBufferSize))
      {
      case OE_CR:
         iOldEndLength = 1;
         szOldLineEnd[0] = 0x0D;
         szOldLineEnd[1] = 0;
         break;

      case OE_LF:
         iOldEndLength = 1;
         szOldLineEnd[0] = 0x0A;
         szOldLineEnd[1] = 0;
         break;

      case OE_LFCR:
         iOldEndLength = 2;
         szOldLineEnd[0] = 0x0A;
         szOldLineEnd[1] = 0x0D;
         szOldLineEnd[2] = 0;
         break;

      default:
      case OE_CRLF:
         iOldEndLength = 2;
         szOldLineEnd[0] = 0x0D;
         szOldLineEnd[1] = 0x0A;
         szOldLineEnd[2] = 0;
         break;

      }

   /* jump through, counting OE_CR/LFs, to get the needed workbuffer size */
   iCount = 0;
   iLength = strlen(lpNext);
   lpTemp = lpNext;
   while (lpNext && *lpNext)
      {
      lpNext = (LPSTR)memchr(lpTemp,szOldLineEnd[0],iLength);
      if (lpNext)
         {
         *lpNext++;
         iCount++;
         iLength -= (lpNext - lpTemp);
         lpTemp = lpNext;
         }
      }

   lpNext = *lpText;

   lpStart = lpWorkBuffer = (LPSTR)SECEditAlloc(iBufferSize+(iCount*iNewEndLength));

   iNewTextSize = 0;

   while (*lpNext && iBufferSize)
      {
      switch (*lpNext)
         {
         case 0x0D:
         case 0x0A:

            {
            int i;
            for (i=0;i<iNewEndLength;i++)
               *lpWorkBuffer++ = szNewLineEnd[i];
            iBufferSize -= iOldEndLength;
            iNewTextSize += iNewEndLength;
            if (iBufferSize)
               lpNext += iOldEndLength;
            break;
            }

         default:
            *lpWorkBuffer++ = *lpNext++;
            iNewTextSize++;
            iBufferSize--;

         }
      }

   iNewTextSize = strlen(lpStart);
   /* round up to the next highest multiple of 16 */
   iRequest = ((iNewTextSize+15) >> 4) << 4;
   /* add a little fudge */
   iRequest += 32;
   *lpText = (LPSTR)realloc(*lpText,iRequest);
   strcpy(*lpText,lpStart);
   free(lpStart);

   return iNewTextSize;
   }

