;/*H>> 
;*=DESCRIPTION
;*    @DESCRIPTION@
;*==This file contains all error, warning and other 
;*==messages produces by ATCMControl I/O subsystem
;*==any language dependent strings should be in this file.  
;*----------------------------------------------------------------------------*
;* =MODIFICATION
;*  09.05.2001  SU      File created
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

;// SU: cannot have multiple message resources in one project
;// Hence all OPC error messages from opcerror.mc are copied to this mc-file.
;// Interface - OPC error messages from opcerror.mc
FacilityNames=(
				  Interface=0x004
;//               CC    =0x200 : E_FACILITY_CONTROL
       IO_MANAG     =0x210 : E_FACILITY_IO_MANAG
       IO_DP        =0x211 : E_FACILITY_IO_DP
       IO_OPC       =0x212 : E_FACILITY_IO_OPC
;//    IO_FF        =0x213 : E_FACILITY_IO_FF
;//               CSC   =0x220 : E_FACILITY_SYSCOM
;//               CCG   =0x230 : E_FACILITY_CODEGEN
;//               CLIB  =0x260 : E_FACILITY_LIBRARIAN
;//               CSET  =0x270 : E_FACILITY_SETUP
;//               CE    =0x280 : E_FACILITY_ENGENEERING
;//               CE_Wiz=0x2A0 : E_FACILITY_WIZARD
;//               BL    =0x2C0 : E_FACILITY_BASELIB
              )

;//currently supported languages:
;//     0x409   English USA
;//     0x407   German
LanguageNames=(
               English=0x409:CIO_ErrorENU
               German=0x407:CIO_ErrorDEU
              )
OutputBase=16

;
;//==========================================================================================
;//------------------------------------------------------------------------------------------
;// IO-Manager messages in range 1000 - 1999
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
MessageId = 1000
Facility = IO_MANAG
Severity = error
SymbolicName = E_CIO_OUTOFMEM
Language=english
IO : fatal error I1000: Out of memory   
.
Language=German
IO : fatal error I1000: Kein freier Speicher mehr vorhanden!
.
;//----


MessageId = 1001
Severity = error
SymbolicName = E_CIO_NOT_INITIALIZED
Language=English
IO : fatal error I1001: %1: IO Manager not initialized.
.
Language=German
IO : fatal error I1001: %1: IO Manager nicht initialisiert.
.
;//----


MessageId = 1002
;// %2    HRESULT as hex-code
Severity = error
SymbolicName = E_CIO_BAD_AVAIL_IO_LAYERS
Language=English
IO : error I1002: error while trying to read list of available IO layers: code = %1.
.
Language=German
IO : error I1002: %Fehler beim Bestimmen der verfügbaren I/O-Layers; Code = %1.
.
;//----

MessageId = 1003
;// %1    Address
Severity = error
SymbolicName = E_CIO_BOARD_NOT_CONFIGURED
Language=English
IO : error I1003: IO variable with address '%1' references unknown IO-Layer.
.
Language=German
IO : error I1003: IO-Variable mit Adresse '%1' referenziert unbekannten IO-Layer.
.
;//----

MessageId = 1004
;// %1    variable name
;// %2    init string
Severity = error
SymbolicName = E_CIO_BAD_IO_VAR_INIT_STRING
Language=English
IO : error I1004: IO variable '%1' has bad initstring: '%2'.
.
Language=German
IO : error I1004: IO-Variable '%1' enthält ungültigen Init-String '%2'.
.
;//----

MessageId = 1005
;// %1    datatype of variable
;// %2    address of variable
Severity = error
SymbolicName = E_CIO_IO_VAR_NOT_INITIALIZED
Language=English
IO : error I1005: %1 IO variable %2 not initialized, will not be accessable from program.
.
Language=German
IO : error I1005: %1 IO Variable %2 nicht initialisiert, kann im Programm nicht referenziert werden.
.

MessageId = 1006
;// %1    name string of unknown layer
;// %2    HRESULT as hex-code
Severity = error
SymbolicName = E_CIO_UNKNOWN_IO_LAYER
Language=English
IO : fatal error I1006: I/O configuration is referencing unknown or not installed IO-Layer CLSID='%1'; code = 0x%2!x!
.
Language=German
IO : fatal error I1006: I/O configuration referenziert unbekannten oder nicht installierten IO-Layer CLSID='%1'; code = 0x%2!x!
.
;//----


MessageId = 1007
;// %1    Channel number - decimal
Severity = error
SymbolicName = E_CIO_ADD_CONFIGURATION
Language=English
IO : fatal error I1007: Configuration for IO-Layer with channel number '%1!d!' could not be loaded.
.
Language=German
IO : fatal error I1007: Konfiguration für den IO-Layer mit Kanal-Nummer '%1!d!' konnte nicht geladen werden.
.
;//----

MessageId = 1008
;// %1    Channel number - decimal
Severity = error
SymbolicName = E_CIO_LAYER_NOT_FOUND
Language=English
IO : fatal error I1008: IO-Layer with channel number '%1!d!' is not loaded or does not exist.
.
Language=German
IO : fatal error I1008: IO-Layer mit Kanal-Nummer '%1!d!' ist nicht geladen oder existiert nicht.
.
;//----


;//*****    Warning messages
MessageId = 1050
;// %1    Channel number - decimal
Severity = Warning
SymbolicName = W_CIO_CLEAR_CONFIG
Language=English
IO : warning I1050: Could not clear IO-Layer configuration (channel number='%1!d!').
.
Language=German
IO : Warnung I1050: Kann Konfiguration für IO-Layer nicht löschen (Kanal-Nummer='%1!d!').
.
;//----

MessageId = 1051
;// %1    Channel number - decimal
Severity = Warning
SymbolicName = W_CIO_STOPONLINE
Language=English
IO : warning I1051: Could not stop IO-Layer (channel number='%1!d!').
.
Language=German
IO : Warnung I1051: Kann IO-Layer nicht stoppen (Kanal-Nummer='%1!d!').
.
;//----


MessageId = 1052
;// %1    Channel number - decimal
Severity = Warning
SymbolicName = W_CIO_STARTONLINE
Language=English
IO : warning I1052: Could not start IO-Layer (channel number='%1!d!').
.
Language=German
IO : Warnung I1052: Kann IO-Layer nicht starten (Kanal-Nummer='%1!d!').
.
;//----


;//*****    Informational messages
;//----
MessageId = 1100
Severity = informational
SymbolicName = S_CIO_INITIALIZED
Language=English
IO : IO Manager is initialized.
.
Language=German
IO : IO Manager initialisiert.
.
;//----


MessageId = 1101
Severity = informational
SymbolicName = S_CIO_TERMINATED
Language=English
IO : IO Manager is terminated.
.
Language=German
IO : IO Manager terminiert.
.
;//----

MessageId = 1102
Severity = informational
SymbolicName = S_CIO_CONFDOM_NOVERSION
Language=English
IO : version of configuration domain '%1' was not found.
.
Language=German
IO : Version der Konfigurationsdomain '%1' wurde nicht gefunden.
.
;//----

MessageId = 1103
Severity = informational
SymbolicName = S_CIO_IO_LAYER_RELEASE
Language=English
IO : Release IO-Layer: CLSID='%1' channel number='%2!d!'.
.
Language=German
IO : Freigabe des IO-Layer: CLSID='%1' Kanal-Nummer='%2!d!'.
.
;//----


;//
;//==========================================================================================
;//

;//------------------------------------------------------------------------------------------
;// IO_DP messages in range 2000 - 2999
;//------------------------------------------------------------------------------------------
;//*****    Error messages
MessageId = 2001
;// %1    BIN file name
Severity = error
SymbolicName = E_CIO_DP_BAD_BIN_FILE_FORMAT
Language=English
DP: fatal error I2001: BIN file %1 has bad format, cannot initialize PROFIboard
.
Language=German
DP: fatal error I2001: BIN Datei %1 hat ungültiges Format, PROFIboard kann nicht initialisiert werden
.
;//----

MessageId = 2002
;// %1    board number as integer !!
Severity = error
SymbolicName = E_CIO_DP_MASTER_TO_CLEAR
Language=English
DP: error I2002: Cannot set DP-Master on board %1!d! to CLEAR mode, no I/O possible!
.
Language=German
DP: error I2002: Cannot set DP-Master on board %1!d! to CLEAR mode, no I/O possible!
.
;//----

MessageId = 2003
;// %1    board number as integer !!
Severity = error
SymbolicName = E_CIO_DP_MASTER_TO_OPERATE
Language=English
DP: error I2003: Cannot set DP-Master on board %1!d! to OPERATE mode, no I/O possible!
.
Language=German
DP: error I2003: Cannot set DP-Master on board %1!d! to OPERATE mode, no I/O possible!
.
;//----

MessageId = 2004
;// %1    channel number as integer !!
Severity = error
SymbolicName = E_CIO_DP_NO_PROFI_BOARD
Language=English
DP: error I2004: No Profiboard hardware/driver installed or no access (channel=%1!d!) - no I/O possible.
.
Language=German
DP: error I2004: Keine Profiboard Hardware/Treiber installiert oder kein Zugriff (channel=%1!d!) - kein I/O möglich.
.
;//----

MessageId = 2006
;// %1    string with %-address
Severity = error
SymbolicName = E_CIO_DP_BAD_SYNTAX_IN_IO_ADDRESS
Language=English
DP: error I2006: address format %%-address: '%1'
.
Language=German
DP: error I2006: address format %%-address: '%1'
.
;//----

MessageId = 2007
;// %1    string with %-address
Severity = error
SymbolicName = E_CIO_DP_UNKNOWN_DATATYPE
Language=English
DP: error I2007: Datatype '%1' not supported for Profibus IO variable (%%-address: '%2')
.
Language=German
DP: error I2007: Datentyp '%1' wird für Profibus IO variable (%%-address: '%2') nicht unterstützt
.
;//----

MessageId = 2008
;// %1    variable name
Severity = error
SymbolicName = E_CIO_DP_NO_MASTER_FOR_VARIABLE
Language=English
DP: error I2008: No DP-Master is configured for IO variable %1
.
Language=German
DP: error I2008: No DP-Master is configured for IO variable %1
.
;//----

MessageId = 2009
;// %1    slave address as integer !!
;// %2    string with %-address
Severity = error
SymbolicName = E_CIO_DP_NO_SLAVE_FOR_VARIABLE
Language=English
DP: error I2009: DP slave %1!d! is not configured (%%-address: '%2')
.
Language=German
DP: error I2009: DP slave %1!d! is not configured (%%-address: '%2')
.
;//----

MessageId = 2010
;// %1    variable name
Severity = error
SymbolicName = E_CIO_DP_RW_PAST_BITFIELD_SIZE
Language=English
DP: error I2010: reading/writing past configured DP slave bitfield size in IO variable : %1
.
Language=German
DP: error I2010: reading/writing past configured DP slave bitfield size in IO variable : %1
.
;//----

MessageId = 2011
;// %1    string with %-address
Severity = error
SymbolicName = E_CIO_DP_UNKNOWN_DP_STATION
Language=English
DP: error I2011: no DP station found at %%-address: '%1'
.
Language=German
DP: error I2011: no DP station found at %%-address: '%1'
.
;//----

MessageId = 2012
;// %1    master address as integer !!
Severity = error
SymbolicName = E_CIO_DP_NO_MASTER
Language=English
DP: error I2012: Cannot access DP-Master on address %1!d!
.
Language=German
DP: error I2012: Cannot access DP-Master on address %1!d!
.
;//----

MessageId = 2013
;// %1    master address as integer !!
Severity = error
SymbolicName = E_CIO_DP_CANNOT_SET_BUSPARAM
Language=English
DP: error I2013: Cannot set busparameters for DP-Master %1!d!
.
Language=German
DP: error I2013: Cannot set busparameters for DP-Master %1!d!
.
;//----

MessageId = 2014
;// %1    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_CANNOT_DLOAD_SLAVE_PARAM
Language=English
DP: error I2014: Cannot download slave parameter set for slave %1!d! to DP-Master
.
Language=German
DP: error I2014: Cannot download slave parameter set for slave %1!d! to DP-Master
.
;//----

MessageId = 2015
;// %1    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_CANNOT_READ_SLAVE_IO_OFFSETS
Language=English
DP: error I2015: Cannot read input/output offsets from DP-Master for slave %1!d!
.
Language=German
DP: error I2015: Cannot read input/output offsets from DP-Master for slave %1!d!
.
;//----

MessageId = 2016
;// %1    master address as integer !!
Severity = error
SymbolicName = E_CIO_DP_NO_SLAVES_FOR_MASTER
Language=English
DP: error I2016: No slave parameter sets were downloaded to DP-Master %1!d!, master has no I/O
.
Language=German
DP: error I2016: No slave parameter sets were downloaded to DP-Master %1!d!, master has no I/O
.
;//----

MessageId = 2017
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_MASTER_LOCK
Language=English
DP: error I2017: %1 Slave %2!d! MASTER_LOCK: Slave already parameterized by other master.
.
Language=German
DP: error I2017: %1 Slave %2!d! MASTER_LOCK: Slave already parameterized by other master.
.
;//----


MessageId = 2018
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_PRM_FAULT
Language=English
DP: error I2018: %1 Slave %2!d! PRM_FAULT: Faulty data in last slave parameters.
.
Language=German
DP: error I2018: %1 Slave %2!d!
.
;//----

MessageId = 2019
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_INVALID_SLAVE_RES
Language=English
DP: error I2019: %1 Slave %2!d! INVALID_SLAVE_RES: Invalid response of slave
.
Language=German
DP: error I2019: %1 Slave %2!d! INVALID_SLAVE_RES: Invalid response of slave
.
;//----

MessageId = 2020
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_NOT_SUPPORTED
Language=English
DP: error I2020: %1 Slave %2!d! NOT_SUPPORTED: Function not supported by DP slave.
.
Language=German
DP: error I2020: %1 Slave %2!d! NOT_SUPPORTED: Function not supported by DP slave.
.
;//----


MessageId = 2021
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_EXT_DIAG
Language=English
DP: error I2021: %1 Slave %2!d! EXT_DIAG: Extended diagnostics in slave are errors
.
Language=German
DP: error I2021: %1 Slave %2!d! EXT_DIAG: Extended diagnostics in slave are errors
.
;//----

MessageId = 2022
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_CFG_FAULT
Language=English
DP: error I2022: %1 Slave %2!d! CFG_FAULT: Config data of master does not match with DP slave config data.
.
Language=German
DP: error I2022: %1 Slave %2!d! CFG_FAULT: Config data of master does not match with DP slave config data.
.
;//----

MessageId = 2023
;// %2    time string
;// %3    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_STATION_NOT_READY
Language=English
DP: error I2023: %1 Slave %2!d! STATION_NOT_READY: Slave station not ready for data exchange.
.
Language=German
DP: error I2023: %1 Slave %2!d! STATION_NOT_READY: Slave station not ready for data exchange.
.
;//----

MessageId = 2024
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_STATION_NOT_EXISTENT
Language=English
DP: error I2024: %1 Slave %2!d! STATION_NOT_EXISTENT: DP slave station not existent on bus.
.
Language=German
DP: error I2024: %1 Slave %2!d! STATION_NOT_EXISTENT: DP slave station not existent on bus.
.
;//----


MessageId = 2025
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_DEACTIVATED
Language=English
DP: error I2025: %1 Slave %2!d! DEACTIVATED: Slave station is deactivated.
.
Language=German
DP: error I2025: %1 Slave %2!d! DEACTIVATED: Slave station is deactivated.
.
;//----


MessageId = 2026
;// %1    time string
;// %2    slave address as integer !!
Severity = informational
SymbolicName = S_CIO_DP_SLAVE_DIAG_SYNC_MODE
Language=English
DP: diag  I2026: %1 Slave %2!d! SYNC_MODE: DP slave is in Sync Mode (status)
.
Language=German
DP: diag  I2026: %1 Slave %2!d! SYNC_MODE: DP slave is in Sync Mode (status)
.
;//----


MessageId = 2027
;// %1    time string
;// %2    slave address as integer !!
Severity = informational
SymbolicName = S_CIO_DP_SLAVE_DIAG_FREEZE_MODE
Language=English
DP: diag  I2027: %1 Slave %2!d! FREEZE_MODE: DP slave is in Freeze Mode (status)
.
Language=German
DP: diag  I2027: %1 Slave %2!d! FREEZE_MODE: DP slave is in Freeze Mode (status)
.
;//----


MessageId = 2028
;// %1    time string
;// %2    slave address as integer !!
Severity = informational
SymbolicName = S_CIO_DP_SLAVE_DIAG_WD_ON
Language=English
DP: diag  I2028: %1 Slave %2!d! WD_ON: Watchdog in DP slave is enabled (status)
.
Language=German
DP: diag  I2028: %1 Slave %2!d! WD_ON: Watchdog in DP slave is enabled (status)
.
;//----


MessageId = 2029
;// %1    time string
;// %2    slave address as integer !!
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_STAT_DIAG
Language=English
DP: error I2029: %1 Slave %2!d! STAT_DIAG: Static diagnostics, no data available
.
Language=German
DP: error I2029: %1 Slave %2!d! STAT_DIAG: Static diagnostics, no data available
.
;//----

MessageId = 2030
;// %1    time string
;// %2    slave address as integer !!
Severity = informational
SymbolicName = S_CIO_DP_SLAVE_DIAG_PRM_REQ
Language=English
DP: diag  I2030: %1 Slave %2!d! PRM_REQ: DP slave requires parametering
.
Language=German
DP: diag  I2030: %1 Slave %2!d! PRM_REQ: DP slave requires parametering
.
;//----

MessageId = 2031
;// %1    time string
;// %2    slave address as integer !!
;// %3    user diag string
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_UNIT_DIAG
Language=English
DP: error I2031: %1 Slave %2!d! UNIT Diagnostic: %3
.
Language=German
DP: error I2031: %1 Slave %2!d! GERÄTE Diagnose: %3
.
;//----


MessageId = 2032
;// %1    time string
;// %2    slave address as integer !!
;// %3    user diag string
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_MODULE_DIAG
Language=English
DP: error I2032: %1 Slave %2!d! MODULE Diagnostic: %3
.
Language=German
DP: error I2032: %1 Slave %2!d! MODUL Diagnose: %3
.
;//----

MessageId = 2034
;// %1    time string
;// %2    slave address as integer !!
;// %3    user diag string
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_CHANNEL_DIAG
Language=English
DP: error I2034: %1 Slave %2!d! CHANNEL Diagnostic: %3
.
Language=German
DP: error I2034: %1 Slave %2!d! KANAL Diagnose: %3
.
;//----

MessageId = 2035
;// %1    time string
;// %2    slave address as integer !!
;// %3    user diag string
Severity = error
SymbolicName = E_CIO_DP_SLAVE_DIAG_REV_NUMBER_DIAG
Language=English
DP: error I2035: %1 Slave %2!d! REVISION NUMBER: %3
.
Language=German
DP: error I2035: %1 Slave %2!d! REVISIONS NUMMER: %3
.
;//----

MessageId = 2036
;// %1    BIN file name
Severity = error
SymbolicName = E_CIO_DP_NO_BIN_FILE_FOUND
Language=English
DP: fatal error I2036: No BIN file %1 found, cannot initialize PROFIboard
.
Language=German
DP: fatal error I2036: No BIN file %1 found, cannot initialize PROFIboard
.
;//----

MessageId = 2037
;// %1    BIN file name
Severity = error
SymbolicName = E_CIO_DP_BAD_BIN_FILE_NAME
Language=English
DP: fatal error I2037: Bad syntax in BIN filename %1, expected board number in name; cannot initialize PROFIboard!
.
Language=German
DP: fatal error I2037: Bad syntax in BIN filename %1, expected board number in name; cannot initialize PROFIboard!
.
;//----

MessageId = 2038
;// %2    board number as integer!!
Severity = error
SymbolicName = E_CIO_DP_UNKNOWN_BOARD_NUMBER
Language=English
DP: fatal error I2038: Bad/unknown PROFIboard number %1!d! for cached IO image!
.
Language=German
DP: fatal error I2038: Bad/unknown PROFIboard number %1!d! for cached IO image!
.
;//----

MessageId = 2039
;// %1    board number
Severity = error
SymbolicName = S_CIO_DP_RTCE_CODE_NO_RTCE
Language=English
DP: fatal error I2039: RTCE code for board %1!d! found. No RTCE available.
.
Language=German
DP: fatal error I2039: RTCE code for board %1!d! found. No RTCE available.
.
;//----

MessageId = 2040
;// %1    board number
Severity = error
SymbolicName = S_CIO_DP_RTCE_LOAD_ERROR
Language=English
DP: error I2040: RTCE code for board %1!d! not loaded
.
Language=German
DP: error I2040: RTCE code for board %1!d! not loaded
.
;//----

MessageId = 2041
;// %1    board number
Severity = error
SymbolicName = S_CIO_DP_RTCE_CLEAR_ERROR
Language=English
DP: error I2041: RTCE code for board %1!d! not cleared
.
Language=German
DP: error I2041: RTCE code for board %1!d! not cleared
.
;//----

MessageId = 2042
;// %1    board number
Severity = error
SymbolicName = S_CIO_DP_RTCE_WRONG_IO_SEGMENT
Language=English
DP: error I2042: I/O segment in RTCE on board %1!d! does not match DP configuration
.
Language=German
DP: error I2042: I/O segment in RTCE on board %1!d! does not match DP configuration
.
;//----

MessageId = 2043
Severity = error
SymbolicName = E_CIO_DP_INV_DATATYPE_PREDEFVAR
Language=English
DP: error I2043: Invalid datatype for predefined var '%1' %%-address: '%2'
.
Language=German
DP: error I2043: Vordefinierte Variable '%1' (%%-Adresse: '%2') hat ungültigen Datentyp
.
;//----


MessageId = 2044
Severity = error
SymbolicName = E_CIO_DP_UNKNOWN_PREDEFVAR
Language=English
DP: error I2044: Unknown predefined var '%1' (%%-address: '%2')
.
Language=German
DP: error I2044: Unbekannte vordefinierte Variable '%1' (%%-Adresse: '%2')
.
;//----

MessageId = 2045
Severity = error
SymbolicName = E_CIO_DP_NO_DEVICE
Language=English
DP: error I2045: No device found or configured.
.
Language=German
DP: error I2045: Kein Gerät gefunden oder konfiguriert.
.
;//----


MessageId = 2046
Severity = error
SymbolicName = E_CIO_DP_BAD_BITSIZE
Language=English
DP: error I2046: IEC data type '%1' needs address bit size '%2'
.
Language=German
DP: error I2046: IEC Datentyp '%1' erfordert address Bitgröße '%2'
.
;//----


MessageId = 2047
Severity = error
SymbolicName = E_CIO_DP_BAD_ADDR_NEED_BIT
Language=English
DP: error I2047: bit address: missing .<bitoffset> field
.
Language=German
DP: error I2047: Bit-Adresse: benötig .<bitoffset> field
.
;//----

MessageId = 2048
Severity = error
SymbolicName = E_CIO_DP_BAD_ADDR_NEED_SLAVEID
Language=English
DP: error I2048: second field must be slave or master number
.
Language=German
DP: error I2048: Zweites Feld muß Slave- oder Masternummer sein
.
;//----


MessageId = 2049
Severity = error
SymbolicName = E_CIO_DP_BAD_ADDR_NEED_BYTEOFF
Language=English
DP: error I2049: third field must be the byte offset
.
Language=German
DP: error I2049: Drittes Feld muß der Byteoffset sein
.
;//----


;//*****    Informational messages

MessageId = 2100
Severity = informational
SymbolicName = S_CIO_DP_INIT_IO_LAYER
Language=English
DP: Initializing IO layer for PROFIBUS DP...
.
Language=German
DP: IO layer für PROFIBUS DP wird initialisiert...
.
;//----

MessageId = 2101
;// %1    board number as integer !!
Severity = informational
SymbolicName = S_CIO_DP_GOING_ONLINE
Language=English
DP: Going ONLINE with board %1!d!
.
Language=German
DP: Going ONLINE with board %1!d!
.
;//----

MessageId = 2102
;// %1    version string
Severity = informational
SymbolicName = S_CIO_DP_FIRMWARE_VERSION
Language=English
DP: firmware version: %1
.
Language=German
DP: firmware version: %1
.
;//----

MessageId = 2103
;// %1    slave address as integer !!
Severity = informational
SymbolicName = S_CIO_DP_SLAVE_OK
Language=English
DP: Slave %1!d! is OK!
.
Language=German
DP: Slave %1!d! is OK!
.
;//----

MessageId = 2104
;// %1    board number
Severity = informational
SymbolicName = S_CIO_DP_RTCE_OPEN
Language=English
DP: RTCE for board %1!d! opened
.
Language=German
DP: RTCE for board %1!d! opened
.
;//----

MessageId = 2105
;// %1    board number
Severity = informational
SymbolicName = S_CIO_DP_RTCE_CLOSED
Language=English
DP: RTCE for board %1!d! closed
.
Language=German
DP: RTCE for board %1!d! closed
.
;//----

MessageId = 2106
;// %1    board number
Severity = informational
SymbolicName = S_CIO_DP_RTCE_CODE_LOADED
Language=English
DP: RTCE code for board %1!d! loaded
.
Language=German
DP: RTCE code for board %1!d! loaded
.
;//----

MessageId = 2107
;// %1    board number
Severity = informational
SymbolicName = S_CIO_DP_RTCE_CODE_CLEARED
Language=English
DP: RTCE code for board %1!d! cleared
.
Language=German
DP: RTCE code for board %1!d! cleared
.
;//----

MessageId = 2108
Severity = informational
SymbolicName = S_CIO_DP_CREATE_TASK_CACHE
Language=English
DP: Create task cache.
.
Language=German
DP: Erzeuge Task-Abbild.
.
;//----

;//==========================================================================================
;//------------------------------------------------------------------------------------------
;// Original OPC error messages from opcerror.mc
;// Cannot have multiple message resources in one project.
;// Hence all OPC error messages from opcerror.mc are copied to this mc-file.
;//------------------------------------------------------------------------------------------

MessageID=1
Severity=Error
Facility=Interface
SymbolicName=OPC_E_INVALIDHANDLE
Language=English
The value of the handle is invalid.
.
Language=German
Eine ungültige OPC-Zugriffsnummer wurde übergeben
.

MessageID=4
SymbolicName=OPC_E_BADTYPE
Language=English
The server cannot convert the data between the 
requested data type and the canonical data type.
.
Language=German
Der Server kann den Wert nicht zwischen dem geforderten
und dem ursprünglichen Datentyp konvertieren.
.

MessageID=
SymbolicName=OPC_E_PUBLIC
Language=English
The requested operation cannot be done on a public group.

.
Language=German
Die gewünschte Operation kann nicht in einer 
öffentlichen Gruppe augeführt werden.
.

MessageID=
SymbolicName=OPC_E_BADRIGHTS
Language=English
The Items AccessRights do not allow the operation.
.
Language=German
Die Zugriffsrechte des Items erlauben die Operation nicht.
.

MessageID=
SymbolicName=OPC_E_UNKNOWNITEMID
Language=English
The item is no longer available in the server address space

.
Language=German
Das Item ist nicht länger im Namensraum des Servers verfügbar.
.

MessageID=
SymbolicName=OPC_E_INVALIDITEMID
Language=English
The item definition doesn't conform to the server's syntax.
.
Language=German
Die Item Definition entspricht nicht der Syntax des Servers.
.

MessageID=
SymbolicName=OPC_E_INVALIDFILTER
Language=English
The filter string was not valid
.
Language=German
Der Filter String war nicht gültig.
.

MessageID=
SymbolicName=OPC_E_UNKNOWNPATH
Language=English
The item's access path is not known to the server.

.
Language=German
Der Zugriffspfad des Items ist dem Server nicht bekannt.
.

MessageID=
SymbolicName=OPC_E_RANGE
Language=English
The value was out of range.

.
Language=German
Der Wert war auserhalb des Wertebereiches.
.

MessageID=
SymbolicName=OPC_E_DUPLICATENAME
Language=English
Duplicate name not allowed.

.
Language=German
Ein doppelter Name ist nicht erlaubt.
.

MessageID=
Severity=Success
SymbolicName=OPC_S_UNSUPPORTEDRATE
Language=English
The server does not support the requested data rate
but will use the closest available rate.

.
Language=German
Der Server unterstützt die angeforderte Daten Rate nicht.
Er wird aber die näheste verfügbare Rate benutzen.
.

MessageID=
SymbolicName=OPC_S_CLAMP
Language=English
A value passed to WRITE was accepted but the output was clamped.
.
Language=German
Der an WRITE übergebene Wert wurde akzeptiert, 
aber die Ausgabe wurde gehalten.
.

MessageID=
SymbolicName=OPC_S_INUSE
Language=English
The operation cannot be completed because the 
object still has references that exist.

.
Language=German
Die Funktion konnte nicht beendet werden,
da noch Referenzen auf das Objekt existieren.
.

MessageID=
Severity=Error
SymbolicName=OPC_E_INVALIDCONFIGFILE
Language=English
The server's configuration file is an invalid format.
.
Language=German
Das Konfigurationsfile des Servers hat ein ungültiges Format.
.

MessageID=
SymbolicName=OPC_E_NOTFOUND
Language=English
The server could not locate the requested object.
.
Language=German
Der Server kann das angeforderte Objekt nicht finden.
.

MessageID=0x203
SymbolicName=OPC_E_INVALID_PID
Language=English
The server does not recognize the passed property ID.
.
Language=German
Der Server unterstützt die übergebene Eigenschaftskennung nicht.
.

;//==========================================================================================
;//------------------------------------------------------------------------------------------
;// IO_OPC messages in range 3000 - 3999
;//------------------------------------------------------------------------------------------
MessageId = 3000
;// %1    group config filename
Severity = error
SymbolicName = E_CIO_OPC_NO_OPC_GROUP_CONFIG_FILE
Language=English
OPC: error I3000: No OPC group data file '%1' found, cannot initialize OPC servers for IO access
.
Language=German
OPC: Fehler I3000: OPC Konfigurations-Datei '%1' nicht gefunden, IO-Layer lässt sich nicht initialisieren
.
;//----

MessageId = 3001
;// %1    group config filename
;// %2    line number as integer
;// %3    bad syntax as string
Severity = error
SymbolicName = E_CIO_OPC_BAD_OPC_GROUP_CONFIG_LINE
Language=English
OPC: error I3001: bad group description at line %2!d! in file '%1': %3
.
Language=German
OPC: Fehler I3001: Syntax Fehler in Gruppen-Beschreibung - %1(%2!d!): %3
.
;//----

MessageId = 3002
;// %1    variable name string
;// %2    machine name
;// %3    OPC server name / ProgID
;// %4    group name
Severity = error
SymbolicName = E_CIO_OPC_ERROR_ADDING_VARIABLE
Language=English
OPC: error I3002: cannot add variable '%1' to server '\\\\%2\\%3' / group '%4'
.
Language=German
OPC: Fehler I3002: Kann Variable '%1' nicht dem Server '\\\\%2\\%3' (Gruppe '%4') hinzufügen
.
;//----

MessageId = 3003
;// %1    server name string
Severity = error
SymbolicName = E_CIO_OPC_SERVER_NOT_REGISTERED
Language=English
OPC: error I3003: Cannot lookup a CLSID for OPC server %1, server not registered
.
Language=German
OPC: Fehler I3003: Kann CLSID für OPC server %1 nicht finden, Server ist nicht registriert
.
;//----

MessageId = 3004
;// %1    name of server
;// %2    returned error message
;// %3    error code as HRESULT
Severity = error
SymbolicName = E_CIO_OPC_SERVER_CANNOT_CONNECT
Language=English
OPC: error I3004: Error during connection establishment to OPC server %1 reason=(0x%3!08X!) %2
.
Language=German
OPC: Fehler I3004: Fehler während Verbindungsaufbau zu OPC Server %1; reason=(0x%3!08X!) %2
.
;//----

MessageId = 3007
;// %1    error code as HRESULT
;// %2    error code as message
Severity = error
SymbolicName = E_CIO_OPC_CANNOT_DISCONNECT
Language=English
OPC: error I3007: Cannot disconnect from OPC server reason=(0x%1!08X!) %2
.
Language=German
OPC: Fehler I3007: Kann Verbindung zu OPC Server nicht abbauen: reason=(0x%1!08X!) %2
.
;//----

MessageId = 3008
;// %1    group name
;// %2    server name
;// %3    error code as HRESULT
Severity = error
SymbolicName = E_CIO_OPC_CANNOT_ADD_GROUP_TO_SERVER
Language=English
OPC: error I3008: Error during activation of group %1 to OPC server %2; reason=0x%3!08X! - '%4'
.
Language=German
OPC: Fehler I3008: Fehler während Aktivierung der Gruppe %1 auf OPC Server %2; Fehler=0x%3!08X! - '%4'
.
;//----

MessageId = 3009
;// %1    variable name
;// %2    init string
Severity = error
SymbolicName = E_CIO_OPC_BAD_FORMAT_INIT_STRING
Language=English
OPC: error I3009: OPC variable '%1': The init string '%2' has unknown or invalid format!
.
Language=German
OPC: Fehler I3009: OPC Variable '%1': Zeichenkette zur Initialisierung '%2' ist unbekannt oder hat ungültiges Format.
.
;//----

MessageId = 3010
;// %1    variable name
;// %2    init value string
Severity = error
SymbolicName = E_CIO_OPC_BAD_FORMAT_INITIAL_VALUE
Language=English
OPC: error I3010: OPC variable '%1': The initial value string '%2' is invalid
.
Language=German
OPC: Fehler I3010: OPC variable '%1': The Initialwert '%2' ist ungültig
.
;//----

MessageId = 3011
;// %1    variable name
;// %2    machine name
;// %3    OPC server name
Severity = error
SymbolicName = E_CIO_OPC_UNKNOWN_SERVER_CONFIG
Language=English
OPC: error I3011: OPC variable '%1' references an unknown server '%3' on machine '%2'
.
Language=German
OPC: Fehler I3011: OPC Variable '%1' referenziert Unbekannten Server '%3' auf Maschine '%2'
.
;//----

MessageId = 3012
;// %1    server name
;// %2    group name
;// %3    item id
;// %4    error description
Severity = error
SymbolicName = E_CIO_OPC_CANNOT_ADD_ITEM
Language=English
OPC: error I3012: Cannot add OPC item '%3': server=%1, group=%2, description=%4
.
Language=German
OPC: error I3012: OPC Item '%3' kann nicht angemeldet werden: Server=%1, Gruppe=%2, Beschreibung=%4
.
;//----

MessageId = 3017
;// %1    server name
;// %2    group name
;// %3    Error description
Severity = error
SymbolicName = E_CIO_OPC_CANNOT_WRITE_ITEMS
Language=English
OPC: error I3017: Error while writing OPC items: Server=%1, group=%2, description=%3
.
Language=German
OPC: error I3017: Fehler bei Schreibvorgang von OPC Items: Server=%1, Gruppe=%2, Beschreibung=%3
.
;//----

MessageId = 3018
;// %1    item name
;// %2    error description
Severity = error
SymbolicName = E_CIO_OPC_WRITE_ITEM
Language=English
OPC: error I3017: Error while writing OPC item '%1': description=%2
.
Language=German
OPC: error I3017: Fehler bei Schreibvorgang von OPC Item '%1': Beschreibung=%3
.
;//----

MessageId = 3019
;// %1    datatype
Severity = error
SymbolicName = E_CIO_OPC_INVALID_DATATYPE
Language=English
OPC: error I3019: Invalid datatype '%1' for IO variable.
.
Language=German
OPC: Fehler I3019: Ungültiger Datentyp '%1' für IO-Variable.
.
;//----

MessageId = 3020
;// %1 datatype 
;// %2 bit size
Severity = error
SymbolicName = E_CIO_OPC_DATATYPE_MISMATCH
Language=English
OPC: error I3020: Datatype '%1' does not match specified bit size '%2!d!'.
.
Language=German
OPC: Fehler I3020: Datentyp '%1' stimmt nicht mit angegebener Bitgröße '%2!d!' überein.
.
;//----

MessageId = 3021
;// %1 full server name
;// %2 server error description
;// %3 error code as hexadecimal
Severity = error
SymbolicName = E_CIO_OPC_SERVER_ERROR
Language=English
OPC: error I3021: Server error 0x%3!08X! with server %1 - description: '%2'
.
Language=German
OPC: Fehler I3021: Server Fehler 0x%3!08X! mit Server %1 - Beschreibung: '%2'
.
;//----

MessageId = 3022
;// %1 file path
Severity = error
SymbolicName = E_CIO_OPC_NO_CHANNEL_PATH
Language=English
OPC: error I3022: Could not find channel number in config file '%1'
.
Language=German
OPC: Fehler I3022: Kann Kanalnummer in Konfigurationsdatei '%1' nicht finden
.
;//----

MessageId = 3023
;// %1 file path
Severity = error
SymbolicName = E_CIO_FILE_READONLY
Language=English
File is read only: %1
.
Language=German
Datei ist schreibgeschützt: %1
.

;//*****    Informational messages

MessageId = 3100
Severity = informational
SymbolicName = S_CIO_OPC_INIT_IO_LAYER
Language=English
OPC: Initializing IO layer for IO OPC servers...
.
Language=German
OPC: Initializing IO layer for IO OPC servers...
.
;//----

MessageId = 3101
;// %1    number of OPC sessions as integer !!
Severity = informational
SymbolicName = S_CIO_OPC_GOING_ONLINE
Language=English
OPC: Going ONLINE with >>%1!d!<< OPC server sessions
.
Language=German
OPC: Gehe ONLINE with >>%1!d!<< OPC Server Sitzungen
.
;//----

MessageId = 3102
;// %1    full server name
Severity = informational
SymbolicName = S_CIO_OPC_DISCONNECTING_SERVER
Language=English
OPC: Disconnecting OPC server %1...
.
Language=German
OPC: Verbindungabbau zu OPC server %1...
.
;//----

MessageId = 3103
;// %1    full server name
Severity = informational
SymbolicName = S_CIO_OPC_DISCONNECTED_SERVER
Language=English
OPC: OPC server %1 is disconnected
.
Language=German
OPC: Verbindung zu OPC server %1 ist abgebaut
.
;//----

MessageId = 3104
;// %1    server name
;// %2    group name
Severity = informational
SymbolicName = S_CIO_OPC_ATTACHED_GROUP
Language=English
OPC: Activated group %2 on OPC server %1
.
Language=German
OPC: Gruppe %2 auf OPC server %1 aktiviert
.
;//----

MessageId = 3106
;// %1    server name
;// %2    group name
;// %3    var count as int
Severity = informational
SymbolicName = S_CIO_OPC_TRY_ITEM_RECONNECT
Language=English
OPC: Trying to subscribe %3!d! unlinked items to OPC server %1, group %2
.
Language=German
OPC: Versuche %3!d! nicht verbundene OPC Items bei OPC Server %1, Gruppe %2 anzumelden
.
;//----

MessageId = 3109
;// %1    full server name
Severity = informational
SymbolicName = S_CIO_OPC_DISCONNECT_SERVER_TIMEOUT
Language=English
OPC: OPC server %1 did not disconnect within timeout
.
Language=German
OPC: Zeitüberschreitung bei Verbindungabbau zu OPC Server %1
.
;//----

MessageId = 3110
;// %1    Error code as HRESULT
Severity = informational
SymbolicName = S_CIO_UNKNOWN_OPC_ERROR_CODE
Language=English
unknown error code 0x%1!08X!
.
Language=German
unbekannter Fehlercode 0x%1!08X!
.
;//----

MessageId = 3111
;// %1    full server name
Severity = informational
SymbolicName = S_CIO_OPC_CONNECTED_SERVER
Language=English
OPC: OPC server %1 is connected
.
Language=German
OPC: OPC server %1 ist verbunden
.
;//----

MessageId = 3112
;// %1    full server name
Severity = informational
SymbolicName = S_CIO_OPC_REMOVE_SERVERCONFIG
Language=English
OPC: Remove configuration for OPC server %1
.
Language=German
OPC: Lösche Konfiguration für OPC server %1
.
;//----

MessageId = 3113
;// %1    full server name
;// %2    shutdown reason text
Severity = informational
SymbolicName = S_CIO_OPC_RECEIVED_SHUTDOWN
Language=English
OPC: Server %1 will shut down. Reason: %2
.
Language=German
OPC: Server %1 wird herunter fahren. Grund: %2
.
;//----

;//
;//==========================================================================================
;//

