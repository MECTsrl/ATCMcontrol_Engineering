/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_stringFile.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_stringFile.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_stringFile.c $
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
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  26.02.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


//----  Includes:   -------------------------------------------*
#include "fc_tools\fc_string.h"


FC_TOAPI int FC_TODECL FC_StringIsAbsPath(const TCHAR* pszPath)
{
    //use isalpha, no multbyte char should be a drive letter.
    if(isalpha(pszPath[0]) && pszPath[1]==_T(':'))
        return 1;
    if(pszPath[0]==_T('\\') && pszPath[1]==_T('\\'))
        return 2;
    return 0;
}


#if 0  //TODO
//------------------------------------------------------------------*
/**
 * FC_StringMakeFullPath creates a full path from pszFile and pszPath.
 * If pszFile is absolute pszPath is ignored. If pszPath is NULL use
 * the current working directory. If pszPath is not absolute the
 * result is also not absolute. 
 * If pszFile is relative pszPath and pszFile are concatinated
 * textually with a \ seperated if needed.
 * The result is NOT necessarily the canonical form of the file name.
 *
 * @param     pszFile: file name with opt. relative or full path
 * @param     pszPath: NULL or an absolute path
 * @param     szFull:  buffer for result.            
 * @param     nBuffer: size of buffer in TCHARs
 * @return    >0   the _tcslen of the final result
 *            =0   the buffer was to short (the buffer may now
 *                 contain invalid and possibly not null terminated 
 *                 text)
 */
FC_TOAPI int FC_TODECL FC_StringMakeFullPath(
    const TCHAR* pszFile, 
    const TCHAR* pszPath,
    TCHAR        szFull[/*nBuffer*/],
    int          nBuffer
);

FC_TOAPI int FC_TODECL FC_StringMakeFullPath(
    const TCHAR* pszFile, 
    const TCHAR* pszPath,
    TCHAR        szFull[/*nBuffer*/],
    int          nBuffer
)
{
    int  iLenP;  
    int  iLenF;  

    iLenP = 0;
    if(!FC_StringIsAbsPath(pszFile))
    {
        if(!pszPath || !pszPath[0])
        {
            iLenP = GetCurrentDirectory(nBuffer, szFull);
        }
        else
        {
            iLenP = _tcslen(pszPath);
            if(iLenP + 1 > nBuffer);
               return 0; //will not exist..
            memcpy(szFull, pszPath, iLenP*sizeof(TCHAR));
        }
        
        //always end in a '\'
        if(szFull[iLenP-1] == _T('/'))
            szFull[iLenP-1] = _T('\\');
        else if(szFull[iLenP-1] != _T('\\'))
            szFull[iLenP++] = _T('\\');        
    }

    iLenF = _tcslen(pszFile);
    if(iLenP + iLenF + 1 > nBuffer);
        return 0; //will not exist..

    memcpy(szFull+iLenP, pszFile, (iLenF+1)*sizeof(TCHAR));

    return iLenP + iLenF;
}
#endif


FC_TOAPI const TCHAR* FC_TODECL FC_StringGetFilePart(
    const TCHAR* pszPath 
)
{
    const TCHAR* p     = pszPath;
    const TCHAR* pLast = NULL;

    while(*p)
    {
        if(*p==_T('\\') || *p==_T('/'))
            pLast = p;
        p = _tcsinc(p);
    }
    return pLast?++pLast:pszPath;
}
FC_TOAPI int FC_TODECL FC_StringCutFilePart(
    TCHAR* pszPath 
)
{
    TCHAR* p     = pszPath;
    TCHAR* pLast = NULL;

    while(*p)
    {
        if(*p==_T('\\') || *p==_T('/'))
            pLast = p;
        p = _tcsinc(p);
    }
    if(pLast)
    {
        *pLast = 0;
        return (pLast - pszPath);
    }
    return (p-pszPath);
}

FC_TOAPI const TCHAR* FC_TODECL FC_StringGetFileExt(
    const TCHAR* pszPath
)
{
    const TCHAR* p     = pszPath;
    const TCHAR* pLast = NULL;

    while(*p)
    {
        if(*p==_T('.'))
            pLast = p;
        else if(*p==_T('\\') || *p==_T('/'))
            pLast = NULL;

        p = _tcsinc(p);
    }
    return pLast?++pLast:p;
}



FC_TOAPI int FC_TODECL FC_StringCanonicalPath(
    const TCHAR* pszPath,
    TCHAR        szCanPath[/*nBuffer*/],
    int          nBuffer, 
    const TCHAR* pszRoot,
    int          nMode
)
{
    int          i;
    int          j;
    int          nLen;
    int          nLen2;
    int          nRest;
    int          nLenFull;
    TCHAR        fullcan[2*MAX_PATH+1];
    TCHAR        rootcan[2*MAX_PATH+1];
    const TCHAR* psz;
    TCHAR*       pszFile;
    int          nResult;

    if(nBuffer<=0)
        return 0;

    //TODO nMode == FC_PATHM_TRYREL 
    szCanPath[0] = 0;

    if(FC_StringIsAbsPath(pszPath))
    {   //simple case:
        nResult = GetFullPathName(pszPath, nBuffer, szCanPath, &pszFile);
        if(nResult==0 || nResult>=nBuffer)
            goto EndeErr;

        goto EndeOk;
    }


    //not simple case:
    if(pszRoot)
        //make the given root canonical first:
        nLen = GetFullPathName(pszRoot, 2*MAX_PATH, rootcan, &pszFile);
    else if(nMode)
        nLen = _tcslen(_tcscpy(rootcan, _T("c:\\x\\x\\x\\x\\x\\x\\x\\")));//HACK, absorb some "..\"
    else
        nLen = GetCurrentDirectory(2*MAX_PATH, rootcan);
    
    if(nLen==0 || nLen>=2*MAX_PATH)
        goto EndeErr;

    
    //always end in a '\'
    if(rootcan[nLen-1] == _T('/'))
        rootcan[nLen-1] = _T('\\'); //<-will never happen ?
    else if(rootcan[nLen-1] != _T('\\'))
        rootcan[nLen++] = _T('\\');

    //append the relative path to root:
    nLen2 = _tcslen(pszPath);
    if(nLen+nLen2 >= 2*MAX_PATH)
        goto EndeErr;
    memcpy(rootcan+nLen, pszPath, (nLen2+1)*sizeof(TCHAR));


    if(nMode)
    {
        //make it canonical:
        nLenFull = GetFullPathName(rootcan, 2*MAX_PATH, fullcan, &pszFile);
        if(nLenFull==0 || nLenFull>=2*MAX_PATH)
            goto EndeErr;
    }
    else
    {
        //make it canonical and absolute & return:
        nResult = GetFullPathName(rootcan, nBuffer, szCanPath, &pszFile);
        if(nResult==0 || nResult>=nBuffer)
            goto EndeErr;

        goto EndeOk;
    }

    //restore rootcan to the canonical root ending in a '\' (pszPath was appended above!): 
    rootcan[nLen] = 0;
    
    //rootcan = C:\4c2\Projects\xt\
    //fullcan = C:\4c2\Projects\xt\Resource1.gvl
    //or 
    //rootcan = C:\4c2\Projects\xt\
    //fullcan = C:\4c2\Projects2\Resource1.gvl
    for(j=0, i=0; rootcan[i] && rootcan[i]==fullcan[i]; i++)
    {
        if(rootcan[i]=='\\')
            j = i+1;
    }
        
    nRest = nLenFull - j;

    if(rootcan[j])
    {
        nResult = 0;
        for(psz = &rootcan[j]; *psz; psz++)
        {
            if(*psz=='\\')
            {
                if(nResult+3 >= nBuffer)
                    goto EndeErr;

                szCanPath[nResult++] = '.';
                szCanPath[nResult++] = '.';
                szCanPath[nResult++] = '\\';
            }
        }

        if(nRest+nResult >= nBuffer)
            goto EndeErr;

        memcpy(szCanPath+nResult, fullcan+j, (nRest+1)*sizeof(TCHAR));
        nResult += nRest;
    }
    else
    {
        if(nRest >= nBuffer)
            goto EndeErr;
        memcpy(szCanPath, fullcan+i, (nRest+1)*sizeof(TCHAR));
        nResult = nRest;
    }

EndeOk:
    if(nResult>0 && szCanPath[nResult-1]=='\\')
    {
        --nResult;
        szCanPath[nResult] = 0;
    }
    return nResult;

EndeErr:
    szCanPath[0] = 0;
    return 0;

}



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_stringFile.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 27.11.02   Time: 12:31
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.11.02   Time: 21:30
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * FC_StringCanonicalPath: algorithm still 'olala', but better in this lib
 * than multiple copies of it!
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 26.02.01   Time: 10:35
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 *==
 *----------------------------------------------------------------------------*
*/
