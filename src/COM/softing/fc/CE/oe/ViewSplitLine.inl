#ifndef __VIEWSPLITLINE_INL__
#define __VIEWSPLITLINE_INL__
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
//@mfunc Break the current line at the caret position and splice
//              any trailing text into a new line.
//@rdesc int 0 on success, errorcode on failure
//@parm BOOL bUpdate Update the display?
//@parm  BOOL bSaveUndoRecord Save undo information for this operation?
template <class BC>
int SECEditCore<BC>::SplitLine(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   TEXTPOS TextPos;
   RECT rect;
   int iRval;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	pEdit->SetUndoGroup(TRUE);

   if (pEdit->m_bReadOnly)
      return IDS_OE_READONLY;

   if (bUpdate)
      MakeCursorVisible();
   TextPos.pLine = m_pCurLine;
   TextPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
   iRval = pEdit->SplitLine(&TextPos,bSaveUndoRecord);
   if (iRval)
		{
		pEdit->SetUndoGroup();
      return iRval;
		}

	DownLine(bUpdate);
   //m_pCurLine = m_pCurLine->pNext;
   //m_iLineNo++;
   m_iColNo = 0;

	if (pEdit->m_bAutoIndent)
		AutoIndent(bSaveUndoRecord);

   if (bUpdate)
      {
      rect = m_rect;
      rect.top = ((m_iLineNo-1) - m_iFirstLineNo)*m_iLineHeight;
      InvalidateRect(&rect,FALSE);
      UpdateWindow();
      CountLines();
      MakeCursorVisible();
	  UpdateViews(this,NULL,NULL);
      }

	pEdit->SetUndoGroup();

   return 0;
   }

//@doc SECEditCore
//@mfunc An internal implementation function that determines if the line has text or not.
//@rdesc BOOL returns TRUE if there is text, FALSE if not.
//@parm PLINEDESC pLine The line to query.
//@parm  int& iFirst The first character to start at.
template <class BC>
BOOL SECEditCore<BC>::HasText(PLINEDESC pLine, int& iFirst)
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	int iLength = pLine->iTxtSize;
	if (!iLength)
		return FALSE;
	else
		{
		if (pLine->pText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
			iLength -= pEdit->m_iLineEndLength;
		for (int i = 0; i < iLength; i++)
			{
			if (!isspace(pLine->pText[i]))
				{
				iFirst = i;
				return TRUE;
				}
			}
		}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  non-smart version:  line up the caret (and any trailing text)
//                      with the first non-blank character in the 
//                      line above

//@doc SECEditCore
//@mfunc line up the caret (and any trailing text)
//                      with the first non-blank character in the 
//                      line above. 
//@rdesc void 
//@parm BOOL bSaveUndoRecord Should we save undo information?

template <class BC>
void SECEditCore<BC>::AutoIndent(BOOL bSaveUndoRecord)
{
	USES_CONVERSION_T;

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	PLINEDESC pLine = m_pCurLine->pPrev;
	int i;
	while (pLine && !HasText(pLine,i))
		pLine = pLine->pPrev;
	if (!pLine)
		return;
	int iTargetCol = 0;
	while (isspace(pLine->pText[iTargetCol]))
		iTargetCol++;
	iTargetCol = pEdit->GetTabbedOffset(pLine,iTargetCol);
	int iFirst;
	if (HasText(m_pCurLine, iFirst))
		{
		iFirst = pEdit->GetTabbedOffset(pLine,iFirst);

		TEXTPOS pos;
		pos.pLine = m_pCurLine;
		pos.iOffset = 0;

		if (iFirst > iTargetCol)
			pEdit->DeleteString(&pos,iFirst - iTargetCol,bSaveUndoRecord);
		else if (iTargetCol && (iTargetCol-iFirst))
			{
			CString str;
			LPTSTR p = str.GetBuffer(iTargetCol);
			if (pEdit->m_bKeepTabs)
				{
				int iNumTabs = (iTargetCol - iFirst) / pEdit->m_iTabSize;
				int iNumSpaces = (iTargetCol - iFirst) % pEdit->m_iTabSize;
				memset(p,9,iNumTabs);
				memset(&p[iNumTabs],' ',iNumSpaces);
				p[iNumTabs+iNumSpaces] = 0;
				}
			else
				{
				memset(p,' ',iTargetCol - iFirst);
				p[iTargetCol-iFirst] = 0;
				}
			if (p[0])
				pEdit->InsertString(&pos,T2A(p),TRUE,bSaveUndoRecord);
			str.ReleaseBuffer();
			}
		}
	
	m_iColNo = iTargetCol;

	}


#endif //__VIEWSPLITLINE_INL__