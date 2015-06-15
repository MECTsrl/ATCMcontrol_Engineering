/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Error.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Error.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Error.cpp $
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

#include "BAR_Error.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_Error
//=============================================================================*

//=============================================================================*
/**
 * <B>constructor</B>
 * @param           iErrCode: numeric error code 
 * @param           wsErrMsg:  error message 
 * @param           wsDetails: description of error details
 * @exception       -
 */
//=============================================================================*
BAR_Error::BAR_Error(int iErrCode,
                     const wstring& wsErrMsg,
                     const wstring& wsDetails)
: m_iErrCode(iErrCode),
  m_wsErrMsg(wsErrMsg),
  m_wsDetails(wsDetails)
{
}

//=============================================================================*
/**
 * <B>default constructor</B>
 * @param           void
 * @exception       -
 */
//=============================================================================*
BAR_Error::BAR_Error()
: m_iErrCode(0),
  m_wsErrMsg(L"unknown error"),
  m_wsDetails(L"")
{}

//=============================================================================*
/**
 * <B>print a prettily formatted error message</B>
 * @param           out: output stream to write on
 * @return          reference to output stream
 * @exception       -
*/
//=============================================================================*
wostream& BAR_Error::PrettyPrint(wostream& out) const
//=============================================================================*
{ 
    out << GetType() << L"(bar";
    out.width(4);
    out.fill('0');
    return out << m_iErrCode << L"): " << GetMessage() 
               << ( GetDetails().empty() ? L"" : L", ") << GetDetails();
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Error.cpp $
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
// $Workfile: BAR_Error.cpp $
