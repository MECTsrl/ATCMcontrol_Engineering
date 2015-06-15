/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/csc_onlineerr.mc 1     28.02.07 19:01 Ln $
----------------------------------------------------------------------------*

 =FILENAME            $Workfile: csc_onlineerr.mc $
                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/csc_onlineerr.mc $

 =PROJECT             CAK1020 ATCMControlV2.0

 =SWKE                4CSC

 =COMPONENT           Error/Messages common

 =CURRENT      $Date: 28.02.07 19:01 $
               $Revision: 1 $

 =REFERENCES

----------------------------------------------------------------------------*
----------------------------------------------------------------------------*
 =DESCRIPTION
    @DESCRIPTION@
==
----------------------------------------------------------------------------*
 =MODIFICATION
  19.03.2001  SU      File created
  see history at end of file !
==
******************************************************************************
H<<*/

#ifndef _CSC_ONLINE_ERR_H
#define _CSC_ONLINE_ERR_H

//standard setting for severity:
//predefined system wide facility codes (do not change!). You may
//insert additional facility codes between the main areas:
//currently supported languages:
//     0x409   English USA
//     0x407   German

//-----------------------------------------------------------------------------
// CSC_Online Error messages
//-----------------------------------------------------------------------------


//==========================================================================================
//------------------------------------------------------------------------------------------
// CSC_Online Server messages in range 1000 - 1999
//------------------------------------------------------------------------------------------
//
//
// %1      error number as integer; for all error and warning messages!
//         %1 is not present for all informational messages
//
// %2 to %4    inserts, must always be a string
//
//
//
//*****    Error messages
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define E_FACILITY_TARGET                0xA30
#define E_FACILITY_SYSCOM                0x220


//
// Define the severity codes
//


//
// MessageId: E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED
//
// MessageText:
//
//  error SC%1!d!: Only a single download session at the same time allowed to 4C control
//
#define E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED ((HRESULT)0xC22003E9L)

//----
//
// MessageId: E_CSC_NO_VALID_LICENSE_INFO
//
// MessageText:
//
//  error SC%1!d!: Cannot access license info for CSC_Online server
//
#define E_CSC_NO_VALID_LICENSE_INFO      ((HRESULT)0xC22003EAL)

//----
//
// MessageId: E_CSC_NO_REMOTE_DLOAD_FOR_BUNDLE_ALLOWED
//
// MessageText:
//
//  error SC%1!d!: This bundled license allows no remote download
//
#define E_CSC_NO_REMOTE_DLOAD_FOR_BUNDLE_ALLOWED ((HRESULT)0xC22003EBL)

//----
//
// MessageId: E_CSC_NO_FTP_SESSION
//
// MessageText:
//
//  error SC%1!d!: Connection to Web server for transfer of visualisation domains failed
//
#define E_CSC_NO_FTP_SESSION             ((HRESULT)0xC22003ECL)

//----
//
// MessageId: E_CSC_NO_CONNECT_TO_CONTROL
//
// MessageText:
//
//  error SC%1!d!: COM Connection to control failed
//
#define E_CSC_NO_CONNECT_TO_CONTROL      ((HRESULT)0xC22003EDL)

//----
//
// MessageId: E_CSC_NO_DEBUG_SESSION_TO_CONTROL
//
// MessageText:
//
//  error SC%1!d!: Cannot establish a debug connection to control
//
#define E_CSC_NO_DEBUG_SESSION_TO_CONTROL ((HRESULT)0xC22003EEL)

//----
//
// MessageId: E_CSC_BAD_SECURITY_FILE_SYNTAX
//
// MessageText:
//
//  error SC%1!d!: Bad security file syntax
//
#define E_CSC_BAD_SECURITY_FILE_SYNTAX   ((HRESULT)0xC22003EFL)

//----
//
// MessageId: S_CSC_COMM_SESSION_LOGOUT_FORCED
//
// MessageText:
//
//  warning SC%1!d!: User %2 from engineering %3 forced disconnect of debugger
//
#define S_CSC_COMM_SESSION_LOGOUT_FORCED ((HRESULT)0x422003F0L)

//----
//
// MessageId: E_CSC_CONTROL_NOT_RUNNING
//
// MessageText:
//
//  fatal error SC%1!d!: 4C Control is not running
//
#define E_CSC_CONTROL_NOT_RUNNING        ((HRESULT)0xC22003F1L)

//----
//
// MessageId: E_CSC_RESOURCE_CLEARED
//
// MessageText:
//
//  fatal error SC%1!d!: The resource is cleared
//
#define E_CSC_RESOURCE_CLEARED           ((HRESULT)0xC22003F2L)

//----
//==========================================================================================
//------------------------------------------------------------------------------------------
// Target specific messages
//------------------------------------------------------------------------------------------

//
// MessageId: E_TARGET_UNKNOWN
//
// MessageText:
//
//  error T0001: Unknown target error
//
#define E_TARGET_UNKNOWN                 ((HRESULT)0xCA300001L)

//----
//==========================================================================================
//------------------------------------------------------------------------------------------
// CSC_Online ActiveX-Control messages in range 2000 - 2999
//------------------------------------------------------------------------------------------
//
//
// %1      error number as integer; for all error and warning messages!
//         %1 is not present for all informational messages
//
// %2 to %4    inserts, must always be a string
//
//
//
//*****    Error messages
//
// MessageId: E_CSC_SESSION_BAD_ADDRESS
//
// MessageText:
//
//  error SC2000: Connection address string format wrong
//
#define E_CSC_SESSION_BAD_ADDRESS        ((HRESULT)0xC22007D0L)

//
// MessageId: E_CSC_SESSION_NOT_CONNECTED
//
// MessageText:
//
//  error SC2001: No connection to target.
//
#define E_CSC_SESSION_NOT_CONNECTED      ((HRESULT)0xC22007D1L)

//
// MessageId: E_CSC_REQUEST_TIMEOUT
//
// MessageText:
//
//  error SC2002: %1 command request timed out.
//
#define E_CSC_REQUEST_TIMEOUT            ((HRESULT)0xC22007D2L)


//*****    Informational messages
//----
//
// MessageId: S_CSC_VAR_NOT_CONNECTED
//
// MessageText:
//
//  warning SC%1!d!: Variable was added but is not yet connected
//
#define S_CSC_VAR_NOT_CONNECTED          ((HRESULT)0x422007E4L)

//----

#endif // _CSC_ONLINE_ERR_H

/*
 *----------------------------------------------------------------------------*
 *  $History: csc_onlineerr.mc $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/res
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/res
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 27.03.02   Time: 19:38
 * Updated in $/4Control/COM/softing/fc/res
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 10.07.01   Time: 12:12
 * Updated in $/4Control/COM/softing/fc/res
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 19.03.01   Time: 19:03
 * Updated in $/4Control/COM/softing/fc/res
 * Adapted header, checked facility code and visualization of error
 * messages.
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 19:49
 * Created in $/4Control/COM/softing/fc/res
 * 
 *==
 *----------------------------------------------------------------------------*
*/
