;/*++
;H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/CEBPManError.mc 1     28.02.07 19:01 Ln $
;----------------------------------------------------------------------------*
;
; =FILENAME            $Workfile: CEBPManError.mc $
;                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/CEBPManError.mc $
;
; =PROJECT             CAK1020  ATCMControl V2.0
;
; =SWKE                4CE
;
; =COMPONENT           BPMAN
;
; =CURRENT             $Date: 28.02.07 19:01 $
;                      $Revision: 1 $
;
; =REFERENCES
;==
;----------------------------------------------------------------------------*
;----------------------------------------------------------------------------*
; =DESCRIPTION
; Error codes and messages for the breakpoint manager component (CEBPMAN)
;== 
;----------------------------------------------------------------------------*
; =MODIFICATIONS
; $History: CEBPManError.mc $
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
           CE_BPMAN = 0x286 : E_FACILITY_CEBPMAN
            )

;//currently supported languages:
LanguageNames=(
               English=0x409:CEBPMANMessagesENU
               German=0x407:CEBPMANMessagesDEU
              )
OutputBase=16

;//////////ERROR MESSAGES/////////////////////////////////////////////
MessageId= 1
Severity = Warning
Facility = CE_BPMAN
SymbolicName = E_CE_BPMAN_INVALID_BREAKPOINT_POSITION

Language=English
BPManager: warning S2001: Invalid breakpoint position
.
Language=German
BPManager: Warnung S2001: Ungültige Breakpoint-Position
.
;//---------------

MessageId= 2
Severity = Warning
Facility = CE_BPMAN
SymbolicName = E_CE_BPMAN_CANNOT_SUBSCRIBE_BREAKPOINT

Language=English
BPManager: warning S2002: Failed to set breakpoint at control
.
Language=German
BPManager: Warnung S2002: Breakpoint konnte auf dem Control nicht gesetzt werden
.
;//---------------

MessageId= 3
Severity = Warning
Facility = CE_BPMAN
SymbolicName = E_CE_BPMAN_CANNOT_UNSUBSCRIBE_BREAKPOINT

Language=English
BPManager: warning S2003: Failed to clear breakpoint at control
.
Language=German
BPManager: Warnung S2003: Breakpoint konnte auf dem Control nicht gelöscht werden
.
;//---------------
