/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_system.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_system.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_system.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
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
 *==Implementation of OS level helper routins
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  22.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


//----  Includes:   -------------------------------------------*
#include <windows.h>
#include <malloc.h>
#include "fc_tools\fc_system.h"



int FC_System(
    const TCHAR*  pszEXE, 
    const TCHAR*  pszCMDL, 
    const TCHAR*  pszInitalDir,
    long*         pExitCode
)
{
    STARTUPINFO         si;
    PROCESS_INFORMATION pi;
    LPTSTR              pszTmp;
    int                 iBytes;
    DWORD               dwExitCode = (DWORD)-1;
    memset(&si,0,sizeof(si));
    si.cb = sizeof(si);

    iBytes = sizeof(TCHAR);//space for the 0

    if(pszEXE)//extra space pszEXE and two " and one blank:
        iBytes += (_tcslen(pszEXE)+2+1)*sizeof(TCHAR);

    if(pszCMDL)
        iBytes += _tcslen(pszCMDL)*sizeof(TCHAR);
    
    pszTmp = (TCHAR*)alloca(iBytes);
    pszTmp[0]=0;
    if(pszEXE)
    {
        _tcscat(pszTmp,_T("\""));
        _tcscat(pszTmp, pszEXE);
        _tcscat(pszTmp,_T("\" "));
    }
    if(pszCMDL)
        _tcscat(pszTmp, pszCMDL);

    //param #6 CREATE_DEFAULT_ERROR_MODE: we want to have 
    //a message box if a DLL is missing.
    if (CreateProcess(NULL, pszTmp, NULL, NULL, TRUE,
                     CREATE_DEFAULT_ERROR_MODE, NULL, pszInitalDir, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &dwExitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        return FALSE;
    }
  
    if(pExitCode)
        *pExitCode = (long)dwExitCode;

    return TRUE;
}




/*
 *----------------------------------------------------------------------------*
 *  $History: fc_system.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.01.02   Time: 16:44
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * FC_System: use CREATE_DEFAULT_ERROR_MODE 
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 22.01.01   Time: 11:03
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 *==
 *----------------------------------------------------------------------------*
*/
