/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcBackup.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcBackup.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcBackup.hpp $
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

#ifndef _BAR_ARCBACKUP_H_
#define _BAR_ARCBACKUP_H_

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
 *  class BAR_ArcBackup
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_ArcBackup : public BAR_Arc
{

public: //=== con-, destruction and assignment
    BAR_ArcBackup(BAR_Callback& callback);   

    virtual void Open(const string& sFileName);
    virtual void Close();
    
    void Store(BYTE byte);
    void Store(LPBYTE byteData, DWORD dwSize);
    void Store(long l);
    void Store(const string& s);
    void Store(const wstring& ws);

    fstream::pos_type GetFilePos() { return m_ofsArchive.tellp(); }
    void GotoFilePos(fstream::pos_type pos)    { m_ofsArchive.seekp(pos); }

public: //=== access

public: //=== operations

public: //=== overrides

protected: //=== helpers

    enum {
        ERR_0700 = 700
    };

    void Throw_ArcFileExistsAlready(const string& sArcFile) const;

private: //=== attributes
    
    ofstream m_ofsArchive;
};


//=============================================================================*
#endif // _BAR_ARCPROJECT_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_ArcBackup.hpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:23
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcBackup.hpp $
