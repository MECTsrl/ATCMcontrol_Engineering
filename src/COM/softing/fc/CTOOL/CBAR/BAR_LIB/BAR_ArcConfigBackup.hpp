/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigBackup.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcConfigBackup.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcConfigBackup.hpp $
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
 *  $History: BAR_ArcConfigBackup.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:26
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 * 
 * *****************  Version 1  *****************
 * User: Ff           Date: 8.03.00    Time: 15:28
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
*==
*******************************************************************************
H<<*/

#ifndef _BAR_ARCCONFIGBACKUP_H_
#define _BAR_ARCCONFIGBACKUP_H_

//====  Aggregate Includes:   =================================================*
#include <atlbase.h>
#include "BAR_ArcConfig.hpp"

//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_ArcConfigBackup
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_ArcConfigBackup : public BAR_ArcConfig
{
public: //=== con-, destruction and assignment

    BAR_ArcConfigBackup(BAR_Callback& callback);

public: //=== access
    
public: //=== operations

    virtual void Open(const string& sFileName);
    virtual void Close();

    void StoreDomain(CSC_TRANSFER_TARGET trg,
                     const string& sDomain,
                     const void* pDomainCont,
                     ULONG ulDomainSize,
                     ULONG& ulBytesWritten);
    void StoreAttr(const string& sAttrName,
                   CComBSTR strAttrVal);

protected: //=== overrides

    virtual IStorage* OpenDomainListStorage(CSC_TRANSFER_TARGET trg);

    virtual void Cleanup();

private: //=== helpers
    string GetNextDomainId(CSC_TRANSFER_TARGET trg);

    void CreateDomainListStorage(CSC_TRANSFER_TARGET trg);

private: //=== attributes

    ULONG m_ulVisDomId;
    ULONG m_ulPrgDomId;

    bool m_bProgramDomainListStorageCreated;
    bool m_bVisualDomainListStorageCreated;
};

//=============================================================================*
#endif // _BAR_ARCCONFIGBACKUP_H

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcConfigBackup.hpp $
