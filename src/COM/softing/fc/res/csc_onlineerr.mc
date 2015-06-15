;/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/csc_onlineerr.mc 1     28.02.07 19:01 Ln $
;----------------------------------------------------------------------------*
;
; =FILENAME            $Workfile: csc_onlineerr.mc $
;                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/csc_onlineerr.mc $
;
; =PROJECT             CAK1020 ATCMControlV2.0
;
; =SWKE                4CSC
;
; =COMPONENT           Error/Messages common
;
; =CURRENT      $Date: 28.02.07 19:01 $
;               $Revision: 1 $
;
; =REFERENCES
;
;----------------------------------------------------------------------------*
;----------------------------------------------------------------------------*
; =DESCRIPTION
;    @DESCRIPTION@
;==
;----------------------------------------------------------------------------*
; =MODIFICATION
;  19.03.2001  SU      File created
;  see history at end of file !
;==
;******************************************************************************
;H<<*/
;
;#ifndef _CSC_ONLINE_ERR_H
;#define _CSC_ONLINE_ERR_H
;
MessageIdTypedef=HRESULT

;//standard setting for severity:
SeverityNames=(
               Success=0x0
               Informational=0x1
               Warning=0x2
               Error=0x3
              )

;//predefined system wide facility codes (do not change!). You may
;//insert additional facility codes between the main areas:
FacilityNames=(
       CSC          =0x220 : E_FACILITY_SYSCOM
	   TARGET		=0xa30 : E_FACILITY_TARGET
              )

;//currently supported languages:
;//     0x409   English USA
;//     0x407   German
LanguageNames=(
               English=0x409:CSC_OnlineErrENU
               German=0x407:CSC_OnlineErrDEU
              )

OutputBase=16

;
;//-----------------------------------------------------------------------------
;// CSC_Online Error messages
;//-----------------------------------------------------------------------------
;
;
;//==========================================================================================
;//------------------------------------------------------------------------------------------
;// CSC_Online Server messages in range 1000 - 1999
;//------------------------------------------------------------------------------------------
;//
;//
;// %1      error number as integer; for all error and warning messages!
;//         %1 is not present for all informational messages
;//
;// %2 to %4    inserts, must always be a string
;//
;//
;//
;//*****    Error messages
MessageId = 1001
Severity = error
Facility = CSC
SymbolicName=E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED
Language=English
error SC%1!d!: Only a single download session at the same time allowed to 4C control
.
Language=German
Fehler SC%1!d!: Nur eine Download Verbindung zu Control erlaubt
.
;//----

MessageId = 1002
Severity = error
Facility = CSC
SymbolicName=E_CSC_NO_VALID_LICENSE_INFO
Language=English
error SC%1!d!: Cannot access license info for CSC_Online server
.
Language=German
error SC%1!d!: Cannot access license info for CSC_Online server
.
;//----

MessageId = 1003
Severity = error
Facility = CSC
SymbolicName=E_CSC_NO_REMOTE_DLOAD_FOR_BUNDLE_ALLOWED
Language=English
error SC%1!d!: This bundled license allows no remote download
.
Language=German
error SC%1!d!: This bundled license allows no remote download
.
;//----

MessageId = 1004
Severity = error
Facility = CSC
SymbolicName=E_CSC_NO_FTP_SESSION
Language=English
error SC%1!d!: Connection to Web server for transfer of visualisation domains failed
.
Language=German
error SC%1!d!: Connection to Web server for transfer of visualisation domains failed
.
;//----

MessageId = 1005
Severity = error
Facility = CSC
SymbolicName=E_CSC_NO_CONNECT_TO_CONTROL
Language=English
error SC%1!d!: COM Connection to control failed
.
Language=German
error SC%1!d!: COM Connection to control failed
.
;//----

MessageId = 1006
Severity = error
Facility = CSC
SymbolicName=E_CSC_NO_DEBUG_SESSION_TO_CONTROL
Language=English
error SC%1!d!: Cannot establish a debug connection to control
.
Language=German
error SC%1!d!: Cannot establish a debug connection to control
.
;//----

MessageId = 1007
Severity = error
Facility = CSC
SymbolicName=E_CSC_BAD_SECURITY_FILE_SYNTAX
Language=English
error SC%1!d!: Bad security file syntax
.
Language=German
error SC%1!d!: Bad security file syntax
.
;//----

MessageId = 1008
Severity = Informational
Facility = CSC
SymbolicName=S_CSC_COMM_SESSION_LOGOUT_FORCED
Language=English
warning SC%1!d!: User %2 from engineering %3 forced disconnect of debugger
.
Language=German
warning SC%1!d!: User %2 from engineering %3 forced disconnect of debugger
.
;//----

MessageId = 1009
Severity = error
Facility = CSC
SymbolicName=E_CSC_CONTROL_NOT_RUNNING
Language=English
fatal error SC%1!d!: 4C Control is not running
.
Language=German
fatal error SC%1!d!: 4C Control is not running
.
;//----

MessageId = 1010
Severity = error
Facility = CSC
SymbolicName=E_CSC_RESOURCE_CLEARED
Language=English
fatal error SC%1!d!: The resource is cleared
.
Language=German
fatal error SC%1!d!: Die Resource ist gelöscht
.
;//----

;//==========================================================================================
;//------------------------------------------------------------------------------------------
;// Target specific messages
;//------------------------------------------------------------------------------------------
;

MessageId = 0001
Facility= TARGET
Severity = error
SymbolicName=E_TARGET_UNKNOWN
Language=English
error T0001: Unknown target error
.
Language=German
Fehler T0001: Unbekannter Target Fehler
.
;//----


;//==========================================================================================
;//------------------------------------------------------------------------------------------
;// CSC_Online ActiveX-Control messages in range 2000 - 2999
;//------------------------------------------------------------------------------------------
;//
;//
;// %1      error number as integer; for all error and warning messages!
;//         %1 is not present for all informational messages
;//
;// %2 to %4    inserts, must always be a string
;//
;//
;//
;//*****    Error messages
MessageID=2000
Facility = CSC
Severity = error
SymbolicName=E_CSC_SESSION_BAD_ADDRESS
Language=English
error SC2000: Connection address string format wrong
.
Language=German
Fehler SC2000: Format der Verbindungsadresse falsch
.

MessageID=2001
Facility = CSC
Severity = error
SymbolicName=E_CSC_SESSION_NOT_CONNECTED
Language=English
error SC2001: No connection to target.
.
Language=German
Fehler SC2001: Keine Verbindung zum Zielsystem.
.

MessageID=2002
Facility = CSC
Severity = error
SymbolicName=E_CSC_REQUEST_TIMEOUT
Language=English
error SC2002: %1 command request timed out.
.
Language=German
Fehler SC2002: Zeitüberschreitung bei Kommando Anforderung für Kommando %1.
.

;
;//*****    Informational messages
;//----
MessageID=2020
Severity=informational
Facility = CSC
SymbolicName=S_CSC_VAR_NOT_CONNECTED
Language=English
warning SC%1!d!: Variable was added but is not yet connected
.
Language=German
Warnung SC%1!d!: Variable hinzugefügt, noch keine Verbindung zu Konfiguration
.
;//----

;
;#endif // _CSC_ONLINE_ERR_H
;
;/*
; *----------------------------------------------------------------------------*
; *  $History: csc_onlineerr.mc $
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.02.07   Time: 19:01
; * Created in $/4CReleased/V2.20.00/COM/softing/fc/res
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.08.03   Time: 16:34
; * Created in $/4Control/COM/softing/fc/res
; * 
; * *****************  Version 4  *****************
; * User: Su           Date: 27.03.02   Time: 19:38
; * Updated in $/4Control/COM/softing/fc/res
; * 
; * *****************  Version 3  *****************
; * User: Su           Date: 10.07.01   Time: 12:12
; * Updated in $/4Control/COM/softing/fc/res
; * 
; * *****************  Version 2  *****************
; * User: Su           Date: 19.03.01   Time: 19:03
; * Updated in $/4Control/COM/softing/fc/res
; * Adapted header, checked facility code and visualization of error
; * messages.
; * 
; * *****************  Version 1  *****************
; * User: Su           Date: 14.03.01   Time: 19:49
; * Created in $/4Control/COM/softing/fc/res
; * 
; *==
; *----------------------------------------------------------------------------*
;*/



