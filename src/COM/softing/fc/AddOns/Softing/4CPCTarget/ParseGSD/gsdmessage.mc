;/*H>> *----------------------------------------------------------------------------*
;* =DESCRIPTION
;*    @DESCRIPTION@
;*== Errors,Warnings and informational messages issued by GSD parser
;*----------------------------------------------------------------------------*
;* =MODIFICATION
;*  02.05.2002  SU      File created
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
;//               CC    =0x200 : E_FACILITY_CONTROL
       PARSE_GSD    =0x214 : E_FACILITY_PARSE_GSD
              )

;//currently supported languages:
;//     0x409   English USA
;//     0x407   German
LanguageNames=(
               English=0x409:gsdmessageENU
               German=0x407:gsdmessageDEU
              )
OutputBase=16

;//		Messages with %0 on the end do not generate newline with FormatMessage.
;//		Error messages
MessageId = 1
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_FILE_NOT_FOUND
Language=english
error GSD0001: GSD file '%1' not found%0
.
Language=German
Fehler GSD0001: GSD Datei '%1' nicht gefunden%0
.
;//----

MessageId = 2
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_REVISION
Language=english
error GSD0002: Revision of GSD file = %1!d!, GSD Parser support only until GSD revision %2!d!%0
.
Language=German
Fehler GSD0002: Version der GSD Datei = %1!d!, GSD Parser unterstützt nur bis GSD Version %2!d!%0
.
;//----

MessageId = 3
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_TEXT_REF
Language=english
error GSD0003: Text reference %1!d! accessed but no such definition%0
.
Language=German
Fehler GSD0003: Zugriff auf nicht definierte Text-Reference %1!d!%0
.
;//----

MessageId = 4
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_YYERR_FMT1
Language=english
error GSD0004: %1(%2!d!) : %3 near '%4'%0
.
Language=German
Fehler GSD0004: %1(%2!d!) : %3 bei '%4'%0
.
;//----

MessageId = 5
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_YYERR_FMT2
Language=english
error GSD0005: %1 : %2%0
.
Language=German
Fehler GSD0005: %1 : %2%0
.
;//----

MessageId = 6
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_EOF_COMMENT
Language=english
error GSD0006: %1 : End of file in comment%0
.
Language=German
Fehler GSD0006: %1 : Dateiende im Kommentar erkannt%0
.
;//----

MessageId = 7
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_BAD_CHAR
Language=english
error GSD0007: %1(%2!d!) : Bad character '%3'%0
.
Language=German
Fehler GSD0007: %1(%2!d!) : Ungültiges Zeichen '%3' erkannt%0
.
;//----

MessageId = 8
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_MODREF_MISSING
Language=english
error GSD0008: Module '%1': module reference required as of GSD Revision 3%0
.
Language=German
Fehler GSD0008: Module '%1': Module Referenz ab GSD Revision 3 verbindlich%0
.
;//----

MessageId = 9
Facility = PARSE_GSD
Severity = error
SymbolicName = E_GSD_MODULE_ASSIGNMENT
Language=english
error GSD0009: Could not find module with reference '%1!d!' for assignment to slot '%2'%0
.
Language=German
Fehler GSD0009: Kann Modul mit Referenz '%1!d!' für Zuweisung zu Slot '%2' nicht finden%0
.
;//----


;//		Messages with %0 on the end do not generate newline with FormatMessage.
;//		Warning messages

MessageId = 100
Facility = PARSE_GSD
Severity = Warning
SymbolicName = W_GSD_SKIP_USERDEF
Language=english
warning GSD0100: %1 : Skipping a line with user definition '%2'%0
.
Language=German
Warnung GSD0100: %1 : Überspringe Zeile mit Anwenderdefinition '%2'%0
.
;//----

MessageId = 101
Facility = PARSE_GSD
Severity = Warning
SymbolicName = W_GSD_CONFORMANCE_CLASS
Language=english
warning GSD0101: %1 : Conformance class of the DP-Master(Class1) mandatory as of GSD Revision 3%0
.
Language=German
Warnung GSD0101: %1 : Conformance Klasse für DP-Master(Class1) ist ab GSD Revision 3 verbindlich%0
.
;//----

MessageId = 102
Facility = PARSE_GSD
Severity = Warning
SymbolicName = W_GSD_DPV1_CHECK01
Language=english
warning GSD0102: No DPV1-Slave specified but '%1' = true - invalid combination%0
.
Language=German
Warnung GSD0102: Kein DPV1-Slave angegeben aber '%1' gesetzt - ungültige Kombination%0
.
;//----

MessageId = 103
Facility = PARSE_GSD
Severity = Warning
SymbolicName = W_GSD_DPV1_CHECK02
Language=english
warning GSD0103: No C1 Read/Write support specified but '%1' is set - invalid combination%0
.
Language=German
Warnung GSD0103: Keine C1 Lese-/Schreibunterstützung aber '%1' gesetzt - ungültige Kombination%0
.
;//----

MessageId = 104
Facility = PARSE_GSD
Severity = Warning
SymbolicName = W_GSD_DPV1_CHECK03
Language=english
warning GSD0104: No C2 Read/Write support specified but '%1' is set - invalid combination%0
.
Language=German
Warnung GSD0104: Keine C2 Lese-/Schreibunterstützung aber '%1' gesetzt - ungültige Kombination%0
.
;//----

MessageId = 105
Facility = PARSE_GSD
Severity = Warning
SymbolicName = W_GSD_PRMTEXT_VALUE
Language=english
warning GSD0105: Value within PrmText parameter description out of range (value='%1!d!' description='%2')%0
.
Language=German
Warnung GSD0105: Wert innerhalb der PrmText Parameterbeschreibung außerhalb Wertebereich Wert='%1!d!' Beschreibung=description='%2'%0
.
;//----


;//		Messages with %0 on the end do not generate newline with FormatMessage.
;//		Informational

