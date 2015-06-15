;/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/CEGRError.mc 1     28.02.07 19:01 Ln $
;*----------------------------------------------------------------------------*
;*
;* =FILENAME            $Workfile: CEGRError.mc $
;*   $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/CEGRError.mc $
;*
;* =PROJECT             CAK1020    ATCMControl V2.0
;*
;* =SWKE                CE
;*
;* =COMPONENT           GREDITOR
;*
;* =CURRENT      $Date: 28.02.07 19:01 $
;*               $Revision: 1 $
;*
;* =REFERENCES
;*
;*----------------------------------------------------------------------------*
;*----------------------------------------------------------------------------*
;* =DESCRIPTION
;*    
;*==
;*----------------------------------------------------------------------------*
;* =MODIFICATION
;*  20.02.01  HA/RW      File created
;*  see history at end of file !
;*==
;*******************************************************************************
;H<<*/

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
           CE_GR = 0x287 : E_FACILITY_CEGR
            )

;//currently supported languages:
LanguageNames=(
               English=0x409:CEGRMessagesENU
               German=0x407:CEGRMessagesDEU
              )
OutputBase=16

;//////////ERROR MESSAGES/////////////////////////////////////////////
MessageId= 1
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_NO_VALID_BREAKPOINT_POSITION

Language=English
Graphic Editor: warning S4001: No valid breakpoint position
.
Language=German
Graphischer Editor: Warnung S4001: Keine gültige Breakpoint-Position
.
;//---------------

MessageId= 2
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_CANNOT_SET_BREAKPOINT

Language=English
Graphic Editor: warning S4002: Failed to set breakpoint
.
Language=German
Graphischer Editor: Warnung S4002: Breakpoint konnte nicht gesetzt werden
.
;//---------------

MessageId= 3
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_CANNOT_CLEAR_BREAKPOINT

Language=English
Graphic Editor: warning S4003: Failed to remove breakpoint
.
Language=German
Graphischer Editor: Warnung S4003: Breakpoint konnte nicht entfernt werden
.
;//---------------

MessageId= 4
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_CANNOT_ACTIVATE_BREAKPOINT

Language=English
Graphic Editor: warning S4004: Failed to enable breakpoint
.
Language=German
Graphischer Editor: Warnung S4004: Breakpoint konnte nicht aktiviert werden
.
;//---------------

MessageId= 5
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_CANNOT_DEACTIVATE_BREAKPOINT

Language=English
Graphic Editor: warning S4005: Failed to disable breakpoint
.
Language=German
Graphischer Editor: Warnung S4005: Breakpoint konnte nicht deaktiviert werden
.
;//---------------

MessageId= 6
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_INVALID_BREAKPOINT_POSITION

Language=English
Graphic Editor: warning S4006: Invalid breakpoint position
.
Language=German
Graphischer Editor: Warnung S4006: Ungültige Breakpoint-Position
.
;//---------------

MessageId= 7
Severity = Warning
Facility = CE_GR
SymbolicName = E_CE_GR_INVALID_ELEMENT_POSTION

Language=English
Graphic Editor: warning S4007: Invalid element positions in source
.
Language=German
Graphischer Editor: Warnung S4007: Ungültige Element-Positionen in der Quelldatei
.
;//---------------

;/*
; *----------------------------------------------------------------------------*
; *  $History: CEGRError.mc $
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
; * User: Jd           Date: 4.03.03    Time: 14:01
; * Updated in $/4Control/COM/softing/fc/res
; * 
; * *****************  Version 1  *****************
; * User: Rw           Date: 23.02.01   Time: 11:27
; * Created in $/4Control/COM/softing/fc/res
; * initial version for V2.0
; * 
; *==
; *----------------------------------------------------------------------------*
;*/
