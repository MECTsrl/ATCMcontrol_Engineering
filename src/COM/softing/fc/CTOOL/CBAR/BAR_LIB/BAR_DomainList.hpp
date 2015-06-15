/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_DomainList.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_DomainList.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_DomainList.hpp $
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

#ifndef _BAR_DOMAINLIST_H_
#define _BAR_DOMAINLIST_H_

//====  Aggregate Includes:   =================================================*
#pragma warning(disable:4786)
#include <vector>
#include <string>

#include "CSC_Online.h"

using namespace ::std;


//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*



//=============================================================================*
/**
 *  class BAR_DomainList
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_DomainList
{

public: //=== con-, destruction and assignment
    BAR_DomainList(CSC_TRANSFER_TARGET trg);

public: //=== access

    CSC_TRANSFER_TARGET GetDomainType() const;

    size_t GetSize() const;

    string GetDomainName(size_t i) const;

public: //=== operations

    void ParseDomainList(char* pszText, const DWORD dwSize);


public: //=== overrides

private: //=== helpers
    void AddDomain(const string& sDomain);

private: //=== attributes
    CSC_TRANSFER_TARGET m_trg;

    vector<string> m_vecDomainNames;
};

//=============================================================================*
inline CSC_TRANSFER_TARGET BAR_DomainList::GetDomainType() const
//=============================================================================*
{ return m_trg; }

//=============================================================================*
inline size_t BAR_DomainList::GetSize() const
//=============================================================================*
{ return m_vecDomainNames.size(); }

//=============================================================================*
inline string BAR_DomainList::GetDomainName(size_t i) const
//=============================================================================*
{ return m_vecDomainNames[i]; }

//=============================================================================*
#endif // _BAR_CONFIG_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_DomainList.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_DomainList.hpp $
