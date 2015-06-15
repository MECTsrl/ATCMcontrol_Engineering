/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigRestore.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcConfigRestore.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigRestore.hpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CBAR
 *
 * =COMPONENT           4CBAR_LIB
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =ENVIRONMENT             Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V6.0 / / ATL 2.0
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
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  $History: BAR_ArcConfigRestore.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:28
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 * 
 * *****************  Version 1  *****************
 * User: Ff           Date: 8.03.00    Time: 15:29
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
*==
*******************************************************************************
H<<*/

#ifndef _BAR_ARCCONFIGRESTORE_H_
#define _BAR_ARCCONFIGRESTORE_H_

//====  Aggregate Includes:   =================================================*
#include <atlbase.h>
#include <vector>

#include "BAR_ArcConfig.hpp"

//====  Forward Class Definitions:   ==========================================*
class BAR_Callback;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_ArcConfigRestore
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_ArcConfigRestore : public BAR_ArcConfig
{
public: //=== con-, destruction and assignment

    BAR_ArcConfigRestore(BAR_Callback& callback);

public: //=== access
    

public: //=== operations

    virtual void Open(const string& sArcFileName);
    virtual void Close();

    CComBSTR GetAttr(const string& sAttrName);

    void GetDomainNodes(CSC_TRANSFER_TARGET trg,
                        vector<CComBSTR>& vecDomainNodes);
    void GetDomain(CSC_TRANSFER_TARGET trg,
                   CComBSTR strDomainNode,
                   CComBSTR& strDomainName,
                   SAFEARRAY** ppsaDomainContent);

protected: //=== overrides

    virtual IStorage* OpenDomainListStorage(CSC_TRANSFER_TARGET trg);

private: //=== helpers

private: //=== attributes

};

//=============================================================================*
#endif // _BAR_ARCCONFIGRESTORE_H

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcConfigRestore.hpp $
