
;//standard setting for severity:
SeverityNames=(
               Success=0x0
               Informational=0x1
               Warning=0x2
               Error=0x3
              )

;//predefined system wide facility codes are listed in 
;//inc\cFacility.h there E_FACILITY_LIBRARIAN is defined as 0x231
;//The Message Compiler can only use hardcoded values...
FacilityNames=(CL=0x231)

;//currently supported languages:
LanguageNames=(
               English=0x409:liberrENU
              )
OutputBase=16


;//---------------
MessageId = 0001
Severity = error
Facility = CL
SymbolicName = CL_E_OUTOFMEM
Language=english
error CL0001: out of memory
.

;//---------------
MessageId = 0002
Severity = error
Facility = CL
SymbolicName = CL_E_READ_FILE
Language=english
error CL0002: cannot read file '%1': %2
.

;//---------------
MessageId = 0003
Severity = error
Facility = CL
SymbolicName = CL_E_WRITE_FILE
Language=english
error CL0003: cannot write file '%1': %2
.
;//---------------
MessageId = 0004
Severity = error
Facility = CL
SymbolicName = CL_E_BAD_FILE_NAME
Language=english
error CL0004: create file/folder '%1': name contains invalid characters
.

;//---------------
MessageId = 0005
Severity = error
Facility = CL
SymbolicName = CL_E_CIM_NO_FILE_FOUND
Language=english
error CL0005: check in multiple '%1': no files found
.



;//---------------
MessageId = 0009
Severity = error
Facility = CL
SymbolicName = CL_E_CORRUPTED
Language=english
fatal error CL0009: archive corrupted!
.

;//---------------
MessageId = 0010
Severity = error
Facility = CL
SymbolicName = CL_E_READ_MAGIC
Language=english
error CL0010: failed to read magic header, not a library file or corrupted
.

;//---------------
MessageId = 0011
Severity = error
Facility = CL
SymbolicName = CL_E_VERSION_4CL
Language=english
error CL0011: version conflict: libary content: expected '%1' found '%2'
.

;//---------------
MessageId = 0012
Severity = error
Facility = CL
SymbolicName = CL_E_VERSION_V1X
Language=english
error CL0012: version conflict: library binary format seems to be V1.X
.

;//---------------
MessageId = 0013
Severity = error
Facility = CL
SymbolicName = CL_E_VERSION_BINARY
Language=english
error CL0013: version conflict: library binary format: expected '%1' found '%2'
.




;//---------------
MessageId = 0014
Severity = error
Facility = CL
SymbolicName = CL_E_NOT_OPEN_FOR_WRITE
Language=english
error CL0014: write archive: was not opened for writing
.

;//---------------
MessageId = 0015
Severity = error
Facility = CL
SymbolicName = CL_E_BUFFER_TO_SMALL
Language=english
error CL0015: check out file to memory: the buffer is too small
.

;//---------------
MessageId = 0016
Severity = error
Facility = CL
SymbolicName = CL_E_IN_ERROR
Language=english
error CL0016: reject archive operation after previous fatal error
.








