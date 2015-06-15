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
// EditFile.cpp
//
//
// Description: low level file read / write routines
//

#include "stdafx.h"
#include "secedit.h"

//@doc SECEdit
//@mfunc Internal method.  Sets m_iLineEndType and m_szLineEnd
//@rdesc void 
//@parm int iLineEnd
void SECEdit::SetLineEndType(int iLineEnd)
   {
	m_iLineEndType = iLineEnd;
   switch(iLineEnd)
      {
      case OE_CR:
         m_iLineEndLength = 1;
         m_szLineEnd[0] = 0x0D;
         m_szLineEnd[1] = 0;
         break;

      case OE_LF:
         m_iLineEndLength = 1;
         m_szLineEnd[0] = 0x0A;
         m_szLineEnd[1] = 0;
         break;

      case OE_CRLF:
         m_iLineEndLength = 2;
         m_szLineEnd[0] = 0x0D;
         m_szLineEnd[1] = 0x0A;
         m_szLineEnd[2] = 0;
         break;

      case OE_LFCR:
         m_iLineEndLength = 2;
         m_szLineEnd[0] = 0x0A;
         m_szLineEnd[1] = 0x0D;
         m_szLineEnd[2] = 0;
         break;
      }
   }


//@doc SECEdit
//@mfunc Internal method.   Checks for null characters in lpBuffer.  Either replaces
//              them or quits semi-gracefully.  Call with iReplace = 0
//              first time, with the returned iReplace subsequently.
//@rdesc int 
//              0 - no NULLS found
//              2 - NULL found
//@parm LPSTR lpBuffer
//@parm  int iTextSize
//@parm  int iReplace
int SECEdit::CheckForNull(LPSTR lpBuffer, int iTextSize, int iReplace)
   {
	ASSERT(lpBuffer);
	iReplace;

   LPSTR lpSearch;
	
   lpSearch = (LPSTR)memchr(lpBuffer,0,iTextSize);
   if (lpSearch)
		return 2;
	return 0;
   }

//@doc SECEdit
//@mfunc Internal function.  Read input file and build linked list of textlines
//
//@rdesc int 
//@parm HANDLE fh
//@parm  LPSTR lpBuffer
//@parm  int iFileBufferSize
int SECEdit::BuildText(HANDLE fh, LPSTR lpBuffer, int iFileBufferSize)
   {
   LPSTR pcTemp;
   int iRval;
   PLINEDESC pLine;
   PLINEDESC pPrevLine;
   BOOL bGotLineEnd;
   int iSize,i;
   int iReplace = 0;
   ASSERT(fh);
	ASSERT(lpBuffer);

   pPrevLine = NULL;

   if (!::ReadFile(fh, lpBuffer, iFileBufferSize, (LPDWORD)&iRval, NULL))
      return IDS_OE_CANTREAD;

   if (iRval == 0)
      /* zero length file, but not an error */
      return 0;

   lpBuffer[iRval] = 0;

   i = FindLineEnd(lpBuffer,iRval);
	if (i == -1)
		{
		if (iRval < OE_MAXLINELENGTH)
			i = OE_CRLF;
		else
			return IDS_OE_LINETOOLONG;
		}

	SetLineEndType(i);

   iReplace = CheckForNull(lpBuffer, iRval, 0);
   if (iReplace == 2)  // null encountered and user says don't replace
      return IDS_OE_NULLSFOUND;
   
   /* check for exceeding maximum line length */
   if (iRval > OE_MAXLINELENGTH)
      {
      pcTemp = (LPSTR)memchr(lpBuffer, m_szLineEnd[0], iRval);
      if (!pcTemp)
         return IDS_OE_LINETOOLONG;
      }

   /* strip Ctrl+Z */
   if (lpBuffer[iRval-1] == 0x1A)
      {
      lpBuffer[iRval-1] = 0;
      iRval--;
      }

   pPrevLine = pLine = LineFromTextBuffer(lpBuffer);

   if (pLine)
      pLine = ListGetFirst(pLine, NULL);
   else
      return IDS_OE_CANTREAD;

   while (iRval == iFileBufferSize)  /* more file to read */
      {
      pPrevLine = ListGetLast(pLine);

		int i = CountLines(pPrevLine);

      if (!::ReadFile(fh,lpBuffer,iFileBufferSize,(LPDWORD)&iRval,NULL))
         {
         LineListFree(pPrevLine);
         return IDS_OE_CANTREAD;
         }

      if (iRval > 0)
         {
         lpBuffer[iRval] = 0;
         iReplace = CheckForNull(lpBuffer, iRval, iReplace);
         if (iReplace == 2)
            {
            LineListFree(pPrevLine);
            return IDS_OE_CANTREAD;
            }

         /* strip Ctrl+Z */
         if (lpBuffer[iRval-1] == 0x1A)
            {
            lpBuffer[iRval-1] = 0;
            iRval--;
            }

         pLine = LineFromTextBuffer(lpBuffer);

         if (pLine)
            {
            // do we need to join the last (partial) line with this first one?
            bGotLineEnd = FALSE;
            pcTemp = (LPSTR)memchr(pPrevLine->pText, m_szLineEnd[0], pPrevLine->iTxtSize);
            if (pcTemp)
               {
               if (m_iLineEndLength == 2)
                  {
                  if (pcTemp[1] == m_szLineEnd[1])
                     bGotLineEnd = TRUE;
                  }
               else
                  bGotLineEnd = TRUE;
               }
            if (!bGotLineEnd)
               {
               // join the last line of the previous buffer
               // with the first line of this buffer
               iSize = pLine->iTxtSize + pPrevLine->iTxtSize;
               ResizeLine(pPrevLine,iSize);
               strcat(pPrevLine->pText,pLine->pText);
               pPrevLine->iTxtSize = iSize;
					// if a cr/lf was split between two fileread buffers
					// we'll end up here with a cr/lf/lf.  Check for that
					// and strip it if needed.
               if (m_iLineEndLength == 2)
						{
						if (iSize > m_iLineEndLength)
							{
							if (pPrevLine->pText[iSize-m_iLineEndLength-1] == m_szLineEnd[0])
								{
								pPrevLine->pText[iSize-1] = 0;
								pPrevLine->iTxtSize--;
								}
							}
						}
               pPrevLine->pNext = pLine->pNext;
					if (pLine->pNext)
						pLine->pNext->pPrev = pPrevLine;
               FreeTextLine(pLine);
					pLine = pPrevLine;
               }
            else
               {
               pPrevLine->pNext = pLine;
               pLine->pPrev = pPrevLine;
               }
            pPrevLine = ListGetLast(pPrevLine);
            }
         else
            {
            LineListFree(pPrevLine);
            return IDS_OE_CANTREAD;
            }
         }
      }
   m_pLine = ListGetFirst(pPrevLine, NULL);
   return 0;
   }

//@doc SECEdit
//@mfunc Internal function.  
//Read lpName and build a linked list of textlines.
//              If m_bFileLocking, keep the file open and cache the handle
//@rdesc int 
//@parm LPCTSTR lpName
int SECEdit::ReadFile(LPCTSTR lpName)
   {
   LPSTR lpBuffer;
   int iFileBufferSize;
	HANDLE fh;
	int iRval;
   unsigned int uiErrorMode;
	DWORD dwAttr;

	iRval = 0;
   iFileBufferSize = 32767;

   /* allocate the file buffer */
   lpBuffer = (LPSTR)SECEditAlloc(iFileBufferSize+1);
   if (!lpBuffer)
      return IDS_OE_NOMEMORY2;

   uiErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

	if (m_hFile)
		CloseHandle(m_hFile);

   fh = CreateFile(lpName,                    /* open pName         */
                GENERIC_READ,                 /* open for reading and/or writing   */
                FILE_SHARE_READ,              /* share flags        */
                (LPSECURITY_ATTRIBUTES) NULL, /* no security     */
                OPEN_EXISTING,                /* existing file only */
                FILE_ATTRIBUTE_NORMAL,        /* normal file        */
                (HANDLE) NULL);               /* no attr. template  */

   if (fh == INVALID_HANDLE_VALUE)
		{
      free(lpBuffer);
      SetErrorMode(uiErrorMode);
      return GetLastError();
		}

   if (m_pLine)
		{
      LineListFree(m_pLine);
		m_pLine = NULL;
		}

   iRval = BuildText(fh,lpBuffer,iFileBufferSize);
   if (iRval)
      {
      m_pLine = MakeNewLine(32);
      CloseHandle(fh);
      SetErrorMode(uiErrorMode);
      free(lpBuffer);
      return iRval;
      }

	if (!m_pLine)  // it was a zero length file
      m_pLine = MakeNewLine(32);

   dwAttr = GetFileAttributes(lpName);
   if ((dwAttr != -1L) && (dwAttr & FILE_ATTRIBUTE_READONLY))
		m_bReadOnly = TRUE;

	if (!m_bFileLocking)
      CloseHandle(fh);
   else
      m_hFile = fh;
      
   SetErrorMode(uiErrorMode);

   m_iLinesInFile = CountLines(m_pLine);
   free(lpBuffer);

   return 0;
   }


//@doc SECEdit
//@mfunc Internal function.  Write lpName to disk and clear the m_bModified flag
//@rdesc int 
//@parm LPCTSTR lpName
int SECEdit::WriteFile(LPCTSTR lpName)
   {
   PLINEDESC pLine;
	PLINEDESC pNextLine;
   DWORD j;
   DWORD dwAttr;
   HANDLE   fh;
   int iRval;
	unsigned int uiErrorMode;

   dwAttr = GetFileAttributes(lpName);
   if ((dwAttr != -1L) && (dwAttr & FILE_ATTRIBUTE_READONLY))
		return IDS_OE_READONLY;

   uiErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

   // might have to temporarily close the file if it's locked
   if (m_hFile)
		{
      CloseHandle(m_hFile);
      m_hFile = NULL;
		}

   uiErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
   fh = CreateFile(lpName,
             GENERIC_READ|GENERIC_WRITE,
             FILE_SHARE_READ,
             (LPSECURITY_ATTRIBUTES) NULL,
             CREATE_ALWAYS,
             FILE_ATTRIBUTE_NORMAL,
             (HANDLE) NULL);


    /* If file could not be opened, quit */
   if (fh == INVALID_HANDLE_VALUE)
		{
      SetErrorMode(uiErrorMode);
		return GetLastError();
		}

   pLine = ListGetFirst(m_pLine, NULL);

   /* save the file */
	iRval = 0;
   while (pLine)
      {
      pNextLine = pLine->pNext;
      if (pLine->pText)
         {
         ::WriteFile(fh,pLine->pText,pLine->iTxtSize,&j,NULL);
         if (j != (DWORD)pLine->iTxtSize)
				{
				iRval = GetLastError();
            break;
            }
         }
      pLine = pNextLine;
      }

   if (!CloseHandle(fh))
		iRval = GetLastError();

   if (!iRval && m_bFileLocking)
      {
      /* reopen the file */
      fh = CreateFile(lpName,                 /* open pName         */
                   GENERIC_READ,              /* open for reading   */
                   FILE_SHARE_READ,               /* share flags        */
                   (LPSECURITY_ATTRIBUTES) NULL, /* no security     */
                   OPEN_EXISTING,             /* existing file only */
                   FILE_ATTRIBUTE_NORMAL,     /* normal file        */
                   (HANDLE) NULL);            /* no attr. template  */
      if (fh == INVALID_HANDLE_VALUE)
			iRval = GetLastError();
		else
         m_hFile = fh;
      }

	if (!iRval)
      SetModified(FALSE);

	SetErrorMode(uiErrorMode);
   return iRval;
   }


//@doc SECEdit
//@mfunc Internal function.  Renames lpOldName to lpNewName
//@rdesc int 
//@parm LPCTSTR lpOldName
//@parm  LPCTSTR lpNewName
int SECEdit::RenameFile(LPCTSTR lpOldName, LPCTSTR lpNewName)
   {
   HANDLE fh;
	unsigned int uiErrorMode;
	BOOL bLockFile;
	int iRval = 0;

   // if we didn't get two names, just return
   if (!lstrcmpi(lpOldName,lpNewName))
      return 0;

   uiErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

   // might have to temporarily close the file if it's locked
   if (m_hFile)
		{
      CloseHandle(m_hFile);
      m_hFile = NULL;
		bLockFile = TRUE;
		}
	else
		bLockFile = FALSE;

	// if there's a already a dest file with this name, blow it away
   if (_tunlink(lpNewName))
		if (errno == EACCES)
			iRval = IDS_OE_READONLY;
	   else 
			errno = 0;

	//
	if (_trename(lpOldName, lpNewName))
      if (errno == ENOENT)
			iRval = IDS_OE_CANTREAD;

   if (bLockFile)
      {
      /* reopen the file */
      fh = CreateFile(lpNewName,              /* open pName         */
                   GENERIC_READ,              /* open for reading   */
                   FILE_SHARE_READ,               /* share flags        */
                   (LPSECURITY_ATTRIBUTES) NULL, /* no security     */
                   OPEN_EXISTING,             /* existing file only */
                   FILE_ATTRIBUTE_NORMAL,     /* normal file        */
                   (HANDLE) NULL);            /* no attr. template  */
      if (fh == INVALID_HANDLE_VALUE)
			iRval = GetLastError();
		else
         m_hFile = fh;
      }

	SetErrorMode(uiErrorMode);
   return iRval;
   }


//@doc SECEdit
//@mfunc Internal function.  Merges the file lpName into the linked list at pTextPos
//@rdesc int 
//@parm PTEXTPOS pTextPos
//@parm  LPCTSTR lpName
//@parm  BOOL bSaveUndoRecord

int SECEdit::MergeFile(PTEXTPOS pTextPos, LPCSTR lpName, BOOL bSaveUndoRecord)
   {

	USES_CONVERSION_T;

	int iRval;
	PLINEDESC pLine;
	int iLineCount;
	UNDORECORD UndoRecord;
	SECEdit TempWnd;
	ASSERT(pTextPos);

	iRval = 0;

	// link in whole lines only
	if (pTextPos->iOffset > 0)
		pTextPos->iOffset = 0;

	// read the file into TempWnd
	iRval = TempWnd.ReadFile(A2CT(lpName));
	if (iRval)
		return iRval;

	// save the linecount for the undo record
	if (bSaveUndoRecord)
	   iLineCount = TempWnd.CountLines(TempWnd.m_pLine);

	// link the new lines in before pTextPos->pLine
	pLine = TempWnd.m_pLine = ListGetFirst(TempWnd.m_pLine, NULL);
	if (pTextPos->pLine->pPrev)
		{
		TempWnd.m_pLine->pPrev = pTextPos->pLine->pPrev;
		pTextPos->pLine->pPrev->pNext = TempWnd.m_pLine;
		TempWnd.m_pLine = ListGetLast(TempWnd.m_pLine);
		TempWnd.m_pLine->pNext = pTextPos->pLine;
		pTextPos->pLine->pPrev = TempWnd.m_pLine;
		pTextPos->pLine = pLine;
		}
	else
		{
		TempWnd.m_pLine = ListGetLast(TempWnd.m_pLine);
		TempWnd.m_pLine->pNext = pTextPos->pLine;
		pTextPos->pLine->pPrev = TempWnd.m_pLine;
		pTextPos->pLine = ListGetFirst(TempWnd.m_pLine, NULL);
		}
	
	// the lines now belong to pWnd
	TempWnd.m_pLine = NULL;

	if (bSaveUndoRecord)
		{
      UndoRecord.iRow = GetLineNo(pLine);
      UndoRecord.iCol = 0;
      UndoRecord.iEndRow = UndoRecord.iRow + iLineCount;
      UndoRecord.iEndCol = 0;
      UndoRecord.iUndoType = OE_UNDO_MERGE;
      SaveUndoRecord(&UndoRecord, lpName);
		}

	SetModified(TRUE);
   m_iLinesInFile += iLineCount;
	return 0;
   }

