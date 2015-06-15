/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_TypeLib.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_TypeLib.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_TypeLib.hpp $
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

#ifndef _BAR_TYPELIB_H_
#define _BAR_TYPELIB_H_

//====  Aggregate Includes:   =================================================*
#include <string>
using namespace ::std;

#include "windows.h"
#include "oaidl.h"

//====  Forward Class Definitions:   ==========================================*
class BAR_Callback;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

class BAR_TypeLib
{
public://=== con-/destructors

    BAR_TypeLib(BAR_Callback& callback,
                const string& sLabel,
                const IID& iidTlb,
                const string& sTlbHost,
                const string& sResourceIndex);
   ~BAR_TypeLib();

protected://=== helpers

    enum {
        ERR_0400 = 400,
        ERR_0401 = 401,
        ERR_0402 = 402
    };
    
    void Throw_FatalError( const wstring& wsDetails) const;
    void Throw_RegisteringTlbFailed(HRESULT hr, const wstring& wsDetails) const;
    void Warning_UnregisteringTlbFailed(HRESULT hr) const;

    void CompareGuids(const GUID& guidFound, const GUID& guidExpected) const;

private:
    
    BAR_Callback& m_callback;

    const string m_sLabel;

    IID m_iidTlb;
    ITypeLib FAR* m_pITlb;
    TLIBATTR FAR* m_pTlbAttr;

};

//=============================================================================*
#endif // _BAR_TYPELIB_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_TypeLib.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:39
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_TypeLib.hpp $
