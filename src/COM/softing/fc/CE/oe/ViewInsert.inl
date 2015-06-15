#ifndef __VIEWINSERT_INL__
#define __VIEWINSERT_INL__
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
//@mfunc Inserts a character into the current line at the current
//              position
//@rdesc int 0 on success, errorcode on failure.
//@parm int iChar The character to insert.
//@parm  BOOL bUpdate Update the display?
//@parm  BOOL bSaveUndoRecord Save this information in the undo buffer?

template <class BC>
int SECEditCore<BC>::InsertChar(int iChar, BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   RECT rect;
   int i;
   TEXTPOS TextPos;
   SECEdit* pEdit = GetEdit();
   ASSERT(pEdit != NULL);

	pEdit->SetUndoGroup(TRUE);

	if (pEdit->GetTabbedOffset(m_pCurLine,m_pCurLine->iTxtSize) >= OE_MAXLINELENGTH)
		{
		pEdit->SetUndoGroup(FALSE);
		return IDS_OE_LINETOOLONG;
		}

   // if there's a block marked, cut it
   Cut(bUpdate,bSaveUndoRecord);

   TextPos.pLine = m_pCurLine;
   TextPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
   i = pEdit->InsertChar(&TextPos, iChar, pEdit->m_bInsertMode, bSaveUndoRecord);

   // fix our updaterect
   if (!i)
		{
		if (bUpdate)
			{
			if (pEdit->m_bChroma && 
				 ((iChar == '"') || (iChar == '\'') || EndedAComment(pEdit,iChar)))
				{
				// if this char completes the beginning or end
				// of a multiline comment, or a quote might have
				// changed things, do a complete redraw
				InvalidateRect(NULL,FALSE);
				}
			else
				{
				rect = m_rect;
				rect.top = (int)(m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
				rect.bottom = rect.top+m_iLineHeight;
				InvalidateRect(&rect,FALSE);
				}
			UpdateWindow();
			UpdateViews(this,NULL,NULL);
			}

	   CursorRight(bUpdate);
		}

	pEdit->SetUndoGroup();

   return i;
   }



//@doc SECEditCore
//@mfunc Internal method that determines if the character just added into the 
//              current line may have completed either the beginning
//              or the end of a multiline comment
//@rdesc BOOL True if it is, false if it doesn't.
//@parm SECEdit* pEdit Pointer to the SECEdit class for this SECEditCore
//@parm  int iChar The character.
template <class BC>
BOOL SECEditCore<BC>::EndedAComment(SECEdit* pEdit, int iChar)
{
	int i;
	int iFirst1 = 0;
	int iFirst2 = 0;
	int iLast1 = 0;
	int iLast2 = 0;

	if (pEdit->szCommentEnd1[0])
		{
		i = strlen(pEdit->szCommentEnd1);
		if (i)
			iLast1 = pEdit->szCommentEnd1[i-1];
		i = strlen(pEdit->szCommentStart1);
		if (i)
			iFirst1 = pEdit->szCommentStart1[i-1];
		}
	if (pEdit->szCommentEnd2[0])
		{
		i = strlen(pEdit->szCommentEnd2);
		if (i)
			iLast2 = pEdit->szCommentEnd2[i-1];
		i = strlen(pEdit->szCommentStart2);
		if (i)
			iFirst2 = pEdit->szCommentStart2[i-1];
		}

	if ((iChar == iLast1) || (iChar == iLast2) ||
		 (iChar == iFirst1) || (iChar == iFirst2))
		return TRUE;

	return FALSE;	
}


//@doc SECEditCore
//@mfunc Inserts a string into the current line at the current position
//@rdesc int 0 on succes, errorcode on failure.
//@parm LPTSTR lpString The string to insert.
//@parm  BOOL bUpdate Update the display?
//@parm  BOOL bSaveUndoRecord Update the undo buffer?

template <class BC>
int SECEditCore<BC>::InsertString(LPTSTR lpString, BOOL bUpdate, BOOL bSaveUndoRecord)
   {
	USES_CONVERSION_T;

   int i;
   TEXTPOS TextPos;
   RECT rect;
   SECEdit* pEdit = GetEdit();
   ASSERT(pEdit != NULL);

	pEdit->SetUndoGroup(TRUE);

	if ((pEdit->GetTabbedOffset(m_pCurLine,m_pCurLine->iTxtSize) + _tcslen(lpString)) >= OE_MAXLINELENGTH)
		{
		pEdit->SetUndoGroup(FALSE);
		return IDS_OE_LINETOOLONG;
		}

   // if there's a block marked, cut it
   Cut(bUpdate,bSaveUndoRecord);

   TextPos.pLine = m_pCurLine;
   TextPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
   
// #326 08.12.06 SIS >>
// confusion between real and tabbed offset
// cursor position is wrong after inserting string
// additionally auto-indent trick was not considered
// see comments below
#ifdef _ORIGINAL_VERSION
   i = pEdit->InsertString(&TextPos,T2A(lpString),pEdit->m_bInsertMode,bSaveUndoRecord);

   m_iColNo += _tcslen(lpString);
#else
   // remember current text size before inserting string to see if line is empty
   // text size is 2 in this case (CRLF)
   int	iCurSize = m_pCurLine->iTxtSize;

   i = pEdit->InsertString(&TextPos,T2A(lpString),pEdit->m_bInsertMode,bSaveUndoRecord);

   int	iRealCol = TextPos.iOffset;
   // caution in auto indent mode:
   // if line was empty, spaces were padded by InsertString()
   // iRealCol indeed is tabbed col in this case and must be corrected
   if(pEdit->m_bAutoIndent && iCurSize == pEdit->m_iLineEndLength)
   {
	   iRealCol = pEdit->GetRealOffset(m_pCurLine, iRealCol);
   }
   iRealCol += _tcslen(lpString);
   m_iColNo = pEdit->GetTabbedOffset(m_pCurLine, iRealCol);	// miColNo is tabbed offset
#endif
// #326 08.12.06 SIS <<

   if (!i && bUpdate)
      {
      /* fix our updaterect */
      rect = m_rect;
      rect.top = (int)(m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
      rect.bottom = rect.top+m_iLineHeight;

      InvalidateRect(&rect,FALSE);
      UpdateWindow();
      MakeCursorVisible();
	  UpdateViews(this,NULL,NULL);
      }
	pEdit->SetUndoGroup();
   return 0;
   }


#endif //__VIEWINSERT_INL__