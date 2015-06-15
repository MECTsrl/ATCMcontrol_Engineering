/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_FileInfo.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_FileInfo.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_FileInfo.cpp $
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

#include <io.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <sstream>
using namespace ::std;

#include "BAR_FileInfo.hpp"
#include "BAR_ArcBackup.hpp"
#include "BAR_ArcRestore.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Error.hpp"
#include "BAR_Util.hpp"
#include "BAR_Trace.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*

string BAR_FileInfo::m_sStdLibDir = "";

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_FileInfo
//=============================================================================*

//=============================================================================*
BAR_FileInfo::BAR_FileInfo(const string& sDir)
//=============================================================================*
: m_sDir(sDir),
  m_sFile(""),
  m_sPathKeyword(""),
  m_atime(0),
  m_ctime(0),
  m_mtime(0),
  m_mode(0),
  m_bIsStdLib(false)
{
}

//=============================================================================*
BAR_FileInfo::BAR_FileInfo(const string& sDir, const string& sFile)
//=============================================================================*
: m_sDir(sDir),
  m_sFile(sFile),
  m_sPathKeyword("")
{
    ExpandFilePath();
    
    struct _stat buf;
    if ( 0 != ::_stat(GetFullPath().c_str(), &buf ) )
    {
        Throw_FatalError(string("could not get file stats")
                         + "file=" + GetFullPath());
    }

    m_ctime = buf.st_ctime;
    m_atime = buf.st_atime;
    m_mtime = buf.st_mtime;
    m_mode  = buf.st_mode;
}

//=============================================================================*
string BAR_FileInfo::GetFullPath() const
//=============================================================================*
{
    if ( m_sDir == "" )
        return m_sFile;
    else if ( IsAbsolutePath(m_sFile) )
        return m_sFile;
    else
        return m_sDir + "\\" + m_sFile;
}

//=============================================================================*
void BAR_FileInfo::ExpandFilePath()
//=============================================================================*
{
    if ( GetName().find("|4CSTDLIBDIR|") == 0 )
    {
        m_bIsStdLib = true; 
        m_sPathKeyword = "|4CSTDLIBDIR|";

        if ( m_sStdLibDir == "" )
        {
            if ( ! GetStandardLibDir(m_sStdLibDir) )
            {
                Throw_NoStandardLibDirFound();
            }
        }
        m_sDir = m_sStdLibDir;

        m_sFile.erase(0, string("|4CSTDLIBDIR|").length());
    }
}


//=============================================================================*
void BAR_FileInfo::Store(BAR_ArcBackup& arc) const
//=============================================================================*
{
    //=== store path keyword
    arc.Store(m_sPathKeyword);
    
    //=== store name
    arc.Store(GetName());

    //== store times
    arc.Store((long)m_ctime);
    arc.Store((long)m_atime);
    arc.Store((long)m_mtime);

    //=== store mode
    unsigned long mode = m_mode;
    arc.Store((long)mode);

    // SIS: create empty directories, 07.04.04 >>
    if(mode & _S_IFDIR)
    {
        BAR_TRACE1(3, "stored dir=%s", GetFullPath().c_str());
        return;
    }
    // SIS: create empty directories, 07.04.04 <<

    //=== store size placeholder and remember this position
    fstream::pos_type posFileSize = arc.GetFilePos();
    arc.Store((long)0);

    //=== store file content
    ifstream ifsFileContent(GetFullPath().c_str(), ios_base::binary);
    unsigned long ulChecksum = 0;
    BYTE byte;
    long lSize = 0;
    while ( ifsFileContent.get((char&)byte) )
    {
        arc.Store(byte);
        ulChecksum += byte;
        lSize++;
    }
    if ( ! ifsFileContent.eof() )
    {
        Throw_FatalError(string("storing file failed, ") 
                        + "file=" + GetFullPath());
    }
    arc.Store((long)ulChecksum);

    //=== remember actual pos
    fstream::pos_type posActual = arc.GetFilePos();

    //=== goto to pos where file size is stored
    arc.GotoFilePos(posFileSize);

    //=== store filesize
    arc.Store(lSize);

    //=== goto to actual pos
    arc.GotoFilePos(posActual);

    BAR_TRACE3(3, "stored, file=%s, size=%ld, checksum=%lu",
                 GetFullPath().c_str(), lSize, ulChecksum);
}

//=============================================================================*
void BAR_FileInfo::Restore(BAR_Callback& callback, BAR_ArcRestore& arc)
//=============================================================================*
{
    //=== read path keyword
    arc.Restore(m_sPathKeyword);
    
    //=== read filename
    arc.Restore(m_sFile);

    m_sFile.insert(0, m_sPathKeyword);

    ExpandFilePath();
/*
    //=== check if overwrite is allowed
    if ( IsStandardLib() )
    {
        if ( ExistsFile(GetFullPath()) )
        {
            if ( ! callback.IsStdLibOverwriteAllowed("Do want "
                                                     "to overwrite "
                                                     "4C standard "
                                                     "libraries ?"))
            {
                Throw_OverwritingStdLibNotAllowed();
            }
        }
    }
*/

    //=== read timestamps
    long ctime; arc.Restore(ctime); m_ctime = ctime;
    long atime; arc.Restore(atime); m_atime = atime;
    long mtime; arc.Restore(mtime); m_mtime = mtime;

    //== read file mode
    long mode; arc.Restore((long)mode); m_mode = mode;

    // SIS: create empty directories, 07.04.04 >>
    if(mode & _S_IFDIR)
    {
        BAR_TRACE1(3, "restored dir=%s", GetFullPath().c_str());
        CreateDir(GetFullPath());
        return;
    }
    // SIS: create empty directories, 07.04.04 <<

    //=== read size of file content
    long lSize = 0;
    arc.Restore(lSize);

    //=== ensure that parent dir exists
    CreateParentDir(GetFullPath());

    //=== - standard libs that already exist 
    //===   will always be restored to a modified name first
    //===   and only if binary comparison succeeded
    //===   the restored file will be removed again
    //===   in that case an inputstream on the existing libfile
    //===   has to be opened for binary comparison
    //=== - other files and standard libs that do not yet exist
    //===   will simply be restored to its stored file name
    ifstream ifsExistingStdLib;
    string sTargetFile = GetFullPath();

    if ( IsStandardLib() )
    {
        if ( ::ExistsFile(GetFullPath()) )
        {
            //=== open input file stream for binary comparison
            ifsExistingStdLib.open(GetFullPath().c_str(), ios_base::binary);
            if ( ! ifsExistingStdLib.is_open() )
            {
                Throw_FatalError(string("could not open file for reading, ")
                    + "file=" + GetFullPath());
            }

            //=== add suffix .RESTORED to targetpath until no file
            //=== exists under this path
            for( ; ::ExistsFile(sTargetFile); )
            {
                sTargetFile.append(".RESTORED");
            }
        }
    }

    //=== open output stream for writing
    ofstream ofsTargetFile(sTargetFile.c_str(), ios_base::binary);
    if ( ! ofsTargetFile )
    {
        Throw_FatalError(string("could not open file for writing, ")
                         + "file=" + sTargetFile);
    }
    //=== write file content
    //=== do binary comparison on the fly if necessary
    unsigned long ulChecksum = 0;
    bool bDoBinaryComparison = ifsExistingStdLib.is_open();

    bool bBinaryDifferenceDetected = false;
    for( long i = 0; i < lSize; i++ )
    {
        BYTE byteRestored;

        //=== restore byte
        arc.Restore(byteRestored);
        ofsTargetFile.put(byteRestored);
        ulChecksum += byteRestored;
        if ( ! ofsTargetFile )
        {
            Throw_FatalError("error while restoring file");
        }

        //=== do binary comparison
        //=== but only if necessary and
        //=== there has been no difference detected until now
        if ( bDoBinaryComparison && ! bBinaryDifferenceDetected )
        {
            BYTE byteOriginal;
     
            //=== if there is one more byte
            if ( ifsExistingStdLib.get((char&)byteOriginal) )
            {
                //=== and it is not equal to the restored byte
                if ( byteOriginal != byteRestored )
                {    //=== the a difference has been detected
                     bBinaryDifferenceDetected = true;
                    BAR_TRACE(3, "byte difference detected");
                }
            }
            //=== otherwise the existing file is smaller that the 
            //=== file to restore
            else
            {
                //=== and this means also a difference has been detected
                bBinaryDifferenceDetected = true;
                BAR_TRACE(3, "existing file is smaller");
            }
        }
    }
    //=== OK: restored file is written completely, close it now
    ofsTargetFile.close();

    //=== if binary comparison has been done, check that original
    //=== is not bigger than the restored file
    if ( bDoBinaryComparison &&
         ! bBinaryDifferenceDetected )
    {   
        char ch;
        if ( ifsExistingStdLib.get(ch) )
        {
            bBinaryDifferenceDetected = true;
            BAR_TRACE(3, "existing file is bigger");
        }
    }

    ifsExistingStdLib.close();

    //=== compare checksum
    unsigned long ulStoredChecksum = 0;
    arc.Restore((long&)ulStoredChecksum);
    if ( ulChecksum != ulStoredChecksum )
    {
        Throw_ChecksumError(ulChecksum, ulStoredChecksum);
    }
    BAR_TRACE3(3, "file restored, file=%s, size=%ld, checksum=%lu",
                  sTargetFile.c_str(), lSize, ulChecksum);

    //=== restore time of last access and modification
    _utimbuf t;
    t.actime = m_atime;
    t.modtime = m_mtime;
    if ( 0 != _utime(GetFullPath().c_str(), &t) )
    {
        arc.GetCallback().SendWarning(BAR_Warning(0, wstring(L"could not utime")
                                                 + L", file=" + ToWS(GetFullPath())));
    }

    ///=== restore file mode
    if ( 0 != _chmod(GetFullPath().c_str(), m_mode) )
    {
        arc.GetCallback().SendWarning(BAR_Warning(0, wstring(L"could not chmod")
                                                 + L", file=" + ToWS(GetFullPath())));
    }

    //=== if binary comparison was necessary 
    if ( bDoBinaryComparison  )
    {
        //=== and binary differences have been detected
        if ( bBinaryDifferenceDetected )
        {
            //=== then leave the restored file

            //=== but notify the user about the modified file name
            Warning_CanNotOverwriteStdLib(callback, sTargetFile);

        }
        //=== otherwise
        else
        {
            //=== remove the restored file
            ::remove(sTargetFile.c_str());

            callback.SendInfo(wstring(L"standard lib exists but is identical"
                                     L" - leaving original file, file=") +
                              ToWS(GetFullPath()));
        }
    }


    //=== finished
}

//=============================================================================*
void BAR_FileInfo::CreateParentDir(const string& sFile)
//=============================================================================*
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    drive[0] = dir[0] = fname[0] = ext[0] = '0';

    _splitpath(sFile.c_str(), drive, dir, fname, ext);

    string sDirFNameExt = dir;
    sDirFNameExt += fname;
    sDirFNameExt += ext;

    CreateParentDir(drive, sDirFNameExt);
}

// SIS: create empty directories, 07.04.04 >>
//=============================================================================*
void BAR_FileInfo::CreateDir(const string& sDir)
//=============================================================================*
{
    string  sCmd = "md \"" + sDir + "\"";
    system(sCmd.c_str());
}
// SIS: create empty directories, 07.04.04 <<
    
//=============================================================================*
bool BAR_FileInfo::IsAbsolutePath(const string& sPath) const
//=============================================================================*
{
    //=== starts with slash or backslash
    if ( sPath.find_first_of("\\/") == 0 )
        return true;
    //=== starts with drive
    else if ( sPath[1] == ':' )
        return true;
    else
        return false;
}

//=============================================================================*
void BAR_FileInfo::CreateParentDir(const string& sDrive, const string& sDir)
//=============================================================================*
{
    string::size_type pos = sDir.find_last_of("\\/");

    //=== find first position of last separator block
    if ( pos != string::npos )
        pos = sDir.find_last_not_of("\\/", pos);
    if ( pos != string::npos )
        pos = sDir.find_first_of("\\/", pos);

    //=== no parent dir 
    if ( pos == string::npos )
    {
        return;
    }

    string sParentDir = sDir.substr(0, pos);
    string sDriveAndParentDir = sDrive + sParentDir;

    if ( ExistsFile(sDriveAndParentDir) )
    {
        return;
    }
    
    //=== create parents parent
    CreateParentDir(sDrive, sParentDir);

    if ( 0 != _mkdir(sDriveAndParentDir.c_str()) )
    {
        Throw_FatalError(string("could not create directory, ")
                         + "dir=" + sDriveAndParentDir);
    }

}

//=============================================================================*
//
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
bool BAR_FileInfo::GetStandardLibDir(string& sStdLibDir) const
{
    HKEY hKeyReg;
    BOOL bRet= TRUE;
    string sRegKey = "SOFTWARE\\Softing\\"
                     "4CONTROLV2\\2.0";
    LONG lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                sRegKey.c_str(),
                                0,
                                KEY_QUERY_VALUE,
                                &hKeyReg);
    if ( ERROR_SUCCESS != lResult )
    {
        return false;
    }

    char psz4CAppPath[_MAX_PATH];
    DWORD dwType;
    DWORD dwSize = _MAX_PATH;
    lResult = ::RegQueryValueEx(hKeyReg,
                                "LibPath",
                                0,
                                &dwType,
                                (LPBYTE)&psz4CAppPath,
                                &dwSize);
    RegCloseKey(hKeyReg);

    if ( ERROR_SUCCESS != lResult ||
         REG_SZ != dwType )
    {
        return false;
    }

    sStdLibDir = string(psz4CAppPath);
    return true;
}

//=============================================================================*
void BAR_FileInfo::Throw_FatalError(const string& sDetails) const
//=============================================================================*
{
    throw BAR_Error(ERR_0700, L"fatal error", ToWS(sDetails));
}

//=============================================================================*
void BAR_FileInfo::Throw_ChecksumError(unsigned long lActualChecksum,
                                       unsigned long lStoredChecksum) const
//=============================================================================*
{
    wostringstream wosDetails;
    wosDetails << L"actual=" << lActualChecksum 
               << L", stored=" << lStoredChecksum
               << L", file=" << ToWS(GetName());
    throw BAR_Error(ERR_0701, L"check sum error", wosDetails.str());
}

//=============================================================================*
void BAR_FileInfo::Throw_NoStandardLibDirFound() const
//=============================================================================*
{
    throw BAR_Error(ERR_0702, L"could not find 4CE standard library directory");
}

//=============================================================================*
void BAR_FileInfo::Warning_CanNotOverwriteStdLib(BAR_Callback& callback,
                                                 const string& sTargetFile) const
//=============================================================================*
{
    callback.SendWarning(
        BAR_Warning(WRN_0703,
                    wstring(L"different standard lib \"") + ToWS(GetName()) +
                            L"\" exists - restored to \"" +
                    ToWS(sTargetFile) + L"\""));
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_FileInfo.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 7.04.04    Time: 19:54
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * create empty directories
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 23.07.02   Time: 16:12
 * Updated in $/4Control/COM/softing/fc/AddOns/Softing/4CPCTarget/CBAR/BAR_LIB
 * Fixed some mistakes/changes in V2.0 and added flag -all to backup the
 * gen folder too.
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_FileInfo.cpp $
