/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_DomainList.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_DomainList.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_DomainList.cpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
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
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#include <cstdio>

#include "BAR_DomainList.hpp"


//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_DomainList
//=============================================================================*

//=============================================================================*
BAR_DomainList::BAR_DomainList(CSC_TRANSFER_TARGET trg)
: m_trg(trg)
//=============================================================================*
{
}

//=============================================================================*
void BAR_DomainList::ParseDomainList(char* pszText, const DWORD dwSize)
//=============================================================================*
{
    //=== build a new domain list
    m_vecDomainNames.clear();
    
    if ( pszText == NULL )
        return;

    char szTokenBuf[1024];
    szTokenBuf[0] = '\0';
    
    //=== scan contents of list file
    DWORD dwLineStart = 0;
    int iLines = 0;
    char chSep = ';';
    bool bLookForSep = true;

    //=== loop over bytes
    for(DWORD i = 0; i < dwSize; i++)
    {
        //=== end of line: now look for separator
        if ( pszText[i] == '\n' )
        {
            dwLineStart = i + 1;
            bLookForSep = true;
        }
        //=== separator: extract domain name and go to next line
        else if ( bLookForSep && pszText[i] == chSep )
        {
            size_t sizeToken = i - dwLineStart;
            strncpy(szTokenBuf, &pszText[dwLineStart], sizeToken);
            szTokenBuf[sizeToken] = '\0';
            AddDomain(szTokenBuf);
            bLookForSep = false;
        }
    }

    return;
}

//=============================================================================*
void BAR_DomainList::AddDomain(const string& sDomain)
//=============================================================================*
{
    m_vecDomainNames.push_back(sDomain);
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_DomainList.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:32
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/
