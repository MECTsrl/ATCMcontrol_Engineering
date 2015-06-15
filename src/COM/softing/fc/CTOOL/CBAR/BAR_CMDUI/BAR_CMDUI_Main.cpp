/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Main.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CMDUI_Main.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI/BAR_CMDUI_Main.cpp $
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



//=== c/c++ standard includes
#include <iostream>
#include <signal.h>

//=== 4c standard includes
#include "BuildNr.h"
#include "Product.h"

//=== BAR includes
#include "BAR_Trace.hpp"
#include "BAR_CMDUI_Args.hpp"
#include "BAR_CMDUI_Callback.hpp"
#include "BAR_Error.hpp"
#include "BAR_Config.hpp"
#include "BAR_Project.hpp"

//=== only for testing
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/utime.h>

//=== global data
BAR_CMDUI_Callback g_callback(cout); //=== contains a flag, indicating  if user 
                                     //=== requested cancelling

//=============================================================================*
/**
 *	<B>signal handler</B>.
 *	Description = sets cancel flag to true if received a SIGINT
 *	@param      sig: the received signal
 *  @return     void
 *	@exception  none
 *  @see        
 */
//=============================================================================*
void sighandler(int sig)
{
    if ( sig == SIGINT )
        g_callback.RequestCancel();
}

//=============================================================================*
/**
 *	<B>Writes the 4CBAR logo to the specified output stream</B>.
 *	Description = 
 *	@param      out: output stream to write on
 *  @return     void
 *	@exception  none
 *  @see        
 */
//=============================================================================*
void ShowLogo(ostream& out = cout)
{
    string sCopyRight(PROD_COPYRIGHT);
    sCopyRight.replace(sCopyRight.find("\251"), 1, "(C)");
    out << PROD_PRODUCTNAME << " " << PROD_FULL_VERSION_STR
        << " Backup and Restore Tool" << endl
        <<  sCopyRight << endl << endl;
}

//=============================================================================*
/**
 *	<B>main function</B>.
 *	Description = installs a signal handler to handle user interrups via CTRL-C
 *                passes the command line arguments to a parser object
 *                calls the apropriate subfunctions of BAR, depending on the
 *                passed arguments
 *                handles errors
 *	@param      argc: number of passed command line arguments
 *  @param      argv: array with arguments
 *  @return      0: no errors occured
 *              -1: unexpected error occurred
 *              >1: named error occurred, see ERR_XXX enums in hpp files
 *	@exception  none
 *  @see        
 */
//=============================================================================*
int main(int argc, char **argv)
{ 
    try
    {
        //=== set signalhandler
        ::signal(SIGINT, sighandler);

        //=== parse command line options
        BAR_CMDUI_Args args(argc, argv);

        //=== show logo and copyright text
        if ( args.IsLogoOn() )
        {
            ShowLogo();
        }

        //=== set trace level
        BAR_Trace::SetTraceLevel(args.GetTraceLevel());
        BAR_TRACE1(1, "tracelevel=%d", args.GetTraceLevel());

        //=== set overwrite mode
        g_callback.SetOverwriteMode(args.IsOverwriteModeOn());
        g_callback.SetStdLibOverwriteMode(args.IsStdLibOverwriteModeOn());
            
        //=== show help text only
        if ( args.IsHelpRequested() )
        {
            BAR_CMDUI_Args::PrintHelp(cout);
        }
        //=== backup a configuration
        else if ( args.IsConfigBackup() )
        {
            cout << "backing up configuration "         << endl;
            cout << "from control " << args.GetConfigHost()     << endl;
            cout << "to archive "   << args.GetConfigArcFile()  << endl;
            BAR_Config::DoBackup(args.GetConfigHost(),
                                 args.GetPassword(),
                                 args.GetConfigArcFile(),
                                 g_callback);
        }
        //=== restore a configuration
        else if ( args.IsConfigRestore() )
        {
            cout << "restoring configuration "         << endl;
            cout << "from archive " << args.GetConfigArcFile() << endl;
            cout << "to control "   << args.GetConfigHost()    << endl;
            BAR_Config::DoRestore(args.GetConfigHost(),
                                  args.GetPassword(),
                                  args.GetConfigArcFile(),
                                  g_callback);
        }
        //=== backup a project
        else if ( args.IsProjectBackup() )
        {            
            cout << "backing up project" << endl;
            cout << "from directory " << args.GetProjectDir() << endl;

            if ( args.IsProjectArchiveInFile() )
            {
                cout << "to archive " << args.GetProjectArcFile() << endl;

                BAR_Project::DoBackupToFile(args.GetProjectDir(),
                                            args.GetPassword(),
                                            args.GetEncryptKey(),
                                            args.GetProjectArcFile(),
                                            g_callback,
                                            args.IsModeAll());
                    
            }
            else
            {
                 cout << "to control " << args.GetProjectControl() << endl;

                 BAR_Project::DoBackupToControl(args.GetProjectDir(),
                                               args.GetPassword(),
                                               args.GetEncryptKey(),
                                               args.GetProjectControl(),
                                               g_callback);
            }
        }
        //=== restore a project
        else if ( args.IsProjectRestore() )
        {
            cout << "restoring project" << endl;

            if ( args.IsProjectArchiveInFile() )
            {
                cout << "from archive " << args.GetProjectArcFile() << endl;
                cout << "to directory " << args.GetProjectDir() << endl;

                BAR_Project::DoRestoreFromFile(args.GetProjectDir(),
                                               args.GetPassword(),
                                               args.GetEncryptKey(),
                                               args.GetProjectArcFile(),
                                               g_callback);
                                              
            }
            else
            {
                cout << "from control " << args.GetProjectControl() << endl;
                cout << "to directory " << args.GetProjectDir() << endl;

                BAR_Project::DoRestoreFromControl(args.GetProjectDir(),
                                                  args.GetPassword(),
                                                  args.GetEncryptKey(),
                                                  args.GetProjectControl(),
                                                  g_callback);
            }
        }
        //=== should never happen, but for defensive reasons ...
        else
        {
            BAR_CMDUI_Args::PrintHelp(cout);
        }
    }
    catch(BAR_Error err)
    {
        wcout << err << endl;
        BAR_CMDUI_Args::PrintHelpNote(cout);
        return err.GetErrCode();
    }
    catch(...)
    {
        wcout << BAR_Error(0, L"fatal error", L"unhandled exception caught") << endl;
        return -1;
    }
    
    cout << "ok" << endl;
    return 0;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CMDUI_Main.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 21.04.04   Time: 14:40
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 21.04.04   Time: 14:32
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 21.04.04   Time: 14:32
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI
 * return value in error case
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
 * User: Kt           Date: 10.07.01   Time: 16:12
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_CMDUI
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/
