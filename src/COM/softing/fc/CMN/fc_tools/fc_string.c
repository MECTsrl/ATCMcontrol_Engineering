/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_string.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_string.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_string.c $
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
 *==Implementation of basic string routines.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


//----  Includes:   -------------------------------------------*
#include "fc_tools\fc_string.h"



FC_TOAPI int FC_TODECL FC_StringTrim(TCHAR* psz)
{
    FC_StringTrimRight(psz);
    return FC_StringTrimLeft(psz);
}
FC_TOAPI int FC_TODECL FC_StringTrimLeft(TCHAR* psz)
{
    const TCHAR* p    = psz;
    int          iLen = _tcslen(p);

    while (_istspace(*p))
        p = _tcsinc(p);

    if(p != psz)
    {
        int n = (iLen+1)*sizeof(TCHAR) - (p - psz);
        memmove(psz, p, n);
    }

    return iLen - (p - psz);

}
FC_TOAPI int FC_TODECL FC_StringTrimRight(TCHAR* psz)
{
    TCHAR* p     = psz;
    TCHAR* pLast = NULL;

    while(*p)
    {
        if(_istspace(*p))
        {
            if(pLast==NULL)
                pLast = p;
        }
        else
        {
            pLast = NULL;
        }

        p = _tcsinc(p);
    }

    if(pLast)
    {
        *pLast = 0;
        return (pLast - psz);
    }
    else
    {
        return (p - psz);
    }
}

FC_TOAPI TCHAR* FC_TODECL FC_StringGetAfterLast(
    TCHAR*       psz, 
    const TCHAR* pszLastCh
)
{
    TCHAR* p     = psz;
    TCHAR* pLast = NULL;

    while(*p)
    {
        if(_tcschr(pszLastCh, *p))
            pLast = p;

        p = _tcsinc(p);
    }
    return pLast && *pLast ? ++pLast : NULL;
}
FC_TOAPI int FC_TODECL  FC_StringCutAtLast(
    TCHAR*       psz, 
    const TCHAR* pszLastCh
)
{
    TCHAR* p     = psz;
    TCHAR* pLast = NULL;

    while(*p)
    {
        if(_tcschr(pszLastCh, *p))
            pLast = p;

        p = _tcsinc(p);
    }

    if(pLast && *pLast)
    {
        *pLast = 0;
        return (pLast-psz);
    }
    else
        return (p-psz);
}

FC_TOAPI TCHAR FC_TODECL FC_StringRemoveLastChar(
    TCHAR        sb[/* *pLen*/], 
    const TCHAR* pszIfOneOf, 
    int*         pLen
)
{  
     TCHAR  chTillt = 0;
     int    nLen = _tcslen(sb);
     if(nLen>0)
     {
         TCHAR* p = _tcsdec(sb, &sb[nLen]);
         if(p)
         { 
            if(!pszIfOneOf || _tcschr(pszIfOneOf, *p))
            {
                chTillt = *p;
                *p=0;
                nLen = (p - sb);
            }
         }
     }

     if(pLen)
         *pLen = nLen;

     return chTillt;
}

FC_TOAPI int FC_TODECL FC_StringStripBegin(
    TCHAR*       psz,
    const TCHAR* pszWith
)
{
    int i;
    int iLen;

    i = _tcslen(pszWith);
    if(_tcsncmp(psz, pszWith, i))
        return -1;

    iLen = _tcslen(psz);
    iLen-=i;
    memcpy(psz, &psz[i], (iLen+1)*sizeof(TCHAR));
    return iLen;
}
FC_TOAPI int FC_TODECL FC_StringStripBeginI(
    TCHAR*       psz,
    const TCHAR* pszWith
)
{
    int i;
    int iLen;

    i = _tcslen(pszWith);
    if(_tcsnicmp(psz, pszWith, i))
        return -1;

    iLen = _tcslen(psz);
    iLen-=i;
    memcpy(psz, &psz[i], (iLen+1)*sizeof(TCHAR));
    return iLen;
}


FC_TOAPI int FC_TODECL FC_StringToWideChar(
    const char* pszMulti,
    wchar_t     pszWide[/*iLen*/],
    int         iLen
)
{
    int ret;

    ret = MultiByteToWideChar(CP_ACP, 0, pszMulti, -1, pszWide, iLen);
    if(ret==0)
    {
        unsigned long dwErr;

        dwErr = GetLastError();
        if(dwErr == ERROR_INSUFFICIENT_BUFFER)
        {
            pszWide[iLen-1] = L'\0';
            ret = iLen;
        }
    }

    return ret;
}
FC_TOAPI int FC_TODECL FC_StringToMultiByte(
    const wchar_t* pszWide, 
    char           pszMulti[/*nBytes*/], 
    int            nBytes
)
{
    unsigned long dwErr;
    int           ret;

    ret = WideCharToMultiByte(CP_ACP, 0, pszWide, -1, 
                       pszMulti, nBytes, NULL, NULL);
    if(ret==0)
    {
        dwErr = GetLastError();
        if(dwErr == ERROR_INSUFFICIENT_BUFFER)
        {
            pszMulti[nBytes-1] =  '\0';
            ret = nBytes;
        }
    }

    return ret;
}



FC_TOAPI const TCHAR* FC_TODECL FC_StringFmtSysErr(
    DWORD    dwSystemErr, 
    TCHAR    szBuffer[/*nBuffer*/], 
    unsigned nBuffer
)
{
    DWORD dw;
    
    if(nBuffer==0)
        return szBuffer;

    szBuffer[0] = 0;

    dw = FormatMessage( 
        FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwSystemErr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language
        szBuffer,
        nBuffer-2,
        NULL 
    );
    if(dw!=0)
    {
        //no error:
        szBuffer[nBuffer-1] = 0;//be safe...

        //some sys error have an unnice newline at the end:
        FC_StringTrimRight(szBuffer);
    }
    return szBuffer;
} 




FC_TOAPI int FC_TODECL FC_StringSplitBuf(
    TCHAR*       pszBuffer,
    TCHAR        chSep,
    TCHAR*       ppszParts[/*nMaxParts*/],
    int          nMaxParts, 
    int          nTrimStyle
)
{
    int    i   = 0;
    TCHAR* psz = pszBuffer;

    while(i<nMaxParts)
    {
        if(nTrimStyle==1)
        {
            while(_istspace(*psz) && *psz!=chSep)
                psz++;
        }
        ppszParts[i++] = psz;
        psz = _tcschr(psz, chSep);


        if(!psz)
            break;
        *psz++ = 0;
    }

    if(nTrimStyle==1)
    {
        int j;
        for(j=0; j<i; j++)
            FC_StringTrimRight(ppszParts[j]);
    }

    return i;
}


FC_TOAPI int FC_TODECL FC_StrGetID(
    const TCHAR** ppsz, 
    int           bSkipWhite, 
    TCHAR         szId[/*nBuff*/], 
    int           nBuff
)
{
    int          i;
    const TCHAR* psz;

    psz = *ppsz;
    
    if(bSkipWhite)
        while(_istspace(*psz))
            psz++;

    if(!__iscsym(*psz))
        return 1;
    for(i=0; __iscsymf(*psz) && i<nBuff; )
        szId[i++] = *psz++;
    
    if(i>=nBuff)
        return 2;

    szId[i] =  0;
    if(bSkipWhite)
        while(_istspace(*psz))
            psz++;

    *ppsz = psz;
    return 0;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_string.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 26.09.03   Time: 13:11
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * changes for acyclic tasks
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 15  *****************
 * User: Ef           Date: 21.01.03   Time: 15:27
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new functions
 * 
 * *****************  Version 14  *****************
 * User: Ef           Date: 24.10.02   Time: 15:57
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 28.03.01   Time: 17:01
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * trimright returnd wrong  val
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 26.03.01   Time: 17:44
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 26.02.01   Time: 10:32
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new functions in fc_fileIO.h, extra impl. c-file for string file name
 * stuff
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 29.01.01   Time: 10:45
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * changed FC_StringGetFilePart so that a pure file name does not return
 * ""
 * seem much better to work with.
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 22.01.01   Time: 11:03
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new fc_system.c
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 15.01.01   Time: 16:21
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * some new functions
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 13.01.01   Time: 15:09
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * changed FC_StringGetFilePart and FC_StringGetFileExt to return "" and
 * not NULL
 * seems to be more convinient and safe to use
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 9.01.01    Time: 16:39
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * remove tabs
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 8.01.01    Time: 10:51
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed assert
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.01.01    Time: 10:40
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * added some file headers
 *==
 *----------------------------------------------------------------------------*
*/
