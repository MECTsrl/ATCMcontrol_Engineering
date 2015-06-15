/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcBackup.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcBackup.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcBackup.cpp $
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
#include "BAR_ArcBackup.hpp"
#include "BAR_FileInfo.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Util.hpp"
#include "BAR_Trace.hpp"
#include "BAR_ZIP.hpp"

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_ArcBackup
//=============================================================================*

//=============================================================================*
BAR_ArcBackup::BAR_ArcBackup(BAR_Callback& callback)
//=============================================================================*
: BAR_Arc(callback)
{
}

//=============================================================================*
void BAR_ArcBackup::Open(const string& sFileName)
//=============================================================================*
{
    //=== start open
    BAR_Arc::Open(sFileName);

    //=== open temporary file for writing
    m_ofsArchive.open(GetTempFileName().c_str(), ios_base::binary);
    if ( ! m_ofsArchive )
    {
        Throw_FatalError(string("could not open project backup archive for writing, ")
                         + "file=sFileName");
    }
}

//=============================================================================*
void BAR_ArcBackup::Close()
//=============================================================================*
{
    //=== close output stream
    m_ofsArchive.close();

    //=== if archive file exists already
    if ( ::ExistsFile(GetArcFileName()) )
    {
        //=== then ask for permission to overwrite it
        if ( GetCallback().IsOverwriteAllowed("The target file exists already."
                                            " Do you want to overwrite it ?") )
        {
            ::remove(GetArcFileName().c_str());
        }
        else
        {
            BAR_Arc::Close();
            Throw_ArcFileExistsAlready(GetArcFileName());
        }
    }

    //=== zip temporary file
    BAR_ZIP::compressFile(GetCallback(), GetTempFileName(), GetArcFileName());

    //=== finish close
    BAR_Arc::Close();
}

//=============================================================================*
void BAR_ArcBackup::Store(BYTE byte)
//=============================================================================*
{
    if ( m_ofsArchive.good() )
    {   
        m_ofsArchive.put(byte);
        if ( ! m_ofsArchive )
        {
            Throw_FatalError("storing byte failed");
        }
    }
    else
    {
        Throw_FatalError("can not store byte, stream state is not good");
    }
}

//=============================================================================*
void BAR_ArcBackup::Store(LPBYTE byteData, DWORD dwSize)
//=============================================================================*
{
    try 
    {
        for(DWORD dw = 0; dw < dwSize; dw++)
        {
            Store(byteData[dw]);
        }
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("storing raw data failed");
    }
}

//=============================================================================*
void BAR_ArcBackup::Store(const string& s)
//=============================================================================*
{
    try
    {
        Store((long)s.length());
        for(int i = 0; i < s.length(); i++)
        {
            Store((BYTE)s[i]);
        }
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("storing string failed");
    }
}

//=============================================================================*
void BAR_ArcBackup::Store(const wstring& ws)
//=============================================================================*
{
    try
    {
        Store((long)(2 * ws.length()));
        for(int i = 0; i < ws.length(); i++)
        {
            WORD w = ws[i];
            Store(HIBYTE(w));
            Store(LOBYTE(w));
        }
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("storing wstring failed");
    }
}

//=============================================================================*
void BAR_ArcBackup::Store(long l)
//=============================================================================*
{
    try 
    {
        //=== store from lowest to highest byte
        Store(LOBYTE(LOWORD(l)));
        Store(HIBYTE(LOWORD(l)));
        Store(LOBYTE(HIWORD(l)));
        Store(HIBYTE(HIWORD(l)));
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("storing long failed");
    }
}

//=============================================================================*
void BAR_ArcBackup::Throw_ArcFileExistsAlready(const string& sArcFile) const
//=============================================================================*
{
    throw BAR_Error(ERR_0700,
                    L"archive file exists already",
                    wstring(L"arc=") + ToWS(sArcFile));
                   
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_ArcBackup.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:22
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcBackup.cpp $
