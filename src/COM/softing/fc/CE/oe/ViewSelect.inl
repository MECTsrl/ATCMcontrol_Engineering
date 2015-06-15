#ifndef __VIEWSELECT_INL__
#define __VIEWSELECT_INL__
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
// ViewSelect.cpp
//
//
// Description: Text selection functions
//
//

#include "SECEdit.h"


//@doc SECEditCore
//@mfunc Clears all internal marks and removes the block from the screen
//@rdesc int 0 on success, errorcode on failure
//@parm BOOL bUpdate Indicates if the function should update the display.
template <class BC>
int SECEditCore<BC>::ClearSelection(BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   m_bSelecting = FALSE;
	if (!m_bHaveCaret && (GetFocus() == this))
		{
		::CreateCaret(m_hWnd,NULL,2,m_iLineHeight);
		m_bHaveCaret = TRUE;
   	ShowCaret();
		MySetCaretPos();
		}
	if (m_iTimer)
		{
		KillTimer(6);
		m_iTimer = 0;
		}
	if (m_bTrack)
		{
		ReleaseCapture();
		m_bTrack = FALSE;
		}

   if (!m_BlockStart.pLine || !m_BlockEnd.pLine)
      return IDS_OE_NOTEXT;

   if (bUpdate)
      {
      if (m_iBlockType == OE_COLUMN)
         {
         m_BlockStart.pLine = m_BlockEnd.pLine = NULL;
         m_BlockStart.iOffset = m_BlockEnd.iOffset = 0;
         }
      ClearBlockFromScreen();
      }

   m_BlockStart.pLine = m_BlockEnd.pLine = NULL;
   m_BlockStart.iOffset = m_BlockEnd.iOffset = 0;

   m_iBlockType = OE_STREAM;
   return 0;
   }


//
//@doc SECEditCore
//@mfunc Clears the temporary line tags, or markers.
//@rdesc int Always returns 0
template <class BC>
int SECEditCore<BC>::ClearInvalidMarkers()
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   PLINEDESC pLine;

   pLine = pEdit->ListGetFirst(m_pCurLine, NULL);
   while (pLine)
      {
      pLine->bInvalidStart = pLine->bInvalidEnd = 
         pLine->iInvalidStartCol = pLine->iInvalidEndCol = 0;
      pLine = pLine->pNext;
      }
   return 0;
   }


//@doc SECEditCore
//@mfunc Internal function used to begin selection of a region
//@rdesc int 0 on success, error code on failure.
//@parm int iType The type of the block.
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::StartSelection(int iType, BOOL bUpdate)
   {
	bUpdate; //UNUSEd
	if (!m_bReplacing)
		ClearInvalidMarkers();
   m_BlockStart.pLine = m_pCurLine;
   m_BlockStart.iOffset = m_iColNo;
   m_BlockEnd = m_BlockStart;
   m_OriginalBlockStart = m_BlockStart;
   m_iBlockType = iType;
   m_bSelecting = TRUE;
	if (m_bHaveCaret)
		{
	   DestroyCaret();
		m_bHaveCaret = FALSE;
		}
   return 0;
   }

//@doc SECEditCore
//@mfunc Internal function to decide if pFirst occurs in the linked list before
//              pSecond
//@rdesc BOOL 
//@parm PTEXTPOS pFirst
//@parm  PTEXTPOS pSecond
template <class BC>
BOOL SECEditCore<BC>::InOrder(PTEXTPOS pFirst, PTEXTPOS pSecond)
   {
   PLINEDESC pLine;
	if (!pFirst->pLine || !pSecond->pLine)
		return TRUE;
   if (pFirst->pLine == pSecond->pLine)
      {
      if (pFirst->iOffset <= pSecond->iOffset)
         return TRUE;
      }
   else
      {
      pLine = pFirst->pLine->pNext;
      while (pLine)
         {
         if (pLine == pSecond->pLine)
            return TRUE;
         pLine = pLine->pNext;
         }
      }
   return FALSE;
   }

//@doc SECEditCore
//@mfunc Internal function that caches some tags describing the invalid screen area
//@rdesc void 
//@parm PTEXTPOS pStart
//@parm  PTEXTPOS pEnd
template <class BC>
void SECEditCore<BC>::SetUpInvalidBlock(PTEXTPOS pStart, PTEXTPOS pEnd)
   {
   PLINEDESC pLine;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   pStart->pLine = pEnd->pLine = NULL;
   pStart->iOffset = pEnd->iOffset = 0;
   //pLine = pEdit->ListGetFirst(m_pTopLine, NULL);
	pLine = m_pTopLine;

	// search forward
   while (pLine)
      {
      if (pLine->bInvalidStart)
         {
         pStart->pLine = pLine;
         pStart->iOffset = pLine->iInvalidStartCol;
         }
      if (pLine->bInvalidEnd)
         {
         pEnd->pLine = pLine;
         pEnd->iOffset = pLine->iInvalidEndCol;
         }
		if (pStart->pLine && pEnd->pLine)
			return;
      pLine = pLine->pNext;
      }

	// search backwards
	pLine = m_pTopLine;
   while (pLine)
      {
      if (pLine->bInvalidStart)
         {
         pStart->pLine = pLine;
         pStart->iOffset = pLine->iInvalidStartCol;
         }
      if (pLine->bInvalidEnd)
         {
         pEnd->pLine = pLine;
         pEnd->iOffset = pLine->iInvalidEndCol;
         }
		if (pStart->pLine && pEnd->pLine)
			return;
      pLine = pLine->pPrev;
      }
   
	}


//@doc SECEditCore
//@mfunc Internal function that clears the cached invalid block marks
//@rdesc void 
//@parm PTEXTPOS pTextPos
template <class BC>
void SECEditCore<BC>::ClearInvalidBlockMarks(PTEXTPOS pTextPos)
   {
   PLINEDESC pLine;

   pLine = pTextPos->pLine;
   while (pLine)
      {
      pLine->bInvalidStart = pLine->iInvalidStartCol = 0;
      if (pLine->bInvalidEnd)
         {
         pLine->bInvalidEnd = pLine->iInvalidEndCol = 0;
         break;
         }
      pLine = pLine->pNext;
      }
   }

//@doc SECEditCore
//@mfunc Internal function that redraws the invalid portion of a column selection.
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate
template <class BC>
int SECEditCore<BC>::UpdateColumnSelection(BOOL bUpdate)
   {
	bUpdate; //UNUSED
   RECT RectOld;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	// set up the existing coordinates using lines for top and bottom
	// and columns for left and right
	RectOld.left = m_BlockStart.iOffset;
	RectOld.right = m_BlockEnd.iOffset;
	RectOld.top = pEdit->GetLineNo(m_BlockStart.pLine);
	RectOld.bottom = pEdit->GetLineNo(m_BlockEnd.pLine);

	// reset the block markers
   m_BlockEnd.pLine = m_pCurLine;
   m_BlockEnd.iOffset = m_iColNo;

	// stuff 'em in a point
	CPoint ptCurrent(m_BlockEnd.iOffset,pEdit->GetLineNo(m_BlockEnd.pLine));

	// adjust everybody to screen coordinates
	RectOld.left -= m_iLeftEdge;
   RectOld.left *= m_iCharWidth;
	RectOld.left += m_iMarkSpace;  // left margin space in pixels
	RectOld.right -= m_iLeftEdge;
   RectOld.right *= m_iCharWidth;
	RectOld.right += m_iMarkSpace;
   RectOld.top = (RectOld.top-m_iFirstLineNo) * m_iLineHeight;
   RectOld.bottom = (RectOld.bottom+1-m_iFirstLineNo) * m_iLineHeight;

	ptCurrent.x -= m_iLeftEdge;
	ptCurrent.x *= m_iCharWidth;
	ptCurrent.x += m_iMarkSpace;
	ptCurrent.y  = (ptCurrent.y+1-m_iFirstLineNo) * m_iLineHeight;

	// clear the invalid area
	CClientDC dc(this);
	dc.PatBlt(RectOld.left,
		       RectOld.bottom,
				 RectOld.right - RectOld.left,
				 ptCurrent.y - RectOld.bottom,
				 DSTINVERT);

	// highlight the new area
	dc.PatBlt(RectOld.right,
		       RectOld.top,
				 ptCurrent.x - RectOld.right,
				 ptCurrent.y - RectOld.top,
				 DSTINVERT);

	return 0;
   }


//@doc SECEditCore
//@mfunc Updates the invalid selection area
//@rdesc int 
//@parm BOOL bUpdate
template <class BC>
int SECEditCore<BC>::UpdateSelection(BOOL bUpdate)
   {
   PLINEDESC lpLine;
   BOOL bStarted,bFinished;
   int xPos,yPos,iNumLines,iNumChars;
   TEXTPOS NewPos;
   TEXTPOS TempPos;
   TEXTPOS InvalidBlockStart;
   TEXTPOS InvalidBlockEnd;
   int i;
   int iWidth;
   int iLength;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (m_iBlockType == OE_COLUMN)
      return UpdateColumnSelection(bUpdate);

   // update the view's block markers
   TempPos.pLine = m_pCurLine;
   TempPos.iOffset = m_iColNo;

   // put 'em back in original order first
   m_BlockStart = m_OriginalBlockStart;
   m_BlockEnd = TempPos;

   // then put them in stream order
   if (!InOrder(&m_BlockStart,&m_BlockEnd))
      {
      TempPos = m_BlockStart;
      m_BlockStart = m_BlockEnd;
      m_BlockEnd = TempPos;
      }

   if (!bUpdate || !IsWindow(m_hWnd))
      return 0;

   /* get the top of the screen into a TEXTPOS */
   NewPos.pLine = m_pTopLine;
   NewPos.iOffset = 0;

   SetUpInvalidBlock(&InvalidBlockStart, &InvalidBlockEnd);

   if (!InvalidBlockStart.pLine || !InvalidBlockEnd.pLine)
      return 0;

   iNumLines = m_rect.bottom / m_iLineHeight;
   iNumLines++;  // the last possibly partial line

   iNumChars = m_rect.right / m_iCharWidth;
   iNumChars++;

	CClientDC dc(this);

   yPos = 0;
   lpLine = m_pTopLine;
   bFinished = FALSE;

	BOOL bFirst = FALSE;
   bStarted = InOrder(&InvalidBlockStart,&NewPos);
   for (i=0;i<=iNumLines;i++)
      {
      if (lpLine)
         {

         iLength = lpLine->iTxtSize;
         if (lpLine->pText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
            iLength -= pEdit->m_iLineEndLength;

         iLength = pEdit->GetTabbedOffset(lpLine,iLength);
         // 1. find the starting column
         if (!bStarted)
            {
            if (!lpLine->bInvalidStart)
               {
               lpLine = lpLine->pNext;  // not yet, skip to the next line
               yPos += m_iLineHeight;
               continue;
               }
            else   
               {
               bStarted = bFirst = TRUE;  // doing the first line
               xPos = min((unsigned int)iLength,lpLine->iInvalidStartCol);
               }
            }
         else // past the first line
            xPos = 0;

         // 2. Calculate the width
         if (!lpLine->bInvalidEnd)
            iWidth = iLength - xPos;
         else
            {
            iWidth = min(iLength,InvalidBlockEnd.iOffset) - xPos;
            bFinished = TRUE;
            }

         // 3. adjust for iLeftEdge
         if (xPos < m_iLeftEdge)
            iWidth -= m_iLeftEdge - xPos;

			if (m_iLeftEdge)
	         xPos -= m_iLeftEdge;

         if (iWidth < 0) 
            iWidth = 0;

         if (xPos < 0)   
            xPos = 0;

         iWidth = iWidth*m_iCharWidth;
         xPos = xPos*m_iCharWidth;
         xPos += m_iMarkSpace;

         if (iWidth)
            dc.PatBlt(xPos,yPos,iWidth,m_iLineHeight,DSTINVERT);
//			else if (!iLength && !bFirst)
//				dc.PatBlt(xPos,yPos,m_iCharWidth/2,m_iLineHeight,DSTINVERT);
			bFirst = FALSE;
         lpLine = lpLine->pNext;
         yPos += m_iLineHeight;
         }
      if (bFinished)
         break;
         }
   ClearInvalidBlockMarks(&InvalidBlockStart);
   return 0;
   }

//@doc SECEditCore
//@mfunc Selects the current line
//@rdesc int 0 on success, error code on failure.
//@parm BOOL bUpdate Should the screen be updated?
template <class BC>
int SECEditCore<BC>::SelectLine(BOOL bUpdate)
   {
   if (m_bSelecting)
      ClearSelection(bUpdate);

   BeginningOfLine(bUpdate);

   StartSelection(OE_STREAM,bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

   if (DownLine(bUpdate))
		EndOfLine(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;


   UpdateSelection(bUpdate);

   return 0;
   }



//@doc SECEditCore
//@mfunc Selects the current word.
//@rdesc int 0 on success, errorcode on failure
//@parm BOOL bUpdate Update the display?
template <class BC>
int SECEditCore<BC>::SelectWord(BOOL bUpdate)
   {
   PLINEDESC lpLine;
   LPSTR szText;
   int iPos,iLength;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   
	if (m_bSelecting)
      ClearSelection(bUpdate);

   lpLine = m_pCurLine;
   szText = lpLine->pText;
   if (szText)
      iLength = lpLine->iTxtSize;
   else
      iLength = 0;
   if (iLength>1)
      {
      /* (maybe) adjust for OE_CR/OE_LF */
      if (szText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
         iLength -= pEdit->m_iLineEndLength;
      }

   iPos = pEdit->GetRealOffset(lpLine,m_iColNo);

   /*  non-word:
    *     (!isalnum(szText[iPos]) && (szText[iPos] != '_')
    */
    
   /*  word:
    *     (isalnum(szText[iPos]) || (szText[iPos] == '_')
    */
   if (
       !iLength ||
       (iLength < iPos+1) ||
       ((!pEdit->Myisalnum(szText[iPos]) && (szText[iPos] != '_')))
      )
      {

      return 0;
      }

   /* back up to the beginning of the current word */
   while (iPos && 
			 !pEdit->terminatorarray[szText[iPos-1]] && 
			 (pEdit->Myisalnum(szText[iPos-1]) || (szText[iPos-1] == '_')))
      iPos--;

	m_iColNo = pEdit->GetTabbedOffset(lpLine,iPos);

   /* now iPos && m_iColNo point to the first character of the word */
   StartSelection(OE_STREAM,bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

   while ( (iPos <= iLength) && 
		     !pEdit->startarray[szText[iPos]] && 
		     (
			  (pEdit->Myisalnum(szText[iPos]) || szText[iPos] == '_')
			  )
			)
      iPos++;
   
	m_iColNo = pEdit->GetTabbedOffset(lpLine,iPos);

   if (bUpdate)
      MakeCursorVisible();

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

   UpdateSelection(bUpdate);

   return 0;
   }


//@doc SECEditCore
//@mfunc Selects the entire document
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?
template <class BC>
int SECEditCore<BC>::SelectAll(BOOL bUpdate)
   {
   if (m_bSelecting)
      ClearSelection(bUpdate);
   TopOfFile(FALSE);
   StartSelection(OE_STREAM,bUpdate);
   BottomOfFile(FALSE);
   UpdateSelection(bUpdate);

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
      UpdateWindow();
      }
   return 0;
   }


//@doc SECEditCore
//@mfunc Extend the selection one line up
//@rdesc int 0 on success, errorcode on failure.
//@parm int iType Type of selected block.
//@parm  BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::SelectUp(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
      StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

   UpLine(bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
	UpdateSelection(bUpdate);

   return 0;
   }



//@doc SECEditCore
//@mfunc Extend the selection one line down
//@rdesc int 0 on success, errorcode on failure.
//@parm int iType Type of selection block
//@parm  BOOL bUpdate Update the display?
template <class BC>
int SECEditCore<BC>::SelectDown(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;
   
	DownLine(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }



//@doc SECEditCore
//@mfunc  Extend the selection one character left
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectLeft(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif
   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	CursorLeft(bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;
   
	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }



//@doc SECEditCore
//@mfunc Extend the selection one character right
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectRight(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	CursorRight(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }


//@doc SECEditCore
//@mfunc Extend the selection to the beginning of the line
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectHome(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	BeginningOfLine(bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }

//@doc SECEditCore
//@mfunc Extend the selection to the end of the line
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectEnd(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	EndOfLine(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }



//@doc SECEditCore
//@mfunc Extend the selectione one word left
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectWordLeft(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	WordLeft(bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }


//@doc SECEditCore
//@mfunc Extend the selection one word right
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectWordRight(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	WordRight(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }


//@doc SECEditCore
//@mfunc Extend the selection one page up
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectPageUp(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpPage(bUpdate);

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }


//@doc SECEditCore
//@mfunc Extend the selection one page down
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectPageDown(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	DownPage(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }


//@doc SECEditCore
//@mfunc Extend the selection to the beginning of the document
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectTopOfFile(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	TopOfFile(bUpdate);
   
   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }

//@doc SECEditCore
//@mfunc Extend the selection to the end of the document
//@rdesc int 
//@parm int iType
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectEndOfFile(int iType, BOOL bUpdate)
   {
#ifdef ORIGINAL_VERSION
   if (!m_bSelecting)
		StartSelection(iType,bUpdate);
#else
   bool bStart=false;
   int iLine;
   int iColumn;
   if (!m_bSelecting)
   {
       bStart = true;
       iLine = m_iLineNo;
       iColumn = m_iColNo;

       StartSelection (iType, bUpdate);
   };
#endif

   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;

	BottomOfFile(bUpdate);

   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;

	UpdateSelection(bUpdate);
#ifndef ORIGINAL_VERSION
    if (bStart && m_iLineNo == iLine && m_iColNo == iColumn)
        ClearSelection (bUpdate);
#endif
   return 0;
   }

//@doc SECEditCore
//@mfunc Select text starting at pTextPos and extending iLength
//              characters
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  int iLength
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::SelectFoundText(PTEXTPOS pTextPos, int iLength, BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	if (m_bSelecting && bUpdate)
		ClearSelection(bUpdate);
	if (m_pCurLine != pTextPos->pLine)
		{
		m_pCurLine = pTextPos->pLine;
		m_iLineNo = pEdit->GetLineNo(m_pCurLine);
		}
	int iSaveCol = m_iColNo = pEdit->GetTabbedOffset(pTextPos->pLine,pTextPos->iOffset);
   StartSelection(OE_STREAM,bUpdate);
   m_pCurLine->bInvalidStart = TRUE;
   m_pCurLine->iInvalidStartCol = m_iColNo;
	m_iColNo += iLength;
	// help MakeCursorVisible() out a little
	if (m_rect.right && m_iCharWidth)
		{
		int nNumChars = (m_rect.right-m_iMarkSpace) / m_iCharWidth;
		int nFirstVisibleCol = m_iLeftEdge;
		int nLastVisibleCol = m_iLeftEdge + nNumChars;
		if ((m_iColNo+3) < nNumChars)
			{
			m_iLeftEdge = 0;
			InvalidateRect(NULL);
			}
		else if ((m_iColNo-iLength) < nFirstVisibleCol)
			{
			m_iLeftEdge = max((m_iColNo-iLength-10),0);
			}
		else if (m_iColNo >= nLastVisibleCol)
			{
			m_iLeftEdge = m_iColNo - nNumChars + 10;
			if (m_iLeftEdge < 0)
				m_iLeftEdge = 0;
			InvalidateRect(NULL);
			}
		}
	if (bUpdate)
	   MakeCursorVisible(/*m_bUpdate*/);
   m_pCurLine->bInvalidEnd = TRUE;
   m_pCurLine->iInvalidEndCol = m_iColNo;
	UpdateSelection(bUpdate);
	//m_iColNo = iSaveCol;
	if (bUpdate)
	   MakeCursorVisible(/*m_bUpdate*/);
   return 0;
	}


#endif //__VIEWSELECT_INL__