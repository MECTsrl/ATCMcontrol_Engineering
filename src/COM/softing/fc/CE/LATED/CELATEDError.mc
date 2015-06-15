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
		   CELATED = 0x28B : E_FACILITY_CELATED
            )

;//currently supported languages:
LanguageNames=(
               English=0x409:CELATEDErrorENU
               German=0x407:CELATEDErrorDEU
              )
OutputBase=16

;//////////ERROR MESSAGES/////////////////////////////////////////////
MessageId= 0
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_XML_ERR

Language=English
LATED: Could not load document due to the following XML error:%1.
.
Language=German
LATED: Dokument kann aufgrund des folgenden XML Fehlers nicht geladen werden:%1.
.
;//---------------

MessageId= 1
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_XML_ERR2

Language=English
fatal error LAT0001 Load error on line %1!d! found:'%2' expected:'%3'
.
Language=German
Fataler Fehler LAT0001: Lade Fehler auf Zeile %1!d! gefunden:'%2' erwartet:'%3'
.
;//---------------

MessageId= 2
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_NO_LINKS

Language=English
error LAT0002: No lateral communication links are defined
.
Language=German
Fehler LAT0002: Es wurden keine Verbindungen für die laterale Kommunikation definiert
.
;//---------------

MessageId= 3
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_NO_DATATYPES

Language=English
error LAT0003: No data types are defined
.
Language=German
Fehler LAT0003: Es wurden keine Datentypen für die laterale Kommunikation definiert
.
;//---------------

MessageId= 4
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_ASSIGNED_TYPE

Language=English
error LAT0004: No datatype is assigned to the lateral communication link with target '%1'
.
Language=German
Fehler LAT0004: Der Verbindung '%1' für die laterale Kommunikation wurde kein Datentyp zugeordnet
.
;//---------------

MessageId= 5
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_NO_DESTTARGET

Language=English
error LAT0005: No link destination is defined for lateral communication link (link source = '%1')
.
Language=German
Fehler LAT0005: Kein Ziel für laterale Verbindung definiert (Quelle '%1')
.
;//---------------

MessageId= 6
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_INVTARGET

Language=English
error LAT0006: Invalid target '%1' specified
.
Language=German
Fehler LAT0006: Ungültiges Zielsystem '%1'
.
;//---------------

MessageId= 7
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_NAMES_TOO_LONG

Language=English
error LAT0007: Names of datatypes or of the link source too long '%1'
.
Language=German
Fehler LAT0007: Die Namen der Datentypen oder der Quelle sind zu lang '%1'
.
;//---------------

MessageId= 8
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_UNSUPPORTED_TYPE_PC

Language=English
error LAT0008: Datatype '%1' contains unsupported members for PC target (INT, STRING)
.
Language=German
Fehler LAT0008: Der Datentyp '%1' enthält Komponenten, die vom PC Target nicht unterstützt werden (INT, STRING)
.
;//---------------

MessageId= 9
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_UNSUPPORTED_TYPE_EC

Language=English
error LAT0009: Datatype '%1' contains unsupported members for EC target
.
Language=German
Fehler LAT0009: Der Datentyp '%1' enthält Komponenten, die vom EC Target nicht unterstützt werden
.
;//---------------

MessageId= 10
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_LINK_ALREADY_USED

Language=English
error LAT0010: Link '%1' already used to send the same datatype
.
Language=German
Fehler LAT0010: Der Link '%1' wird bereits benutzt um den gleichen Datentyp zu senden
.
;//---------------

MessageId= 11
Severity = Error
Facility = CELATED
SymbolicName = E_CELATED_UNSUPPORTED_TYPE_CMZ

Language=English
error LAT0011: Datatype '%1' contains unsupported members for CMZ target
.
Language=German
Fehler LAT0011: Der Datentyp '%1' enthält Komponenten, die vom CMZ Target nicht unterstützt werden
.
;//---------------

;//////////WARNINGS///////////////////////////////////////////////////
MessageId= 50
Severity = Warning
Facility = CELATED
SymbolicName = W_CELATED_INVHOST

Language=English
warning LAT0050: Invalid host address for target '%1'
.
Language=German
Warnung LAT0050: Ungültige Host Adresse für Zielsystem '%1'
.
;//---------------

;//////////INFORMATIONAL//////////////////////////////////////////////
MessageId= 100
Severity = Informational
Facility = CELATED
SymbolicName = S_CELATED_LOAD

Language=English
LATED: Load document '%1'
.
Language=German
LATED: Lade Dokument '%1'
.
;//---------------

MessageId= 101
Severity = Informational
Facility = CELATED
SymbolicName = S_CELATED_GEN_INFO

Language=English
%1!d! error(s), %2!d! warning(s)
.
Language=German
%1!d! Fehler, %2!d! Warnungen
.
;//---------------

MessageId= 102
Severity = Informational
Facility = CELATED
SymbolicName = S_CELATED_CLOSE

Language=English
LATED: Close document '%1'
.
Language=German
LATED: Dokument '%1' wird geschlossen
.
;//---------------
