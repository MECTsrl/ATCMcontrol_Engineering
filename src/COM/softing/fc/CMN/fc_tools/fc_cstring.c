/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_cstring.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_cstring.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_cstring.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
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
 *==Implementation of string buffer class.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


#include <malloc.h> //define alloca
#include "fc_tools\fc_libmem.h" 

#define FC_STRING_IMPL_C_ //need this define to get the FC_CString 
                          //Class helper functions declared in fc_string.h
#include "fc_tools\fc_cstring.h"



FC_TOAPI TCHAR* FC_TODECL  FC_StringDup(const TCHAR* psz)
{
    int    size;
    TCHAR* pszCopy;

    size = (_tcslen(psz)+1)*sizeof(TCHAR);
    if(!FC_LibNewMemory((void**)&pszCopy,  size))
        return NULL;
    return (TCHAR*)memcpy(pszCopy, psz, size);
}
FC_TOAPI TCHAR* FC_TODECL  FC_StringDupLen(const TCHAR* psz, int nChars)
{
    int    iLen;
    TCHAR* pszCopy;
    
    iLen = _tcslen(psz);
    if(nChars<iLen && nChars>=0)
        iLen = nChars;
    
    if(!FC_LibNewMemory((void**)&pszCopy,  (iLen+1)*sizeof(TCHAR)))
        return NULL;
    memcpy(pszCopy, psz, iLen*sizeof(TCHAR));
    pszCopy[iLen] = 0;
    return pszCopy;
}
#ifndef UNICODE
FC_TOAPI TCHAR* FC_TODECL  FC_StringDupW(const wchar_t* pszw)
{
    char*  pszCopy;
    int    iLen;
    iLen = wcslen(pszw);
    iLen = iLen*2+1;
    pszCopy = (char*)_alloca(iLen);
    FC_StringToMultiByte(pszw, pszCopy, iLen);
    return FC_StringDup(pszCopy);
}
FC_TOAPI TCHAR* FC_TODECL FC_StringDupLenW(const wchar_t* pszw, int nChars)
{
    char*    pszCopy;
    wchar_t* pszwHelp;
    int      iLen;
    
    iLen = wcslen(pszw);
    if(nChars<iLen && nChars>=0)
        iLen = nChars;
    
    pszwHelp = (wchar_t*)_alloca((iLen+1)*sizeof(wchar_t));
    memcpy(pszwHelp, pszw, iLen*sizeof(wchar_t));
    pszwHelp[iLen] = 0;

    iLen = iLen*2+1;
    pszCopy = (char*)_alloca(iLen);
    FC_StringToMultiByte(pszwHelp, pszCopy, iLen);

    return FC_StringDup(pszCopy);
}
#endif
FC_TOAPI void FC_TODECL  FC_StringFree(TCHAR* psz)
{
    if(psz)
        FC_LibFreeMemory(psz);
}




static TCHAR* g_FC_StringEmpty = _T("\0!empty string");
FC_TOAPI int FC_TODECL FC_StringNewString(
    FC_StringTYP* pData, 
    int           iLenPreAlloc
)
{
    FC_LIBASSERT(g_FC_StringEmpty[0]==0 && g_FC_StringEmpty[1]==_T('!'));

    if(iLenPreAlloc>0)
    {
        if(!FC_LibNewMemory((void**)&(pData->psz), (iLenPreAlloc+1)*sizeof(TCHAR)))
            return 0;

        pData->iMax = iLenPreAlloc;
        pData->iLen   = 0;
        pData->psz[0] = 0;
    }
    else
    {
        pData->iMax = 0;
        pData->iLen = 0;
        pData->psz  =  g_FC_StringEmpty;
    }

    return 1;
}

FC_TOAPI void FC_TODECL FC_StringFreeString(
    FC_StringTYP* pData
)
{
    if(pData->psz!=g_FC_StringEmpty) 
    {
        FC_LibFreeMemory(pData->psz);
        pData->iMax = 0;
        pData->iLen = 0;
        pData->psz  = g_FC_StringEmpty;
    }
    else
    {
        FC_LIBASSERT(pData->iMax==0 && pData->iLen==0 &&
            g_FC_StringEmpty[0]==0 && g_FC_StringEmpty[1]==_T('!'));
    }
}

FC_TOAPI int FC_TODECL FC_StringResize(
    FC_StringTYP* pData, 
    int           iNeedChars
)
{
    if(pData->psz!=g_FC_StringEmpty)
    {
        //psz was allocated on heap, check if must grow:
        //iNewBytes: new size needed for the FC_StringTYP
        if(pData->iMax<iNeedChars)
        {
            if(!FC_LibResizeMemory((void**)&(pData->psz), (iNeedChars+1)*sizeof(TCHAR)))
                return 0;
            pData->iMax = iNeedChars;
        }
    }
    else
    {
        if(!FC_StringNewString(pData, iNeedChars))
            return 0;
    }

    if(iNeedChars<pData->iLen)
    {
        pData->iLen = iNeedChars;
        pData->psz[iNeedChars] = 0;
    }

    return 1;
}


FC_TOAPI int FC_TODECL FC_StringAppendString(
    FC_StringTYP* pData, 
    const TCHAR* pszAdd
)
{
    int iLenAdd = _tcslen(pszAdd);

    if(pData->iMax<pData->iLen+iLenAdd)
        if(!FC_StringResize(pData, pData->iLen+iLenAdd))
            return 0;

    memcpy(&pData->psz[pData->iLen], pszAdd, (iLenAdd+1)*sizeof(TCHAR));
    pData->iLen += iLenAdd; //set new length

    return 1;
}

FC_TOAPI int  FC_TODECL FC_StringAppendChar(
    FC_StringTYP* pData, 
    TCHAR         cadd
)
{
    if(pData->iMax<pData->iLen+1)
        if(!FC_StringResize(pData, pData->iLen+1))
            return 0;

    pData->psz[pData->iLen]= cadd;
    pData->iLen += 1; //set new length
    pData->psz[pData->iLen] = 0;
    return 1;
}

FC_TOAPI int  FC_TODECL FC_StringAppendChars(
    FC_StringTYP* pData, 
    const TCHAR*  pAdd,
    int           iChars
)
{
    if(pData->iMax<pData->iLen+iChars)
        if(!FC_StringResize(pData, pData->iLen+iChars))
            return 0;

    memcpy(&pData->psz[pData->iLen], pAdd, iChars*sizeof(TCHAR));
    pData->iLen += iChars; //set new length
    pData->psz[pData->iLen] = 0;
    return 1;
}


FC_TOAPI void FC_TODECL FC_StringChangeFileExt(
    FC_StringTYP*  pData, 
    const TCHAR*   pszNew
)
{
    TCHAR* psz = (TCHAR*)FC_StringGetFileExt(pData->psz);
    if(psz[0])
    {
        //set new length before FC_StringAppendString:
        pData->iLen = psz - pData->psz;
        if(pszNew)
        {
            if(pszNew[0]==_T('.'))
                pszNew++; //psz[-1] is definitely a dot
            FC_StringAppendString(pData, pszNew);
        }
        else
        {
            //cut the dot:
            pData->psz[--(pData->iLen)] = 0;
        }
    }
    else if(pszNew)
    {
        if(pszNew[0]!=_T('.'))
            FC_StringAppendChar(pData, _T('.'));
        FC_StringAppendString(pData, pszNew);
    }
}

FC_TOAPI int FC_TODECL FC_StringAppendWsprintf(
    FC_StringTYP*  pData, 
    const TCHAR*   pszFmt, 
    va_list arglist
)
{
    int    i;
    TCHAR* pszTmp;
    
    pszTmp = (TCHAR*)alloca(1025*sizeof(TCHAR));
    //for assert below:
    pszTmp[1024] = 0;

    i = wvsprintf(pszTmp, pszFmt, arglist);

    //wvsprintf addes always a \0 even if buffer was to small
    //but the max output chars 1024 is from the documentation, 
    //maybe will change? Be safe, assert:
    if(pszTmp[1024]!=0 || i>=1023)
    {
        FC_LIBASSERT(pszTmp[1024]==0);
        return 0;
    }
    return FC_StringAppendString(pData, pszTmp);
}

FC_TOAPI int FC_TODECL FC_StringPrepend(
    FC_StringTYP*  pData, 
    const TCHAR*   psz1, 
    const TCHAR*   psz2
)
{
    int iLen1    = _tcslen(psz1);
    int iLen2    = psz2 ? _tcslen(psz2) : 0;

    if(!FC_StringResize(pData, pData->iLen + iLen1 + iLen2))
        return 0;
    

    memmove(pData->psz + iLen1 + iLen2,  pData->psz,  (pData->iLen+1)*sizeof(TCHAR));
    memmove(pData->psz,  psz1,  iLen1*sizeof(TCHAR));

    if(iLen2>0)
        memmove(pData->psz + iLen1,  psz2,  iLen2*sizeof(TCHAR));

    pData->iLen += iLen1 + iLen2;
    return 1;
}



FC_TOAPI const TCHAR* FC_TODECL FC_StringAppendUpTo(
    FC_StringTYP*  pData, 
    const TCHAR*   pszFrom, 
    const TCHAR*   pszStopCharSet,
    const TCHAR**  ppEndPtr
)
{
    const TCHAR* pc;
    
    pc = _tcspbrk(pszFrom, pszStopCharSet);
    if(!pc)
        pc = pszFrom + _tcslen(pszFrom); //point to the 0-char

    if(ppEndPtr)
        *ppEndPtr = pc; //set *ppEndPtr to the char that stopped the append


    FC_StringAppendChars(pData, pszFrom, pc - pszFrom); 
    
    //get the next char in pszFrom that is not a stop char
    while(*pc && _tcschr(pszStopCharSet, *pc))
        pc = _tcsinc(pc);

    return pc;
}




FC_TOAPI const TCHAR* FC_TODECL FC_StringLoadResource(
    FC_StringTYP*  pData, 
    unsigned int   uiResNum,
    HINSTANCE      hInst
)
{
    int    iRet;
    TCHAR* pszBuf = (TCHAR*)alloca(1024);
    
    iRet = LoadString(hInst, uiResNum, pszBuf, 1023);
    if(iRet<=0)
        iRet=0;//resource not found
    pData->iLen=0; 
    pData->psz[0]=0;


    FC_StringAppendChars(pData, pszBuf, iRet);
    return pData->psz;
}


FC_TOAPI const TCHAR* FC_TODECL FC_StringFmtMsg(
    FC_StringTYP*  pData, 
    HMODULE        hModule, // module handle
    HRESULT        err_id, 
    int            bArgArray,
    va_list*       arglist
)
{
    TCHAR*  pBuf = NULL;     
    DWORD   dwRet;
    DWORD   dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER|
                      FORMAT_MESSAGE_FROM_HMODULE|
                      FORMAT_MESSAGE_FROM_SYSTEM;
    if(bArgArray)
        dwFlags |= FORMAT_MESSAGE_ARGUMENT_ARRAY;

    dwRet = FormatMessage(
             dwFlags, 
             hModule, 
             err_id, 
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language 
             (LPTSTR)&pBuf, 
             0, 
             arglist);

    //clear possible old content:
    pData->iLen=0; 
    pData->psz[0]=0;

    //append message:
    FC_StringAppendChars(pData, pBuf, dwRet);
    LocalFree(pBuf);

    return pData->psz;
}

FC_TOAPI const TCHAR* FC_TODECL FC_StringFmtMsgStr(
    FC_StringTYP*  pData, 
    const TCHAR*   pszFmt,
    int            bArgArray,
    va_list*       arglist
)
{
    TCHAR*  pBuf = NULL;     
    DWORD   dwRet;
    LPCVOID lpSource;
    DWORD   dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER|
                      FORMAT_MESSAGE_FROM_STRING;
    if(bArgArray)
        dwFlags |= FORMAT_MESSAGE_ARGUMENT_ARRAY;

    if(pszFmt)
        lpSource = (HMODULE)pszFmt;
    else //use current content as fmt:
        lpSource = (HMODULE)pData->psz;

    dwRet = FormatMessage(
             dwFlags, 
             lpSource, 
             0, 
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language 
             (LPTSTR)&pBuf, 
             0, 
             arglist);

    //clear possible old content:
    pData->iLen=0; 
    pData->psz[0]=0;

    //append message:
    FC_StringAppendChars(pData, pBuf, dwRet);
    LocalFree(pBuf);

    return pData->psz;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_cstring.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 17  *****************
 * User: Ef           Date: 24.07.02   Time: 12:26
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed AppendUpToTok, fixed bug in prepend, updated comments.
 * 
 * *****************  Version 16  *****************
 * User: Ef           Date: 12.03.02   Time: 12:24
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * added build file list function
 * 
 * *****************  Version 15  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 14  *****************
 * User: Die          Date: 20.11.01   Time: 17:22
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 15.05.01   Time: 17:39
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * loadResource: now has opt. HINSTANCE
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 31.03.01   Time: 19:23
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 30.03.01   Time: 9:38
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * fixed fatal bug in FC_StringAppendUpTo(Tok)
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 26.03.01   Time: 11:44
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * bug fix in FC_StringChangeFileExt, new functions AppenUpTo
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 6.02.01    Time: 16:19
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * append single char 
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 15.01.01   Time: 16:21
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * some new functions
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 13.01.01   Time: 20:53
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * default language in formatmessage
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 13.01.01   Time: 15:07
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new function FC_StringFree, and bug fix 
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 11.01.01   Time: 17:57
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * missing LocalFree
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 10.01.01   Time: 18:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * pass HMODULE to fmtMsg functions
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 9.01.01    Time: 16:36
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * FC_CString in extra source file
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 8.01.01    Time: 10:40
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * splitted fc_string.c
 *==
 *----------------------------------------------------------------------------*
*/
