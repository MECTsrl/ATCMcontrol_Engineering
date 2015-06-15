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
// EditFind.cpp
//
//
// Description: Low level searching of the linked list
//

#include "stdafx.h"
#include "secedit.h"
#include "regex.inl"


LPSTR RegSearchReplace(PSEARCHRECORD pSearch, LPCSTR string, CRegExp& r)
{
	/* Implementaion modified below.
	USES_CONVERSION_T;

   int nPos = 0;
   int nReplaced = 0;
   LPSTR str = (LPSTR)string;

   if( (nPos = r.RegFind(str)) != -1 )
   {
		pSearch->iReturnLength = r.GetFindLen();
		str += nPos;
      char *pReplaceStr = r.GetReplaceString(pSearch->lpReplaceText);
		if (pReplaceStr)
			{
			strcpy(pSearch->lpReplaceText,pReplaceStr);
			delete pReplaceStr;
			}
   }
	else
		str = NULL;

   return str;
   */

	USES_CONVERSION_T;

	int nPos = 0;
	int nReplaced = 0;
	CString str = string;
	LPSTR rstr = (LPSTR)string;
	if( (nPos=str.Find(13))!=-1)
		str = str.Left(nPos);

	if( (nPos = r.RegFind(T2CA(str))) != -1 )
	{
		pSearch->iReturnLength = r.GetFindLen();
		rstr += nPos;
		char *pReplaceStr = r.GetReplaceString(pSearch->lpReplaceText);
		if (pReplaceStr)
		{
			strcpy(pSearch->lpReplaceText,pReplaceStr);
			delete pReplaceStr;
		}
	}
	else
		rstr = NULL;

	return rstr;
}


//@doc SECEdit
//@mfunc Internal function.   Search for pSearch->lpSearchText, starting at pStartPos
//              and continuing to pEndPos (which may be NULL)
//@rdesc BOOL 
//@parm PTEXTPOS pStartPos
//@parm  PTEXTPOS pEndPos
//@parm  PSEARCHRECORD pSearch
BOOL SECEdit::Find(PTEXTPOS pStartPos, PTEXTPOS pEndPos, PSEARCHRECORD pSearch)

   {

	USES_CONVERSION_T;

   LPSTR lpLastFound;
	LPSTR lpFoundText;
	BOOL bRetVal;
	char szText[OE_MAXLINELENGTH+1];
	char szSearchText[OE_MAXLINELENGTH+1];

	strcpy(szSearchText,pSearch->lpSearchText);
	if (!pSearch->fMatchCase)
		strlwr(szSearchText);

	pStartPos->iOffset = min(pStartPos->iOffset,pStartPos->pLine->iTxtSize);

   int iPatLen = strlen(szSearchText);

	PLINEDESC lpLine = pStartPos->pLine;
	int iEndOffset;
	int iTxtSize;
	int iStartOffset;
	bRetVal = FALSE;

    CRegExp r;
    if (pSearch->fRegular)
		if (r.RegComp(szSearchText) == NULL)
			return FALSE;

   for (BOOL bDone = FALSE;
        lpLine && !bDone;
        (pSearch->fForward ? (lpLine = lpLine->pNext):
                      (lpLine = lpLine->pPrev)))
      {
		if ((lpLine == pStartPos->pLine) && pSearch->fForward)
			{
			iStartOffset = pStartPos->iOffset;
			memcpy(szText,&lpLine->pText[iStartOffset],lpLine->iTxtSize-iStartOffset);
   		szText[lpLine->iTxtSize-iStartOffset] = 0;
			if (lpLine==pEndPos->pLine)
				iEndOffset = pEndPos->iOffset - iStartOffset;
			else
				iEndOffset = lpLine->iTxtSize - iStartOffset;
			}
		else if (lpLine == pStartPos->pLine)
			{
			iStartOffset = 0;
			if (lpLine==pEndPos->pLine)
				iEndOffset = pEndPos->iOffset;
			else
				iEndOffset = pStartPos->iOffset;
         memcpy(szText,lpLine->pText,iEndOffset);
	   	szText[iEndOffset] = 0;
			}
		else
			{
			iStartOffset = 0;
			iEndOffset = lpLine->iTxtSize;
         memcpy(szText,lpLine->pText,iEndOffset);
	   	szText[iEndOffset] = 0;
			}

		if (!pSearch->fMatchCase)
			strlwr(szText);

      if (pSearch->fForward)
         bDone = (lpLine==pEndPos->pLine);

      iTxtSize = iEndOffset;

      if (!pSearch->fForward)
         {
         if (pSearch->fRegular)
            {
            lpLastFound = NULL;
				lpFoundText = RegSearchReplace(pSearch, szText, r);
            while (lpFoundText)
               {
				   if ((lpFoundText+pSearch->iReturnLength+1) > szText+iEndOffset)
                  lpFoundText = NULL;
               else
                  {
                  lpLastFound = lpFoundText;
                  lpFoundText = RegSearchReplace(pSearch, ++lpFoundText, r);
                  }
               }
            lpFoundText = lpLastFound;
				if (lpLastFound)
					pStartPos->iOffset = lpFoundText - szText;
            }
         else   
				{
            lpFoundText = FScanBack(szSearchText,
                                    szText,
                                    iTxtSize);
				if (lpFoundText)
					lpFoundText = lpLine->pText + 
										(lpFoundText - szText) + 
										iStartOffset;
				}
         }
      else  // forward
         {
         if (pSearch->fRegular)
				{
				lpFoundText = RegSearchReplace(pSearch, szText, r);
				if (lpFoundText)
					pStartPos->iOffset = lpFoundText - szText + iStartOffset;
				}
         else
				{
            lpFoundText = FScan(szSearchText,
                                szText,
                                iTxtSize);

				if (lpFoundText)
						lpFoundText = lpLine->pText + 
											(lpFoundText - szText + iStartOffset);
				}
			}

		if (lpFoundText)
			{
			bRetVal = TRUE;
			break;
			}

      } /** end of for() loop. **/

   if (bRetVal)
      {
		if (!pSearch->fRegular)
			{
	      pStartPos->iOffset     = lpFoundText - lpLine->pText;
			pSearch->iReturnLength = iPatLen;
			}
      pStartPos->pLine = lpLine;
      }

   return bRetVal;
   }


//@doc SECEdit
//@mfunc Internal function.  Determines if pat is a regular expression pattern
//@rdesc BOOL 
//@parm unsigned char *pat
BOOL SECEdit::IsRegular(unsigned char *pat)
   {
   unsigned char *lpTemp;
   BOOL bRval;

   for (lpTemp = pat,bRval = FALSE;*lpTemp && !bRval;lpTemp++)
      {
      switch (*lpTemp)
         {
         case '^':     /* start-of-line anchor                 */
         case '$':     /* end-of-line anchor                   */
         case '.':     /* matches any character                */
         case '[':     /* start a character class              */
         case ']':     /* end a character class                */
         case '*':     /* Kleene closure (matches 0 or more)   */
         case '+':     /* Positive closure (1 or more)         */
         case '?':     /* Optional closure (0 or 1)            */
         case '\\':     /* escape character                     */
            bRval = TRUE;
            break;
         }
      }
   return bRval;
   }


//@doc SECEdit
//@mfunc Internal function.  Brute force forward search.  Since only one line of
//              text is searched at a time, brute force works pretty
//              well.
//@rdesc LPTSTR Pointer to found text or NULL
//@parm LPTSTR lpPattern
//@parm LPTSTR lpText
//@parm int   iLength
LPSTR SECEdit::FScan(LPSTR lpPattern,LPSTR lpText,int   iLength)
   {
   LPSTR lpNext;
   LPSTR lpFound;
   LPSTR lpTemp;
   LPSTR lpCheckText;
   char cFirstChar;

   cFirstChar = lpPattern[0];
   lpTemp = lpText;
   /* search for the first character */
   lpFound = lpNext = (LPSTR)memchr(lpTemp,cFirstChar,iLength);

   while (lpNext)
      {
      /* check for a complete match */
      for (lpCheckText = lpPattern;
           *lpCheckText && *lpNext && (*lpCheckText == *lpNext);
           *lpCheckText++,*lpNext++);

      if (!(*lpCheckText))
         /* we got a complete match */
         return lpFound;

      iLength--;

      lpTemp++;

      if (iLength > 0)
         lpFound = lpNext = (LPSTR)memchr(lpTemp,cFirstChar,iLength);
      else
         break;
      }

   return NULL;
   }

//@doc SECEdit
//@mfunc Internal function.  Brute force backward search
//@rdesc LPTSTR 
//@parm LPTSTR lpPattern
//@parm  LPTSTR lpText
//@parm  int   iLength
LPSTR SECEdit::FScanBack(LPSTR lpPattern, LPSTR lpText, int   iLength)
   {
   LPSTR lpNext;
   LPSTR lpLastFound;
   LPSTR lpFound;
   LPSTR lpTemp;
   LPSTR lpCheckText;
   char cFirstChar;

   cFirstChar = lpPattern[0];
   lpTemp = lpText;
   /* search for the first character */
   lpLastFound = NULL;
   lpNext = lpFound = (LPSTR)memchr(lpTemp,cFirstChar,iLength);

   while (lpNext)
      {
      /* check for a complete match */
      for (lpCheckText = lpPattern;
           *lpCheckText && *lpNext && (*lpCheckText == *lpNext);
           *lpCheckText++,*lpNext++);

      if (!(*lpCheckText))
         /* we got a complete match */
         lpLastFound = lpFound;

      iLength--;

      lpTemp++;
      if (iLength > 0)
         lpFound = lpNext = (LPSTR)memchr(lpTemp,cFirstChar,iLength);
      else
         break;
      }

   return lpLastFound;
   }
