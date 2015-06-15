/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_string.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_string.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_string.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==String handling helper functions and classes
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_STRING_H_
#define FC_STRING_H_

#include <tchar.h>  //define TCHAR 
#include <wtypes.h> //define BSTR and GUID
#include <malloc.h> //define alloca

#include "fc_defines.h" 

#ifdef __cplusplus
extern "C"{
#endif

//-------------- Usefull string routines --------------------------------
/**
 * FC_StringTrim[Left|Right] trims a string left or right or both
 * @param           psz : string to work on
 * @return          The new _tcslen
*/
FC_TOAPI int FC_TODECL FC_StringTrim(TCHAR* psz);
FC_TOAPI int FC_TODECL FC_StringTrimLeft(TCHAR* psz);
FC_TOAPI int FC_TODECL FC_StringTrimRight(TCHAR* psz);

//------------------------------------------------------------------*
/**
 * FC_StringRemoveLastChar removes the last char if one of a set
 * or if set is NULL the remove always the last char.
 * @param           sz : string to work on
 * @param           pszIfOneOf : NULL: remove last. !NULL: only if one of
 * @param           pLen  : if !NULL: receives the new _tcslen
 * @return          The char that has been removed or 0 if nothing was removed
*/
FC_TOAPI TCHAR FC_TODECL FC_StringRemoveLastChar(
    TCHAR        sz[/* *pLen*/],//string where to remove last
    const TCHAR* pszIfOneOf,    //NULL: remove last. !NULL: only if one of
    int*         pLen           //if pLen!=NULL: receives the new (shorter) _tcslen
);


//------------------------------------------------------------------*
/**
 * FC_StringStripBegin[I] test if a string start with another
 * [ignoring case] and if so also strips of that begin substring.
 * Note: if pszWith=="" the function always returns >=0.
 * @param   psz     : string to work on
 * @param   pszWith : assumed begin of psz
 * @return  -1: if not begins with 
            >=0 the new (shorter) _tcslen(psz)
*/
FC_TOAPI int FC_TODECL FC_StringStripBegin(
    TCHAR*       psz,    
    const TCHAR* pszWith
);
FC_TOAPI int FC_TODECL FC_StringStripBeginI(
    TCHAR*       psz,
    const TCHAR* pszWith
);
//------------------------------------------------------------------*
/**
 * FC_StringBegin[I] test if a string starts with another
 * [ignoring case].
 * Note: if pszWith=="" the function always returns true.
 * @param   psz     : string to work on
 * @param   pszWith : assumed begin of psz
 * @return  true if beginns with false if not
*/
__inline int FC_StringBegin(
    const TCHAR* psz,    
    const TCHAR* pszWith
)
{
    return _tcsncmp(psz, pszWith, _tcslen(pszWith))==0;
}
__inline int FC_StringBeginI(
    const TCHAR* psz,    
    const TCHAR* pszWith
)
{
    return _tcsnicmp(psz, pszWith, _tcslen(pszWith))==0;
}

//------------------------------------------------------------------*
/**
 * FC_StringIncBegin[I] test if a string start with another
 * [ignoring case]. If *ppsz begins with pszWith *ppsz is incremented
 * to point to the first char right after pszWith.
 * Note: if pszWith=="" the function always returns true.
 * @param   ppsz    : address of string to work on
 * @param   pszWith : assumed begin of psz
 * @return  true if begins with false if not
*/
__inline int FC_StringIncBegin(const TCHAR** ppsz, const TCHAR* pszWith)
{
    int iLen = _tcslen(pszWith);
    return _tcsncmp(*ppsz, pszWith, iLen)==0 ? *ppsz+=iLen, 1 : 0;
}
__inline int FC_StringIncBeginI(const TCHAR** ppsz, const TCHAR* pszWith)
{
    int iLen = _tcslen(pszWith);
    return _tcsnicmp(*ppsz, pszWith, iLen)==0 ? *ppsz+=iLen, 1 : 0;
}


//------------------------------------------------------------------*
/**
 * FC_StringToWideChar convert a multi byte to unicode string
 * @param           pszMulti : multi byte string to convert
 * @param           pszWide : buffer to receive the unicode result
 * @param           iLen  : size of buffer in wchars
 * @return          0 if any error except if buffer to small error 
 *                  if buffer was to small iLen is returned and pszWide[iLen-1]==0
 *                  if successfull wcslen(pszWide) +1
 *                   
*/
FC_TOAPI int FC_TODECL   FC_StringToWideChar(
    const char* pszMulti,
    wchar_t     pszWide[/*iLen*/],
    int         iLen
);


//------------------------------------------------------------------*
/**
 * FC_StringToMultiByte convert a unicode string to multi byte
 * @param           pszWide : unicode string to convert
 * @param           pszMulti : buffer to receive the multi byte result
 * @param           nBytes  : size of buffer in bytes
 * @return          0 if any error except if buffer to small error 
 *                  if buffer was to small nBytes is returned and 
 *                  pszMulti[nBytes-1]==0
 *                  if successfull return is strlen(pszMulti) +1
 *                   
*/
FC_TOAPI int  FC_TODECL  FC_StringToMultiByte(
    const wchar_t* pszWide, 
    char           pszMulti[/*nBytes*/], 
    int            nBytes
);

//------------------------------------------------------------------*
/**
 * FC_StringToBSTR  converts a string to a BSTR
 * @param           psz  : string to convert to a BSZT
 * @return          pointer to new BSTR or NULL if out of memory
 */
__inline FC_TOAPI BSTR FC_TODECL FC_StringToBSTR(const TCHAR* psz)
{
#ifdef UNICODE
    return SysAllocString(psz);
#else
    int      nWChars = _tcslen(psz)+1;
    wchar_t* pszWide = (wchar_t*)alloca(sizeof(wchar_t)*nWChars);
    FC_StringToWideChar(psz, pszWide, nWChars);
    return SysAllocString(pszWide);
#endif
}


//------------------------------------------------------------------*
/**
 * FC_String[C]GetAfterLast searches for the last occurence of 
 *                  a set of chars either as const TCHAR* or non const.
 *                  
 * @param           psz  : string where to search
 * @param           pszLastCh  : string containing the chars to find
 * @return          pointer to the char right after last occurence of 
 *                  one of pszLastCh in psz or NULL if psz has no char of 
 *                  pszLastCh.
 */
FC_TOAPI TCHAR* FC_TODECL  FC_StringGetAfterLast(
    TCHAR*       psz, 
    const TCHAR* pszLastCh
);
__inline  const TCHAR* FC_StringCGetAfterLast(
    const TCHAR* psz, 
    const TCHAR* pszLastCh
)
{
    return FC_StringGetAfterLast((TCHAR*)psz, pszLastCh);
}

//------------------------------------------------------------------*
/**
 * FC_StringCutAtLast searches for the last occurence of a set of chars
 *                    and truncates at last occurence.
 * @param           psz  : string to cut
 * @param           pszLastCh  : string containing the chars to find
 * @return          the new length after cut psz at last occurence of
 *                  one of pszLastCh. 
 *                  returns old length if no last occurence.
 */
FC_TOAPI int FC_TODECL  FC_StringCutAtLast(
    TCHAR*       psz, 
    const TCHAR* pszLastCh
);


//------------------------------------------------------------------*
/**
 * FC_StringCopyToBuffer can be used to safely copy a null-terminated 
 * string to a fixed sized buffer. If the buffer is to small the function 
 * returns false and does not modify the buffer at all. Otherwise return 
 * true and the buffer contains a null-terminated copy of pszSrc.
 *                    
 * @param    dest  : buffer for copy result
 * @param    nBuf  : size of dest in TCHARs
 * @param    pszSrc: source string
 * @return   false if buffer is to short
 */
__inline int FC_StringCopyToBuffer(TCHAR dest[/*nBuf*/], int nBuf, const TCHAR* pszSrc)
{
    int iLen = _tcslen(pszSrc)+1;
    return iLen<=nBuf? memcpy(dest, pszSrc, iLen*sizeof(TCHAR)), 1 : 0;
}


//------------------------------------------------------------------*
/**
 * FC_StringStartsWith tests if psz starts with a substring.
 *                    
 * @param    psz  : string to test if starts with
 * @param    pszStart  : the assumed start sub string
 * @return   -1 if does not start with or >=0 the index into psz 
 *           of the next char right after the start.
 */
__inline int FC_StringStartsWith(const TCHAR* psz, const TCHAR* pszStart)
{
    int i;
    for(i=0; pszStart[i] && psz[i]==pszStart[i]; i++)
        ;
    return pszStart[i]?-1:i;
}

//------------------------------------------------------------------*
/**
 * FC_StringStartsWithI tests if psz starts with a substring, but 
 * ignore case.
 *                    
 * @param    psz  : string to test if starts with
 * @param    pszStart  : the assumed start sub string
 * @return   -1 if does not start with or >=0 the index into psz 
 *           of the next char right after the start.
 */
__inline int FC_StringStartsWithI(const TCHAR* psz, const TCHAR* pszStart)
{
    int i = _tcslen(pszStart);
    return _tcsncicmp(psz, pszStart, i)?-1:i;
}


//------------------------------------------------------------------*
/**
 * FC_StringSkipWhite increments psz up to the first non white space
 * character.
 *                    
 * @param    psz  : in/out will be incremented as long as _istspace(*psz)
 *                  is true.
 * @return   the number of white space TCHARs skipped
 *           or 0 if psz does not beginn with any white space.
 */
__inline int FC_StringSkipWhite(const TCHAR** ppsz)
{
    const TCHAR* p;
    int          nc;
    for(p = *ppsz; _istspace(*p); p = _tcsinc(p))
        ;
    nc = p - *ppsz;
    *ppsz = p;
    return nc;
}



//------------------------------------------------------------------*
/**
 * FC_StringMatchFilePattern win32 file pattern match. The pszPattern
 * may contain multiple * and ? wildcards. The function works always
 * ignore case (as file names are too)
 *
 *
 * @param           pszToken   the token to match, leading and
 *                             trailing whitespace is not automatically 
 *                             stripped by this function.
 * @param           pszPattern the pattern to match
 * @return          0          if no match
 *                  1          if pattern matches token
 * @exception       -
 * @see             
*/
FC_TOAPI int FC_TODECL FC_StringMatchFilePattern(
    const TCHAR*  pszToken, 
    const TCHAR*  pszPattern 
);


//------------------------------------------------------------------*
/**
 * FC_StrMatchI win32 file pattern match enhanced by alternatives.
 * The pszPattern may contain multiple * and ? wildcards and | will seperate 
 * individual unrelated patterns. This function works ignore case.
 * If | is used the first maching pattern wins.
 *
 * @param           pszToken:   The token to match, leading and trailing whitespace is 
 *                              not automatically stripped by this function.
 * @param           pszPattern: The pattern to match, examples: 
 *                              "*",  "*.gif",  "*.gif|*.bmp", 
 *                              "d:\*.?zyz|c:\thesefiles.*|\\S21\myShare\y.txt"
 *
 * @return          NULL:       If no match possible.
 *                  not NULL:   Pointer to the start of the first matching pattern 
 *                              (points into pszPattern), example: 
 *                              "c:\thesefiles.*|\\S21\myShare\y.txt
*/
FC_TOAPI const TCHAR* FC_TODECL FC_StrMatchI(
    const TCHAR*  pszToken, 
    const TCHAR*  pszPattern 
);



//------------------------------------------------------------------*
/**
 * FC_StringSplitBuf splitts a buffer in place into tokens seperated by 
 * a given character.
 *
 * @param           pszBuffer:  in/out seperators are replaced by 0-chars,
 *                              and if trimmed, also the white spaces are 0ed.
 * @param           chSep:      split seperator
 * @param           ppszParts:  receives pointers into pszBuffer
 * @param           nMaxParts:  size of ppszParts
 * @param           nTrimStyle: 0: fields are not trimmed  
 *                              1: the fields are trimmed left and right
 *
 * @return          >=0:        Number of tokens found, at most nMaxParts.
 *                              The function stopes processing the buffer
 *                              if nMaxParts is reached.
*/
FC_TOAPI int FC_TODECL FC_StringSplitBuf(
    TCHAR*       pszBuffer,
    TCHAR        chSep,
    TCHAR*       ppszParts[/*nMaxParts*/],
    int          nMaxParts, 
    int          nTrimStyle
);

//------------------------------------------------------------------*
/**
 * FC_StrGetID read a C/C++ idenfifer *ppsz and stores it into szId.
 *
 * @param           ppsz:       gets incremented to the next char after id
 *                              and optionally following white space. 
 * @param           bSkipWhite: increment any white space befor and after the id.
 * @param           szId:       receives the identifer
 * @param           nBuff:      size of szId buffer
 *
 * @return          0:          OK
 *                  1:          first char of id is not [_a-zA-Z]
 *                  2:          buffer too short (or id too long..)
*/
FC_TOAPI int FC_TODECL FC_StrGetID(
    const TCHAR** ppsz, 
    int           bSkipWhite, 
    TCHAR         szId[/*nBuff*/], 
    int           nBuff
);

//------------------------------------------------------------------*
/**
 * FC_StringHasWildCards test if a file name contains win32 wildcards.
 * @param           pszPattern  : file name to test.
 * @return          1 yes
 *                  0 no
 */
__inline int FC_StringHasWildCards(
    const TCHAR* pszPattern
)
{
    return _tcspbrk(pszPattern, _T("*?"))!=NULL;
}

//------------------------------------------------------------------*
/**
 * FC_StringIsValidFileName test if a file/dir name is valid for win32
 * @param           pszPattern  : file name to test.
 * @return          1 yes 0 no
 */
__inline int FC_StringIsValidFileName(
    const TCHAR* pszFile
)
{
    return _tcspbrk(pszFile, _T("\\/:*?\"<>|"))==NULL;
}


//------------------------------------------------------------------*
/**
 * FC_StringHasPath test if a file name contains a path seperator.
 * @param           pszFile  : file name to test.
 * @return          1 yes 0 no
 */
__inline int FC_StringHasPath(
    const TCHAR* pszFile
)
{
    return _tcspbrk(pszFile, FC_PATH_SEP) != NULL;
}

//------------------------------------------------------------------*
/**
 * FC_StringIsAbsPath test for absolute path.
 * @param           pszPath  : file path to test.
 * @return          1 [driveletter]: 
 *                  2 UNC path 
 *                  0 if pszPath is relative
 */
FC_TOAPI int          FC_TODECL  FC_StringIsAbsPath(
    const TCHAR* pszPath
);





//------------------------------------------------------------------*
/**
 * FC_StringGetFilePart returns a pointer to the file part if any or "".
 * @param           pszPath  : file path where to get the file part.
 * @return          pointer after the last FC_PATH_SEP char or pszPath.
 *                  If pszPath has no path seperator the function
 *                  returns the input string.
*/
FC_TOAPI const TCHAR* FC_TODECL FC_StringGetFilePart(
    const TCHAR* pszPath 
);

//------------------------------------------------------------------*
/**
 * FC_StringCutFilePart truncates the file path at the file part, if any.
 * @param           pszPath  : file path where to cut the file part. 
 * @return          returns new _tcslen after cut at the last FC_PATH_SEP
 *                  or old length if no path seperator found.
*/
FC_TOAPI     int      FC_TODECL FC_StringCutFilePart(
    TCHAR* pszPath 
);

//------------------------------------------------------------------*
/**
 * FC_StringGetFileExt gets a pointer to the file extension if any or ""
 * @param           pszPath  : file path to work on 
 * @return          a pointer after the last '.' before any FC_PATH_SEP
 *                  or "". Note:  "myfile." and "myfile" returns both ""
 *                  The empty string pointer "" is actually a pointer
 *                  to the input pszPath's 0-char
*/
FC_TOAPI const TCHAR* FC_TODECL FC_StringGetFileExt(
    const TCHAR* pszPath
);



#define FC_PATHM_FULL    0
#define FC_PATHM_KEEPREL 1
#define FC_PATHM_TRYREL  2  //<-----NOT YET IMPLEMENTED !!
//------------------------------------------------------------------*
/**
 * FC_StringCanonicalPath converts a full or relative path into the 
 * system canonical form. That is all ..\/ are resolved and if pszPath 
 * is not absolute it will be made absolute or keept relative to pszRoot.
 * The difference to win32 GetFullPathName is 
 *  - the possibillity to keep a relative path still relative to a given root.
 *  - if the pszPath ends in a path seperators, they are removed, this is to avoid
 *    problems when adding something to the path, if it is a path or a file must be 
 *    known anyway be context not from that syntax.
 *
 * @param    pszPath     : path to convert to canonical form.
 * @param    szCanPath   : result baffer
 * @param    nBuffer     : size of result buffer
 * @param    pszRoot     : an absolute path or NULL, 
 *                         if NULL and nMode==0   : use current working directory
 *                         if NULL and nMode==1,2 : pszPath is made canonical 
 *                                                  without any root info.
 * @param    nMode       : FC_PATHM_FULL: 
 *                            szCanPath will always be absolute
 *                         FC_PATHM_KEEPREL: 
 *                            if pszPath is relative szCanPath will also be relative
 *                            and may start with one or more "..\".
 *                         FC_PATHM_TRYREL: <-----NOT YET IMPLEMENTED !!
 *                            szCanPath is forced to be relative if at least one leading
 *                            directory part equals to pszRoot (or current working dir if 
 *                            pszRoot is NULL) 
 *
 * @return   the strlen of the final result, if returns 0 the buffer was to 
 *           short or any internal error, in this case szCanPath is always 
 *           set to an empty string.
 * @see      win32 GetFullPathName
 */
FC_TOAPI int FC_TODECL FC_StringCanonicalPath(
    const TCHAR* pszPath,
    TCHAR        szCanPath[/*nBuffer*/],
    int          nBuffer, 
    const TCHAR* pszRoot,
    int          nMode
);

 
//------------------------------------------------------------------*
/**
 * FC_StringFmtSysErr gets a system defined error description using 
 * the current local. Format system message, only system messages!
 * returns always a null terminated input szBuffer
 * If message not found: szBuffer is set to empty string
 * @param           dwSystemErr : the error number e.g. from GetLastError()
 * @param           szBuffer : receives message (right trimmed)
 * @param           nBuffer  : size of buffer in TCHARs must be at least >=64 
 * @return          pointer to szBuffer. If any error szBuffer[0] is set to 0
*/
FC_TOAPI const TCHAR* FC_TODECL FC_StringFmtSysErr(
    DWORD    dwSystemErr,           //from e.g.GetLastError()
    TCHAR    szBuffer[/*nBuffer*/], //receives message (right trimmed)
    unsigned nBuffer                //must be at least >=64
); 

#ifdef __cplusplus
}
#endif

#endif //FC_STRING_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: fc_string.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 26.09.03   Time: 13:11
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changes for acyclic tasks
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 23  *****************
 * User: Ef           Date: 21.01.03   Time: 15:26
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions
 * 
 * *****************  Version 22  *****************
 * User: Ef           Date: 27.11.02   Time: 12:32
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 21  *****************
 * User: Ef           Date: 26.11.02   Time: 21:28
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_StringCanonicalPath
 * 
 * *****************  Version 20  *****************
 * User: Ef           Date: 11.03.02   Time: 15:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * removed dead code
 * 
 * *****************  Version 19  *****************
 * User: Ef           Date: 22.10.01   Time: 11:12
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions
 * 
 * *****************  Version 18  *****************
 * User: Ef           Date: 11.05.01   Time: 13:01
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected some commets
 * 
 * *****************  Version 17  *****************
 * User: Ef           Date: 26.03.01   Time: 11:55
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fun: FC_StringSkipWhitSpace
 * 
 * *****************  Version 16  *****************
 * User: Ef           Date: 26.02.01   Time: 13:07
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 15  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 14  *****************
 * User: Ef           Date: 26.02.01   Time: 10:27
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions in fc_fileIO.h
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 15.02.01   Time: 18:40
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * fixed bug in tobstr
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 12.02.01   Time: 13:31
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fun FC_StringCGetAfterLast
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 29.01.01   Time: 10:47
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changed FC_StringGetPilePArt so that a pure file name does not 
 * return "", seem much better to work with.
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 22.01.01   Time: 11:01
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fc_System.h, some minor changes
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 15.01.01   Time: 17:34
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new funs
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 15.01.01   Time: 16:22
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * some new functions
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 13.01.01   Time: 15:12
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changed FC_StringGetFilePart and FC_StringGetFileExt to return "" and
 * not NULL seems to be more convinient and safe to use
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 12.01.01   Time: 17:13
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new stupid functions
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 9.01.01    Time: 16:44
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected some comments
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 9.01.01    Time: 16:36
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_CString in extra source file
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 8.01.01    Time: 18:51
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.01.01    Time: 10:41
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:08
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * from V1.3 tools.dll, and some new features
 *==
 *----------------------------------------------------------------------------*
*/
