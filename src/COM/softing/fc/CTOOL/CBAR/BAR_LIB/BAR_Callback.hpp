/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Callback.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Callback.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Callback.hpp $
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

#ifndef _BAR_CALLBACK_H_
#define _BAR_CALLBACK_H_

//====  Aggregate Includes:   =================================================*
#include <string>
using namespace ::std;

#include "BAR_Warning.hpp"

//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_Callback
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_Callback
{

public: //=== con-, destruction and assignment

            BAR_Callback(); //=== default constructor
   virtual ~BAR_Callback(); //=== default destructor

public: //=== access
    bool IsCancelRequested() const;

public: //=== operations
    void RequestCancel();
    
    virtual void SendWarning(const BAR_Warning& wrn) = 0;
    virtual void SendInfo(const wstring& sInfo) = 0;

    virtual bool IsOverwriteAllowed(const string& sQuestion) = 0;
    virtual bool IsStdLibOverwriteAllowed(const string& sQuestion) = 0;

public: //=== overrides

private: //=== helpers

private: //=== attributes
    bool m_bIsCancelRequested;

};

//=============================================================================*
inline bool BAR_Callback::IsCancelRequested() const
//=============================================================================*
{ return m_bIsCancelRequested; }

//=============================================================================*
inline void BAR_Callback::RequestCancel()
//=============================================================================*
{ m_bIsCancelRequested = true; }

//=============================================================================*
#endif // _BAR_CALLBACK_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Callback.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:30
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Callback.hpp $
