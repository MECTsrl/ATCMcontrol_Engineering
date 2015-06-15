/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regex.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_regex.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regex.c $
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
 *==Simple reg ex support
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  26.02.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


//----  Includes:   -------------------------------------------*
#include "fc_tools\fc_string.h"





FC_TOAPI int FC_TODECL FC_StringMatchFilePattern(
    const TCHAR*  pszToken, 
    const TCHAR*  pszPattern 
)
{
    const TCHAR* pszT = pszToken;
    const TCHAR* pszT2;
    const TCHAR* pszP = pszPattern;

    while(*pszT && *pszP)
    {
        if(*pszP==_T('?'))
        {
            pszP = _tcsinc(pszP);
            pszT = _tcsinc(pszT);
            continue;
        }
        else if(*pszP==_T('*'))
        {
            // SIS, 16.03.01: if pattern is "*" return true
            if(_tcslen(pszP) == 1)
            {
                return 1;
            }
            //text=xyzuvw
            //patt=*uvw
            pszP = _tcsinc(pszP);
            for(pszT2=pszT;*pszT2;pszT2=_tcsinc(pszT2))
            {
                if(FC_StringMatchFilePattern(pszT2, pszP))
                    return 1; 
            }
            return 0;
        }

        if(tolower(*pszT)==tolower(*pszP))
        {
            pszP = _tcsinc(pszP);
            pszT = _tcsinc(pszT);
        }   
        else
        {
            break;
        }
    }
    // SIS, 16.03.01: if pattern only contains "*" return true
    if(_tcscmp(pszP, _T("*")) == 0)
    {
        return 1;
    }
    return !*pszT && !*pszP;
}


FC_TOAPI const TCHAR* FC_TODECL FC_StrMatchI(
    const TCHAR*  pszToken, 
    const TCHAR*  pszPattern 
)
{
    int          i;
    TCHAR        buffer[4096];
    const TCHAR* psz;
    const TCHAR* pszBeg;

    psz = pszPattern;
    
    do
    {
        for(i=0, pszBeg = psz; *psz && *psz!='|' && i<4095; buffer[i++] = *psz++)
            ;
            
        buffer[i] = 0;
        if(*psz=='|')
            psz++;

        if(FC_StringMatchFilePattern(pszToken, buffer))
            return pszBeg;

    }while(*psz);
    
    return NULL;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_regex.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.10.03   Time: 16:11
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 21.01.03   Time: 15:27
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new functions
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 16.03.01   Time: 13:54
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed bug in FC_StringMatchFilePattern
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * corrected SWKE name
 *==
 *----------------------------------------------------------------------------*
*/
