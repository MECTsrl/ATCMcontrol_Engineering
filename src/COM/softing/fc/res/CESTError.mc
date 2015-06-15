;/*++
;H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/CESTError.mc 1     28.02.07 19:01 Ln $
;----------------------------------------------------------------------------*
;
; =FILENAME            $Workfile: CESTError.mc $
;                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/CESTError.mc $
;
; =PROJECT             CAK1020  ATCMControl V2.0
;
; =SWKE                4CE
;
; =COMPONENT           STEDITOR
;
; =CURRENT             $Date: 28.02.07 19:01 $
;                      $Revision: 1 $
;
; =REFERENCES
;==
;----------------------------------------------------------------------------*
;----------------------------------------------------------------------------*
; =DESCRIPTION
; Error codes and messages for the ST editor component
;== 
;----------------------------------------------------------------------------*
; =MODIFICATIONS
; $History: CESTError.mc $
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.02.07   Time: 19:01
; * Created in $/4CReleased/V2.20.00/COM/softing/fc/res
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.08.03   Time: 16:34
; * Created in $/4Control/COM/softing/fc/res
; * 
; * *****************  Version 2  *****************
; * User: Scd          Date: 31.01.01   Time: 14:02
; * Updated in $/4Control/COM/softing/fc/res
; * new headers added
; * 
;==
;*****************************************************************************
;H<<
;--*/

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
           CE_ST = 0x288 : E_FACILITY_CEST
            )

;//currently supported languages:
LanguageNames=(
               English=0x409:CESTMessagesENU
               German=0x407:CESTMessagesDEU
              )
OutputBase=16

;//////////ERROR MESSAGES/////////////////////////////////////////////
MessageId= 1
Severity = Warning
Facility = CE_ST
SymbolicName = E_CE_ST_NO_VALID_BREAKPOINT_POSITION

Language=English
STEditor: warning S3001: No valid breakpoint position in visible part of source
.
Language=German
STEditor: Warnung S3001: Keine gültige Breakpoint-Position im sichtbaren Teil der Quelle
.
;//---------------

MessageId= 2
Severity = Warning
Facility = CE_ST
SymbolicName = E_CE_ST_CANNOT_SET_BREAKPOINT

Language=English
STEditor: warning S3002: Failed to set breakpoint
.
Language=German
STEditor: Warnung S3002: Breakpoint konnte nicht gesetzt werden
.
;//---------------

MessageId= 3
Severity = Warning
Facility = CE_ST
SymbolicName = E_CE_ST_CANNOT_CLEAR_BREAKPOINT

Language=English
STEditor: warning S3003: Failed to remove breakpoint
.
Language=German
STEditor: Warnung S3003: Breakpoint konnte nicht entfernt werden
.
;//---------------

MessageId= 4
Severity = Warning
Facility = CE_ST
SymbolicName = E_CE_ST_CANNOT_ACTIVATE_BREAKPOINT

Language=English
STEditor: warning S3004: Failed to enable breakpoint
.
Language=German
STEditor: Warnung S3004: Breakpoint konnte nicht aktiviert werden
.
;//---------------

MessageId= 5
Severity = Warning
Facility = CE_ST
SymbolicName = E_CE_ST_CANNOT_DEACTIVATE_BREAKPOINT

Language=English
STEditor: warning S3005: Failed to disable breakpoint
.
Language=German
STEditor: Warnung S3005: Breakpoint konnte nicht deaktiviert werden
.
;//---------------

MessageId= 6
Severity = Warning
Facility = CE_ST
SymbolicName = E_CE_ST_INVALID_BREAKPOINT_POSITION

Language=English
STEditor: warning S3006: Invalid breakpoint position
.
Language=German
STEditor: Warnung S3006: Ungültige Breakpoint-Position
.
;//---------------
