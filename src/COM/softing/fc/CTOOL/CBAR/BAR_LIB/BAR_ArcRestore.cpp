/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcRestore.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_ArcRestore.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_ArcRestore.cpp $
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
#include "BAR_Trace.hpp"
#include "BAR_ArcRestore.hpp"
#include "BAR_FileInfo.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Util.hpp"
#include "BAR_ZIP.hpp"

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_ArcRestore
//=============================================================================*

//=============================================================================*
BAR_ArcRestore::BAR_ArcRestore(BAR_Callback& callback)
//=============================================================================*
: BAR_Arc(callback)
{
}

//=============================================================================*
void BAR_ArcRestore::Open(const string& sFileName)
//=============================================================================*
{
    //=== start open
    BAR_Arc::Open(sFileName);

    //=== uncompress archive into temporary file
    BAR_ZIP::uncompressFile(GetCallback(), GetArcFileName(), GetTempFileName());

    //=== open archive file for reading
    m_ifsArchive.open(GetTempFileName().c_str(), ios_base::binary);
    if ( ! m_ifsArchive )
    {
        Throw_FatalError(string("could not open project restore archive for reading, ")
                         + "file=sFileName");
    }
}

//=============================================================================*
void BAR_ArcRestore::Close()
//=============================================================================*
{
    //=== close input stream
    m_ifsArchive.close();

    //=== finish close
    BAR_Arc::Close();
}

//=============================================================================*
void BAR_ArcRestore::Restore(BYTE& byte)
//=============================================================================*
{
    m_ifsArchive.get((char&)byte);
    if ( ! m_ifsArchive )
    {
        Throw_FatalError("restoring byte failed");
    }
}

//=============================================================================*
void BAR_ArcRestore::Restore(LPBYTE& byteData, DWORD dwSize)
//=============================================================================*
{
    if ( byteData == NULL )
    {
        Throw_FatalError("can not restore to null byte array");
    }
    try 
    {
        for(DWORD dw = 0; dw < dwSize; dw++)
        {
            Restore(byteData[dw]);
        }
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("restoring raw data failed");
    }
}

//=============================================================================*
void BAR_ArcRestore::Restore(string& s)
//=============================================================================*
{
    try
    {
        s = "";
        long length = 0;
        Restore(length);
        for(int i = 0; i < length; i++)
        {
            BYTE byte;
            Restore(byte);
            s += byte;
        }
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("restoring wstring failed");
    }
}

//=============================================================================*
void BAR_ArcRestore::Restore(wstring& ws)
//=============================================================================*
{
    try
    {
        ws = L"";
        long length = 0;
        Restore(length);
        for(int i = 0; i < length; i+=2)
        {
            BYTE hibyte;
            BYTE lobyte;
            Restore(hibyte);
            Restore(lobyte);
            ws += MAKEWORD(lobyte, hibyte);
        }
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("restoring wstring failed");
    }
}

//=============================================================================*
void BAR_ArcRestore::Restore(long& l)
//=============================================================================*
{
    try 
    {
        BYTE loword_lobyte;
        BYTE loword_hibyte;
        BYTE hiword_lobyte;
        BYTE hiword_hibyte;

        Restore(loword_lobyte);
        Restore(loword_hibyte);
        Restore(hiword_lobyte);
        Restore(hiword_hibyte);

        l = MAKELONG( MAKEWORD(loword_lobyte, loword_hibyte),
                      MAKEWORD(hiword_lobyte, hiword_hibyte) );
    }
    catch(BAR_Error e)
    {
        Throw_FatalError("restoring long failed");
    }
}

//=============================================================================*
void BAR_ArcRestore::Skip(long l)
//=============================================================================*
{
    char byte;
    for(long ll = 0; ll < l; ll++)
    {
        m_ifsArchive.get((char&)byte);
        if ( ! m_ifsArchive )
        {
            Throw_FatalError("skipping failed");
        }
    }
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_ArcRestore.cpp $
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
 * User: Kt           Date: 10.07.01   Time: 16:28
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_ArcRestore.cpp $
