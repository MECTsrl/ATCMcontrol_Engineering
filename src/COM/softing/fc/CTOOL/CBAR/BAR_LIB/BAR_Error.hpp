/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Error.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Error.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Error.hpp $
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

#ifndef _BAR_ERROR_H_
#define _BAR_ERROR_H_

//====  Aggregate Includes:   =================================================*
#include <iostream>
#include <string>
//using namespace ::std;
using ::std::wstring;
using ::std::wostream;

//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_Error
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_Error
{

public: //=== con-, destruction and assignment

    BAR_Error(int iErrCode,
              const wstring& wsErrMsg=L"",
              const wstring& wsDetails=L"");

    BAR_Error(); //=== default constructor

public: //=== access
    int GetErrCode() const;
    const wstring& GetMessage() const;
    const wstring& GetDetails() const;


public: //=== operations
    wostream& PrettyPrint(wostream& out) const;

public: //=== overrides

protected: //== methods
    
    virtual wstring GetType() const;

private: //=== hidden constructors and assignments

private: //=== helpers

private: //=== attributes
    int    m_iErrCode;
    wstring m_wsErrMsg;
    wstring m_wsDetails;
};

//=============================================================================*
inline int BAR_Error::GetErrCode() const 
//=============================================================================*
{ return m_iErrCode; }

//=============================================================================*
inline const wstring& BAR_Error::GetMessage() const
//=============================================================================*
{ return m_wsErrMsg;  }

//=============================================================================*
inline const wstring& BAR_Error::GetDetails() const
//=============================================================================*
{ return m_wsDetails; }

//=============================================================================*
inline wstring BAR_Error::GetType() const
//=============================================================================*
{ return L"error"; }

//=============================================================================*
inline wostream& operator<<(wostream& out, const BAR_Error& err)
//=============================================================================*
{ return err.PrettyPrint(out); }

//=============================================================================*
#endif // _BAR_ERROR_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Error.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Error.hpp $
