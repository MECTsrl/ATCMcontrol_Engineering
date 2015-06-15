#ifndef __VIEWCOPY_INL__
#define __VIEWCOPY_INL__
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
//@mfunc Copies the current selection to the clipboard
//@rdesc int 
//@parm BOOL bUpdate
//@parm  BOOL bSaveUndoRecord
template <class BC>
int SECEditCore<BC>::Copy(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
   LPSTR lpData;
   LPSTR lpCopyText;
   int iClipSize;
   HANDLE hData;
   TEXTPOS Start,End;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (m_iBlockType == OE_COLUMN)
      return ColumnCopy(bUpdate,bSaveUndoRecord);
   
   if (m_BlockStart.pLine && m_BlockEnd.pLine)
      {
      // BlockStart and BlockEnd positions are tabbed,
      // WndCopy gets the real offset
      Start = m_BlockStart;
      End = m_BlockEnd;
      Start.iOffset = pEdit->GetRealOffset(Start.pLine,Start.iOffset);
      End.iOffset = pEdit->GetRealOffset(End.pLine,End.iOffset);
      lpCopyText = pEdit->Copy(&Start,&End,bSaveUndoRecord);
      }
   else
      lpCopyText = NULL;

   if (!lpCopyText)
      return IDS_OE_NOMEMORY2;

   iClipSize = strlen(lpCopyText);

   /* Clear the current contents of the clipboard, and set
    * the data handle to the new string.
    */
   if (OpenClipboard())
      {
      EmptyClipboard();
      hData = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,iClipSize+1);
      lpData = (LPSTR)GlobalLock(hData);
      strcpy(lpData,lpCopyText);
      GlobalUnlock(hData);
      SetClipboardData(CF_TEXT, hData);
      CloseClipboard();
      }

   free(lpCopyText);
   return 0;
   }


//@doc SECEditCore
//@mfunc Copies the current column selection to the clipboard
//@rdesc int 
//@parm BOOL bUpdate
//@parm  BOOL bSaveUndoRecord
template <class BC>
int SECEditCore<BC>::ColumnCopy(BOOL bUpdate, BOOL bSaveUndoRecord)
   {
	bUpdate; //UNUSED
	LPSTR lpData;
	LPSTR lpCopyText;
	int iClipSize,i;
	HANDLE hData;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

   if (m_BlockStart.pLine && m_BlockEnd.pLine)
		{
		TEXTPOS Start = m_BlockStart;
		TEXTPOS End = m_BlockEnd;
      Start.iOffset = pEdit->GetRealOffset(Start.pLine,Start.iOffset);
      End.iOffset = pEdit->GetRealOffset(Start.pLine,End.iOffset);
		lpCopyText = pEdit->ColumnCopy(&Start,&End,bSaveUndoRecord);		
		}

   if (!lpCopyText)
      return IDS_OE_NOMEMORY2;

   iClipSize = strlen(lpCopyText);

   /* Clear the current contents of the clipboard, and set
    * the data handle to the new string.
    */
   if (OpenClipboard())
      {
      EmptyClipboard();
      hData = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,iClipSize+32);
      lpData = (LPSTR)GlobalLock(hData);
      strcpy(lpData,"*SECEdit Column Block*");
      for (i=0;i<pEdit->m_iLineEndLength;i++)
         lpData[i+22] = pEdit->m_szLineEnd[i];
      lpData[i+22] = 0;
      strcat(lpData,lpCopyText);
      GlobalUnlock(hData);
      SetClipboardData(CF_TEXT, hData);
      CloseClipboard();
      }

   free(lpCopyText);
   return 0;
   }

#endif //__VIEWCOPY_INL__