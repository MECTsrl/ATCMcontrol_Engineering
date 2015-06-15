/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_FileInfo.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_FileInfo.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_FileInfo.hpp $
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

#ifndef _BAR_FILEINFO_H_
#define _BAR_FILEINFO_H_

//====  Aggregate Includes:   =================================================*
#include <string>
#include <ostream>

using namespace ::std;

//====  Forward Class Definitions:   ==========================================*
class BAR_ArcBackup;
class BAR_ArcRestore;
class BAR_Callback;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_FileInfo
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_FileInfo
{

public: //=== con-, destruction and assignment

    BAR_FileInfo(const string& sDir, const string& sFile);
    BAR_FileInfo(const string& sDir);

public: //=== access

    const string& GetDir() const;
    const string& GetName() const;
    string GetFullPath() const;
    time_t GetCTime() const;
    time_t GetATime() const;
    time_t GetMTime() const;
    unsigned short GetMode() const;

    void Store(BAR_ArcBackup& arc) const;
    void Restore(BAR_Callback& callback, BAR_ArcRestore& arc);


public: //=== operations

public: //=== overrides

private: //=== helpers
    
    enum {
        ERR_0700 = 700,
        ERR_0701 = 701,
        ERR_0702 = 702,
        WRN_0703 = 703
    };

    void ExpandFilePath();

    // SIS: create empty directories, 07.04.04 >>
    void CreateDir(const string& sDir);
    // SIS: create empty directories, 07.04.04 <<

    void CreateParentDir(const string& sFile);
    void CreateParentDir(const string& sDrive, const string& sDir);

    bool GetStandardLibDir(string& sStdLibDir) const;

    bool IsStandardLib() const;

    bool IsAbsolutePath(const string& sPath) const;

    void Throw_FatalError(const string& sDetails) const;
    void Throw_ChecksumError(unsigned long lActualChecksum,
                             unsigned long lStoredChecksum) const;
    void Throw_NoStandardLibDirFound() const;
    void Throw_OverwritingStdLibNotAllowed() const;

    void Warning_CanNotOverwriteStdLib(BAR_Callback& callback,
                                       const string& sTargetFile) const;

private: //=== attributes

    string m_sDir;
    mutable string m_sFile;
    string m_sPathKeyword;

    time_t m_ctime;
    time_t m_atime;
    time_t m_mtime;
    unsigned short m_mode;

    bool m_bIsStdLib;
        
    static string m_sStdLibDir;
};

//=============================================================================*
inline const string& BAR_FileInfo::GetDir() const
//=============================================================================*
{ return m_sDir; }

//=============================================================================*
inline const string& BAR_FileInfo::GetName() const
//=============================================================================*
{ return m_sFile; }

//=============================================================================*
inline time_t BAR_FileInfo::GetCTime() const
//=============================================================================*
{ return m_ctime; }

//=============================================================================*
inline time_t BAR_FileInfo::GetATime() const
//=============================================================================*
{ return m_atime; }

//=============================================================================*
inline time_t BAR_FileInfo::GetMTime() const
//=============================================================================*
{ return m_mtime; }

//=============================================================================*
inline unsigned short BAR_FileInfo::GetMode()  const
//=============================================================================*
{ return m_mode; }

//=============================================================================*
inline bool BAR_FileInfo::IsStandardLib() const
//=============================================================================*
{ return m_bIsStdLib; }

#endif

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_FileInfo.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 7.04.04    Time: 19:55
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * create empty directories
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
// $Workfile: BAR_FileInfo.hpp $
