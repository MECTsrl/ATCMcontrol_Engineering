/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Arc.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Arc.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Arc.cpp $
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

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


#include <assert.h>
#include <ctime>

#include "BAR_Trace.hpp"
#include "BAR_Arc.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Util.hpp"

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_Arc
//=============================================================================*

//=============================================================================*
BAR_Arc::BAR_Arc(BAR_Callback& callback)
//=============================================================================*
: m_callback(callback),
  m_sTmpFile(""),
  m_sArcFile(""),
  m_bIsOpened(false)
{
    //=== intialize random seed
    srand( (unsigned)time( NULL ) );
}

//=============================================================================*
BAR_Arc::~BAR_Arc()
//=============================================================================*
{
    Cleanup();
}

//=============================================================================*
void BAR_Arc::Open(const string& sFile)
//=============================================================================*
{
    if ( IsOpened() )
    {
        Throw_FatalError("archive opened twice");
    }

    //=== remember name of archive file
    m_sArcFile = sFile;

    //== get a new temporary file name
    m_sTmpFile = CreateTempFileName();

    //=== finished
}

//=============================================================================*
string BAR_Arc::CreateTempFileName() const
//=============================================================================*
{
    //=== get path to temp dir
    char szBuf[MAX_PATH];
    string sTmpDir("."); //=== use current working dir as default
    if ( 0 != ::GetTempPath(MAX_PATH, szBuf) )
    {
        sTmpDir = szBuf;
    }
  
    //=== get a unique tmp file name
    if ( 0 == ::GetTempFileName(sTmpDir.c_str(),
                                "4CBAR",
                                (UINT)rand()%1000,
                                szBuf)
       )
    {
        Throw_FatalError("could not get a temporary file name");
    }

    //=== finished
    return szBuf;    
}

//=============================================================================*
void BAR_Arc::Close()
//=============================================================================*
{
    Cleanup();
    SetOpened(false);
}

//=============================================================================*
void BAR_Arc::Cleanup()
//=============================================================================*
{
    //=== remove temporary file
    if ( ! GetTempFileName().empty() )
    {

        int err = ::remove(GetTempFileName().c_str());
        if ( err == 0 )
        {
            BAR_TRACE1(2, 
                       "removed temporary file \"%s\"",
                       GetTempFileName().c_str());
        }
        else if ( IsOpened() )
        {
            Warning_CouldNotRemoveTempFile(GetTempFileName());
        }
    }

    m_sTmpFile = "";
}

//=============================================================================*
void BAR_Arc::Throw_FatalError(const string& sDetails) const
//=============================================================================*
{
    throw BAR_Error(ERR_0600, L"fatal error", ToWS(sDetails));
}

//=============================================================================*
void BAR_Arc::Warning_CouldNotRemoveTempFile(const string& sTmpFile) const
//=============================================================================*
{
    m_callback.SendWarning(BAR_Warning(ERR_0601,
                                       L"could not remove temporary file",
                                       wstring(L"tmp=") + ToWS(sTmpFile)));
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Arc.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:21
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Arc.cpp $
