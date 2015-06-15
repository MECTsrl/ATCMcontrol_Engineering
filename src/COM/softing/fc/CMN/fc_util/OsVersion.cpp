/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/OsVersion.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: OsVersion.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/OsVersion.cpp $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *           $Revision: 1 $
 *
 * =ENVIRONMENT             Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V6.0 / MFC 6.0 / ATL 3.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0/6.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 *                http://www.4control.de

 

 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.02.99  Je      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



#include "stdafx.h"
#ifndef UTIL_IMP
#define UTIL_IMP    // declare as dllexports
#endif
#include "UtilIF.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//----  Local Prototypes:   ----------------------------------------*
static OS_ENV   QueryActualVersion(OUT DWORD & dwWindowsMajorVersion, OUT DWORD& dwWindowsMinorVersion, OUT DWORD& dwBuild);


//------------------------------------------------------------------*
/**
 * UTIL_GetOS_Version
 *  retrieves a enumerator that describes to Windows OS version currently running
 *
 * @param           void
 * @return          enumerator describing present OS
 * @exception       -
 * @see             
*/
UTILIF OS_ENV
    UTIL_GetOS_Version(void)
{
    DWORD   dwMajorVersion = 0;
    DWORD   dwMinorVersion = 0;
    DWORD   dwBuild = 0;
    return QueryActualVersion(dwMajorVersion, dwMinorVersion, dwBuild);
}



//------------------------------------------------------------------*
OS_ENV
    QueryActualVersion(OUT DWORD& dwWindowsMajorVersion,
                       OUT DWORD& dwWindowsMinorVersion,
                       OUT DWORD& dwBuild)
/*>>>> 
query version and set enumerator accordingly

Result
  void
<<<<*/
{
    DWORD   dwVersion = ::GetVersion();
    dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
    dwBuild = 0;

    OS_ENV env = unknown;

    // GetVersion returns different values when 
    // compiling as Win16 or Win32 app.
#ifdef _WIN32
    // compiling under Win32 ----------------------------------
    if (dwVersion < 0x80000000)
    {
        // must be Windows NT, decide which version:
        if (dwWindowsMajorVersion >= 0x04) {
            env = WindowsNT40;
        }
        else {
            if (dwWindowsMinorVersion >= 0x51) {
                env = WindowsNT351;
            }
            else {
                // older than NT 3.51 (=> has no common controls etc !)
                env = WindowsNT;
            }
        }
        dwBuild = (DWORD)(HIWORD(dwVersion));
    }
    else if (dwWindowsMajorVersion < 4)
    {
        // Win32s
        env = Win32s;
        dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
    }
    else
    {
        // Windows 95:
        env = Windows95;
        dwBuild =  0;   // Windows 95 -- No build numbers provided
    }

#else   // ! _WIN32
    // compiling under Win16 ----------------------------------
    if (dwWindowsMajorVersion < 3) {
        // must be groofy old!!!!!
        env = Windows2;
    }
    else {
        // which Windows 3 ??
        switch (dwWindowsMinorVersion) {
        case 0:
            env = Windows30;
            break;
        case 10:
            // looks like Win3.1,
            //  but is it real Win16 or is it running as WoW (under NT)
            if (::GetWinFlags() & 0x4000)
                // WIN_NT (0x4000) identifies Windows NT (Windows SDK KBase, Q131371)
                env = WindowsNT;
            else
                env = Windows31;
            break;
        case 11:
            // commonly not set, since WfW uses Win3.1 signature!
            env = WfW311;
            break;
        case 95:
            // running as Win16 under Win95:
            env = Windows95;
            break;
        default:
            env = Windows30;
            break;
        }
    }
#endif  // ! _WIN32

    return env;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: OsVersion.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:10
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * utils and baselib from V1.3
 * 
 * *****************  Version 1  *****************
 * User: Je           Date: 3.02.99    Time: 18:35
 * Created in $/4Control/COM/softing/fc/CMN/UTIL
 * * added OS version query
 *==
 *----------------------------------------------------------------------------*
*/

// 03.02.99  Generated by VC++6.0 VBScript '4C Macros.dsm' Version 1.3/Je
// ***************************  E O F  ******************************
// $Workfile: OsVersion.cpp $
