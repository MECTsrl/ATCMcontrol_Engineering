
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
		KADMAN = 0x281 : E_FACILITY_KADMAN	
              )



;//currently supported languages:
LanguageNames=(
               English=0x409:CEKadManMessagesENG
               German=0x407:CEKadManMessagesDEU
              )

MessageIdTypedef=HRESULT
OutputBase=16



;//---------------
MessageId= 0
Severity = Error
Facility = KADMAN
SymbolicName = E_FAILED_TO_ACCESS_XML
Language=English
FC_CEKADMAN: ???
.
Language=German
FC_CEKADMAN: ???
.

