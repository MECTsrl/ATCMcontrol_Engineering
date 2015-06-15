#ifndef __VIEWCURSOR_INL__
#define __VIEWCURSOR_INL__
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
//@mfunc Internal function that determines the nearest valid column position to iPos.
//              Tabs occupy virtual space that the caret can't be
//              moved to.
//@rdesc int 0-based column offset.
//@parm int iPos

template <class BC>
int SECEditCore<BC>::GetValidColNo(int iPos)
{
	int i;
	SECEdit* pEdit = GetEdit();
	ASSERT( pEdit != NULL );

	i = pEdit->GetRealOffset( m_pCurLine, iPos );

	if ( (i < m_pCurLine->iTxtSize )   && 
	     ( m_pCurLine->pText[i] == 9 ) && 
		 ( iPos % pEdit->m_iTabSize ) )
	{
		iPos = pEdit->GetTabbedOffset( m_pCurLine, i );
	}

	// Change to eliminate "virtual whitespace"
	// find maximum valid column
	// but not if we are doing a column select
	if(!m_bUseVirtualWhitespace && !(m_bSelecting && (m_iBlockType == OE_COLUMN)))
	{
		for ( int MaxPos = max( 0, ( m_pCurLine->iTxtSize - 1 ) );
			  MaxPos > 0; MaxPos-- )
		{
			if ( m_pCurLine->pText[ MaxPos ] == char('\r') )
				break;
		}

		// account for no line end
		if (!MaxPos)
			MaxPos = iPos;

// #352 20.02.07 SIS >>
#ifndef _ORIGINAL_VERSION
		MaxPos = min(m_pCurLine->iTxtSize, MaxPos);
#endif
// #352 20.02.07 SIS <<
		
		// account for any embedded tabs
		MaxPos = pEdit->GetTabbedOffset( m_pCurLine, MaxPos );
		// govern 
		iPos = min( iPos, MaxPos );

		// If the line is empty, then we shoud not advance the column
		// iTxtSize = Bytes of text

		if ((m_pCurLine->iTxtSize == 0) || 
			  (m_pCurLine->iTxtSize == pEdit->m_iLineEndLength))
			{
			// Blank line, so...
			iPos = 0;
			}

		//////////////////////////////////////////////////////
		// End Change
	}

	return iPos;
}



//@doc SECEditCore
//@mfunc Internal function that determines the first valid column position previous
//              to iPos. Tabs occupy virtual space that the caret
//              can't be moved to.
//@rdesc int 0 based column offset.
//@parm int iPos The position to use in the calculation
template <class BC>
int SECEditCore<BC>::GetPreviousColNo(int iPos)
   {
   int i;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   
   iPos--;
   i = pEdit->GetRealOffset(m_pCurLine,iPos);
	if ((i < m_pCurLine->iTxtSize) && (m_pCurLine->pText[i] == 9) && (iPos % pEdit->m_iTabSize))
		iPos = pEdit->GetTabbedOffset(m_pCurLine,i);
   return iPos;
   }

//@doc SECEditCore
//@mfunc Internal function. Determines the first valid column position after
//              iPos. Tabs occupy virtual space that the caret
//              can't be moved to.
//@rdesc int 0-based column offset
//@parm int iPos
template <class BC>
int SECEditCore<BC>::GetNextColNo(int iPos)//ADMacroDone
   {
   int i;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   i = pEdit->GetRealOffset(m_pCurLine,iPos);
	if ((i < m_pCurLine->iTxtSize) && (m_pCurLine->pText[i] == 9) && (iPos+1 % pEdit->m_iTabSize))
      {
      i = pEdit->GetTabbedOffset(m_pCurLine,i);
		if (i)
			iPos = (((i / pEdit->m_iTabSize)+1) * pEdit->m_iTabSize);
		else
			iPos = pEdit->m_iTabSize;
      }
	else
		iPos++;

	// Return the next
   return GetValidColNo(iPos);
   }

//@doc SECEditCore
//@mfunc Moves the caret one valid position to the left
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?
template <class BC>
int SECEditCore<BC>::CursorLeft(BOOL bUpdate)
   {

   if (m_iColNo == 0)
      {
      // can we go back a line?
      if (!UpLine(bUpdate))
         {
         EndOfLine(bUpdate);
         return 0;
         }
      else
         return IDS_OE_ATTOP;
      }

   m_iColNo = GetPreviousColNo(m_iColNo);

   if (bUpdate)
      MakeCursorVisible();

   UpdateViews(this, OE_SYNCH_SPLITTERS);

   return 0;
   }

//@doc SECEditCore
//@mfunc Moves the caret one valid position to the right
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::CursorRight(BOOL bUpdate)
   {
   if (m_iColNo >= OE_MAXLINELENGTH)
      return IDS_OE_LINETOOLONG;

   // See if we are at the end of the line
	int nCurrentCol = m_iColNo;

	// Get next column
	m_iColNo = GetNextColNo(m_iColNo);

	if (nCurrentCol == m_iColNo)
		{
		// We have hit the end of the line, so let's 
		// move down to the next line
		if (IDS_OE_EOF != DownLine(TRUE))
			{
			// not at the end of the file, so set the
			// cursor position to the 1st column
			BeginningOfLine(FALSE);
			}
		}

   if (bUpdate)
		MakeCursorVisible();

   UpdateViews(this, OE_SYNCH_SPLITTERS);

   return 0;
   }




//@doc SECEditCore
//@mfunc Moves the caret one line up
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::UpLine(BOOL bUpdate)
   {
   PLINEDESC pLine;
   RECT rect;

   pLine = m_pCurLine->pPrev;
   if (!pLine)
		return IDS_OE_ATTOP;

   if (bUpdate)
		MakeCursorVisible();

   m_pCurLine = pLine;

	if (m_iLineNo == m_iFirstLineNo)
	{
		m_pTopLine = m_pTopLine->pPrev;
		m_iFirstLineNo--;
		if (bUpdate)
		{
			ScrollWindow(0,m_iLineHeight,NULL,NULL);

			/* Erase the newly-exposed area */
			rect = m_rect;
			rect.bottom = rect.top+m_iLineHeight;
			InvalidateRect(&rect,FALSE);
			UpdateWindow();
		}
		// We were at the top so we needed to scroll. So
		// if we are embedded into a splitter window
	}

   m_iLineNo--;

   // calculate the column number
   m_iColNo = GetValidColNo(m_iColNo);
	
   if (bUpdate)
		MakeCursorVisible();

	UpdateViews(this, OE_SYNCH_SPLITTERS);

   return 0;
   }



//@doc SECEditCore
//@mfunc Moves the caret one line down
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::DownLine(BOOL bUpdate)
   {
   PLINEDESC pLine;
   RECT rect;
   int iNumLines;

   pLine = m_pCurLine->pNext;
   if (!pLine)
		return IDS_OE_EOF;

   if (bUpdate)
		MakeCursorVisible();

   m_pCurLine = pLine;

   iNumLines = m_rect.bottom / m_iLineHeight;
   iNumLines--;
   if (m_iLineNo == (m_iFirstLineNo + iNumLines))
      {
      m_pTopLine = m_pTopLine->pNext;
      m_iFirstLineNo++;
      if (bUpdate)
         {
         ScrollWindow(0,-m_iLineHeight,NULL,NULL);

         /* now we have to find the last onscreen line and repaint it */
         rect = m_rect;
         rect.top = rect.bottom - (rect.bottom - (iNumLines*m_iLineHeight));
         InvalidateRect(&rect,FALSE);
			UpdateWindow();
         }
		}
   m_iLineNo++;

   // calculate the column number
   m_iColNo = GetValidColNo(m_iColNo);
	
   if (bUpdate)
		MakeCursorVisible();

   UpdateViews(this, OE_SYNCH_SPLITTERS);

	return 0;
   }


//@doc SECEditCore
//@mfunc Scrolls the window view up one line, without 
//              resetting the cursor/caret position.
//
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::ScrollUpLine(BOOL bUpdate)
   {
   PLINEDESC pLine;
   RECT rect;

   pLine = m_pTopLine->pPrev;
   if (!pLine)
      return IDS_OE_ATTOP;

   m_pTopLine = pLine;
   m_iFirstLineNo--;

   if (bUpdate)
      {
      ScrollWindow(0,m_iLineHeight,NULL,NULL);

      /* Erase the newly-exposed area */
      rect = m_rect;
      rect.bottom = rect.top+m_iLineHeight;
      InvalidateRect(&rect,FALSE);
   	UpdateWindow();
      MySetScrollPos(SB_VERT,m_iFirstLineNo-1,TRUE);
      }
   return 0;
   }


//@doc SECEditCore
//@mfunc Scoll the window down one line without updating the cursor.
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::ScrollDownLine(BOOL bUpdate)
   {
   PLINEDESC pLine;
   RECT rect;
   int iNumLines;

   pLine = m_pTopLine->pNext;
   if (!pLine)
      return IDS_OE_EOF;

   m_pTopLine = pLine;
   m_iFirstLineNo++;

   if (bUpdate)
      {
      ScrollWindow(0,-m_iLineHeight,NULL,NULL);

      /* Erase the newly-exposed area */
      rect = m_rect;
      iNumLines = rect.bottom / m_iLineHeight;
      iNumLines--;
      rect.top = rect.bottom - (rect.bottom - (iNumLines*m_iLineHeight));
      InvalidateRect(&rect,FALSE);
		UpdateWindow();
      MySetScrollPos(SB_VERT,m_iFirstLineNo-1,TRUE);
      }
   return 0;
   }


//@doc SECEditCore
//@mfunc Moves the caret to column 0
//@rdesc int  0 on success, errorcode on success.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::BeginningOfLine(BOOL bUpdate)
   {

   m_iColNo = 0;

   if (bUpdate)
      MakeCursorVisible();

   return 0;
   }


//@doc SECEditCore
//@mfunc Moves the caret to the end of the current line
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::EndOfLine(BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	int iLength = m_pCurLine->iTxtSize;

   if (iLength && (m_pCurLine->pText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0]))
      iLength -= pEdit->m_iLineEndLength;

   m_iColNo = pEdit->GetTabbedOffset(m_pCurLine,iLength);

   if (bUpdate)
		MakeCursorVisible();

	return 0;
   }




//@doc SECEditCore
//@mfunc Moves the caret one page (screen) up
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::UpPage(BOOL bUpdate)
   {
   PLINEDESC pLine;
   int iPageSize,i,iOldFirstLineNo;

   if (!m_pCurLine->pPrev)
      return IDS_OE_ATTOP;

   if (bUpdate)
      MakeCursorVisible();

   iPageSize = m_rect.bottom / m_iLineHeight;
	iOldFirstLineNo = m_iFirstLineNo;

   for (i=0;(i < iPageSize);i++)
      {
      pLine = m_pTopLine->pPrev;
      if (pLine)
         {
         m_iFirstLineNo--;
         m_iLineNo--;
         m_pTopLine = pLine;
         m_pCurLine = m_pCurLine->pPrev;
         }
		else
			{
			pLine = m_pCurLine->pPrev;
			if (pLine)
				{
            m_iLineNo--;
				m_pCurLine = pLine;
				}
			}
      }

   if (bUpdate)
      {
		if (iOldFirstLineNo != m_iFirstLineNo)
			{
         InvalidateRect(NULL,FALSE);
			UpdateWindow();
			}
      MakeCursorVisible();
      }

   UpdateViews(this, OE_SYNCH_SPLITTERS);

   return 0;
   }

//@doc SECEditCore
//@mfunc Moves the caret one page (screen) down
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::DownPage(BOOL bUpdate)
   {
   PLINEDESC pLine;
   int iPageSize,i;

   if (!m_pCurLine->pNext)
      return IDS_OE_EOF;

   iPageSize = m_rect.bottom / m_iLineHeight;
   for (i=0;(i < iPageSize);i++)
      {
      pLine = m_pCurLine->pNext;
      if (pLine)
         {
         m_iFirstLineNo++;
         m_iLineNo++;
         m_pCurLine = pLine;
         m_pTopLine = m_pTopLine->pNext;
         }
      }

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
		UpdateWindow();
      MakeCursorVisible();
      }

   UpdateViews(this, OE_SYNCH_SPLITTERS);

   return 0;
   }

//@doc SECEditCore
//@mfunc Moves the view one page (screen) up, without resetting
//              the caret position
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::ScrollUpPage(BOOL bUpdate)
   {
   PLINEDESC pLine;
   int iPageSize,i,iOldFirstLineNo;

   if (!m_pTopLine->pPrev)
      return IDS_OE_ATTOP;

   iPageSize = m_rect.bottom / m_iLineHeight;
	iOldFirstLineNo = m_iFirstLineNo;

   for (i=0;(i < iPageSize);i++)
      {
      pLine = m_pTopLine->pPrev;
      if (pLine)
         {
         m_iFirstLineNo--;
         m_pTopLine = pLine;
         }
      }

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
      UpdateWindow();
      }

   UpdateViews(this, OE_SYNCH_SPLITTERS);

	MySetCaretPos();
   return 0;
   }


//@doc SECEditCore
//@mfunc Moves the view one page (screen) down, without resetting
//              the caret position
//@rdesc int 0 on success, errorcode on failure
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::ScrollDownPage(BOOL bUpdate)
   {
   PLINEDESC pLine;
   int iPageSize,i;

   if (!m_pTopLine->pNext)
      return IDS_OE_EOF;

   iPageSize = m_rect.bottom / m_iLineHeight;
   for (i=0;(i < iPageSize);i++)
      {
      pLine = m_pTopLine->pNext;
      if (pLine)
         {
         m_iFirstLineNo++;
         m_pTopLine = pLine;
         }
      }

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
		UpdateWindow();
      }

	MySetCaretPos();
   return 0;
   }

//@doc SECEditCore
//@mfunc Moves line #iPos into view, without resetting the caret
//@rdesc int 0 on succes, errorcode on failure.
//@parm int iPos Line number
//@parm  BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::ScrollGoToLine(int iPos, BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   PLINEDESC pLine;
   int i;

	i = 0;
	m_pTopLine = pLine = pEdit->ListGetFirst(m_pTopLine, NULL);
	while (pLine && (i < iPos))
		{
		m_pTopLine = pLine;
		i++;
		pLine = pLine->pNext;
		}

	m_iFirstLineNo = i;

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
		UpdateWindow();
      }

	MySetCaretPos();
   return 0;
   }


//@doc SECEditCore
//@mfunc Moves the caret to the top of the file
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display.

template <class BC>
int SECEditCore<BC>::TopOfFile(BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   if (!m_pCurLine->pPrev && !m_iColNo)
      return IDS_OE_ATTOP;

   CountLines();
   m_pCurLine = m_pTopLine = pEdit->ListGetFirst(m_pCurLine, NULL);
   m_iColNo = 0;
   m_iLineNo = m_iFirstLineNo = 1;
   m_iLeftEdge = 0;

   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
      MakeCursorVisible();
      }

   return 0;
   }


//@doc SECEditCore
//@mfunc Moves the caret to the bottom of the file
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
 int SECEditCore<BC>::BottomOfFile(BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   PLINEDESC pLine;
   int i,iPageSize;

   CountLines();
   pLine = pEdit->ListGetLast(m_pCurLine);
   m_pTopLine = m_pCurLine = pLine;
   m_iLineNo = m_iFirstLineNo =
       pEdit->m_iLinesInFile;
   iPageSize = (m_rect.bottom / m_iLineHeight) - 1;
   for (i=0;i<iPageSize;i++)
      {
      pLine = m_pTopLine->pPrev;
      if (pLine)
         {
         m_pTopLine = pLine;
         m_iFirstLineNo--;
         }
      else
         break;
      }
   m_iColNo = 0;
   m_iLeftEdge = 0;

   EndOfLine(FALSE);
   if (bUpdate)
      {
      InvalidateRect(NULL,FALSE);
      MakeCursorVisible();
      }
   return 0;
   }

//@doc SECEditCore
//@mfunc Move the caret one word to the left.  See also 
//              SetExtraChars() and MyIsAlnum()
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::WordLeft(BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   PLINEDESC pLine;
   LPSTR szText;
   int iLength;
	int iPos;

   if (m_iColNo == 0)
      {
      pLine = m_pCurLine->pPrev;
      if (pLine)
         /* move to end of previous line */
         CursorLeft(bUpdate);
      else
         return IDS_OE_ATTOP;
      }

   pLine = m_pCurLine;
   szText = pLine->pText;
   if (szText)
      iLength = strlen(szText);
   else
      return IDS_OE_NOTEXT;

   if (iLength)
      {
      /* (maybe) adjust for OE_CR/OE_LF */
      if (szText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
         iLength -= pEdit->m_iLineEndLength;
      }

   /* just go to the beginning of the line if iLength == 0 */
   if (!iLength)
      {
      BeginningOfLine(TRUE);
      return 0;
      }

	iPos = pEdit->GetRealOffset(pLine,m_iColNo);

   /* we want to start working one char to the left of wColNo */
   if (iLength > iPos)
      iLength = iPos;

   /* if we're at the beginning of a word, back up
      into the previous word */
//      while ((iLength-1) && (!isalnum(szText[iLength-1]) && (szText[iLength-1] != '_')))
     while ((iLength-1) && (!pEdit->Myisalnum(szText[iLength-1]) && 
            (szText[iLength-1] != '_') && (szText[iLength-1] != '.')))
      iLength--;

   /* now iLength-1 points to a non-whitespace char, or it's zero.
      back up to the beginning of the word */
//      while ((iLength-1) && (isalnum(szText[iLength-1]) || (szText[iLength-1] == '_')))
     while ((iLength-1) && (pEdit->Myisalnum(szText[iLength-1]) ||
            (szText[iLength-1] == '_') || (szText[iLength-1] == '.')))
      iLength--;

   iPos = iLength;
	if (iPos == 1)
		{
		if (!isspace(szText[iPos-1]))
			iPos = 0;
		else
			{
			pLine = m_pCurLine->pPrev;
			if (pLine)
				{
				/* move to end of previous line */
				UpLine(bUpdate);
				EndOfLine(bUpdate);
				return 0;
				}
			else
				return IDS_OE_ATTOP;
			}
		}

	m_iColNo = pEdit->GetTabbedOffset(pLine,iPos);

   if (m_iLeftEdge > m_iColNo)
      {
      m_iLeftEdge = m_iColNo;
      if (bUpdate)
         InvalidateRect(NULL,FALSE);
      }

   if (bUpdate)
		MakeCursorVisible();

   return 0;
   }

//@doc SECEditCore
//@mfunc Moves the caret one word to the right.  See also 
//              SetExtraChars() and MyIsAlnum()
//@rdesc int 0 on success, errorcode on failure.
//@parm BOOL bUpdate Update the display?

template <class BC>
int SECEditCore<BC>::WordRight(BOOL bUpdate)
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   PLINEDESC pLine;
   PLINEDESC pCurLine;
   LPSTR szText;
   int iLength,iPos;

   pCurLine = m_pCurLine;
   szText = pCurLine->pText;
   if (szText)
      iLength = strlen(szText);
   else
      iLength = 0;

   if (iLength>1)
      {
      /* (maybe) adjust for OE_CR/OE_LF */
      if (szText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
         iLength -= pEdit->m_iLineEndLength;
      }

   /* if we're at the end of the line, go to the next line */
   iPos = pEdit->GetTabbedOffset(m_pCurLine,iLength);
   if (m_iColNo >= iPos)
      {
      pLine = pCurLine->pNext;
      if (pLine)
         {
         /* move to the beginning of the next line */
         DownLine(bUpdate);
         BeginningOfLine(bUpdate);
         return 0;
         }
      else
         {
         EndOfLine(bUpdate);
         return 0;
         }
      }

   /*  non-word:
    *     (!isalnum(szText[iLength-1]) && (szText[iLength-1] != '_')
    */
    
   /*  word:
    *     (isalnum(szText[iLength-1]) || (szText[iLength-1] == '_')
    */

   iPos = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
   /* are we sitting in some whitespace? */
   if ((!pEdit->Myisalnum(szText[iPos]) &&
        (szText[iPos] != '_') &&
        (szText[iPos] != '.')
      ))
      {
      /* get past any whitespace */
      while (
         (iLength > iPos) &&
             (
              (
                 !pEdit->Myisalnum(szText[iPos]) &&
                 (szText[iPos] != '_') &&
                 (szText[iPos] != '.')
              )
             )
         )
         iPos++;
      }
   else
      {
      /* we're sitting on a character.  get past that, and then
         try for a non-space after it. */
      while ((iLength > iPos) &&
             ((pEdit->Myisalnum(szText[iPos]) ||
              (szText[iPos] == '_') ||
              (szText[iPos] == '.')) ))
         iPos++;
      while ((iLength > iPos) &&
             ((!pEdit->Myisalnum(szText[iPos]) && (szText[iPos] != '_')
               && (szText[iPos] != '.'))))
         iPos++;
      }

   /* now m_iColNo points to a non-whitespace char,
      or it's at the end of the line.  If we're at the end of the line,
      advance to the beginning of the next line */
   if (iLength && (iPos >= iLength))
      {
      pLine = pCurLine->pNext;
      if (pLine)
         {
         /* move to the beginning of the next line */
         DownLine(bUpdate);
         BeginningOfLine(bUpdate);
         }
      return 0;
      }

	m_iColNo = pEdit->GetTabbedOffset(m_pCurLine,iPos);
   if (bUpdate)
      MakeCursorVisible();

	return 0;
   }



//@doc SECEditCore
//@mfunc Moves the caret to the point passed in
//@rdesc int 0 on success, errorcode on failure.
//@parm int x X Coordinate
//@parm  int y Y Coordinate

template <class BC>
int SECEditCore<BC>::MouseGoTo(int x, int y)
   {
   PLINEDESC pLine;
   int nNumLines,nNumChars;
   int nTargetCol,iTargetLine;

   m_iLineNo = m_iFirstLineNo;
   m_pCurLine = m_pTopLine;
   m_iColNo = m_iLeftEdge;

   nNumChars = m_rect.right / m_iCharWidth;
   nNumLines = m_rect.bottom / m_iLineHeight;

   if (x < m_iMarkSpace)
		{
		x = m_iMarkSpace;
		if (m_iLeftEdge)
			{
			m_iLeftEdge--;
	      InvalidateRect(NULL,FALSE);
			}
		}
   nTargetCol = ( (x-m_iMarkSpace) + (m_iCharWidth / 2) ) / m_iCharWidth;

   iTargetLine = y / m_iLineHeight;
   if (((iTargetLine*m_iLineHeight)+m_iLineHeight) > m_rect.bottom)
      iTargetLine--;

   pLine = m_pCurLine;
   if (!pLine)
      return IDS_OE_NOTEXT;

   while (pLine && (iTargetLine !=
                    (m_iLineNo-m_iFirstLineNo)))
      {
      pLine = pLine->pNext;
      if (pLine)
         {
         m_iLineNo++;
         m_pCurLine = pLine;
         }
      else
         iTargetLine--;
      }

	int iPos = nTargetCol+m_iLeftEdge;
	if (iPos > OE_MAXLINELENGTH)
		iPos = OE_MAXLINELENGTH;
   m_iColNo = GetValidColNo(iPos);

   MakeCursorVisible();
   return 0;
   }


//@doc SECEditCore
//@mfunc Override this member to customize status line updates

template <class BC>
void SECEditCore<BC>::UpdateStatusLine(void)
   {
   }


//@doc SECEditCore
//@mfunc Scrolls the line and column containing the caret into view
//@rdesc void 

template <class BC>
void SECEditCore<BC>::MakeCursorVisible()
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   int nOldLeftEdge,nNumChars,nNumLines;
   int iOldFirstLineNo;

   if (!m_iCharWidth || !m_iLineHeight)
      return;

   // range enforcing
   if (m_iColNo < 0)
	   m_iColNo = 0;

   // CCCC - ET Bug Fix
   nNumChars = ( m_rect.right / m_iCharWidth ) - pEdit->m_iTabSize;
   // Safety
   nNumChars = max( 0, nNumChars );
   // Old Line
   // nNumChars = m_rect.right / m_iCharWidth;
   // CCCC

   nNumLines = m_rect.bottom / m_iLineHeight;
   if (!nNumLines || !nNumChars)
      return;

   nOldLeftEdge = m_iLeftEdge;
   iOldFirstLineNo = m_iFirstLineNo;

   while (m_iLeftEdge > m_iColNo)
      m_iLeftEdge -= min(pEdit->m_iTabSize,m_iLeftEdge);

   while ((m_iColNo - m_iLeftEdge) >= nNumChars-1)
      m_iLeftEdge += pEdit->m_iTabSize;

   if  ((m_iFirstLineNo > m_iLineNo) || 
        ((m_iLineNo-m_iFirstLineNo) >= nNumLines))
      {
      /* we need to shrink the distance between topline & curline */
      m_pTopLine = m_pCurLine;
      m_iFirstLineNo = m_iLineNo;
      }

	if (!m_badrect.IsRectEmpty())
		{
		// try to make m_pCurLine outside m_badrect
		ScreenToClient(&m_badrect);
      CRect rect = m_rect;
      rect.top = (m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
      rect.bottom = rect.top+m_iLineHeight;
		if ((rect.top > m_badrect.top) && (rect.bottom < m_badrect.bottom))
			{
			int linesabove = m_badrect.top / m_iLineHeight;
			int linesbelow = (m_rect.bottom - m_badrect.bottom) / m_iLineHeight;
			if (linesabove > linesbelow)
				{
            m_pTopLine = m_pCurLine;
            m_iFirstLineNo = m_iLineNo;
				}
			else
				{
				// increase the distance between topline and curline til it fits
				while ((rect.top < m_badrect.bottom) && m_pTopLine->pPrev)
					{
					m_pTopLine = m_pTopLine->pPrev;
					m_iFirstLineNo--;
               rect.top = (m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
					}
				}
			}
		m_badrect.SetRectEmpty();
		}

   if (m_bUpdate && ((iOldFirstLineNo != m_iFirstLineNo) || (nOldLeftEdge != m_iLeftEdge)))
      InvalidateRect(NULL,FALSE);

	if (m_bUpdate)
		{
		// CountLines();
		int iMin,iMax;
		MyGetScrollRange(SB_VERT, &iMin, &iMax);
		if (iMax != (pEdit->m_iLinesInFile-1))
			MySetScrollRange(SB_VERT, 0, pEdit->m_iLinesInFile-1, m_bUpdate);
		if (MyGetScrollPos(SB_HORZ) != m_iLeftEdge)
			MySetScrollPos(SB_HORZ,m_iLeftEdge,TRUE);
		//if (MyGetScrollPos(SB_VERT) != m_iFirstLineNo-1)
			MySetScrollPos(SB_VERT,m_iFirstLineNo-1,TRUE);
		MySetCaretPos();
		UpdateStatusLine();
		}
   }

//@doc SECEditCore
//@mfunc Counts the number of lines in the document
//@rdesc int The number of lines in the document.

template <class BC>
int SECEditCore<BC>::CountLines()
   {
   int i;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (!m_pTopLine)
      return 0;

   i = pEdit->CountLines(m_pTopLine);

   if (i != pEdit->m_iLinesInFile)
		{
		pEdit->m_iLinesInFile = i;
		MySetScrollRange(SB_VERT, 0, i-1, m_bUpdate);
		}

   return i;
   }


//@doc SECEditCore
//@mfunc Sets the caret position - useful as an example of how to set the caret position if you need to do this manually.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::MySetCaretPos()
   {
	int nHPos = ((m_iColNo-m_iLeftEdge)*m_iCharWidth)+m_iMarkSpace;
	if (nHPos < m_iMarkSpace)
		nHPos = -10;  // send it offscreen
   if (m_bHaveCaret) {
      DestroyCaret();
	   ::CreateCaret(m_hWnd,NULL,2,m_iLineHeight);
   }
   SetCaretPos(CPoint(nHPos,
               ((m_iLineNo-m_iFirstLineNo)*m_iLineHeight)));
   if (m_bHaveCaret) {
     	ShowCaret();
   }
   }



//@doc SECEditCore
//@mfunc Sets the scroll range - useful as an example of how to do it if you want to customize the scrolling range.
//@rdesc void 
//@parm int fnBar Index of the scroll bar.
//@parm  int iMin Minimum
//@parm  int iMax Maximum
//@parm  BOOL bRedraw Redraw the view?

template <class BC>
void SECEditCore<BC>::MySetScrollRange(int fnBar, int iMin, int iMax, BOOL bRedraw)
   {
	CScrollBar* pBar;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	m_iVScale = ((pEdit->m_iLinesInFile-1) / 0xFFFF) + 1;
	int iScale = (fnBar == SB_VERT ? m_iVScale : 1);

	pBar = GetScrollBarCtrl(fnBar);

	if (!m_iLineHeight)
		return;

   SCROLLINFO si;
	memset(&si,0,sizeof(si));

   int iPageSize;

	if (fnBar == SB_VERT)
		{
		if (!iMax)
			iMax = 1;
		iPageSize = m_rect.bottom / m_iLineHeight;
		if (!iPageSize)
			{
			iPageSize = 1;
			iMax = 2;
			}

		// if the scrollbar is going to be disabled, bring m_pTopLine into view
		if (m_pTopLine && (iPageSize >= pEdit->m_iLinesInFile))
			{
			if (m_iFirstLineNo > 1)
				{
				m_pTopLine = pEdit->ListGetFirst(m_pTopLine,NULL);
				m_iFirstLineNo = 1;
				Invalidate();
				MySetCaretPos();
				}
			}

	   si.fMask = SIF_RANGE|SIF_PAGE|SIF_DISABLENOSCROLL;
		}
	else
		{
		iPageSize = 32;
	   si.fMask = SIF_RANGE|SIF_PAGE;
		}
	si.nMax = iMax / iScale;
   si.cbSize = sizeof(si);
   si.nMin = iMin;
   si.nPage = iPageSize / iScale;

	if (iPageSize)
		if (pBar)
			pBar->SetScrollInfo(&si,bRedraw);
		else
			SetScrollInfo(fnBar,&si,bRedraw);
   }

//@doc SECEditCore
//@mfunc Sets the scroll position
//@rdesc void 
//@parm int fnBar Index of the bar (vert or horz)
//@parm  int iPos Position to scroll to.
//@parm  BOOL bRedraw Redraw the display?

template <class BC>
void SECEditCore<BC>::MySetScrollPos(int fnBar, int iPos, BOOL bRedraw)
   {
	CScrollBar* pBar;
	int iScale = (fnBar == SB_VERT ? m_iVScale : 1);

	pBar = GetScrollBarCtrl(fnBar);

   SCROLLINFO si;
   memset(&si, 0, sizeof(si));

   si.cbSize = sizeof(si);
   si.fMask = SIF_POS;
   si.nPos = iPos / iScale;

	if (pBar)
		pBar->SetScrollInfo(&si,bRedraw);
	else
		SetScrollInfo(fnBar,&si,bRedraw);

   }


//@doc SECEditCore
//@mfunc Gets the scroll position
//@rdesc int position of scrollbar 
//@parm int fnBar Index of the bar (vert or horz)

template <class BC>
int SECEditCore<BC>::MyGetScrollPos(int fnBar)
   {
	CScrollBar* pBar;
	int iScale = (fnBar == SB_VERT ? m_iVScale : 1);

	pBar = GetScrollBarCtrl(fnBar);

   SCROLLINFO si;
   memset(&si, 0, sizeof(si));

   si.cbSize = sizeof(si);
   si.fMask = SIF_POS;

	if (pBar)
		pBar->GetScrollInfo(&si);
	else
		GetScrollInfo(fnBar,&si,si.fMask);

	return si.nPos * iScale;
   }


//@doc SECEditCore
//@mfunc Gets the current scroll range
//@rdesc void 
//@parm int fnBar Index of the bar.
//@parm  int *iMin Minimum of the range.
//@parm  int *iMax Maximum of the range.

template <class BC>
void SECEditCore<BC>::MyGetScrollRange(int fnBar, int *iMin, int *iMax)
   {
	CScrollBar* pBar;
	int iScale = (fnBar == SB_VERT ? m_iVScale : 1);
	pBar = GetScrollBarCtrl(fnBar);

   SCROLLINFO si;
   memset(&si, 0, sizeof(si));

   si.cbSize = sizeof(si);
   si.fMask = SIF_RANGE;

	if (pBar)
		pBar->GetScrollInfo(&si);
	else
		GetScrollInfo(fnBar,&si,si.fMask);
   *iMin = si.nMin;
   *iMax = si.nMax * iScale;
   }

#endif //__VIEWCURSOR_INL__