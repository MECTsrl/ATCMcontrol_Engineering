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
//@mfunc Internal function that initializes the UNDO buffer used by Objective Edit.
//@rdesc void 
//@parm void
void SECEdit::InitUndo(void)
   {
	m_iRedoPos = m_iUndoPos = 0;
	m_iUndoCount = m_iUndoStart = m_dwUndoTimestamp = 0;
	}

//@doc SECEdit
//@mfunc Internal functin that frees any allocated memory in the undo buffer.
//@rdesc void 
//@parm void
void SECEdit::FreeUndoBuffers(void)
   {
   PUNDORECORD lpUndoBuffer,lpStart;
   int i;

   lpStart = lpUndoBuffer = m_pUndoBuffer;

   for (i=0;i<OE_MAXUNDO;i++)
      {
      if ((lpUndoBuffer->iUndoType == OE_UNDO_CUT) ||
          (lpUndoBuffer->iUndoType == OE_UNDO_PASTE) ||
			 (lpUndoBuffer->iUndoType == OE_UNDO_COLUMNCUT) ||
			 (lpUndoBuffer->iUndoType == OE_UNDO_COLUMNPASTE) ||
			 (lpUndoBuffer->iUndoType == OE_UNDO_MERGE))
			if (lpUndoBuffer->lpBuffer)
            free(lpUndoBuffer->lpBuffer);

			lpUndoBuffer->lpBuffer = NULL;
			lpUndoBuffer->iUndoType = 0;

      lpUndoBuffer++;
      }

	m_iRedoPos = m_iUndoPos = 0;
	m_iUndoCount = m_iUndoStart = 0;
	}


//@doc SECEdit
//@mfunc Internal function. The undo list is circular.  If we've wrapped, whack
//              off the 512 oldest records.
//
//@rdesc BOOL 
//@parm void
BOOL SECEdit::ShrinkUndoStack(void)
   {
   PUNDORECORD lpUndoBuffer,lpStart;
   int i;

   lpStart = lpUndoBuffer = m_pUndoBuffer;

   for (i=0;i<512;i++)
      {
      if ((lpUndoBuffer->iUndoType == OE_UNDO_CUT) ||
          (lpUndoBuffer->iUndoType == OE_UNDO_PASTE) ||
			 (lpUndoBuffer->iUndoType == OE_UNDO_COLUMNCUT) ||
			 (lpUndoBuffer->iUndoType == OE_UNDO_COLUMNPASTE) ||
			 (lpUndoBuffer->iUndoType == OE_UNDO_MERGE))
			{
			if (lpUndoBuffer->lpBuffer)
	         free(lpUndoBuffer->lpBuffer);
			lpUndoBuffer->lpBuffer = NULL;
			lpUndoBuffer->iUndoType = 0;
			}

      lpUndoBuffer++;
      }

   /* the undo buffer now points at the first record we'll save */
   memmove(lpStart,lpUndoBuffer,sizeof(UNDORECORD)*(OE_MAXUNDO-512));

	// clear the cobwebs out of the end of the buffer
	lpStart += (OE_MAXUNDO-512);
	memset(lpStart,0,sizeof(UNDORECORD)*512);

   m_iUndoPos -= 512;
   m_iUndoCount -= 512;

   return TRUE;
   }


//@doc SECEdit
//@mfunc Internal function. Saves an undo record.
//@rdesc BOOL 
//@parm PUNDORECORD pUndoRecord
//@parm  LPCTSTR lpText

BOOL SECEdit::SaveUndoRecord(PUNDORECORD pUndoRecord, LPCSTR lpText)
   {
   PUNDORECORD lpUndoBuffer;
   int i;
	ASSERT(pUndoRecord);

   if (m_iUndoCount == OE_MAXUNDO)
      ShrinkUndoStack();

   lpUndoBuffer = m_pUndoBuffer;

   /* wUndoCount points us to the next available slot */
   lpUndoBuffer += m_iUndoPos;

   /* every record has at least these items */
   lpUndoBuffer->iUndoType		= pUndoRecord->iUndoType;
   lpUndoBuffer->iRow			= pUndoRecord->iRow;
   lpUndoBuffer->iCol			= pUndoRecord->iCol;
   lpUndoBuffer->dwTimestamp	= GetUndoTimestamp();

	if (lpUndoBuffer->lpBuffer)
		{
      free(lpUndoBuffer->lpBuffer);
		lpUndoBuffer->lpBuffer = NULL;
		}

   switch (pUndoRecord->iUndoType)
      {
		case OE_UNDO_MERGE:
      case OE_UNDO_CUT:
      case OE_UNDO_PASTE:
      case OE_UNDO_COLUMNCUT:
      case OE_UNDO_COLUMNPASTE:
         lpUndoBuffer->lpBuffer = (LPSTR)SECEditAlloc(strlen(lpText)+1);
         if (lpUndoBuffer->lpBuffer)
            {
            strcpy(lpUndoBuffer->lpBuffer,lpText);
            }
         else
            {
            return FALSE;
            }
         lpUndoBuffer->iEndRow = pUndoRecord->iEndRow;
         lpUndoBuffer->iEndCol = pUndoRecord->iEndCol;

			// an empty block doesn't get saved...
			if ((lpUndoBuffer->iEndRow == pUndoRecord->iRow) &&
				 (lpUndoBuffer->iEndCol == pUndoRecord->iCol))
				{
				free(lpUndoBuffer->lpBuffer);
				lpUndoBuffer->lpBuffer = NULL;
				}
         //swap the row/columns if it was marked from bottom to top
         // we want it to be top to bottom for the undo paste
         if (lpUndoBuffer->iRow > lpUndoBuffer->iEndRow)
            {
            int iSaveStartRow;
            int iSaveStartCol;

            iSaveStartRow = lpUndoBuffer->iRow;
            iSaveStartCol = lpUndoBuffer->iCol;
            lpUndoBuffer->iRow = lpUndoBuffer->iEndRow;
            lpUndoBuffer->iCol = lpUndoBuffer->iEndCol;
            lpUndoBuffer->iEndRow = iSaveStartRow;
            lpUndoBuffer->iEndCol = iSaveStartCol;
            }
         break;

      case OE_UNDO_JOINLINE:
      case OE_UNDO_NEWLINE:
         lpUndoBuffer->iEndRow = pUndoRecord->iEndRow;
         lpUndoBuffer->iEndCol = pUndoRecord->iEndCol;
         break;

		case OE_UNDO_INS_OVER:
			lpUndoBuffer->cOvertypeChar = pUndoRecord->cOvertypeChar;
		case OE_UNDO_INSERT:
		case OE_UNDO_DELETE:
			lpUndoBuffer->cInsertChar = pUndoRecord->cInsertChar;
			break;

      default:
			// we already have all the info we need
         break;
      }

   /* if we're throwing away some undo records, free
      their memory
   */
   if (m_iUndoCount > (m_iUndoPos+1))
      {
      lpUndoBuffer = m_pUndoBuffer;

      lpUndoBuffer += m_iUndoPos+1;

      for (i=(m_iUndoPos+1);i<m_iUndoCount;i++)
         {
			if ((lpUndoBuffer->iUndoType == OE_UNDO_CUT) ||
				 (lpUndoBuffer->iUndoType == OE_UNDO_PASTE) ||
				 (lpUndoBuffer->iUndoType == OE_UNDO_COLUMNCUT) ||
				 (lpUndoBuffer->iUndoType == OE_UNDO_COLUMNPASTE) ||
				 (lpUndoBuffer->iUndoType == OE_UNDO_MERGE))
				{
				if (lpUndoBuffer->lpBuffer)
					{
					free(lpUndoBuffer->lpBuffer);
					lpUndoBuffer->lpBuffer = NULL;
					}
				}

         lpUndoBuffer++;
         }

      }

   m_iUndoCount = ++m_iUndoPos;

   return TRUE;
   }

//@doc SECEdit
//@mfunc Get a timestamp for the current undo record.  Undo and Redo group records based on the timestamp.
//
//@rdesc DWORD
DWORD SECEdit::GetUndoTimestamp()
{
	return m_dwUndoTimestamp;
}

//@doc SECEdit
//@mfunc Set a timestamp for the current undo record.  Undo and Redo group records based on the timestamp.
//
//@rdesc void
void SECEdit::SetUndoGroup(BOOL bStart /* = FALSE */)
{
	static int nState = 0;
	if(bStart)
	{
		if (!nState)
			m_dwUndoTimestamp = GetTickCount();
		nState++;
	}
	else
	{
		nState--;
	if (!nState)
		m_dwUndoTimestamp = 0;
	}
}

//@doc SECEdit
//@mfunc Internal function that reverses the actions recorded in the Undo record at
//              m_iUndoPos-1
//
//@rdesc BOOL 
BOOL SECEdit::UndoEdit()
   {
	PUNDORECORD pUndoRecord;
   TEXTPOS TextPos;
	TEXTPOS EndPos;

   if (!m_iUndoPos)
      return FALSE;

	pUndoRecord = m_pUndoBuffer;

   pUndoRecord += (m_iUndoPos - 1);

   TextPos.pLine = GoToLineNo(pUndoRecord->iRow);
   TextPos.iOffset = pUndoRecord->iCol;

	DWORD dwLastTimestamp = pUndoRecord->dwTimestamp;

   switch (pUndoRecord->iUndoType)
      {
      case OE_UNDO_INSERT:
         DeleteChar(&TextPos,FALSE);
         break;

      //case OE_UNDO_INS_OVER:
      //   InsertChar(&TextPos, pUndoRecord->cOvertypeChar, FALSE, FALSE);
      //   break;
	//////////////////////////////////////////

		case OE_UNDO_INS_OVER:
			if(pUndoRecord->cOvertypeChar==13) // replace line-end
				DeleteChar(&TextPos,FALSE); // treat like inserted char
			else
				InsertChar(&TextPos, pUndoRecord->cOvertypeChar, FALSE, FALSE);

			break;	
	  
	  ///////////////////////////////////////
      case OE_UNDO_DELETE:
         InsertChar(&TextPos, pUndoRecord->cInsertChar, TRUE, FALSE);
         break;

      case OE_UNDO_CUT:
			if (pUndoRecord->lpBuffer)
	         Paste(&TextPos,pUndoRecord->lpBuffer,strlen(pUndoRecord->lpBuffer),FALSE);
			break;

      case OE_UNDO_COLUMNCUT:
			if (pUndoRecord->lpBuffer)
	         ColumnPaste(&TextPos,pUndoRecord->lpBuffer,strlen(pUndoRecord->lpBuffer),FALSE);
         break;

      case OE_UNDO_COLUMNPASTE:
			{
			int iEndRow = pUndoRecord->iEndRow;
			if (iEndRow != pUndoRecord->iRow)
				iEndRow--;
			EndPos.pLine = GoToLineNo(iEndRow);
         EndPos.iOffset = pUndoRecord->iEndCol;
         ColumnCut(&TextPos,&EndPos,FALSE);
         break;
			}
		case OE_UNDO_MERGE:
      case OE_UNDO_PASTE:
			EndPos.pLine = GoToLineNo(pUndoRecord->iEndRow);
         EndPos.iOffset = pUndoRecord->iEndCol;
         Cut(&TextPos,&EndPos,FALSE);
         break;

      case OE_UNDO_JOINLINE:
         SplitLine(&TextPos,FALSE);
         break;

      case OE_UNDO_NEWLINE:
         JoinLines(&TextPos,FALSE);
         break;

      }

   m_iUndoPos--;

	if (m_iUndoPos)
		{
		PUNDORECORD pNextUndoRecord = m_pUndoBuffer;
		pNextUndoRecord += (m_iUndoPos - 1);
		// call ourselves recursively if the next record matches the last timestamp
		if (dwLastTimestamp && (pNextUndoRecord->dwTimestamp == dwLastTimestamp))
			UndoEdit();
		else switch (pUndoRecord->iUndoType)
			{
			case OE_UNDO_INSERT:
			case OE_UNDO_INS_OVER:
			case OE_UNDO_DELETE:
			if ((pUndoRecord->iUndoType == pNextUndoRecord->iUndoType) && 
				 (pUndoRecord->iRow == pNextUndoRecord->iRow))
					UndoEdit();
			}
		}

	return TRUE;
   }



//@doc SECEdit
//@mfunc Internal function that redoes the actions recorded in the UndoRecord at m_iUndoPos
//
//@rdesc BOOL 
BOOL SECEdit::RedoEdit()
   {
   TEXTPOS TextPos;
	TEXTPOS EndPos;
	PUNDORECORD pUndoRecord;

   if (!m_iUndoCount ||
       ((m_iUndoPos+1) > m_iUndoCount))
      return FALSE;

   pUndoRecord = m_pUndoBuffer;

   pUndoRecord += m_iUndoPos;

   TextPos.pLine = GoToLineNo(pUndoRecord->iRow);
   TextPos.iOffset = pUndoRecord->iCol;

	DWORD dwLastTimestamp = pUndoRecord->dwTimestamp;

   switch (pUndoRecord->iUndoType)
      {
      case OE_UNDO_DELETE:
         DeleteChar(&TextPos,FALSE);
         break;

      case OE_UNDO_INS_OVER:
         InsertChar(&TextPos, pUndoRecord->cInsertChar, FALSE, FALSE);
         break;

      case OE_UNDO_INSERT:
         InsertChar(&TextPos, pUndoRecord->cInsertChar, TRUE, FALSE);
         break;

      case OE_UNDO_PASTE:
  			if (pUndoRecord->lpBuffer)
		      Paste(&TextPos,pUndoRecord->lpBuffer,strlen(pUndoRecord->lpBuffer),FALSE);
         break;

      case OE_UNDO_COLUMNPASTE:
			if (pUndoRecord->lpBuffer)
	         ColumnPaste(&TextPos,pUndoRecord->lpBuffer,strlen(pUndoRecord->lpBuffer),FALSE);
         break;

      case OE_UNDO_COLUMNCUT:
			EndPos.pLine = GoToLineNo(pUndoRecord->iEndRow);
         EndPos.iOffset = pUndoRecord->iEndCol;
         ColumnCut(&TextPos,&EndPos,FALSE);
         break;

      case OE_UNDO_CUT:
			EndPos.pLine = GoToLineNo(pUndoRecord->iEndRow);
         EndPos.iOffset = pUndoRecord->iEndCol;
         Cut(&TextPos,&EndPos,FALSE);
         break;

      case OE_UNDO_NEWLINE:
         SplitLine(&TextPos,FALSE);
         break;

      case OE_UNDO_JOINLINE:
         JoinLines(&TextPos,FALSE);
         break;

      case OE_UNDO_MERGE:
  			if (pUndoRecord->lpBuffer)
		      MergeFile(&TextPos,pUndoRecord->lpBuffer,FALSE);
         break;

      }

   m_iUndoPos++;

	PUNDORECORD pNextUndoRecord = m_pUndoBuffer;
	pNextUndoRecord += m_iUndoPos;
	// call ourselves recursively if the next record matches the last timestamp
	if (dwLastTimestamp && (pNextUndoRecord->dwTimestamp == dwLastTimestamp))
		RedoEdit();
	else switch (pUndoRecord->iUndoType)
		{
		case OE_UNDO_INSERT:
		case OE_UNDO_INS_OVER:
		case OE_UNDO_DELETE:
			if ((pUndoRecord->iUndoType == pNextUndoRecord->iUndoType) && 
				 (pUndoRecord->iRow == pNextUndoRecord->iRow))
				RedoEdit();
		}

	return TRUE;
   }
