/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/SyntaxCheck.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: SyntaxCheck.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/SyntaxCheck.cpp $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==Contains syntax checker routines for e.g Identifier,...
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: SyntaxCheck.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:10
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * utils and baselib from V1.3
 * 
 * *****************  Version 8  *****************
 * User: Rf           Date: 13.08.99   Time: 16:38
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * Always use _istascii before specialized _ist...
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 2.06.99    Time: 20:12
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * more st syntax
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 1.06.99    Time: 21:50
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 1.06.99    Time: 19:16
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * started to integrate 4ccg sts_scan to util.dll
 * 
 * *****************  Version 4  *****************
 * User: Ed           Date: 4.12.98    Time: 10:06
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 11/30/98   Time: 4:14p
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * max id length
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 11/24/98   Time: 6:39p
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * new fun
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 11/24/98   Time: 12:06p
 * Created in $/4Control/COM/softing/fc/CMN/UTIL
 *==
 ******************************************************************************
H<<*/



#include <tchar.h>
#include <assert.h>
#include "stdafx.h"
#ifndef UTIL_IMP
#define UTIL_IMP    // declare as dllexports
#endif
#include "UtilIF.h"










//include file was generated KW Map:
#include "IECKeywordMap.cpp"

UTILIF UTIL_KW_ENUM UTIL_IsIECKeyword(LPCTSTR psz)
{
    assert(psz);
    if(psz)
        return UTIL_MapKW(psz, _tcslen(psz));//is defined in IECKeywordMap.cpp
    else
        return UTIL_KW_NO_KEYWORD;
}

UTILIF LPCTSTR UTIL_EnumToKeyword(UTIL_KW_ENUM kw)
{
    return UTIL_GetKeyword(kw);//is defined in IECKeywordMap.cpp
}



//------------------------------------------------------------------*
/**
 * UTIL_CheckIdentifier
 * Checks for a valid IEC1131-3 Identifier, this means 
 * a) Syntax  identifier := (letter | ('_' (letter|digit))) {['_'] (letter|digit)}
 * b) identifier must not be one of the Keywords (ignoring the case). This function 
 *    checks also for all ATCMControl specific Keywords, e.g. NATIVE,SYNCHRONIZED,... .
 * c) the maximum lenght is checked, see the define UTIL_MAX_ID_LENGTH.
 *
 * @param           pszId   string to be checked for valid IEC1131-3 Identifier
 * @return                  One of the following:           meaning
 *                          S_OK                        valid identifier
 *
 *                          E_UTIL_ID_SYNTAX            the pure syntax is incorrect
 *                          E_UTIL_ID_IS_KEYWORD        syntax is ok, but pszId is a keyword
 *                          E_UTIL_ID_TOO_LONG          syntax is ok, but pszId is too long
 * @see 
 */
UTILIF HRESULT UTIL_CheckIdentifier(LPCTSTR pszId)
{
    HRESULT      hr;
    UTIL_KW_ENUM kwEnum;
    
    hr = UTIL_CheckIdentifierEx(pszId, &kwEnum);

    if(FAILED(hr))
        return hr;
    
    if(kwEnum != UTIL_KW_NO_KEYWORD)
        return E_UTIL_ID_IS_KEYWORD;
    
    return S_OK;
}



UTILIF HRESULT UTIL_CheckIdentifierEx(LPCTSTR pszId, UTIL_KW_ENUM* pKwEnum)
{
    LPCTSTR      psz = pszId;
    int          iLen = 0;
    
    //assert input:
    assert(psz && pKwEnum);

    //default value for out param:
    *pKwEnum = UTIL_KW_NO_KEYWORD;

    if(!psz)
        return E_UTIL_ID_SYNTAX;
    
 
    //check pure IEC syntax: 
    //identifier := (letter | ('_' (letter|digit))) {['_'] (letter|digit)}
   
    //begin of id must not be a digit and min lenght >0:
    if(*psz==0 || !_istascii(*psz) || _istdigit(*psz))
        return E_UTIL_ID_SYNTAX;

    while(*psz)
    {
        if(*psz==_T('_'))
        {
           iLen++;
        #ifdef UNICODE 
            psz++;
        #else
            psz = CharNext(psz);
        #endif
        }

        //letter|digit must follow:
        if(!_istascii(*psz) || !_istalnum(*psz))
            return E_UTIL_ID_SYNTAX;

        iLen++;
    #ifdef UNICODE 
        psz++;
    #else
        psz = CharNext(psz);
    #endif
    }


    //max len?
    if(iLen > UTIL_MAX_ID_LENGTH)
        return E_UTIL_ID_TOO_LONG;

    *pKwEnum = UTIL_MapKW(pszId, iLen);

    return S_OK;
}




