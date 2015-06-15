/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcRestore.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcRestore.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcRestore.hpp $
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

#ifndef _BAR_ARCRESTORE_H_
#define _BAR_ARCRESTORE_H_

//====  Aggregate Includes:   =================================================*
#include <string>
using namespace ::std;

#include "BAR_Arc.hpp"

//====  Forward Class Definitions:   ==========================================*
class BAR_Callback;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*



//=============================================================================*
/**
 *  class BAR_ArcRestore
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_ArcRestore : public BAR_Arc
{

public: //=== con-, destruction and assignment

    BAR_ArcRestore(BAR_Callback& callback);   

    virtual void Open(const string& sFileName);
    virtual void Close();
    
    void Restore(BYTE& byte);
    void Restore(LPBYTE& byteData, DWORD dwSize);
    void Restore(long& l);
    void Restore(string& s);
    void Restore(wstring& ws);

    void Skip(long l);

public: //=== access

public: //=== operations

public: //=== overrides

protected: //=== helpers

private: //=== attributes

    ifstream m_ifsArchive;

};


//=============================================================================*
#endif // _BAR_ARCRESTORE_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_ArcRestore.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:29
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcRestore.hpp $
