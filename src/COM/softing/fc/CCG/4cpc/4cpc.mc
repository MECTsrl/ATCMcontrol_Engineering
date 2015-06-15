



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
               English=0x409:fcpcENU
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

                    IEC 1131-3 Compiler Options
-------------------------------------------------------------------------------
-c[:4cpc] <file>
Check:
      Check only specified <file>, no output

-ml[:<target type>] [path]<libname>.4cl
  Make Library:
        Convert the project into a new <libname>.4cl.
        If the library already exists its old content will
        be removed. [path] may be $(LIBDIR)\
        target type is one of PC,PAM
        If not specified it's the PC target by default

-al[:<target type>] [path]<libname>.4cl
  Append Library:
        Same as -ml but works additive, existing library will not be removed

-i
  incremental build, default is rebuild all

-nologo                  
  suppress copyright message

-? (or -help)           
  displays this help text
.


;//---------------
MessageId = 6000
Severity = error
Facility = CCG
SymbolicName = CGB_E_NO_IOCONFIG
Language=english
error 4CPC6000: no IO-Configurator available for field bus type '%1' (<FBUS LINK="%2"/>)
.

;//---------------
MessageId = 6001
Severity = error
Facility = CCG
SymbolicName = CGB_E_MULT_TARGETVERSION_NOTSUP
Language=english
error 4CPC6001: not supported: build multible targtes with different firmware versions (build targets individually!)
.






