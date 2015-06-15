;/*++
;H>> $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/4cvm.mc 1     28.02.07 19:04 Ln $
;----------------------------------------------------------------------------*
;
; =FILENAME            $Workfile: 4cvm.mc $
;                      $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/4cvm.mc $
;
; =PROJECT             CAK1020  ATCMControl V2.0
;
; =SWKE                4CCG
;
; =COMPONENT           4CG
;
; =CURRENT             $Date: 28.02.07 19:04 $
;                      $Revision: 1 $
;
; =ENVIRONMENT         MS Windows NT 4.0 / MS Windows 2000
;                      MSVC++ V6.0
;                      MS Java 3.2
;                      MS Visual SourceSafe 6.0
;                      Perl5
;
; =REFERENCES
;==
;----------------------------------------------------------------------------*
; =DESCRIPTION
;== Diagnostic messages of PC backend
;----------------------------------------------------------------------------*
; =MODIFICATIONS
; $History: 4cvm.mc $
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.02.07   Time: 19:04
; * Created in $/4CReleased/V2.20.00/TargetBase/CCG/4CVMLib
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.08.03   Time: 16:35
; * Created in $/4Control/TARGETBASE/CCG/4CVMlib
; * 
; * *****************  Version 1  *****************
; * User: Jd           Date: 28.02.02   Time: 10:53
; * Created in $/4Control/TargetBase/CCG/compiler
; * KM changes. Moved to new location.
; * 
; * *****************  Version 3  *****************
; * User: Ef           Date: 2.08.01    Time: 14:20
; * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
; * check file is now haneled by backends (autodecl !)
; * 
; * *****************  Version 2  *****************
; * User: Ef           Date: 26.07.01   Time: 16:13
; * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
; * bugt fixed
; * 
; * *****************  Version 1  *****************
; * User: Die          Date: 4.04.01    Time: 16:50
; * Created in $/4Control/COM/softing/fc/CCG/4vm/compiler
; * initial check in
; * 
; * *****************  Version 2  *****************
; * User: Ef           Date: 1.03.01    Time: 20:59
; * Updated in $/4Control/COM/softing/fc/CCG/4cpc
; * 
; * *****************  Version 1  *****************
; * User: Ef           Date: 15.02.01   Time: 15:04
; * Created in $/4Control/COM/softing/fc/CCG/4cg
; * initial check in
;==
;*****************************************************************************
;H<<
;--*/





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
               German=0x407:fcpcDEU
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
-c <file>
Check:
      Check only specified <file>, no output

-ml:<target type> [path]<libname>.4cl
  Make Library:
        Convert the project into a new <libname>.4cl.
        If the library already exists its old content will
        be removed. [path] may be $(LIBDIR)\

-i
  incremental build, default is rebuild all

-nologo                  
  suppress copyright message

-? (or -help)           
  displays this help text
.
Language=German
Verwendung:
      4cg <Projectname>.4cp {Targetliste} {-<opt> [<Optionsargument>]}

                    IEC 1131-3 Compiler Options
-------------------------------------------------------------------------------
-c:<target type> <file>
Prüfe (Check):
      Check only specified <file> using <target type>, no output

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
MessageId= 6001
Severity = Informational
Facility = CCG
SymbolicName = FCG_I_FINAL_ERR_SUM
Language=English

%1 error(s), %2 warning(s)

.
Language=German

%1 Fehler, %2 Warnung(en)

.





;/*++
;----------------------------------------------------------------------------*
;  $History: 4cvm.mc $
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.02.07   Time: 19:04
; * Created in $/4CReleased/V2.20.00/TargetBase/CCG/4CVMLib
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.08.03   Time: 16:35
; * Created in $/4Control/TARGETBASE/CCG/4CVMlib
; * 
; * *****************  Version 1  *****************
; * User: Jd           Date: 28.02.02   Time: 10:53
; * Created in $/4Control/TargetBase/CCG/compiler
; * KM changes. Moved to new location.
; * 
; * *****************  Version 3  *****************
; * User: Ef           Date: 2.08.01    Time: 14:20
; * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
; * check file is now haneled by backends (autodecl !)
; * 
; * *****************  Version 2  *****************
; * User: Ef           Date: 26.07.01   Time: 16:13
; * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
; * bugt fixed
; * 
; * *****************  Version 1  *****************
; * User: Die          Date: 4.04.01    Time: 16:50
; * Created in $/4Control/COM/softing/fc/CCG/4vm/compiler
; * initial check in
; * 
; * *****************  Version 2  *****************
; * User: Ef           Date: 1.03.01    Time: 20:59
; * Updated in $/4Control/COM/softing/fc/CCG/4cpc
; * 
; * *****************  Version 1  *****************
; * User: Ef           Date: 15.02.01   Time: 15:04
; * Created in $/4Control/COM/softing/fc/CCG/4cg
; * initial check in
;==
;----------------------------------------------------------------------------*
;
;--*/
