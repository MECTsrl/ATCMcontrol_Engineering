/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Args.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CMDUI_Args.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Args.cpp $
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



//=== c++ standard includes
#include <cstdlib>

//=== windows includes

//==== specific includes
#include "BAR_CMDUI_Args.hpp"

//=============================================================================*
/**
 * <B>Constructor</B>
 * @param     argc: number of elements in argv
 * @param     argv: vector with command line arguments
 * @exception BAR_Error
 * @see       
 */
//=============================================================================*
BAR_CMDUI_Args::BAR_CMDUI_Args(int argc, char** argv)
: m_iTraceLevel(0),
  m_bHelp(false),
  m_bNoLogo(false),
  m_bOverwrite(false),
  m_bOvwStdLibs(false),
  m_sConfig(""),
  m_sBackup(""),
  m_sRestore(""),
  m_bFile(false),
  m_bControl(false),
  m_bAll(false),
  m_sPassword("")
{
    EvalArguments(argc, argv);
}


//=============================================================================*
/**
 * <B>Testing if an argument is an option</B>
 * Any string starting with a slash (/) or a dash(-)
 * is reckognized as an option
 * @param     sArg: argument to be tested
 * @return    true : sArg is an option
 *            false: sArg is not an option
 * @exception none
 * @see       
 */
//=============================================================================*
bool BAR_CMDUI_Args::IsOption(const string& sArg) const
{
    return ( sArg.find_first_of("/-") == 0 );
}

//=============================================================================*
/**
 * <B>Removing the option prefix from an argument</B>
 * If the specified argument is an option the first character will be removed
 * @param     sArg : reference to the argument to be modified
 * @return    void
 * @exception none
 * @see       
 */
//=============================================================================*
void BAR_CMDUI_Args::RemoveOptionPrefix(string& sArg) const
{
    if ( IsOption(sArg) )
        sArg.erase(0, 1);
}

//=============================================================================*
/**
 * <B>Evaluating command line arguments</B>
 * Parses the argument vector
 * @param     argc: length of argv
 * @param     argv: vector of arguments
 * @return    void
 * @exception none
 * @see       
 */
//=============================================================================*
void BAR_CMDUI_Args::EvalArguments(int argc, char** argv)
{
    //=== loop over all argv elements
    for(int i = 1, k = 0; i < argc; i++)
    {
        string sArg = argv[i];
        if ( ! IsOption(sArg) )
        {
            Throw_UnexpectedArg(sArg);
        }
        RemoveOptionPrefix(sArg);
        //=== -help, -?
        if ( sArg == "help" || sArg == "?" )
        {
            m_bHelp = true;
            continue;
        }
        //=== -nologo
        else if ( sArg == "nologo" )
        {
            m_bNoLogo = true;
        }
        //=== -trace <tracelevel>
        else if ( sArg == "trace" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

            m_iTraceLevel = atoi(argv[i]);
        }
        //=== -config <hostname>
        else if ( sArg == "config" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

            m_sConfig = argv[i];
        }
        //=== -project <dir>
        else if ( sArg == "project" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

            m_sProject= argv[i];
        }
        //=== -backup <arcfile>|<hostname>
        else if ( sArg == "backup" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

           m_sBackup= argv[i];
        }
        //=== -restore <arcfile>|<hostname>
        else if ( sArg == "restore" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

            m_sRestore= argv[i];
        }
        //=== -overwrite
        else if ( sArg == "overwrite" )
        {
            m_bOverwrite = true;
        }
//=== HACK: disabling "overwrite standard libs"
//=== DO NOT FORGET: to cleanup dead code belonging to this option
//        //=== -ovwstdlibs
//        else if ( sArg == "ovwstdlibs" )
//        {
//            m_bOvwStdLibs = true;
//        }
//=== END HACK

        //=== -control
        else if ( sArg == "control" )
        {
            m_bControl = true;
        }
        //=== -file
        else if ( sArg == "file" )
        {
            m_bFile = true;
        }
        //=== -password <text>
        else if ( sArg == "password" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

            m_sPassword= argv[i];
        }
        //=== -encrypt <key>
        else if ( sArg == "encrypt" )
        {
            if ( ++i >= argc    ) Throw_MissingArg(sArg);
            if ( IsOption(argv[i]) ) Throw_MissingArg(sArg);

            m_sEncryptKey = argv[i];
        }
        else if ( sArg == "all" )
        {
            m_bAll = true;
        }
        //=== error
        else
        {
            Throw_UnknownOption(sArg);
        }   
    } //=== end of for loop

    //=== check option consistence
    Check();
}

//=============================================================================*
/**
 * <B>Check options consistence</B>
 * @return    void
 * @exception BAR_Error
 * @see       
 */
//=============================================================================*
void BAR_CMDUI_Args::Check() const
{
    //=== if help is requested, ignore syntax
    if ( m_bHelp )
        return;
    
    //=== either -config or -project
    int j = 0;
    j += ( m_sConfig.empty()  ? 0 : 1);
    j += ( m_sProject.empty() ? 0 : 1);
    if ( j >= 2 )
        Throw_OptionSyntax("use only one out of -config, - project");
    if ( j <= 0 )
        Throw_OptionSyntax("you must use -config or -project");

    //=== either -backup or -restore
    j = 0;
    j += ( m_sBackup.empty() ? 0 : 1 );
    j += ( m_sRestore.empty() ? 0 : 1 );
    if ( j >= 2 )
    {
        Throw_OptionSyntax("use only one out of -backup, -restore");
    }
    if ( j <= 0 )
    {
        Throw_OptionSyntax("you must use -backup or -restore");
    }

    //=== with -config 
    if ( ! m_sConfig.empty() )
    {
        //=== no -file or -control
        if ( m_bFile || m_bControl )
        {
            Throw_OptionSyntax("use -file and -control only with -project");
        }
    }

    //=== with -project only one from -control or -file is allowed
    if ( ! m_sProject.empty() )
    {
        int j = 0;
        j += ( m_bFile    ? 1 : 0 );
        j += ( m_bControl ? 1 : 0 );
        if ( j >= 2 )
        {
            Throw_OptionSyntax("use only one out of -file, -control");
        }
        if ( j <= 0 )
        {
            Throw_OptionSyntax("you must use -file or -control");
        }
    }

    if (m_bAll)
    {
        if (!m_bFile || !IsProjectBackup())
        {
            Throw_OptionSyntax("use -all with -file and -backup only");
        }
    }
}

//=============================================================================*
/**
 * <B>Print note on how to display help text</B>
 * @param     out: target output stream
 * @return    reference to output stream
 * @exception none
 * @see       
 */
//=============================================================================*
ostream& BAR_CMDUI_Args::PrintHelpNote(ostream& out)
{
    return out << "use -? or -help to show help text" << endl;
}

//=============================================================================*
/**
 * <B>Print help text</B>
 * @param     out: target output stream
 * @return    reference to output stream
 * @exception none
 * @see       
 */
//=============================================================================*
ostream& BAR_CMDUI_Args::PrintHelp(ostream& out)
{
    out << "syntax variants:" << endl;
    out << endl;
    out << "for all target types:" << endl;
    out << " 4cbar -?|-help" << endl;
    out << " 4cbar -project <dir>  -backup  <arcfile> -file [-all]"    << endl;
    out << " 4cbar -project <dir>  -restore <arcfile> -file"    << endl;
    out << endl;
    out << "for pc based targets only:" << endl;
    out << " 4cbar -project <dir>  -backup  <host>    -control" << endl;
    out << " 4cbar -project <dir>  -restore <host>    -control" << endl;
    out << " 4cbar -config  <host> -backup  <arcfile>"          << endl;
    out << " 4cbar -config  <host> -restore <arcfile>"          << endl;
    out << endl;
    out << "additional options:" << endl;
    out << " -?, -help       : show this help text"         << endl;
    out << " -nologo         : suppress logo"  << endl;
    out << " -password <text>: password for an archive"     << endl;
    out << " -encrypt  <key> : encrypt key to use after backup / before restore" << endl;
    out << " -overwrite      : switch on overwrite mode"    << endl;
    out << " -all            : backup project gen folder, too" << endl;
//=== HACK: option "overwrite standard libs" disabled 
//    out << " -ovwstdlibs     : allows overwriting of libs under ATCMControl\\lib" << endl 
//        << "                   only together with option -project" << endl;
//=== END HACK
    return out;
}

//=============================================================================*
void BAR_CMDUI_Args::Throw_UnexpectedArg(const string& sArg) const
//=============================================================================*
{
    throw BAR_Error(ERR_0001,
                    L"unexpected argument",
                    ToWS("arg=") + ToWS(sArg));
 }

//=============================================================================*
void BAR_CMDUI_Args::Throw_UnknownOption(const string& sOpt) const
//=============================================================================*
{
    throw BAR_Error(ERR_0002,
                    L"unknown option",
                    ToWS("opt=-") + ToWS(sOpt));
}

//=============================================================================*
void BAR_CMDUI_Args::Throw_MissingArg(const string& sOpt="") const
//=============================================================================*
{ 
    wstring wsDetails = L"";
    if ( ! sOpt.empty() )
    {
        wsDetails += L"opt=-" + ToWS(sOpt);
    }
   throw BAR_Error(ERR_0003,
                   L"missing argument",
                   wsDetails);
}

//=============================================================================*
void BAR_CMDUI_Args::Throw_NotYetImplemented(const string& sFunc) const
//=============================================================================*
{
    throw BAR_Error(ERR_0004,
                    L"not yet implemented",
                    ToWS(sFunc));
 }

//=============================================================================*
void BAR_CMDUI_Args::Throw_OptionSyntax(const string& sDescr) const
//=============================================================================*
{
    throw BAR_Error(ERR_0005,
                    L"invalid option syntax",
                    ToWS(sDescr) );
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CMDUI_Args.cpp $
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
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 23.07.02   Time: 16:12
 * Updated in $/4Control/COM/softing/fc/AddOns/Softing/4CPCTarget/CBAR/BAR_CMDUI
 * Fixed some mistakes/changes in V2.0 and added flag -all to backup the
 * gen folder too.
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:10
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_CMDUI
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/
