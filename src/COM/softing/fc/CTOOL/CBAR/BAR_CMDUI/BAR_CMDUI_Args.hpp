/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Args.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CMDUI_Args.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Args.hpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           4cbar
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

#ifndef _BAR_CMDUI_Args_H_
#define _BAR_CMDUI_Args_H_

//====  Aggregate Includes:   =================================================*
#include <string>
using namespace::std;

#include <windows.h>

#include "BAR_Error.hpp"
#include "BAR_Util.hpp"

//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_CMDUI_Args
 *  @see    ....
*/
//=============================================================================*
class BAR_CMDUI_Args
{
public: //=== con-, destruction and assignment
    BAR_CMDUI_Args(int argc, char** argv);

public: //=== access
    
    bool IsHelpRequested() const;
    bool IsLogoOn()        const;

    bool IsConfigBackup()   const;
    bool IsConfigRestore()  const;
    bool IsProjectBackup()  const;
    bool IsProjectRestore() const;

    bool IsProjectArchiveOnControl() const;
    bool IsProjectArchiveInFile()    const;

    bool IsOverwriteModeOn() const;
    bool IsStdLibOverwriteModeOn() const;

    bool IsModeAll() const;

    int GetTraceLevel() const;

    string GetPassword() const;
    string GetEncryptKey() const;

    string GetConfigHost()    const;
    string GetConfigArcFile() const;

    string GetProjectDir()     const;
    string GetProjectArcFile() const;
    string GetProjectControl() const;

    static ostream& PrintHelpNote(ostream& out);
    static ostream& PrintHelp    (ostream& out);

public: //=== operations
    void EvalArguments(int argc, char** argv);

public: //=== overrides

private: //=== hidden constructors and assignments

private: //=== helpers

    enum {
        ERR_0001 = 1,
        ERR_0002 = 2,
        ERR_0003 = 3,
        ERR_0004 = 4,
        ERR_0005 = 5
    };

    bool IsOption(const string& sArg) const;
    void RemoveOptionPrefix(string& sArg) const;
    
    void Throw_UnexpectedArg    (const string& sArg) const;
    void Throw_UnknownOption    (const string& sArg) const;
    void Throw_MissingArg       (const string& sArg) const;
    void Throw_NotYetImplemented(const string& sFunc) const;
    void Throw_OptionSyntax     (const string& sDescr) const;

    void Check() const;

private: //=== attributes

    int m_iTraceLevel;

    bool m_bHelp;
    bool m_bNoLogo;
    bool m_bOverwrite;
    bool m_bOvwStdLibs;

    bool m_bFile;
    bool m_bControl;
    bool m_bAll;

    string m_sConfig;
    string m_sProject;
    string m_sBackup;
    string m_sRestore;
    string m_sPassword;
    string m_sEncryptKey;
};

//=============================================================================*
inline bool BAR_CMDUI_Args::IsHelpRequested() const
//=============================================================================*
{ return m_bHelp; }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsLogoOn() const
//=============================================================================*
{ return ! m_bNoLogo; }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsConfigBackup() const
//=============================================================================*
{ return ( ( ! m_sConfig.empty() ) && ( ! m_sBackup.empty() ) ); }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsConfigRestore() const
//=============================================================================*
{ return ( ( ! m_sConfig.empty() ) && ( ! m_sRestore.empty() ) ); }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsProjectBackup() const
//=============================================================================*
{ return ( ( ! m_sProject.empty() ) && ( ! m_sBackup.empty() ) ); }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsProjectRestore() const
//=============================================================================*
{ return ( ( ! m_sProject.empty() ) && ( ! m_sRestore.empty() ) ); }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsProjectArchiveInFile() const
//=============================================================================*
{ return m_bFile; }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsProjectArchiveOnControl() const
//=============================================================================*
{ return m_bControl; }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsOverwriteModeOn() const
//=============================================================================*
{ return m_bOverwrite; }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsStdLibOverwriteModeOn() const
//=============================================================================*
{ return m_bOvwStdLibs; }

//=============================================================================*
inline int BAR_CMDUI_Args::GetTraceLevel() const
//=============================================================================*
{ return m_iTraceLevel; }

//=============================================================================*
inline string BAR_CMDUI_Args::GetPassword()const
//=============================================================================*
{ return m_sPassword; }

//=============================================================================*
inline string BAR_CMDUI_Args::GetEncryptKey()const
//=============================================================================*
{ return m_sEncryptKey; }

//=============================================================================*
inline string BAR_CMDUI_Args::GetConfigHost() const
//=============================================================================*
{ return m_sConfig; }

//=============================================================================*
inline string BAR_CMDUI_Args::GetConfigArcFile() const
//=============================================================================*
{ return ( m_sBackup.empty() ? m_sRestore : m_sBackup ); }

//=============================================================================*
inline string BAR_CMDUI_Args::GetProjectDir() const
//=============================================================================*
{ return m_sProject; }

//=============================================================================*
inline string BAR_CMDUI_Args::GetProjectArcFile() const
//=============================================================================*
{ return ( m_sBackup.empty() ? m_sRestore : m_sBackup ); }

//=============================================================================*
inline string BAR_CMDUI_Args::GetProjectControl() const
//=============================================================================*
{ return ( m_sBackup.empty() ? m_sRestore : m_sBackup ); }

//=============================================================================*
inline bool BAR_CMDUI_Args::IsModeAll() const
//=============================================================================*
{ return m_bAll; }

//=============================================================================*
#endif // _BAR_CMDUI_ARGS_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CMDUI_Args.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 21.04.04   Time: 13:19
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * added encrypt
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 23.07.02   Time: 16:12
 * Updated in $/4Control/COM/softing/fc/AddOns/Softing/4CPCTarget/CBAR/BAR_CMDUI
 * Fixed some mistakes/changes in V2.0 and added flag -all to backup the
 * gen folder too.
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:11
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_CMDUI
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_CMDUI_Args.hpp $
