




;//standard setting for severity:
SeverityNames=(
               Success=0x0
               Informational=0x1
               Warning=0x2
               Error=0x3
              )

;//predefined system wide facility codes are listed in 
;//inc\cFacility.h there E_FACILITY_CODEGEN is defined as 0x230
;//The Message Compiler can only use hardcoded values...
FacilityNames=(CCG=0x230)

;//currently supported languages:
LanguageNames=(
               English=0x409:cgframeENU
              )
OutputBase=16



;//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;//pure information messages
;//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageId = 6000
Severity = Informational
Facility = CCG
SymbolicName = FCG_I_USAGE
Language=english
Usage:
      4cg <projectname>.4cp {target-list} {-<opt> [<option argument>]}
        or 
      4cg -<lib command> {<lib>.4cl}

--- Build options:
-c <file>
  Check:
      Check only specified <file>, no output

-ml[:<target type>] [path]<libname>.4cl
  Make Library:
        Convert the project into a new <libname>.4cl.
        If the library already exists its old content will
        be removed. [path] may be $(LIBDIR)\
        <target type> is one of the installed targets
        and may be ommitted if only one installed.

-i
  incremental build, default is rebuild all



--- Library commands:
-al [path]<source>.4cl [path]<dest>.4cl
  Append Library:
        Merges all target code of <source>.4cl into <dest>.4cl.

-rl:<targets type> [path]<source>.4cl
  Remove Library:
        Removes all <targets type> specific code from a library.
        
-ll [path]<source>.4cl
  List Library:
        Prints library content summary.

-lll [path]<source>.4cl
  List Long Library:
        Prints library content summary including all stored files.


--- Miscellaneous:
-nologo                  
  suppress copyright message

-? (or -help)           
  displays this help text
.

;//---------------
MessageId= 6001
Severity = Informational
Facility = CCG
SymbolicName = FCG_I_FINAL_ERR_SUM
Language=English

%1 error(s), %2 warning(s)

.



