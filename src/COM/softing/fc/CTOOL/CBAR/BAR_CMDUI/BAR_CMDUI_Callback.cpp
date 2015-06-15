/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Callback.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CMDUI_Callback.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Callback.cpp $
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

#include "BAR_CMDUI_Callback.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_CMDUI_Callback
//=============================================================================*

//=============================================================================*
/**
 * <B>constructor</B>
 * @param           out: output stream to send warning messages to
 * @exception       -
 */
//=============================================================================*
BAR_CMDUI_Callback::BAR_CMDUI_Callback(ostream& outWarnings)
: m_outWarnings(outWarnings),
  m_bIsOverwriteAllowed(false)
{
}

//=============================================================================*
/**
 * <B>Print a warning</B>
 * @param           wrn: warning to be printed
 * @return          void
 * @exception       -
 */
//=============================================================================*
void BAR_CMDUI_Callback::SendWarning(const BAR_Warning& wrn)
{
    wcout << wrn << endl;
}

//=============================================================================*
/**
 * <B>Print a warning</B>
 * @param           sInfo: Info message to be printed
 * @return          void
 * @exception       -
 */
//=============================================================================*
void BAR_CMDUI_Callback::SendInfo(const wstring& sInfo)
{
    wcout << sInfo << endl;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CMDUI_Callback.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:11
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_CMDUI
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_CMDUI_Callback.cpp $
