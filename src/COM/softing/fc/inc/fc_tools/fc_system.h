/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_system.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_system.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_system.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
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
 *  22.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_SYSTEM_H_
#define FC_SYSTEM_H_

#include <tchar.h>
#include "fc_defines.h"

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------*
/**
 * FC_System executes a command line and waitsfor completion.
 * It does not pass the command line to the shell (line ANSI system)
 * It directly uses win32 CreateProcess (it's done this way because
 * the shell does not understand UNC pathes on all systems. 
 * Note: because the shell is bypassed you cannot use redirection > 
 * or pipe | in the command line.
 * @param    pszEXE       : pszEXE is the (best: full) exe path
 *                          to be executed. Note: pszCMDL must not contain the 
 *                          exe path.  If pszEXE may contain blanks, this 
 *                          is handeled by FC_System.
 * @param    pszCMDL      : If not NULL the command line parameters. 
 *                          Note: pszCMDL must not contain the  exe path.
 * @param    pszInitalDir : if not NULL the initial directory of the process
 * @param    pExitCode    : if not NULL and CreateProcess succeeded,
 *                          pExitCode receives the process exit code.
 * @return                : false if CreateProcess failed, true if succeeded.
 *                          if false is returned use GetLastError for more info.
 *                          
*/
int FC_System(
    const TCHAR*  pszEXE, 
    const TCHAR*  pszCMDL, 
    const TCHAR*  pszInitalDir,
    long*         pExitCode
);


#ifdef __cplusplus
}
#endif

#endif //FC_SYSTEM_H_



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_system.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 22.01.01   Time: 10:56
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 *==
 *----------------------------------------------------------------------------*
*/
