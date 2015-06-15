/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Arc.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Arc.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Arc.hpp $
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

#ifndef _BAR_ARC_H_
#define _BAR_ARC_H_

//====  Aggregate Includes:   =================================================*
#include <Windows.h>

#include <string>
#include <fstream>
using namespace ::std;

//====  Forward Class Definitions:   ==========================================*
class BAR_Callback;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*



//=============================================================================*
/**
 *  class BAR_Arc
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_Arc
{

public: //=== con-, destruction and assignment

   BAR_Arc(BAR_Callback& callback);
   virtual ~BAR_Arc();

    virtual void Open(const string& sFileName)=0;
    virtual void Close();
    
public: //=== access

    bool IsOpened() const;

    BAR_Callback& GetCallback() const;
    const string& GetTempFileName() const;
    const string& GetArcFileName() const;

public: //=== operations

public: //=== overrides

protected: //=== helpers

    enum {
        ERR_0600 = 600,
        ERR_0601 = 601,
    };

    void SetOpened(bool bIsOpened);
        
    virtual void Cleanup();

    string CreateTempFileName() const;
    
    void Throw_FatalError(const string& sDetails) const;
    void Warning_CouldNotRemoveTempFile(const string& sTmpFile) const;

protected: //=== data

    BAR_Callback& m_callback;

    string m_sTmpFile; //=== uncompressed working file
    string m_sArcFile; //=== compressed file

    bool m_bIsOpened;
};

//=============================================================================*
inline void BAR_Arc::SetOpened(bool bIsOpened)
//=============================================================================*
{ m_bIsOpened = bIsOpened; }

//=============================================================================*
inline bool BAR_Arc::IsOpened() const
//=============================================================================*
{ return m_bIsOpened; }

//=============================================================================*
inline BAR_Callback& BAR_Arc::GetCallback() const
//=============================================================================*
{ return m_callback; }

//=============================================================================*
inline const string& BAR_Arc::GetTempFileName() const
//=============================================================================*
{ return m_sTmpFile; }

//=============================================================================*
inline const string& BAR_Arc::GetArcFileName() const
//=============================================================================*
{ return m_sArcFile; }

//=============================================================================*
#endif // _BAR_ARC_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Arc.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 21.04.04   Time: 15:32
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * error numbers ERR_0600, 1
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:22
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Arc.hpp $
