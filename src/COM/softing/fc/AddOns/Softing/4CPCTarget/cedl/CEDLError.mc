
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
           CE_DL       =0x283 : E_FACILITY_CEDL
            )

;//currently supported languages:
LanguageNames=(
               English=0x409:CEDLMessagesENU
               German=0x407:CEDLMessagesDEU
              )
OutputBase=16

;//////////ERROR MESSAGES/////////////////////////////////////////////
MessageId= 1
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_INVALID_STATE

Language=English
Downloader[%1]: error D0001: Invalid downloader state
.
Language=German
Downloader[%1]: Fehler D0001: Ungültiger Zustand des Downloaders
.
;//---------------

MessageId= 2
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_GENERAL

Language=English
Downloader[%1]: error D0002: General downloader error
.
Language=German
Downloader[%1]: Fehler D0002: allgemeiner Downloader-Fehler
.
;//---------------

MessageId= 3
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_ADD_DOM

Language=English
Downloader[%1]: error D0003: Cannot add new domain entry
.
Language=German
Downloader[%1]: Fehler D0003: Es konnte kein neuer Domain-Eintrag erzeugt werden
.
;//---------------

MessageId= 4
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_DOM_NOT_FOUND

Language=English
Downloader[%1]: error D0004: Domain entry not found
.
Language=German
Downloader[%1]: Fehler D0004: Domain-Eintrag nicht gefunden
.
;//---------------

MessageId= 5
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_NOT_INITED

Language=English
Downloader: error D0005: Downloader not initialized
.
Language=German
Downloader: Fehler D0005: Downloader nicht initialisiert
.
;//---------------

MessageId= 6
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_ALREADY_INITED

Language=English
Downloader[%1]: error D0006: Downloader already initialized
.
Language=German
Downloader[%1]: Fehler D0006: Downloader bereits initialisiert
.
;//---------------

MessageId= 7
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_STATION_NOT_FOUND

Language=English
Downloader[%1]: error D0007: Station not found
.
Language=German
Downloader[%1]: Fehler D0007: Station nicht gefunden
.
;//---------------

MessageId= 8
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_STATION_NOT_CONN;

Language=English
Downloader[%1]: error D0008: Station not connected or waiting for connection
.
Language=German
Downloader[%1]: Fehler D0008: Station nicht verbunden/ wartet auf Verbindung
.
;//---------------

MessageId= 9
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_DOMFILE_NOTFOUND;

Language=English
Downloader[%1]: error D0009: Domain file %2 not found
.
Language=German
Downloader[%1]: Fehler D0009: Domain Datei %2 nicht gefunden
.
;//---------------

MessageId= 10
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_NOGENPATH;

Language=English
Downloader: error D0010: Path %1 for target %2 not found - incomplete build ?
.
Language=German
Downloader: Fehler D0010: Pfad %1 für Target %2 nicht gefunden - Übersetzungsvorgang unvollständig ?
.
;//---------------

MessageId= 11
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_SUBSCRIBE;

Language=English
Downloader[%1]: error D0011: Cannot subscribe downloader variable %2
.
Language=German
Downloader[%1]: Fehler D0011: Downloader Variable %2 kann nicht angemeldet werden
.
;//---------------

MessageId= 12
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_PENDING;

Language=English
Downloader[%1]: error D0012: Pending download
.
Language=German
Downloader[%1]: Fehler D0012: Downloadvorgang anhängig
.
;//---------------


MessageId= 13
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_FILE_ERROR;

Language=English
Downloader[%1]: error D0013: file error: %2 (file=%3)
.
Language=German
Downloader[%1]: Fehler D0013: Dateifehler: %2 (Datei=%3)
.
;//---------------

MessageId= 14
Severity = Error
Facility = CE_DL
SymbolicName = E_CE_DL_INVALID_CONN;

Language=English
Downloader[%1]: error D0014: Invalid connection to %2
.
Language=German
Downloader[%1]: Fehler D0014: Ungültige Verbinung zu %2
.
;//---------------

;//////////END ERROR MESSAGES/////////////////////////////////////////////

;//////////INFORMATIONAL MESSAGES/////////////////////////////////////////////
MessageId= 25
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_READY;

Language=English
Downloader[%1]: Status D0025: Download finished
.
Language=German
Downloader[%1]: Status D0025: Ladevorgang beendet
.
;//---------------

MessageId= 26
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_TRANSFER;

Language=English
Downloader[%1]: Status D0026: Download domain %2
.
Language=German
Downloader[%1]: Status D0026: Lade domain %2
.
;//---------------

MessageId= 27
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_ABORT;

Language=English
Downloader[%1]: Status D0027: Download aborted
.
Language=German
Downloader[%1]: Status D0027: Ladevorgang abgebrochen
.
;//---------------

MessageId= 28
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_ERROR;

Language=English
Downloader[%1]: Status D0028: Download error hr = 0x%2
.
Language=German
Downloader[%1]: Status D0028: Fehler bei Ladevorgang hr = 0x%2
.
;//---------------

MessageId= 29
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_CONN;

Language=English
Downloader[%1]: Status D0029: Download connection interrupted
.
Language=German
Downloader[%1]: Status D0029: Verbindungsabbruch bei Ladevorgang
.
;//---------------

MessageId= 30
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_DLLIST_READY;

Language=English
Downloader[%1]: Status D0030: Download list ready
.
Language=German
Downloader[%1]: Status D0030: Ladeliste fertig
.
;//---------------

MessageId= 31
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_UPLD_READY;

Language=English
Downloader[%1]: Status D0031: Upload ready
.
Language=German
Downloader[%1]: Status D0031: Upload fertig
.
;//---------------

MessageId= 32
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_UPLD;

Language=English
Downloader[%1]: Status D0032: Upload domain %2
.
Language=German
Downloader[%1]: Status D0032: Lade domain %2 hoch
.
;//---------------

MessageId= 33
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_NEWPRJ;

Language=English
Downloader[%1]: Status D0033: New project detected
.
Language=German
Downloader[%1]: Status D0033: Neues Projekt
.
;//---------------

MessageId= 34
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DOWNLD_DEL;

Language=English
Downloader[%1]: Status D0034: Delete domain %2
.
Language=German
Downloader[%1]: Status D0034: Lösche domain %2
.
;//---------------

MessageId= 35
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_CONNECT;

Language=English
Downloader[%1]: Status D0035: Connecting ...
.
Language=German
Downloader[%1]: Status D0035: Verbinde...
.
;//---------------

MessageId= 36
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_LOGIN;

Language=English
Downloader[%1]: Status D0036: User %2 logged in
.
Language=German
Downloader[%1]: Status D0036: Anwender %2 angemeldet
.
;//---------------


MessageId= 37
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_STARTCHANGES;

Language=English
Downloader[%1]: Status D0037: Download changes started (address = %2)
.
Language=German
Downloader[%1]: Status D0037: Inkrementeller Ladervorgang gestartet (Adresse = %2)
.
;//---------------

MessageId= 38
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_START;

Language=English
Downloader[%1]: Status D0038: Download started (address = %2)
.
Language=German
Downloader[%1]: Status D0038: Ladevorgang gestartet (Adresse = %2)
.
;//---------------

MessageId= 39
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_ONLCHANGE_FAIL;

Language=English
Downloader[%1]: Status D0039: Online change failed
.
Language=German
Downloader[%1]: Status D0039: Fehler bei inkrementeller Umkonfiguration 
.
;//---------------

MessageId= 40
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_USR_LOCK;

Language=English
Downloader[%1]: Status D0040: User %2 logged in exclusively
.
Language=German
Downloader[%1]: Status D0040: Anwender %2 hat sich exklusiv angemeldet
.
;//---------------


MessageId= 41
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_TRANSFER_ERROR;

Language=English
Downloader[%1]: Status D0041: Cannot transfer domain %2. error description - %3
.
Language=German
Downloader[%1]: Status D0041: Kann domain %2 nicht übertragen. Fehlerbeschreibung - %3
.
;//---------------


MessageId= 42
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_DEL_ERROR;

Language=English
Downloader[%1]: Status D0042: Cannot delete domain %2. error description - %3
.
Language=German
Downloader[%1]: Status D0042: Kann domain %2 nicht löschen. Fehlerbeschreibung - %3
.
;//---------------

MessageId= 43
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_CONFIG_CHANGE

Language=English
Downloader[%1]: Status D0043: Online change started...
.
Language=German
Downloader[%1]: Status D0043: Beginn der Umkonfiguration...
.
;//---------------

MessageId= 44
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_LOGOUT;

Language=English
Downloader[%1]: Status D0044: User %2 logged out
.
Language=German
Downloader[%1]: Status D0044: Anwender %2 abgemeldet
.
;//---------------


MessageId= 45
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_FIRMWARECHANGE

Language=English
Downloader[%1]: Status D0045: Changed or added Control's binaries: Restarting control service.
.
Language=German
Downloader[%1]: Status D0045: Progammdateien des Control wurden geändert/hinzugefügt: Der Control Serice wird neugestartet.
.


MessageId= 46
Severity = Informational
Facility = CE_DL
SymbolicName = S_CE_DL_FIRMWARE_UPDATE_INFO

Language=English
Downloader[%1]: Status D0046: %2.
.
Language=German
Downloader[%1]: Status D0046: %2.
.

;//---------------

;//////////END INFORMATIONAL MESSAGES/////////////////////////////////////////////