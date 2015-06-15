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
// EditChroma.cpp
//
// Description: Syntax coloring support for SECEdit
//
//

#include "stdafx.h"
#include "secedit.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

//@doc SECEdit
//@mfunc Internal function that sets the registry key for syntax coloring lookups
//@rdesc void 
//@parm CString strSection
void SECEdit::SetFileMappingKey(CString strSection)
   {
	m_strSection = strSection;
	}

// AUTOFMT 09.08.06 SIS >>
#ifndef ORIGINAL_VERSION
CString SECEdit::GetFileMappingKey()
{
    return m_strSection;
}
#endif
// AUTOFMT 09.08.06 SIS <<

//@doc SECEdit
//@mfunc Internal function 
//@rdesc void 
void SECEdit::SetFileMappingKey()
   {
	m_strSection.Empty();
	}


//@doc SECEdit
//@mfunc Internal function.  Looks in the m_strSection key of the Registry for
//              additional alphanumeric characters to be considered
//              part of a word.
//@rdesc void 
void SECEdit::SetExtraChars()
{

	USES_CONVERSION_T;

	char szArray[256];
	int i,j;
	CString str;

	memset(alnumarray,0,sizeof(alnumarray));       
	memset(terminatorarray,0,sizeof(terminatorarray));       
	memset(startarray,0,sizeof(startarray));       
	memset(szArray,0,sizeof(szArray));       

	if (m_strSection.IsEmpty())
		return;

	str = AfxGetApp()->GetProfileString(m_strSection,_T("Extra word chars"),afxEmptyString);


	strcpy(szArray,T2CA(str));
	i = strlen(szArray);
	if (i)
	{
		for (j=0;j<i;j++)
		alnumarray[szArray[j]] = szArray[j];
	}
	memset(szArray,0,sizeof(szArray));       
	str = AfxGetApp()->GetProfileString(m_strSection,_T("Terminator chars"),afxEmptyString);
	strcpy(szArray,T2CA(str));
	i = strlen(szArray);
	if (i)
	{
		for (j=0;j<i;j++)
			terminatorarray[szArray[j]] = szArray[j];
	}
	memset(szArray,0,sizeof(szArray));       
	str = AfxGetApp()->GetProfileString(m_strSection,_T("Start chars"),afxEmptyString);
	strcpy(szArray,T2CA(str));
	i = strlen(szArray);
	if (i)
	{
		for (j=0;j<i;j++)
		startarray[szArray[j]] = szArray[j];
	}
}

//@doc SECEdit
//@mfunc Internal Function. The keyword list used for syntax coloring is an
//              old-style Windows INI file.  This function mimics
//              the SDK function but searches our buffer which
//              holds the entire section in memory.
//@rdesc int Length of found string on success.
//@parm LPCTSTR lpSectionName
//@parm  LPCTSTR lpKeyName
//@parm   LPCTSTR lpDefault
//@parm  LPTSTR lpReturnedString
//@parm DWORD nSize
//@parm  LPCTSTR
int SECEdit::MyGetPrivateProfileString(LPCSTR lpSectionName, LPCSTR lpKeyName,  LPCSTR lpDefault, LPSTR lpReturnedString,DWORD nSize, LPCSTR /* ?? */)
   {

   char szTemp[512];
   int i,iWordSize;
   LPSTR lpFound;
   LPSTR lpStart;

   if (!giMyIniLength)
      {
      strcpy(lpReturnedString,lpDefault);
      return 0;
      }
   i = 0;


   // find the section
   sprintf(szTemp,"[%s]",lpSectionName);
   iWordSize = strlen(szTemp);
   nSize = giMyIniLength;
   lpFound = ScanFwd(szTemp,glpMyIniFileBuffer,(int)nSize);
   if (lpFound)
      {
      lpFound += iWordSize;
      nSize = giMyIniLength - (lpFound - glpMyIniFileBuffer);
      lpStart = lpFound;
      sprintf(szTemp,"%s=",lpKeyName);
      lpFound = ScanFwd(szTemp,lpStart,(int)nSize);
      // check to see that we found a whole word
      if (lpFound > lpStart)
         {
         LPSTR lpTemp;
         LPSTR lpCheckSpot;

         lpCheckSpot = lpFound;
         lpCheckSpot -= 1;
         lpFound++;
         nSize = giMyIniLength - (lpFound - glpMyIniFileBuffer);
         while (lpFound && (*lpCheckSpot != '\r') && (*lpCheckSpot != '\n'))
            {
            lpTemp = lpFound;
            lpFound = ScanFwd(szTemp,lpTemp,(int)nSize);
            if (lpFound)
               {
               lpCheckSpot = lpFound;
               lpCheckSpot -= 1;
               lpFound++;
               nSize = giMyIniLength - (lpFound - glpMyIniFileBuffer);
               }
            }
         if (lpFound)
            lpFound--;
         }

      if (lpFound)
         {
         iWordSize = strlen(szTemp);
         lpFound += iWordSize;
         while ((*lpFound != '\r') && (*lpFound != '\n'))
            szTemp[i++] = *lpFound++;
         szTemp[i] = 0;
         strcpy(lpReturnedString,szTemp);
         return (strlen(szTemp));
         }
      }
   else
      {
      strcpy(lpReturnedString,lpDefault);
      return 0;
      }
	return 0;
   }

//@doc SECEdit
//@mfunc Internal function. Determines if lpStartLine is one line of a multi-
//              line comment.
//@rdesc BOOL 
//@parm PLINEDESC lpStartLine
//@parm  LPTSTR lpCommentStart
//@parm  LPTSTR lpCommentEnd
BOOL SECEdit::_InComment(PLINEDESC lpStartLine, LPSTR lpCommentStart, LPSTR lpCommentEnd, 
								 LPSTR lpSingleLineComment)
{

	PLINEDESC lpLine = lpStartLine;
	LPSTR lpText;
	int iLength;
	LPSTR lpFound;

   // if no comment defined, we're done
   if (!lpCommentStart[0])
      return FALSE;

   // if no multiline comments, we're done
   if (!lpCommentEnd[0])
      return FALSE;

	lpLine = lpLine->pPrev;
	if (!lpLine)
		return FALSE;

   while (lpLine)
      {
      lpText = lpLine->pText;

      if (!lpText[0])
         return FALSE;

      iLength = lpLine->iTxtSize;

		lpFound = ScanBack(lpCommentStart,lpText,iLength);
      if (!lpFound || InQuote(lpText,lpFound))
			{
			if (lpFound)
				{
				// we were in a quote, so search the rest of the line
searchagain:
				LPSTR lpRest;
				while ((lpRest = ScanBack(lpCommentStart,lpText,iLength-strlen(lpFound))) != NULL)
					{
					if (!InQuote(lpText, lpRest))
						{
						lpFound = lpRest;
						goto searchforward;
						}
					// what we found was in a quote again, so keep searching
					lpFound = lpRest;
					}
				}
			lpLine = lpLine->pPrev;
			continue;
			}
		else
			{
			// one more possibility:  what we found might be inside a single line comment
			if (lpSingleLineComment)
				{
				LPSTR lpRest = NULL;
				while ((lpRest = ScanBack(lpSingleLineComment,lpText,
					     iLength-strlen(lpFound)+1)) != NULL)
					{
					// we were inside a single line comment,
					// unless it was inside a quote
					lpFound = lpRest;
					if (!InQuote(lpText, lpRest))
						{
						// now we have to resume searching the rest of the line
						// for another lpCommentStart
						goto searchagain;
						}
					}
				}
searchforward:
			// found a start comment.  Search forward
			// for a matching end
			lpFound = ScanFwd(lpCommentEnd, lpFound, strlen(lpFound));
			if (lpFound && !InQuote(lpText,lpFound))
				return FALSE;

			lpLine = lpLine->pNext;
			while (lpLine && (lpLine != lpStartLine))
				{
				lpText = lpLine->pText;

				if (!lpText[0])
					return FALSE;

				iLength = lpLine->iTxtSize;

				lpFound = ScanFwd(lpCommentEnd,lpText,iLength);
				if (lpFound && !InQuote(lpText,lpFound))
					return FALSE;
		
				lpLine = lpLine->pNext;
				}

			return TRUE;
			}
		}

	return FALSE;
}

//@doc SECEdit
//@mfunc Internal function. Determines if lpStartLine is one line of a multi-
//              line comment.
//@rdesc BOOL 
//@parm PLINEDESC lpStartLine
//@parm  int *iType
BOOL SECEdit::InComment(PLINEDESC lpStartLine, int *iType)
{
	PLINEDESC lpLine;
	LPSTR lpSingleLineComment = NULL;

   lpLine = lpStartLine;

   if (!lpLine)
      return FALSE;

	// run through once for each multiline comment defined
	if (!szCommentEnd2[0])
		lpSingleLineComment = szCommentStart2;
	if (_InComment(lpLine,szCommentStart1, szCommentEnd1, lpSingleLineComment))
		{
		*iType = 1;
		return TRUE;
		}

	if (!szCommentEnd1[0])
		lpSingleLineComment = szCommentStart1;
	else
		lpSingleLineComment = NULL;
	if (_InComment(lpLine,szCommentStart2, szCommentEnd2, lpSingleLineComment))
		{
		*iType = 2;
		return TRUE;
		}
   return FALSE;
}

#if 0
//@doc SECEdit
//@mfunc Internal function. Determines if lpStartLine is one line of a multi-
//              line comment.
//@rdesc BOOL 
//@parm PLINEDESC lpStartLine
//@parm  int *iType
BOOL SECEdit::InComment(PLINEDESC lpStartLine, int *iType)
{
   PLINEDESC lpLine;
   LPTSTR lpText, lpTemp;
   int i,j,iLength;

   // if no comment defined, we're done
   if (!szCommentStart1[0] && !szCommentStart2[0])
      return FALSE;

   // if no multiline comments, we're done
   if (!szCommentEnd1[0] && !szCommentEnd2[0])
      return FALSE;

   lpLine = lpStartLine->pPrev;

   if (!lpLine)
      return FALSE;

   for (i=0,j=1000;(i<j) && lpLine;i++)
      {
      lpText = lpLine->pText;

      if (!lpText[0])
         return FALSE;

      iLength = strlen(lpText)-m_iLineEndLength;

      if (
          (szCommentEnd1[0] && ScanBack(szCommentEnd1,lpText,iLength)) ||
          (szCommentEnd2[0] && ScanBack(szCommentEnd2,lpText,iLength))
         )
         return FALSE;

      if (szCommentEnd1[0])
			{
			lpTemp = ScanBack(szCommentStart1,lpText,iLength);
			if (lpTemp)
				{
				*iType = 1;
				if (!szCommentEnd2[0] && ScanBack(szCommentStart2,lpText,iLength))
					return FALSE;
				if (InQuote(lpText,lpTemp))
					return FALSE;
				return TRUE;
				}
         }

      if (szCommentEnd2[0])
			{
			lpTemp = ScanBack(szCommentStart2,lpText,iLength);
			if (lpTemp)
				{
				*iType = 2;
				if (!szCommentEnd1[0] && ScanBack(szCommentStart1,lpText,iLength))
					return FALSE;
				if (InQuote(lpText,lpTemp))
					return FALSE;
				return TRUE;
				}
         }


      lpLine = lpLine->pPrev;
      }

   return FALSE;
}
#endif

//@doc SECEdit
//@mfunc Internal function. IsCharAlphaNumeric plus a check of our extra chars.
//              See SetExtraChars()
//@rdesc BOOL 
//@parm int iChar
BOOL SECEdit::Myisalnum(int iChar)//ADMacroDone
   {
   if (!IsCharAlphaNumeric((char)iChar))
          // check our addon array
          if (!alnumarray[(unsigned char)iChar])
             return FALSE;

   return TRUE;
   }

//@doc SECEdit
//@mfunc Internal function. Brute force forward direction search
//@rdesc LPTSTR A pointer to successful match or NULL
//@parm LPTSTR lpPattern
//@parm LPTSTR lpText
//@parm  UINT  iLength
LPSTR SECEdit::ScanFwd(LPSTR lpPattern,LPSTR lpText, UINT  iLength)
   {
   LPSTR lpNext;
   LPSTR lpFound;
   LPSTR lpTemp;
   LPSTR lpCheckText;
   char cFirstChar;

	if (!lpScanBuffer)
		lpScanBuffer = (LPSTR)SECEditAlloc(OE_MAXLINELENGTH+32);

   if (!m_bCaseSense)
      {
       strcpy(lpScanBuffer,lpText);
      strupr(lpPattern);
      strupr(lpText);
      }

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
         {
         if (!m_bCaseSense)
            strcpy(lpText,lpScanBuffer);
         return lpFound;
         }

      iLength--;

      lpTemp++;

      if (iLength > 0)
         lpFound = lpNext = (LPSTR)memchr(lpTemp,cFirstChar,iLength);
      else
         break;
      }

   if (!m_bCaseSense)
      strcpy(lpText,lpScanBuffer);
   return NULL;
   }

//@doc SECEdit
//@mfunc Internal function that implements a brute force backward direction search
//@rdesc LPTSTR pointer to successful match or NULL
//@parm LPTSTR lpPattern
//@parm LPTSTR lpText
//@parm  int   iLength
LPSTR SECEdit::ScanBack(LPSTR lpPattern,LPSTR lpText, int   iLength)
   {
   LPSTR lpNext;
   LPSTR lpLastFound;
   LPSTR lpFound;
   LPSTR lpTemp;
   LPSTR lpCheckText;
   char cFirstChar;

	if (!lpScanBuffer)
		lpScanBuffer = (LPSTR)SECEditAlloc(OE_MAXLINELENGTH+32);

	// comment delimiters are assumed to be case-insensitive
	// restore this code if that's not the case
#if 0
   if (!m_bCaseSense)
      {
      strcpy(lpScanBuffer,lpText);
      strupr(lpPattern);
      strupr(lpText);
      }
#endif

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

      iLength --;
      lpTemp++;

      if (iLength > 0)
         lpFound = lpNext = (LPSTR)memchr(lpTemp,cFirstChar,iLength);
      else
         break;
      }

	// comment delimiters are assumed to be case-insensitive
	// restore this code if that's not the case
#if 0
   if (!m_bCaseSense)
      strcpy(lpText,lpScanBuffer);
#endif
   return lpLastFound;
   }

//@doc SECEdit
//@mfunc Internal function to look up lpWord in our hash table
//
//@rdesc int index
//@parm LPTSTR lpWord
//@parm  int iLength
int SECEdit::GetHashValue(LPSTR lpWord, int iLength)
   {
   char szTemp[_MAX_PATH];
   int i;
   int iHashValue;
   int iMyLength;

   iMyLength = min(iLength,_MAX_PATH-1);
   memcpy(szTemp,lpWord,iMyLength);
   szTemp[iMyLength] = 0;

   iHashValue = 0;
   for (i=0;i<iMyLength;i++)
      iHashValue = (iHashValue<<5) + iHashValue + szTemp[i];

   iHashValue &= (KEYWORDARRAYSIZE-1);
   return iHashValue;
   }

//@doc SECEdit
//@mfunc Internal function that finds the first complete word in lpString.  Letters 
//              considered parts of a word are alphanumeric plus
//              our extra chars.
//@rdesc LPTSTR pointer to word on success or NULL
//@parm LPTSTR lpString
//@parm  int *iWordLength

LPSTR SECEdit::GetNextWord(LPSTR lpString, int *iWordLength)
   {
   int iPos,iStart,iLength;
   LPSTR lpTemp;

   if (lpString)
      iLength = strlen(lpString);
   else
      iLength = 0;
   if (iLength>1)
      {
      /* (maybe) adjust for OE_CR/OE_LF */
      if (lpString[iLength-m_iLineEndLength] == m_szLineEnd[0])
         iLength -= m_iLineEndLength;
      }
   else
      return NULL;

   iPos = 0;

   // go forward til we find a starting char
   while ((iPos < iLength) &&
          !Myisalnum(lpString[iPos]) &&
          lpString[iPos] != '_')
      iPos++;

   iStart = iPos;

   if (iPos == iLength)
      return NULL;

   /* now iPos points to the first character of a word */
   *iWordLength = 0;
   while ((iPos <= iLength) && (Myisalnum(lpString[iPos]) || (lpString[iPos] == '_')))
      {
      (*iWordLength)++;
      // is it one of our terminator characters?
      if (terminatorarray[lpString[iPos]])
         break;

      iPos++;
      if (startarray[lpString[iPos]])
         break;
      }

   lpTemp = lpString + iStart;
   return lpTemp;
   }


//@doc SECEdit
//@mfunc Internal function - Determines whether lpszText contains any keywords.
//@rdesc BOOL FALSE - no keywords found.  TRUE - keyword found the starting and ending columsn are returned in iStart and iEnd.
//@parm LPTSTR *s
//@parm  int *iStart
//@parm  int *iEnd
BOOL SECEdit::KeyWord (LPSTR *s, int *iStart, int *iEnd)
   {

	USES_CONVERSION_T;

   LPSTR lpNext;
   LPSTR lpWord;
   int iHashValue;
   int iLength;
   int iIndex;
   int iMyLength;
   LPCKEYWORD lpTempNames;
   char szWord[_MAX_PATH];

	if (!lpNames)
		if (!GetKeyWords())
			return FALSE;

   lpNext = *s;

   while (*lpNext)
      {

      lpWord = GetNextWord(lpNext,&iLength);
      if (!lpWord)
         return FALSE;

      // make an sz string out of it
      iMyLength = min(iLength,sizeof(szWord)-1);
      memcpy(szWord,lpWord,iMyLength);
      szWord[iMyLength] = 0;

      if (!m_bCaseSense)
         strupr(szWord);

      //we can handle KEYWORDSIZE-1 word lengths
      if (iMyLength >= (KEYWORDSIZE-1))
         {
         // this word is too long for our hash table, so look it up directly
         char szBuf[128];
			char szGodzilla[] = "";
         if (GetPrivateProfileStringA(T2CA(strKeyWordSection),szWord,szGodzilla,szBuf,
                                       sizeof(szBuf),T2CA(m_strKeywordList)))
            {
            *iStart = lpWord - *s;
            *iEnd = *iStart+iLength;
            *s = lpWord;
				m_crKeyWord = GetColorRef(szWord);
            return TRUE;
            }
         else
				{

                // Noooooooo! Do it like it is done at the end of the while loop.
                // Otherwise text like
                //
                // xxxxxx  xxxxxxxxxxxxxxxxxxxxxIF
                //
                // syntax colors the "IF" substring. The conditions under which
                // this error occurs are as follows
                //
                // 1) There must be a whitespace gap between two identifiers that
                //    is as long as the keyword (here: two spaces for the two
                //    character long keyword "IF")
                //
                // 2) The string containing the keyword must be long enough to be
                //    not hashed (i.e. >= KEYWORDSIZE-1)
                //
                // 3) The keyword must be the trailing substring of the oversize
                //    identifier.

#ifdef ORIGINAL_VERSION
				lpNext += iLength;
#else
                lpNext = lpWord + iLength;
#endif
				continue;
				}
         }

      iIndex = iHashValue = GetHashValue(szWord,iMyLength);

      lpTempNames = lpNames + iHashValue;

      if (lpTempNames->szWord[0])
         {
			BOOL bDifferent;
			if (m_bCaseSense)
				bDifferent = strcmp(lpTempNames->szWord,szWord);
			else
				bDifferent = strcmpi(lpTempNames->szWord,szWord);
         while (lpTempNames->szWord[0] && bDifferent)
            {
            iIndex++;
            if (iIndex == iHashValue)        // been around once?
					{

                    // Noooooooo! Do it like it is done at the end of the outermost while loop.

#ifdef ORIGINAL_VERSION
					lpNext += iLength;
#else
                    lpNext = lpWord + iLength;
#endif
					continue;
					}
            if (iIndex == KEYWORDARRAYSIZE)
               {
               iIndex = 0;
               lpTempNames = lpNames;
               }
            else
               lpTempNames++;
				if (m_bCaseSense)
					bDifferent = strcmp(lpTempNames->szWord,szWord);
				else
					bDifferent = strcmpi(lpTempNames->szWord,szWord);
            }                              // look at the next guy

         if (lpTempNames->szWord[0])    // we either found it or it's empty
            {
            *iStart = lpWord - *s;
            *iEnd = *iStart+iLength;
            *s = lpWord;
				if (lpTempNames->cr == (DWORD)-1)
					lpTempNames->cr = GetColorRef(lpTempNames->szWord);
				m_crKeyWord = lpTempNames->cr;
            return TRUE;
            }
         }

      lpNext = lpWord + iLength;
      }

   return FALSE;
   }

COLORREF SECEdit::GetColorRef(LPSTR lpWord)
	{

	USES_CONVERSION_T;

	char szColorTag[128];
	char szColorName[128];
	int dwRval;
	COLORREF cr;

	// get a default color value to return
	cr = m_crKeyWord;

	// get the color tag for this word
	dwRval = GetPrivateProfileStringA(T2CA(strKeyWordSection),lpWord,"1",szColorTag,
												sizeof(szColorTag)-1,T2CA(m_strKeywordList));
	if (dwRval > (sizeof(szColorTag)-1))
		return cr;

	// try to look up this color tag
	char szGodZilla[] = ("godzilla");
	dwRval = GetPrivateProfileStringA("Colors",szColorTag,szGodZilla,szColorName,
												sizeof(szColorName)-1,T2CA(m_strKeywordList));
	if (dwRval > (sizeof(szColorName)-1))
		return cr;

	if (strcmp(szGodZilla,szColorName))
		cr = COLORREFFromString(szColorName);

	return cr;
	}

//@doc SECEdit
//@mfunc Internal function. Search forward for a comment start.
//@rdesc int -1 no comments, >-1 a comment was found, return is the starting column.
//@parm LPTSTR s
//@parm  int iLength
//@parm  int *iType
int SECEdit::CommentStart(LPSTR s, int iLength, int *iType)
   {
   LPSTR lpTemp;
   LPSTR lpTemp1;
   LPSTR lpTemp2;
   int iType1;

   lpTemp1 = ScanFwd(szCommentStart1,s,iLength);
   lpTemp2 = ScanFwd(szCommentStart2,s,iLength);
   if (lpTemp1 && lpTemp2)
      {
      // which one happens first ?
      if (lpTemp1 < lpTemp2)
         {
         lpTemp = lpTemp1;
         iType1 = 1;
         }
      else
         {
         lpTemp = lpTemp2;
         iType1 = 2;
         }
      }
   else if (lpTemp1)
      {
      lpTemp = lpTemp1;
      iType1 = 1;
      }
   else if (lpTemp2)
      {
      lpTemp = lpTemp2;
      iType1 = 2;
      }
   else
      return -1;

	if (InQuote(s,lpTemp))
      return -1;
   else
      {
      *iType = iType1;
      return (lpTemp-s);
      }

   }

//@doc SECEdit
//@mfunc Internal function. Determine if lpSpot is inside a quote.
//@rdesc BOOL 
//@parm LPTSTR lpString
//@parm  LPTSTR lpSpot
BOOL SECEdit::InQuote(LPSTR lpString, LPSTR lpSpot)
	{
   int i = 0;
   while (lpString != lpSpot)
      {
      if ((*lpString == '"') || (*lpString == '\'')) 
			{
            // Escape character in ST is '$'
#ifdef ORIGINAL_VERSION
			if (!i || (i && (lpString[-1] != '\\')))
#else
			if (!i || (i && (lpString[-1] != '$')))
#endif
	         i++;
			}
      lpString++;
      }

	if (i % 2)  // look for a closing quote
		{
		while (*lpString)
			{
	      if (((*lpString == '"') || (*lpString == '\'')) &&
                 // Escape character in ST is '$'
#ifdef ORIGINAL_VERSION
				 (lpString[-1] != '\\'))
#else
				 (lpString[-1] != '$'))
#endif
				return TRUE;
	      lpString++;
			}
		}
	return FALSE;
	}


 
//@doc SECEdit
//@mfunc Internal function that searches forward for a comment end.
//@rdesc int the ending column of comment. 
//@parm LPTSTR s
//@parm  int iLength
//@parm  int iType
int SECEdit::CommentEnd(LPSTR s, int iLength, int iType)
   {
   LPSTR lpTemp;
   int i;

   if (iType == 1)
      {
      if (!szCommentEnd1[0])
         {
         m_bInComment = FALSE;
         return iLength;
         }

     i = strlen(szCommentEnd1);
      if (i <= iLength)
         lpTemp = ScanFwd(szCommentEnd1,s,iLength);
      else
         lpTemp = NULL;
      if (!lpTemp)
         {
         m_bInComment = TRUE;
         return iLength;
         }
      else
         {
         m_bInComment = FALSE;
			for (i = 0;szCommentEnd1[i];i++);
         return ((lpTemp-s)+i);
         }
      }
      
   if (iType == 2)
      {
      if (!szCommentEnd2[0])
         {
         m_bInComment = FALSE;
         return iLength;
         }

      i = strlen(szCommentEnd2);
      if (i <= iLength)
			{
         lpTemp = ScanFwd(szCommentEnd2,s,iLength);
			if (lpTemp && InQuote(s,lpTemp))
				lpTemp = NULL;
			}
      else
         lpTemp = NULL;
      if (!lpTemp)
         {
         m_bInComment = TRUE;
         return iLength;
         }
      else
         {
         m_bInComment = FALSE;
			for (i = 0;szCommentEnd2[i];i++);
         return ((lpTemp-s)+i);
         }
     }
   return NULL;
   }


//@doc SECEdit
//@mfunc Internal function. Converts a COLORREF to a string of the format
//              "%d, %d, %d"
//@rdesc void 
//@parm COLORREF cr
//@parm  LPTSTR lpString
void SECEdit::StringFromCOLORREF(COLORREF cr, LPSTR lpString)
   {
	int red,green,blue;
	red   = GetRValue(cr);
	green = GetGValue(cr);
	blue  = GetBValue(cr);
	sprintf(lpString,"%d,%d,%d",red,green,blue);
	}

//@doc SECEdit
//@mfunc Internal function.  Converts a string of the format "%d, %d, %d" to a 
//              COLORREF
//@rdesc COLORREF 
//@parm LPCTSTR lpString
COLORREF SECEdit::COLORREFFromString(LPCSTR lpString)
   {
   char szTemp[6];
   int i,j;
   int red,green,blue;
   COLORREF ref;

   red = green = blue = 0;

   for (i=j=0;lpString[i] && (lpString[i] != ',');i++,j++)
      szTemp[j] = lpString[i];
   szTemp[j] = 0;
   red = atoi(szTemp);

   if (lpString[i])
      i++;
   for (j=0;lpString[i] && (lpString[i] != ',');i++,j++)
      szTemp[j] = lpString[i];
   szTemp[j] = 0;
   green = atoi(szTemp);

   if (lpString[i])
      i++;
   for (j=0;lpString[i] && (lpString[i] != ',');i++,j++)
      szTemp[j] = lpString[i];
   szTemp[j] = 0;
   blue = atoi(szTemp);

   ref = RGB(red,green,blue);

   return ref;
   }



//@doc SECEdit
//@mfunc Internal function.  Builds a hashed table of our keywords
//@rdesc void 
//@parm LPCKEYWORD lpNames
//@parm LPTSTR lpTemp
void SECEdit::BuildNameTable(LPCKEYWORD lpNames,LPSTR lpTemp)
   {
   LPCKEYWORD lpTempNames;
   char szWord[128];
   int iHashValue;
   int i,j;
	
   lpTempNames = lpNames;


   while (*lpTemp)
      {
      i = 0;
      while (*lpTemp)
         {
         szWord[i] = *lpTemp;
         lpTemp++;
         i++;
         }

      szWord[i] = 0;
      j = i;
      szWord[i+1] = 0;
          
      //we can handle KEYWORDSIZE-1 word lengths
      if (i > (KEYWORDSIZE-1))
         {
         lpTemp++;
         continue;
         }

      if (!m_bCaseSense)
         strupr(szWord);

      iHashValue = GetHashValue(szWord,j);
      lpTempNames = lpNames;
      lpTempNames += iHashValue;

      j = iHashValue;

      while ((j < KEYWORDARRAYSIZE) && lpTempNames->szWord[0])
         {

         
         if (j==(KEYWORDARRAYSIZE-1)) // wrap around
            {
            j = 0;
            lpTempNames = lpNames;
            }
         else
            {
            lpTempNames++;
            j++;
            }
         }

      strcpy(lpTempNames->szWord,szWord);
		lpTempNames->cr = (DWORD)-1;
      lpTempNames->iLength = i;
      lpTemp++;
      }

   }


//@doc SECEdit
//@mfunc Internal function. Sets up the data for syntax coloring.  Gets default 
//              values from the Registry key set in m_strSection.
//
//@rdesc void 
void SECEdit::InitChroma()
	{
	USES_CONVERSION_T;

	CString strTemp;
  
	FreeTempBuffers();

	strKeyWordSection = _T("Keywords");
	m_strKeywordList = AfxGetApp()->GetProfileString(m_strSection,strKeyWordSection,_T("Default.ini"));

	// see if we can open it as is
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(m_strKeywordList,status))
		{
		// nope, let's see if it's just a basename and look in the app's directory
		TCHAR szTemp[_MAX_PATH];
		::GetModuleFileName(NULL,szTemp,sizeof(szTemp));
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];
		_tsplitpath(szTemp,szDrive,szDir,szName,szExt);
		_tmakepath(szTemp,szDrive,szDir,m_strKeywordList,afxEmptyString);
		m_strKeywordList = szTemp;
		}
	else
		m_strKeywordList = status.m_szFullName;

	m_bChroma = AfxGetApp()->GetProfileInt(m_strSection,_T("Chroma"),0);

   // get the colors
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Comment"),_T("0,128,0"));
   m_crComment = COLORREFFromString(T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Keyword"),_T("0,0,255"));
   m_crKeyWord = COLORREFFromString(T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Quote"),_T("255,0,0"));
   m_crQuote = COLORREFFromString(T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Default Text"),_T("0,0,0"));
   m_crText = COLORREFFromString(T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Background"),_T("255,255,255"));
   m_crBackground = COLORREFFromString(T2CA(strTemp));

	char szColor[64];
	CClientDC dc(AfxGetApp()->m_pMainWnd);
	m_crSQuote = dc.GetTextColor();
	StringFromCOLORREF(m_crSQuote, szColor);
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("SQuote"),A2T(szColor));
   m_crSQuote = COLORREFFromString(T2CA(strTemp));

   // get the comment delimiters
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Comment Start 1"),afxEmptyString);
	strcpy(szCommentStart1,T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Comment Start 2"),afxEmptyString);
	strcpy(szCommentStart2,T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Comment End 1"),afxEmptyString);
	strcpy(szCommentEnd1,T2CA(strTemp));
	strTemp = AfxGetApp()->GetProfileString(m_strSection,_T("Comment End 2"),afxEmptyString);
	strcpy(szCommentEnd2,T2CA(strTemp));

	m_bAutoIndent = AfxGetApp()->GetProfileInt(m_strSection,_T("Autoindent"),1);
   // are we case sensitive?
	m_bCaseSense = AfxGetApp()->GetProfileInt(m_strSection,_T("Case"),0);

   // get the extra word characters if any
   SetExtraChars();
	GetKeyWords();
	}	


//@doc SECEdit
//@mfunc Internal function. Grabs the list of keywords from the disk file
//@rdesc BOOL TRUE on success, FALSE on failure.

BOOL SECEdit::GetKeyWords()
   {
	USES_CONVERSION_T;

   LPSTR lpTemp = NULL;

   // free the old guy
   if (lpNames)
      {
      free(lpNames);
      lpNames = NULL;
      }

   // free the ini file
   if (glpMyIniFileBuffer)
      {
      free(glpMyIniFileBuffer);
      glpMyIniFileBuffer = NULL;
      giMyIniLength = 0;
      }

   // read the ini file into our buffer for later
   struct _stat filestat;
   HANDLE h;
		DWORD dw;

   h = CreateFile(m_strKeywordList,             /* open pName         */
					   GENERIC_READ,                 /* open for reading and/or writing   */
					   FILE_SHARE_READ,              /* share flags        */
                  (LPSECURITY_ATTRIBUTES) NULL, /* no security     */
					   OPEN_EXISTING,                /* existing file only */
					   FILE_ATTRIBUTE_NORMAL,        /* normal file        */
					   (HANDLE) NULL);               /* no attr. template  */

   if (h != INVALID_HANDLE_VALUE)
      {
      _tstat(m_strKeywordList, &filestat );
      giMyIniLength = (int)filestat.st_size;
      glpMyIniFileBuffer = (LPSTR)malloc(giMyIniLength+32);
      memset(glpMyIniFileBuffer,0,giMyIniLength+2);
		::ReadFile(h, glpMyIniFileBuffer, giMyIniLength, (LPDWORD)&dw, NULL);
      CloseHandle(h);
      }
	else
		{
		m_bChroma = FALSE;
		return FALSE;
		}

   // allocate a buffer to hold all the keywords
	DWORD dwSize = 0x9999 * sizeof(TCHAR); // max size for an ini file section
   lpTemp = (LPSTR)malloc(dwSize);
	if (!lpTemp)
		{
		m_bChroma = FALSE;
		return FALSE;
		}
   // get the entire section
	char szDummy[16];

   if (!::GetPrivateProfileStringA(T2CA(strKeyWordSection),NULL,szDummy,lpTemp,dwSize,T2CA(m_strKeywordList)))
      {
		// try to use the base filename, a la old WinEdit
		char szDrive[_MAX_DRIVE];
		char szDir[_MAX_DIR];
		char szName[_MAX_FNAME];
		char szExt[_MAX_EXT];
		_splitpath(T2CA(m_strKeywordList),szDrive,szDir,szName,szExt);
	   if (!::GetPrivateProfileStringA(szName,NULL,szDummy,lpTemp,dwSize,T2CA(m_strKeywordList)))
			{
			m_bChroma = FALSE;
			free(lpTemp);
			return FALSE;
			}
      }
      

   // allocate a buffer to hold the keywords
   lpNames = (LPCKEYWORD)malloc(sizeof(CKEYWORD)*KEYWORDARRAYSIZE);
	if (!lpNames)
		{
		m_bChroma = FALSE;
      free(lpTemp);
      return FALSE;
      }

	memset(lpNames,0, sizeof(CKEYWORD)*KEYWORDARRAYSIZE);
   BuildNameTable(lpNames,lpTemp);

   free(lpTemp);
   return TRUE;
   
   }


//@doc SECEdit
//@mfunc Internal function. Frees allocated items
//@rdesc void 
void SECEdit::FreeTempBuffers()
	{
	if (glpMyIniFileBuffer)
		{
		free(glpMyIniFileBuffer);
		glpMyIniFileBuffer = NULL;
		}
	if (lpNames)
		{
		free(lpNames);
		lpNames = NULL;
		}
	if (lpScanBuffer)
		{
		free(lpScanBuffer);
		lpScanBuffer = NULL;
		}
	}
