/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Project.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Project.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Project.cpp $
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

#include <limits>

#include <fstream>
#include <cstdlib>
using namespace ::std;

#include "BAR_Project.hpp"
#include "BAR_Callback.hpp"
#include "BAR_ArcBackup.hpp"
#include "BAR_ArcRestore.hpp"
#include "BAR_FileInfo.hpp"
#include "BAR_Error.hpp"
#include "BAR_Trace.hpp"
#include "BAR_Util.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


const string BAR_Project::m_sArcType10 = "4CBAR_PROJECT_ARCHIVE_V1.0";
const string BAR_Project::m_sArcType = "4CBAR_PROJECT_ARCHIVE_V2.0";

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_Project
//=============================================================================*

//=============================================================================*
//
// performing a project backup to a local archive file
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Project::DoBackupToFile(const string& sProjectDir, 
                                 const string& sPassword,
                                 const string& sEncryptKey,
                                 const string& sArcFile,
                                 BAR_Callback& callback,
                                 const bool bAll)
{
    //=== check that project dir exists
    if ( ! ::ExistsDir(sProjectDir) )
    {
        Throw_ProjectDirNotFound(sProjectDir);
    }
    
    //=== collect project files in sProjectDir
    vector<string> vecProjectFiles;
    CollectProjectFiles(callback, vecProjectFiles, sProjectDir, bAll);

    //=== collect external libs
    CollectExternalLibs(callback, sProjectDir, vecProjectFiles);

    //=== open project archive
    BAR_ArcBackup arc(callback);
    arc.Open(sArcFile);

    //=== store archive type
    arc.Store(m_sArcType);

    //=== store password
    arc.Store(sPassword);

    //=== store number of files
    arc.Store((long)vecProjectFiles.size());

    //=== store project files
    for( vector<string>::const_iterator p = vecProjectFiles.begin();
         p != vecProjectFiles.end();
         ++p )
    {
        BAR_FileInfo file(sProjectDir, *p);
        file.Store(arc);
    }

    //=== close project archive
    arc.Close();

    // ENCRYPTION >>
    // if encrypt key is not empty
    if(!sEncryptKey.empty())
    {
        // encrypt archive file
        char    szTempPath[_MAX_PATH];
        char    szTempFileName[_MAX_PATH];
        if(!::GetTempPath(_MAX_PATH, szTempPath))
        {
            Throw_FatalError("could not get temp path");
        }
        if(!::GetTempFileName(szTempPath, "cbencrypt", 1, szTempFileName))
        {
            Throw_FatalError("could not get temp file");
        }
        ::DeleteFile(szTempFileName);

        E_EncryptErr eErr = Pack(sArcFile.c_str(), szTempFileName, sEncryptKey.c_str());

        // evaluate return value
        if(eErr != eEE_Ok)
        {
            char szErrorMessage[_MAX_PATH];

            switch(eErr)
            {
            case eEE_OpenFile:
                sprintf(szErrorMessage, "could not open file %s", sArcFile.c_str());
                break;
            case eEE_ReadFile:
                sprintf(szErrorMessage, "could not read file %s", sArcFile.c_str());
                break;
            case eEE_CreateFile:
                sprintf(szErrorMessage, "could not create file %s", szTempFileName);
                break;
            case eEE_WriteFile:
                sprintf(szErrorMessage, "could not write to file %s", szTempFileName);
                break;
            case eEE_WrongPassword:
                Throw_InvalidEncryptionPassword();
            default:
                break;
            }
            Throw_FatalError(szErrorMessage);
        }

        // successful -> now overwrite original archive file with encrypted version
        ::DeleteFile(sArcFile.c_str());
        if(!::CopyFile(szTempFileName, sArcFile.c_str(), FALSE))
        {
            char szErrorMessage[_MAX_PATH];
            sprintf(szErrorMessage, "could not copy file %s -> %s", szTempFileName, sArcFile.c_str());
            Throw_FatalError(szErrorMessage);
        }
    }
    // ENCRYPTION <<
}

//=============================================================================*
//
// performing a project backup to a control
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Project::DoBackupToControl(const string& sProjectDir, 
                                    const string& sPassword,
                                    const string& sEncryptKey,
                                    const string& sRemoteHost,
                                    BAR_Callback& callback)
{
    string sTargetDir = "\\\\" + sRemoteHost + "\\4C_BackupAndRestore$";
    if ( ! ExistsFile(sTargetDir) )
    {
        sTargetDir = "\\\\" + sRemoteHost + "\\4C_BackupAndRestore";
        if ( ! ExistsFile(sTargetDir) )
        {
            Throw_CanNotBackupToControl(sRemoteHost, sTargetDir);
        }
    }

    string sArchive = sTargetDir + "\\" + "ProjectBackup.4ca";
    DoBackupToFile(sProjectDir, sPassword, sEncryptKey, sArchive, callback, false);
}

//=============================================================================*
//
// performing a project restore from a file
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Project::DoRestoreFromFile(const string& sProjectDir, 
                                    const string& sPassword,
                                    const string& sEncryptKey,
                                    const string& sArcFile,
                                    BAR_Callback& callback)
{
    string sArcFileTmp(sArcFile);

    // ENCRYPTION >>
    // encrypt archive file
    char    szTempPath[_MAX_PATH];
    char    szTempFileName[_MAX_PATH];
    // if encrypt key is not empty
    if(!sEncryptKey.empty())
    {
        if(!::GetTempPath(_MAX_PATH, szTempPath))
        {
            Throw_FatalError("could not get temp path");
        }
        if(!::GetTempFileName(szTempPath, "cbencrypt", 1, szTempFileName))
        {
            Throw_FatalError("could not get temp file");
        }
        ::DeleteFile(szTempFileName);

        E_EncryptErr eErr = Unpack(sArcFile.c_str(), szTempFileName, sEncryptKey.c_str());

        // evaluate return value
        if(eErr != eEE_Ok)
        {
            char szErrorMessage[_MAX_PATH];

            switch(eErr)
            {
            case eEE_OpenFile:
                sprintf(szErrorMessage, "could not open file %s", sArcFile.c_str());
                break;
            case eEE_ReadFile:
                sprintf(szErrorMessage, "could not read file %s", sArcFile.c_str());
                break;
            case eEE_CreateFile:
                sprintf(szErrorMessage, "could not create file %s", szTempFileName);
                break;
            case eEE_WriteFile:
                sprintf(szErrorMessage, "could not write to file %s", szTempFileName);
                break;
            case eEE_WrongPassword:
                Throw_InvalidEncryptionPassword();
            default:
                break;
            }
            Throw_FatalError(szErrorMessage);
        }

        // successful -> now overwrite original archive file with encrypted version
        sArcFileTmp = szTempFileName;
    }
    // ENCRYPTION <<

    //=== open archive
    BAR_ArcRestore arc(callback);
    arc.Open(sArcFileTmp);
    
    //=== restore archive type
    string sArcType;
    arc.Restore(sArcType);
    if ( sArcType != m_sArcType )
    {
        Throw_InvalidArchiveType(sArcType);
    }

    //=== restore password
    string sStoredPassword;
    arc.Restore(sStoredPassword);

    //=== check password
    if ( sStoredPassword != "" && //=== but only if password is required
         sStoredPassword != sPassword )
    {
        Throw_InvalidPassword();
    }

    //=== if target directory already exists
    if ( ExistsFile(sProjectDir) )
    {
        //=== ask for permission to overwrite
        if ( ! callback.IsOverwriteAllowed("The project directory already exists."
                                           "Do you want to overwrite its contents ?") )
        {
            Throw_ProjectDirExistsAlready(sProjectDir);
        }
    }

    //=== restore number of files
    long noOfFiles;
    arc.Restore(noOfFiles);

    //=== restore files
    for(long i = 0; i < noOfFiles; i++)
    {
        BAR_FileInfo file(sProjectDir);
        file.Restore(callback, arc);
    }

    //=== close archive
    arc.Close();

    // ENCRYPTION >>
    if(!sEncryptKey.empty())
    {
        // delete temp file
        ::DeleteFile(szTempFileName);
    }
}

//=============================================================================*
//
// performing a project restore from a control
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Project::DoRestoreFromControl(const string& sProjectDir, 
                                       const string& sPassword,
                                       const string& sEncryptKey,
                                       const string& sRemoteHost,
                                       BAR_Callback& callback)
{
    string sTargetDir = "\\\\" + sRemoteHost + "\\4C_BackupAndRestore$";
    if ( ! ExistsFile(sTargetDir) )
    {
        sTargetDir = "\\\\" + sRemoteHost + "\\4C_BackupAndRestore";
        if ( ! ExistsFile(sTargetDir) )
        {
            Throw_CanNotBackupToControl(sRemoteHost, sTargetDir);
        }
    }

    string sArchive = sTargetDir + "\\" + "ProjectBackup.4ca";
    DoRestoreFromFile(sProjectDir, sPassword, sEncryptKey, sArchive, callback);
}


//=============================================================================*
//
// get name of first file in dir
// @param           
// @return          
// @exception       
//
//=============================================================================*
string BAR_Project::GetFirstProjectFile(const string& sDir)
{
    WIN32_FIND_DATA FileData; 
    string sSearchPattern = sDir + "\\*.4cp";
    HANDLE hSearch = FindFirstFile(sSearchPattern.c_str(), &FileData);
    if ( hSearch == INVALID_HANDLE_VALUE ) 
    { 
        Throw_FatalError(string("no file *.4cp found in directory ")
                         + sDir);
    }
    
    const string sProjectFile = GetFullPath(sDir + "\\" + FileData.cFileName);

    return sProjectFile;
}

//=============================================================================*
//
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Project::CollectProjectFiles(BAR_Callback& callback,
                                      vector<string>& vecFiles,
                                      const string& sProjectRootDir,
                                      const bool bAll,
                                      const string& sRelDir
                                     )
{
    const string sFilePattern = sProjectRootDir + "\\" + sRelDir + "\\*";
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = ::FindFirstFile(sFilePattern.c_str(), &FindFileData);
    if ( hFind == INVALID_HANDLE_VALUE )
    {
        return;
    }

    for( ; ::FindNextFile(hFind, &FindFileData);  )
    {
        //=== collect contents of subdirectories
        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            //=== but ignore subdirs gen and ..
            if ( string("..")  != FindFileData.cFileName)
            {
                // ignore gen folder only if -All is not selected
                if (bAll || string("gen") != FindFileData.cFileName)
                {
                    string sNewRelDir = ( sRelDir == "" ? "" : sRelDir + "\\" )
                                        + FindFileData.cFileName;

                    // SIS: create empty directories, 07.04.04 >>
                    vecFiles.push_back(sNewRelDir);
                    // SIS: create empty directories, 07.04.04 <<

                    CollectProjectFiles(callback,
                                        vecFiles,
                                        sProjectRootDir,
                                        bAll,
                                        sNewRelDir);
                }
            }
        }
        else
        {
            string sFile = sRelDir 
                         + ( sRelDir == "" ? "" : "\\" )
                         + FindFileData.cFileName;
            vecFiles.push_back(sFile);
        }
    }
    ::FindClose(hFind);
}

//=============================================================================*
//
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Project::CollectExternalLibs(BAR_Callback& callback,
                                      const string& sProjectDir,
                                      vector<string>& vecProjectFiles)
{
    //=== get project file
    string sProjectFile = GetFirstProjectFile(sProjectDir);

    //=== open project file for reading
    ifstream ifsProjectFile(sProjectFile.c_str());
    if ( ! ifsProjectFile )
    {
        Throw_FatalError(string("could not open file for reading, ")
                         + "file=" + sProjectFile);
    }

    //=== scan lines
    string sLine;
    string sLibPrefix = "file=";
    string sStdLibDir = "$(LIBDIR)\\";
    bool bSectionFound = false;
    while( ifsProjectFile )
    {
        getline(ifsProjectFile, sLine);

        if (bSectionFound)
        {
            if (sLine[0] == '[')
            {
                bSectionFound = false;
                continue;
            }

        }

        if (sLine == "[Libs]")
        {
            bSectionFound = true;
            continue;
        }

        if (bSectionFound)
        {
           
            //=== ignore non-lib-lines
            if ( sLibPrefix != sLine.substr(0, sLibPrefix.length()) )
            {
                continue;
            }

            //=== extract lib path
            string sLibPath = sLine.substr(sLibPrefix.length(),
                                           sLine.find_last_not_of(" "));
            //=== lib paths with $(LIBDIR)\ path prefix are deployed in
            //=== 4C-standard-lib dir
            if (sStdLibDir == sLibPath.substr(0, sStdLibDir.length()))
            {
                sLibPath.replace(0, sStdLibDir.length(), "|4CSTDLIBDIR|");
            }
            vecProjectFiles.push_back(sLibPath);  
        }
    }
}

//=============================================================================*
string BAR_Project::GetFullPath(const string& sPath)
//=============================================================================*
{
    char full[_MAX_PATH];
    if ( _fullpath(full, sPath.c_str(), _MAX_PATH) == NULL )
    {
        return sPath;
    }
    return full;

}

//=============================================================================*
void BAR_Project::Throw_FatalError(const string& sDetails)
//=============================================================================*
{
    throw BAR_Error(ERR_0500, L"fatal error", ToWS(sDetails));
}

//=============================================================================*
void BAR_Project::Throw_InvalidPassword()
//=============================================================================*
{
    throw BAR_Error(ERR_0501, L"invalid password");
}

//=============================================================================*
void BAR_Project::Throw_ProjectDirExistsAlready(const string& sProjectDir)
//=============================================================================*
{
    throw BAR_Error(ERR_0502, L"project directory exists already",
                              wstring(L"dir=") + ToWS(sProjectDir));
}

//=============================================================================*
void BAR_Project::Throw_InvalidArchiveType(const string& sArcType)
//=============================================================================*
{
    throw BAR_Error(ERR_0503, L"invalid archive format",
                              wstring(L"format=") + ToWS(sArcType) +
                              wstring(L", expected=") + ToWS(m_sArcType)
                              );
}
//=============================================================================*
void BAR_Project::Throw_CanNotBackupToControl(const string& sRemoteHost,
                                              const string& sTargetDir)
//=============================================================================*
{
    throw BAR_Error(ERR_0504, L"can not store archive on control",
                              wstring(L"no shared directory ") + ToWS(sTargetDir)
                              );
}
//=============================================================================*
void BAR_Project::Throw_CanNotRestoreFromControl(const string& sRemoteHost,
                                                 const string& sTargetDir)
//=============================================================================*
{
    throw BAR_Error(ERR_0505, L"can not restore archive on control",
                              wstring(L"no shared directory ") + ToWS(sTargetDir)
                              );
}

//=============================================================================*
void BAR_Project::Throw_ProjectDirNotFound(const string& sProjectDir)
//=============================================================================*
{
    throw BAR_Error(ERR_0506, L"project directory not found",
                              wstring(L"dir=") + ToWS(sProjectDir));
}

//=============================================================================*
void BAR_Project::Throw_InvalidEncryptionPassword()
//=============================================================================*
{
    throw BAR_Error(ERR_0507, L"wrong encryption password");
}


/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Project.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 21.04.04   Time: 14:40
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 21.04.04   Time: 13:35
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * restore must not create decrypted archive
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 21.04.04   Time: 13:19
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * added encrypt
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
 * User: Kt           Date: 10.07.01   Time: 16:36
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Project.cpp $
