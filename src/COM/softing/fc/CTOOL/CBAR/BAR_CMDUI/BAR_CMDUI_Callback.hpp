/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Callback.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CMDUI_Callback.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Callback.hpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           4cbar
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

#ifndef _BAR_CMDUI_CALLBACK_H_
#define _BAR_CMDUI_CALLBACK_H_

//====  Aggregate Includes:   =================================================*
#include <iostream>
using namespace ::std;

#include "BAR_Callback.hpp"

//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_CMDUI_Callback
 *
 *  @see    BAR_Callback
*/
//=============================================================================*
class BAR_CMDUI_Callback : public BAR_Callback
{
public: //=== constructors
    BAR_CMDUI_Callback(ostream& outWarnings);

public: //=== operations
    void SetOverwriteMode(bool bOverwriteMode);
    void SetStdLibOverwriteMode(bool bStdLibOverwriteMode);

    virtual void SendWarning(const BAR_Warning& wrn);
    virtual void SendInfo(const wstring& sInfo);

    virtual bool IsOverwriteAllowed(const string& sQuestion);
    virtual bool IsStdLibOverwriteAllowed(const string& sQuestion);

private: //=== attributes
    ostream& m_outWarnings;
    bool m_bIsOverwriteAllowed;
    bool m_bIsStdLibOverwriteAllowed;
};

//=============================================================================*
inline bool BAR_CMDUI_Callback::IsOverwriteAllowed(const string& sQuestion)
//=============================================================================*
{ return m_bIsOverwriteAllowed; }

//=============================================================================*
inline bool BAR_CMDUI_Callback::IsStdLibOverwriteAllowed(const string& sQuestion)
//=============================================================================*
{ return m_bIsStdLibOverwriteAllowed; }

//=============================================================================*
inline void BAR_CMDUI_Callback::SetOverwriteMode(bool bOverwriteMode)
//=============================================================================*
{ m_bIsOverwriteAllowed = bOverwriteMode; }

//=============================================================================*
inline void BAR_CMDUI_Callback::SetStdLibOverwriteMode(bool bStdLibOverwriteMode)
//=============================================================================*
{ m_bIsStdLibOverwriteAllowed = bStdLibOverwriteMode; }

//=============================================================================*
#endif // _BAR_CALLBACK_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CMDUI_Callback.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:12
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_CMDUI
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_CMDUI_Callback.hpp $
