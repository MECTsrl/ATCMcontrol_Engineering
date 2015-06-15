;//standard setting for severity:
SeverityNames=(
               Success=0x0
               Informational=0x1
               Warning=0x2
               Error=0x3
              )


;// TODO: Update facility values
;//predefined system wide facility codes (do not change!). You may
;//insert additional facility codes between the main areas:
FacilityNames=(
;//               CC    =0x200 : E_FACILITY_CONTROL
;//	   IO_MANAG     =0x210 : E_FACILITY_IO_MANAG
;//	   IO_DP        =0x211 : E_FACILITY_IO_DP
;//	   IO_OPC       =0x212 : E_FACILITY_IO_OPC
;//    IO_FF        =0x213 : E_FACILITY_IO_FF
;//	   IO_WAGO		=0x216 : E_FACILITY_IO_WAGO
;//               CSC   =0x220 : E_FACILITY_SYSCOM
;//               CCG   =0x230 : E_FACILITY_CODEGEN
;//               CLIB  =0x260 : E_FACILITY_LIBRARIAN
;//               CSET  =0x270 : E_FACILITY_SETUP
;//               CE    =0x280 : E_FACILITY_ENGENEERING
;//               CE_Wiz=0x2A0 : E_FACILITY_WIZARD
;//               BL    =0x2C0 : E_FACILITY_BASELIB
CE_PROJMAN = 0x282 : E_FACILITY_PRJMAN	
              )



;//currently supported languages:
LanguageNames=(
               English=0x409:CEProjManMessagesENU
               German=0x407:CEProjManMessagesDEU
              )

MessageIdTypedef=HRESULT
OutputBase=16



;//---------------
MessageId= 0
Severity = Informational
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_OPEN_PROJECT
;// %1 = project name
Language=English
CE_PROJMAN: open project %1 
.
Language=German
CE_PROJMAN: Öffne Project %1
.



;//---------------
MessageId= 100
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_NO_PROJ_FILE
;// %2 = project name
Language=English
CE_PROJMAN: error %1!d!: can't open project file: %2 
.
Language=German
CE_PROJMAN: error %1!d!: Projektdatei kann nicht geöffnet werden: %2 
.

;//---------------
MessageId= 101
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_NO_VALID_PROJ_FILE_NAME
;// %2 = project name
Language=English
CE_PROJMAN: error %1!d!: no valid project file name: %2 
.
Language=German
CE_PROJMAN: error %1!d!: Kein gültiger Projektdateiname: %2 
.

;//---------------
MessageId= 102
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_PROJ_FILE_SYNTAX_ERROR
;// %2 = project file
;// %3 = line nr
Language=English
CE_PROJMAN: error %1!d!: syntax error in project file %2 at line %3 
.
Language=German
CE_PROJMAN: error %1!d!: Syntaxfehler in der Projektdatei %2 in Zeile %3
.

;//---------------
MessageId= 103
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_KADMANAG_CREATE
Language=English
CE_PROJMAN: error %1!d!: kad manager could not be created 
.
Language=German
CE_PROJMAN: error %1!d!: Kad-Manager konnte nicht instanziiert werden
.


;//---------------
MessageId= 104
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_BACKPARSER_CREATE
;// %2 = file extension
Language=English
CE_PROJMAN: error %1!d!: backparser for file extension %2 could not be created
.
Language=German
CE_PROJMAN: error %1!d!: Backparser für die Dateiendung %2 konnte nicht instanziiert werden
.



;//---------------
MessageId= 105
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_CONTAINER_CREATE
;// %2 = file extension
Language=English
CE_PROJMAN: error %1!d!: container handler for file extension %2 could not be created
.
Language=German
CE_PROJMAN: error %1!d!: Container-Handler für die Dateiendung %2 konnte nicht instanziiert werden
.



;//---------------
MessageId= 106
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_NO_PROJECT_LOADED
;// %2 = file extension
Language=English
CE_PROJMAN: error %1!d!: no project opened
.
Language=German
CE_PROJMAN: error %1!d!: Kein Projekt geöffnet
.


;//---------------
MessageId= 107
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_SOURCE_NOT_FOUND
;// %2 = file name
Language=English
CE_PROJMAN: error %1!d!: source file not found: %2
.
Language=German
CE_PROJMAN: error %1!d!: Sourcedatei konnte nicht gefunden werden: %2
.


;//---------------
MessageId= 108
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_BACKPARSER_CALL_FAILED
;// %2 = file name
Language=English
CE_PROJMAN: error %1!d!: backparser failed for file: %2
.
Language=German
CE_PROJMAN: error %1!d!: Backparser-Fehler für die Datei: %2 
.


;//---------------
MessageId= 109
Severity = Error
Facility = CE_PROJMAN
SymbolicName = E_CEPROJMAN_WRONG_PROJFILE_VERSION
Language=English
CE_PROJMAN: error %1!d!: wrong project file version
.
Language=German
CE_PROJMAN: error %1!d!: Falsche Version der Projektdatei
.



