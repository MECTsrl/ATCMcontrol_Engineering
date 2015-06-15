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
		CEEXE = 0x289 : E_FACILITY_CEEXE	
              )



;//currently supported languages:
LanguageNames=(
               English=0x409:CEExeMessagesENG
               German=0x407:CEExeMessagesDEU
              )

MessageIdTypedef=HRESULT
OutputBase=16



;//---------------
MessageId= 0
Severity = Error
Facility = CEEXE
SymbolicName = E_CEEXE_NO_THREAD
Language=English
ExeDelegator: Error: Could not create thread: %1
.
Language=German
ExeDelegator: Fehler: Thread konnte nicht erzeugt werden: %1
.

MessageId= 1
Severity = Warning
Facility = CEEXE
SymbolicName = E_CEEXE_NO_MSG_THREAD
Language=English
ExeDelegator: Warning: Could not create message thread
.
Language=German
ExeDelegator: Warnung: Message Thread konnte nicht erzeugt werden
.

MessageId= 2
Severity = Warning
Facility = CEEXE
SymbolicName = E_CEEXE_NO_PIPE
Language=English
ExeDelegator: Warning: Could not create message pipe
.
Language=German
ExeDelegator: Warnung: Message Pipe konnte nicht erzeugt werden
.
