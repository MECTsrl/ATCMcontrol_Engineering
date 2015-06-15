;/*++
;H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/CEMAINError.mc 1     28.02.07 19:01 Ln $
;----------------------------------------------------------------------------*
;
; =FILENAME            $Workfile: CEMAINError.mc $
;                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/CEMAINError.mc $
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
; Error codes and messages for 4CE CEMAIN component
;== FACILITY codes:
;== 0x280 - E_FACILITY_ENGINEERING
;----------------------------------------------------------------------------*
; =MODIFICATIONS
; $History: CEMAINError.mc $
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.02.07   Time: 19:01
; * Created in $/4CReleased/V2.20.00/COM/softing/fc/res
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.08.03   Time: 16:34
; * Created in $/4Control/COM/softing/fc/res
; * 
; * *****************  Version 3  *****************
; * User: Su           Date: 24.04.01   Time: 16:23
; * Updated in $/4Control/COM/softing/fc/res
; * Added facility CESYSTEM and system messages.
; * 
; * *****************  Version 2  *****************
; * User: Sis          Date: 30.03.01   Time: 9:07
; * Updated in $/4Control/COM/softing/fc/res
; * target onl
; * 
; * *****************  Version 1  *****************
; * User: Su           Date: 12.02.01   Time: 15:56
; * Created in $/4Control/COM/softing/fc/res
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
           CE = 0x280 : E_FACILITY_ENGINEERING
		   CESYSTEM = 0x285 : E_FACILITY_CESYSTEM
            )

;//currently supported languages:
LanguageNames=(
               English=0x409:CEMAINErrorENU
               German=0x407:CEMAINErrorDEU
              )
OutputBase=16

;//////////ERROR MESSAGES/////////////////////////////////////////////
MessageId= 0
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_COMPONENT_ALREADY_ADDED

Language=English
Engineering: error S0000: Component already added
.
Language=German
Engineering: Fehler S0000: Komponente bereits hinzugefügt
.
;//---------------

MessageId= 1
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_UNKNOWN_COMPONENT

Language=English
Engineering: error S0001: Unknown component
.
Language=German
Engineering: Error S0001: Unbekannte Komponente
.
;//---------------

MessageId= 2
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_COMPONENT_NOT_ALIVE

Language=English
Engineering: error S0002: Component not alive
.
Language=German
Engineering: Fehler S0002: Komponente lebt nicht
.
;//---------------

MessageId= 3
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_NOT_COM_COMPONENT

Language=English
Engineering: error S0003: Component is not a COM component
.
Language=German
Engineering: Fehler S0003: Die Komponente ist keine COM-Komponente
.
;//---------------

MessageId= 4
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_NOT_DOCKINGTOOL_COMPONENT

Language=English
Engineering: error S0004: Component is not a docking tool
.
Language=German
Engineering: Fehler S0004: Die Komponente ist kein Docking Tool
.
;//---------------

MessageId= 5
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_NOT_TARGETSET_COMPONENT

Language=English
Engineering: error S0005: Component is not a target set
.
Language=German
Engineering: Fehler S0005: Die Komponente ist keine Target-Menge
.
;//---------------

MessageId= 6
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_INVALID_DOCKINGTOOL_CLASS

Language=English
Engineering: error S0006: Invalid docking tool class
.
Language=German
Engineering: Fehler S0006: Ungültige Docking Tool Klasse
.
;//---------------

MessageId= 7
Severity = Error
Facility = CE
SymbolicName = E_CECOMPMAN_FAILED_TO_CREATE_DOCKINGTOOL_WINDOW

Language=English
Engineering: error S0007: Failed to create docking tool window
.
Language=German
Engineering: Fehler S0007: Docking Tool Fenster konnte nicht erzeugt werden
.
;//---------------

MessageId= 8
Severity = Error
Facility = CE
SymbolicName = E_CESTATEMAN_CALLBACK_ALREADY_REGISTERED

Language=English
Engineering: error S0008: State change callback already registered
.
Language=German
Engineering: Fehler S0008: Status Callback ist bereits registriert
.
;//---------------

MessageId= 9
Severity = Error
Facility = CE
SymbolicName = E_CESTATEMAN_CALLBACK_NOT_REGISTERED

Language=English
Engineering: error S0009: State change callback not registered
.
Language=German
Engineering: Fehler S0009: Status Callback ist nicht registriert
.
;//---------------


MessageId= 10
Severity = Error
Facility = CESYSTEM
SymbolicName = E_CESYSTEM_NO_TARGETDATA

Language=English
Engineering: error S0010: No target KAD data for target '%1' of target type '%2'
.
Language=German
Engineering: Fehler S0011: Keine target KAD Daten für das Target '%1' vom Targettyp '%2'
.

MessageId= 11
Severity = Error
Facility = CESYSTEM
SymbolicName = E_CESYSTEM_INV_DOWNLOADER_GUID

Language=English
Engineering: error S0011: Invalid GUID '%1' for downloader of target type '%2'
.
Language=German
Engineering: Fehler S0011: Ungültige GUID '%1' für die Downloader Komponente des Targettyp '%2'
.


;//---------------


