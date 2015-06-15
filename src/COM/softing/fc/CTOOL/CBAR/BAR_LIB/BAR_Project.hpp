/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Project.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Project.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Project.hpp $
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

#ifndef _BAR_PROJECT_H_
#define _BAR_PROJECT_H_

//====  Aggregate Includes:   =================================================*
#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace ::std;

//====  Forward Class Definitions:   ==========================================*

class BAR_Callback;
class BAR_ArcBackup;
class BAR_ArcRestore;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*



//=============================================================================*
/**
 *  class BAR_Project
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_Project
{
public: //=== con-, destruction and assignment

public: //=== access

public: //=== operations
    
    static void DoBackupToFile(const string& sProjectDir, 
                               const string& sPassword,
                               const string& sEncryptKey,
                               const string& sArcFile,
                               BAR_Callback& callback,
                               const bool bAll);
    static void DoBackupToControl(const string& sProjectDir, 
                                  const string& sPassword,
                                  const string& sEncryptKey,
                                  const string& sRemoteHost,
                                  BAR_Callback& callback);
    static void DoRestoreFromFile(const string& sProjectDir,
                                  const string& sPassword,
                                  const string& sEncryptKey,
                                  const string& sArcFile,
                                  BAR_Callback& callback);
    static void DoRestoreFromControl(const string& sProjectDir, 
                                  const string& sPassword,
                                  const string& sEncryptKey,
                                  const string& sRemoteHost,
                                  BAR_Callback& callback);
public: //=== overrides

private: //=== helpers

    static string GetFullPath(const string& sPath);
    static string GetFirstProjectFile(const string& sDir);
    static void CollectProjectFiles(BAR_Callback& callback,
                                    vector<string>& vecProjectFiles,
                                    const string& sProjectDir,
                                    const bool bAll,
                                    const string& sRelDir = ""
                                    );
    static void CollectExternalLibs(BAR_Callback& callback,
                                    const string& sProjectDir,
                                    vector<string>& vecProjectFiles);
    static enum {
        ERR_0500 = 500,
        ERR_0501 = 501,
        ERR_0502 = 502,
        ERR_0503 = 503,
        ERR_0504 = 504,
        ERR_0505 = 505,
        ERR_0506 = 506,
        ERR_0507 = 507
    };

    static void Throw_FatalError(const string& sDetails);
    static void Throw_InvalidPassword();
    static void Throw_ProjectDirExistsAlready(const string& sProjectDir);
    static void Throw_InvalidArchiveType(const string& sArcType);
    static void Throw_CanNotBackupToControl(const string& sRemoteHost,
                                            const string& sTargetDir);
    static void Throw_CanNotRestoreFromControl(const string& sRemoteHost,
                                               const string& sTargetDir);
    static void Throw_ProjectDirNotFound(const string& sProjectDir);
    static void Throw_InvalidEncryptionPassword();

    static const string m_sArcType;
    static const string m_sArcType10;
};

#endif // _BAR_PROJECT_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Project.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 21.04.04   Time: 13:20
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * added encrypt
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
 * User: Kt           Date: 10.07.01   Time: 16:37
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Project.hpp $
