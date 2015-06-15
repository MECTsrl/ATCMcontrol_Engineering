


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
;//CE_PROJMAN = 0x282 : E_FACILITY_PRJMAN
	CE_XREF = 0x28A : E_FACILITY_CEXREF	
              )



;//currently supported languages:
LanguageNames=(
               English=0x409:CEXREFMessagesENU
               German=0x407:CEXREFMessagesDEU
              )

MessageIdTypedef=HRESULT
OutputBase=16



;//---------------
MessageId= 1
Severity = Informational
Facility = CE_XREF
SymbolicName = E_CE_XREF_OPEN_XREFDATA
Language=English
CE_XREF: Xref data succesfully opened 
.
Language=German
CE_XREF: Die Xref Daten wurden erfolgreich geöffnet
.



;//---------------
MessageId= 2
Severity = Informational
Facility = CE_XREF
SymbolicName = E_CE_XREF_FINISH_LINK
Language=English
CE_XREF: Xref data succesfully linked
.
Language=German
CE_XREF: Die Xref Daten wurden erfolgreich gelinkt
.

;//---------------
MessageId= 11
Severity = Warning
Facility = CE_XREF
SymbolicName = E_CE_XREF_WRONG_BINARY_FORMAT
Language=English
CE_XREF: Wrong Version of Xref data, new Build required
.
Language=German
CE_XREF: Die Xref Daten haben die falsche Version, eine Neukompilierung ist erforderlich
.

