



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
               English=0x409:CGMessagesENU
              )
OutputBase=16



;//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;//pure information messages, starting from 10 up to 100
;//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageId= 10
Severity = Informational
Facility = CCG
SymbolicName = CG_I_COMPILE_FILES

Language=English
compile files...
.
;//---------------
MessageId= 11
Severity = Informational
Facility = CCG
SymbolicName = CG_I_LINKING

Language=English
linking...
.
;//---------------
MessageId= 12
Severity = Informational
Facility = CCG
SymbolicName = CG_I_RES_MEMORY

Language=English
memory usage resource '%1': %2 variables %3 IOs %4 retains
.

;//---------------
MessageId = 15
Severity = Informational
Facility = CCG
SymbolicName = CG_I_TO_MANY_ERRORS
Language=english
too many errors, stop compilation
.
;//---------------
MessageId = 17
Severity = Informational
Facility = CCG
SymbolicName = CG_I_PUT_LIB
Language=english
check in %1
.
;//---------------
MessageId = 18
Severity = Informational
Facility = CCG
SymbolicName = CG_I_MAKELIB_NEW
Language=english
creating new library '%1'
.
;//---------------
MessageId = 19
Severity = Informational
Facility = CCG
SymbolicName = CG_I_MAKELIB_APPEND
Language=english
appending to existing library '%1'
.
;//---------------
MessageId= 20
Severity = Informational
Facility = CCG
SymbolicName = CG_I_FINAL_ERR_SUM
Language=English

%1 error(s), %2 warning(s)

.
;//---------------
MessageId= 21
Severity = Informational
Facility = CCG
SymbolicName = CG_I_COMPILE_FILES_DEBUG
Language=English
compile files (generating debug code)...
.
;//---------------
MessageId= 22
Severity = Informational
Facility = CCG
SymbolicName = CG_I_COMPILE_FILE
Language=English
compile '%1'...
.
;//---------------
MessageId= 23
Severity = Informational
Facility = CCG
SymbolicName = CG_I_COMPILE_FILE_DEBUG
Language=English
compile '%1' (generating debug code)...
.






;//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageId = 0000  
Severity = error
Facility = CCG
SymbolicName = CG_E_INTERNAL
Language=english
fatal error C0000: internal compiler error: compiler file '%1' line %2. 
Assumed reason: %3
.
;//---------------
MessageId = 0001  
Severity = error
Facility = CCG
SymbolicName = CG_E_COM_ERROR
Language=english
fatal error C0001: bad return from OLE or System call: %1
.
;//---------------
MessageId = 0002
Severity = error
Facility = CCG
SymbolicName = CG_E_SETUP_REG
Language=english
fatal error C0002: reading info from registry %1%2%3%4: error during setup?
.
;//---------------
MessageId = 0003
Severity = error
Facility = CCG
SymbolicName = CG_E_SETUP_BAD4CVMEXE
Language=english
fatal error C0003: invalid exe 'engineering\bin\4cvm.exe' compiles only '%1' targets but actual target is '%2'
   - Please (re-)install the %2-AddOn - 
.
;//---------------
MessageId = 0004
Severity = error
Facility = CCG
SymbolicName = CG_E_CONSOLE_BAD_OS
Language=english
error C0004: 'Console' feature not supported for operating system '%1'
   To disable the Console select: Settings->Extras->Disable Console Information
.
;//---------------
MessageId = 1000
Severity = error
Facility = CCG
SymbolicName = CG_E_OUTOFMEM
Language=english
fatal error C1000: out of memory
.
;//---------------
MessageId = 1001
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_CMDL_LINE
Language=english
error C1001: bad command line, type "4cg -?" for brief help
.
;//---------------
MessageId = 1002
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_CMDL_FILETYPE
Language=english
error C1002: option '-%1': argument '%2' must have file extension '%3'
.
;//---------------
MessageId = 1003
Severity = error
Facility = CCG
SymbolicName = CG_E_CMDL_NEEDPARAM
Language=english
error C1003: compiler switch '-%1' needs a value
.
;//---------------
MessageId = 1004
Severity = error
Facility = CCG
SymbolicName = CG_E_CMDL_NO4CP
Language=english
error C1004: no 4cp file specified
.
;//---------------
MessageId = 1005
Severity = error
Facility = CCG
SymbolicName = CG_W_LICENSE_INTERNAL
Language=english
error C1005: license information for '%1' corrupted or missing (error code=%2)
   - Please (re-)install the License Manager! - 
.

;//---------------
MessageId = 1006
Severity = error
Facility = CCG
SymbolicName = CG_E_LICENSE_BUNDLE
Language=english
error C1006: license bundle restriction: target '%1' has remote address '%2' 
.

;//---------------
MessageId = 1007
Severity = warning
Facility = CCG
SymbolicName = CG_W_LICENSE_TEMP_EX
Language=english
warning C1007: the temporary product license for '%1' has expired 
   - switch to demo mode, no IO possible -
.

;//---------------
MessageId = 1008
Severity = error
Facility = CCG
SymbolicName = CG_E_LICENSE_TO_MANY_IOS
Language=english
error C1008: your current product license allows only %1 IO-points (used %2)
.

;//---------------
MessageId = 1009
Severity = error
Facility = CCG
SymbolicName = CG_E_LICENSE_TO_MANY_TRG
Language=english
error C1009: your current product license allows only %1 targets(s) (used %2)
.

;//---------------
MessageId = 1010
Severity = error
Facility = CCG
SymbolicName = CG_E_CMDL_UNKNOWN_OPT
Language=english
error C1010: unknown compiler option '%1'
.

;//---------------
MessageId = 1011
Severity = error
Facility = CCG
SymbolicName = CG_E_TEXT
Language=english
error C1011: %1
.

;//-----------------------------------------------------
MessageId = 1012
Severity = error
Facility = CCG
SymbolicName = CG_E_READ_LIB_HEADER
Language=english
fatal error C1012: internal error while reading '%1' from library '%2'
.

;//-----------------------------------------------------
MessageId = 1013
Severity = error
Facility = CCG
SymbolicName = CG_E_READ_LIB_DOMAIN
Language=english
fatal error C1013: internal error: could not extract domain '%1' from library '%2'
.

;//-----------------------------------------------------
MessageId = 1014
Severity = error
Facility = CCG
SymbolicName = CG_E_DOMAIN_LIST_FILE_NAME
Language=english
error C1014: input file '%1': only one '.%2' file allowed in project with fixed name '%3'
.

;//-----------------------------------------------------
MessageId = 1015
Severity = error
Facility = CCG
SymbolicName = CG_E_LIB_DOMAIN_BAD_VERSION
Language=english
error C1015: version conflict on library element '%1' (found version id '%2') 
.

;//-----------------------------------------------------
MessageId = 1016
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_WITH_EXT_TWICE
Language=english
error C1016: input file '%1': only one '.%2' file allowed in project
.

;//-----------------------------------------------------
MessageId = 1017
Severity = error
Facility = CCG
SymbolicName = CG_E_CUST_BUILD_ERROR
Language=english
error C1017: bad return from custom build step '%1'
.

;//-----------------------------------------------------
MessageId = 1018
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_FILE_FORMAT
Language=english
fatal error C1018: link-file seems to be corrupted, cannot check out binaries from library
.

;//---------------
MessageId = 1019
Severity = error
Facility = CCG
SymbolicName = CG_E_NO_TARGETTYPE_EXT
Language=english
error C1019: unknown target type '%1', known types are '%2'  
.

;//---------------
MessageId = 1020
Severity = error
Facility = CCG
SymbolicName = CG_E_CMDL_OPT_CONFLICT
Language=english
error C1020: option '%1' conflicts with other option '%2'
.

;//---------------
MessageId = 1021
Severity = error
Facility = CCG
SymbolicName = CG_E_PRJ_INVALID_FORMAT
Language=english
fatal error C1021: invalid project format %1, expected %2
.

;//---------------
MessageId = 1022
Severity = error
Facility = CCG
SymbolicName = CG_E_CMDL_NO_SUCH_TARGET
Language=english
error C1022: the project does not define a target named '%1'
.

;//---------------
MessageId = 1023
Severity = error
Facility = CCG
SymbolicName = CG_E_CMDL_OPT_NEEDS_TARGET
Language=english
error C1023: option '-%1' must be followed by ":<target type>"
.

;//---------------
MessageId = 1024
Severity = error
Facility = CCG
SymbolicName = CG_E_LIB_MERGE_IECIF_DIFFER
Language=english
error C1024: merge library: '%1' has different IEC interfaces
.

;//---------------






;//-----------------------------------------------------
;//CATEGORY: I/O errors 1100 - 1199
;//---------------
MessageId = 1100
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_OPEN
Language=english
error C1100: cannot open file '%1' for reading (system error='%2')
.

;//---------------
MessageId = 1101
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_DELETE
Language=english
error C1101: cannot delete file '%1' (file in use?)
.

;//---------------
MessageId = 1102
Severity = error
Facility = CCG
SymbolicName = CG_E_MKDIR_FAILED
Language=english
error C1102: failed to create directory '%1'
.

;//---------------
MessageId = 1103
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_NOT_EXIST
Language=english
error C1103: file '%1' does not exist 
.

;//---------------
MessageId = 1104
Severity = error
Facility = CCG
SymbolicName = CG_E_CLEANUP_FILES
Language=english
error C1104: cannot cleanup directory '%1' from previous build results (files read only?)
.

;//---------------
MessageId = 1105
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_STRUCTURE_ERROR
Language=english
error C1105: incorrect XML structure, reading element '%2' failed
.

;//---------------
MessageId = 1106
Severity = error
Facility = CCG
SymbolicName = CG_E_NAME_TOO_LONG
Language=english
error C1106: identifier or filename '%1' too long
.

;//---------------
MessageId = 1107
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_PARSE_ERROR
Language=english
error C1107: XML parse error: %1
.

;//---------------
MessageId = 1108
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_COPY
Language=english
error C1108: cannot copy file from '%1' to '%2'
.

;//---------------
MessageId = 1109
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_STRUCTURE_ERROR_ATTR
Language=english
error C1109: XML structure: attribute '%1' of element '%2' is required but not found
.

;//---------------
MessageId = 1110
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_STRUCTURE_ERROR_PCDATA
Language=english
error C1110: XML structure: element '%1': conversion of #PCDATA text failed
.

;//---------------
MessageId = 1111
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_STRUCTURE_ERROR_VALUE
Language=english
error C1111: XML structure: element '%1': value '%2' of attribute '%3' is illegal
.

;//---------------
MessageId = 1112
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_READ_ERROR
Language=english
error C1112: error reading file '%1'
.

;//---------------
MessageId = 1113
Severity = error
Facility = CCG
SymbolicName = CG_E_WRITE_FILE_FAILED
Language=english
fatal error C1113: cannot write file '%1' (disk full?)
.

;//---------------
MessageId = 1114
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_STRUCTURE_ROOT_NAME
Language=english
error C1114: XML structure: expected root element name '%1' but found '%2'
.

;//---------------
MessageId = 1115
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_MODIFIED
Language=english
error C1115: file '%1' modified since first seen (you must not change files during compile)
.

;//---------------
MessageId = 1116
Severity = error
Facility = CCG
SymbolicName = CG_E_LIB_FILE_WRT_FAILED
Language=english
error C1116: failed to put file '%1' to library '%2' 
.


;//---------------
MessageId = 1117
Severity = error
Facility = CCG
SymbolicName = CG_E_PPROT_CORRUPT
Language=english
error C1117: POU Protection: Project components for the POU protection are missing or corrupt. The project will not be built!
.

;//---------------
MessageId = 1118
Severity = error
Facility = CCG
SymbolicName = CG_E_PPROT_FILE_CHANGED
Language=english
error C1118: POU Protection: The protected source file %1 has been changed. The project will not be built!
.







;//-----------------------------------------------------
;//CATEGORY: syntactical errors 1200 - 1299
;//---------------
MessageId = 1200
Severity = error
Facility = CCG
SymbolicName = CG_E_ONLY_ONE_DUT    
Language=english
error C1200: file contains more than one structure declaration
.

;//---------------
MessageId = 1201
Severity = error
Facility = CCG
SymbolicName = CG_E_ATTRIB_VALUE_NOT_STRING
Language=english
error C1201: attribute value must be a string literal ".."
.

;//---------------
MessageId = 1202
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLEGAL_STMT_IN_FILE
Language=english
error C1202: illegal statement in this file type
.

;//---------------
MessageId = 1203
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLEGAL_STMT_IN_POU_TYP
Language=english
error C1203: illegal statement in this POU type
.

;//---------------
MessageId = 1204
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX_KW_USE
Language=english
error C1204: syntax error: illegal use of IEC1131-3 keyword '%1' at this place
.

;//---------------
MessageId = 1205
Severity = error
Facility = CCG
SymbolicName = CG_E_IDENT_SYNTAX
Language=english
error C1205: syntax error: '%1' is not a valid IEC1131-3 identifier
.

;//---------------
MessageId = 1206
Severity = error
Facility = CCG
SymbolicName = CG_E_RETTYPE_NO_FUN
Language=english
error C1206: function return type syntax for non function
.

;//---------------
MessageId = 1207
Severity = error
Facility = CCG
SymbolicName = CG_E_ID_TOO_LONG
Language=english
error C1207: identifier '%1' is too long, maximum is %2 characters
.

;//---------------
MessageId = 1208
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX_MISSING
Language=english
error C1208: missing '%1'
.

;//---------------
MessageId = 1209
Severity = error
Facility = CCG
SymbolicName = CG_E_ID_IS_KEWORD
Language=english
error C1209: Identifier '%1' is a reserved keyword
.

;//---------------
MessageId = 1210
Severity = error
Facility = CCG
SymbolicName = CG_E_STMTSYNTAX
Language=english
error C1210: statement list in transition
.

;//---------------
MessageId = 1211
Severity = error
Facility = CCG
SymbolicName = CG_E_NEWLINE_IN_STRING
Language=english
error C1211: string literal contains new line
.

;//---------------
MessageId = 1212
Severity = error
Facility = CCG
SymbolicName = CG_E_TRANSYNTAX
Language=english
error C1212: transition syntax
.

;//---------------
MessageId = 1213
Severity = error
Facility = CCG
SymbolicName = CG_E_DT_LITERAL 
Language=english
error C1213: DATE_AND_TIME literal '%1' is out of range  
.

;//---------------
MessageId = 1214
Severity = error
Facility = CCG
SymbolicName = CG_E_INT_LITERAL
Language=english
error C1214: invalid int literal '%1'
.

;//---------------
MessageId = 1215
Severity = error
Facility = CCG
SymbolicName = CG_E_TIME_LITERAL 
Language=english
error C1215: TIME literal '%1' is out of range  
.

;//---------------
MessageId = 1216
Severity = error
Facility = CCG
SymbolicName = CG_E_FLOAT_LITERAL
Language=english
error C1216: invalid float literal '%1'
.

;//---------------
MessageId = 1217
Severity = error
Facility = CCG
SymbolicName = CG_E_DATE_LITERAL 
Language=english
error C1217: DATE literal '%1' is out of range  
.

;//---------------
MessageId = 1218
Severity = error
Facility = CCG
SymbolicName = CG_E_BIT_LITERAL
Language=english
error C1218: invalid bit string '%1'
.

;//---------------
MessageId = 1219
Severity = error
Facility = CCG
SymbolicName = CG_E_TOD_LITERAL 
Language=english
error C1219: TIME_OF_DAY literal '%1' is out of range  
.

;//---------------
MessageId = 1220
Severity = error
Facility = CCG
SymbolicName = CG_E_EOF_IN_COMMENT
Language=english
error C1220: end of file in comment
.

;//---------------
MessageId = 1221
Severity = error
Facility = CCG
SymbolicName = CG_E_UNARY_PLUSMINUS_ON_EXPR
Language=english
error C1221: unary '%1' not allowed on expression of type '%2'
.

;//---------------
MessageId = 1222
Severity = error
Facility = CCG
SymbolicName = CG_E_YACC_STACK
Language=english
error C1222: parser stack overflow
.

;//---------------
MessageId = 1223
Severity = error
Facility = CCG
SymbolicName = CG_E_EXPECTED_LIT_OF_TYPE
Language=english
error C1223: expected a literal of type '%1' but found '%2'
.

;//---------------
MessageId = 1224
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX
Language=english
error C1224: syntax error
.

;//---------------
MessageId = 1225
Severity = error
Facility = CCG
SymbolicName = CG_E_CONSTFOLDING_OVERFLOW
Language=english
error C1225: constant folding '%1': over- or underflow occured
.

;//---------------
MessageId = 1226
Severity = error
Facility = CCG
SymbolicName = CG_E_ONLY_ONE_CON_PER_PRJ
Language=english
error C1226: second configuration: a project may contain only one configuration
.

;//---------------
MessageId = 1227
Severity = error
Facility = CCG
SymbolicName = CG_E_EXPR_NOT_CONST
Language=english
error C1227: expression cannot be evaluated, need a constant expression here
.

;//---------------
MessageId = 1228
Severity = error
Facility = CCG
SymbolicName = CG_E_IMPORT_BAD_EXTENSION
Language=english
error C1228: #import "%1": file must have extension '.%2', found '%3' instead
.

;//---------------
MessageId = 1229
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPED_LIT_TYPE_MISMATCH
Language=english
error C1229: typed literal '%1': type mismatch, value has wrong type '%2'
.

;//---------------
MessageId = 1230
Severity = error
Facility = CCG
SymbolicName = CG_E_TOKEN_TO_LONG
Language=english
error C1230: input token is too long to be processed
.

;//---------------
MessageId = 1231
Severity = error
Facility = CCG
SymbolicName = CG_E_UNTERM_STRING
Language=english
error C1231: unterminated string
.

;//---------------
MessageId = 1232
Severity = error
Facility = CCG
SymbolicName = CG_E_EOF_IN_NATIVE_BLOCK
Language=english
error C1232: end of file in NATIVE block, missing END_NATIVE ?
.

;//---------------
MessageId = 1233
Severity = error
Facility = CCG
SymbolicName = CG_E_NOT_A_CONST
Language=english
error C1233: '%1' is not the name of a CONST
.

;//---------------
MessageId = 1234
Severity = error
Facility = CCG
SymbolicName = CG_E_UNEXPECTED_EOF
Language=english
error C1234: unexpected end of file
.

;//---------------
MessageId = 1235
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX_EXPECTED_FOUND
Language=english
error C1235: syntax: expected '%1' but found '%2'
.

;//---------------
MessageId = 1236
Severity = error
Facility = CCG
SymbolicName = CG_E_MISPLACED_SYS_COMMENT
Language=english
error C1236: misplaced special comment '(*[[....]]*)'
.

;//---------------
MessageId = 1237
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX_UNKNOWNCHAR
Language=english
error C1237: unknown character %1
.

;//---------------
MessageId = 1238
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX_EXPECTED
Language=english
error C1238: syntax error: expected '%1'
.

;//---------------
MessageId = 1239
Severity = error
Facility = CCG
SymbolicName = CG_E_OLD_SYNTAX
Language=english
error C1239: old syntax: %1 in ATCMControl V2 use the following: %2
.

;//---------------
MessageId = 1240
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNTAX_UNEXPECTED
Language=english
error C1240: syntax: '%1' unexpected here
.

;//---------------
MessageId = 1241
Severity = error
Facility = CCG
SymbolicName = CG_E_INVALID_ESCAPE
Language=english
error C1241: string literal: invalid escape '$%1'
.

;//---------------
MessageId = 1242
Severity = error
Facility = CCG
SymbolicName = CG_E_GUID_STRING_SYNTAX
Language=english
error C1242: invalid GUID syntax. Example: "%1"
.

;//---------------
MessageId = 1244
Severity = error
Facility = CCG
SymbolicName = CG_E_ATTR_VALUE_TO_LONG
Language=english
error C1244: attribute value exceedes maximum size of %1 characters
.

;//---------------
MessageId = 1245
Severity = error
Facility = CCG
SymbolicName = CG_E_ATTR_VALUE_BAD
Language=english
error C1245: value '%1' is invalid for attribute '%2', expected '%3'
.

;//---------------
MessageId = 1248
Severity = error
Facility = CCG
SymbolicName = CG_E_MISSING_X_BEFORE_Y
Language=english
error C1248: syntax error: missing '%1' before '%2'
.

;//---------------
MessageId = 1249
Severity = error
Facility = CCG
SymbolicName = CG_E_MISSING_X_AFTER_Y
Language=english
error C1249: syntax error: missing '%1' after '%2'
.

;//---------------
MessageId = 1250
Severity = error
Facility = CCG
SymbolicName = CG_E_ADDR_SYNTAX
Language=english
error C1250: direct address '%1': invalid syntax, expected '%2'
.

;//---------------
MessageId = 1251
Severity = error
Facility = CCG
SymbolicName = CG_E_INVALID_ADDR
Language=english
error C1251: '%1' invalid: %2
.








    
    

;//-----------------------------------------------------
;//CATEGORY: ST declaration errors 1300 - 1499
;//---------------
MessageId = 1300
Severity = error
Facility = CCG
SymbolicName = CG_E_REDEF_VAR
Language=english
error C1300: declaration '%1': variable already defined in %2
.

;//---------------
MessageId = 1302
Severity = error
Facility = CCG
SymbolicName = CG_E_REDEF_VAR_CONST
Language=english
error C1302: declaration '%1': identifier already used as CONST in %2
.

;//---------------
MessageId = 1303
Severity = error
Facility = CCG
SymbolicName = CG_E_IMPORT_NAME_REDEFINED
Language=english
error C1303: #import "%1" redefines variable '%2' (first definition found in %3)
.

;//---------------
MessageId = 1304
Severity = error
Facility = CCG
SymbolicName = CG_E_FBI_CANNOT_CONSTANT
Language=english
error C1304: declaration '%1': function block instance must not be declared CONSTANT
.

;//---------------
MessageId = 1305
Severity = error
Facility = CCG
SymbolicName = CG_E_ADDR_ILL_USE
Language=english
error C1305: direct address '%1': illegal for declaration scope '%2'
.

;//---------------
MessageId = 1306
Severity = error
Facility = CCG
SymbolicName = CG_E_INVALID_DECL_SCOPE
Language=english
error C1306: declaration: illegal use of declaration scope '%1' in POU type '%2'
.

;//---------------
MessageId = 1307
Severity = error
Facility = CCG
SymbolicName = CG_E_INVALID_RETAIN_FUN
Language=english
error C1307: declaration: functions cannot have RETAIN/NON_RETAIN variables
.

;//---------------
MessageId = 1308
Severity = error
Facility = CCG
SymbolicName = CG_E_ANYTYPE_BAD_USE
Language=english
error C1308: declaration: illegal use of polymorphic data type '%1'
.

;//---------------
MessageId = 1309
Severity = error
Facility = CCG
SymbolicName = CG_E_DYNARRAY_BAD_USE
Language=english
error C1309: declaration: assumed size array can only be used as VAR_IN_OUT or function VAR_INPUT
.

;//---------------
MessageId = 1310
Severity = error
Facility = CCG
SymbolicName = CG_E_ADDR_ILL_MODIFIER
Language=english
error C1310: direct address '%1': modifiers are illegal for incomplete address definition
.

;//---------------
MessageId = 1311
Severity = error
Facility = CCG
SymbolicName = CG_E_ADDR_ILL_MODIFIER_SWAP
Language=english
error C1311: direct address '%1': modifier '%2' requires address size W,D or L
.

;//---------------
MessageId = 1312
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_AT_INOUT_VAR
Language=english
error C1312: declaration '%1': VAR_IN_OUT variable cannot be initialized
.

;//---------------
MessageId = 1313
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_STRLENGTH
Language=english
error C1313: string declaration: explicit length [%1] out of range (allowed: 1 to %2)  
.

;//---------------
MessageId = 1314
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_AT_FUN_INPUT
Language=english
error C1314: declaration '%1': function input cannot be initialized
.

;//---------------
MessageId = 1315
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_CONST_IN_ADDR
Language=english
error C1315: bad CONST inside direct address: '%1 (=%2)' must be of type ANY_INT with non negative value 
.

;//---------------
MessageId = 1316
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_AT_NON_LVALUE_ADR
Language=english
error C1316: declaration '%1': initial value is not allowed for direct address '%2' 
.

;//---------------
MessageId = 1317
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_STRLENGTH_TYPE
Language=english
error C1317: string length [<len>] declaration syntax for non string type
.

;//---------------
MessageId = 1318
Severity = error
Facility = CCG
SymbolicName = CG_E_VAR_IN_EXT_NOT_LAST_IN
Language=english
error C1318: declaration '%1': only one VAR_IN_EXT input variable allowed
.

;//---------------
MessageId = 1319
Severity = error
Facility = CCG
SymbolicName = CG_E_NEED_AT
Language=english
error C1319: declaration '%1': data type '%2' needs 'AT %%<incomplete address definition>'
.

;//---------------
MessageId = 1320
Severity = error
Facility = CCG
SymbolicName = CG_E_NEED_INCOMPLETE_ADDR
Language=english
error C1320: direct address '%1': data type '%2' needs incomplete address definition, but '%1' isn't
.

;//---------------
MessageId = 1321
Severity = error
Facility = CCG
SymbolicName = CG_E_IDEF_ADDR_ILL_USE2
Language=english
error C1321: incomplete address definition '%1': data type '%2' has no incomplete address references 
.

;//---------------
MessageId = 1322
Severity = error
Facility = CCG
SymbolicName = CG_E_IDEF_ADDR_ILL_USE
Language=english
error C1322: incomplete address definition '%1': illegal for declaration scope '%2'
.

;//---------------
MessageId = 1323
Severity = error
Facility = CCG
SymbolicName = CG_E_IREF_ADDR_ILL_USE
Language=english
error C1323: incomplete address reference '%1': illegal for declaration scope '%2'
.

;//---------------
MessageId = 1324
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_AT_NON_INVAR
Language=english
error C1324: initial values: only function block VAR_INPUT can be initialized from outside ('%1' is a %2) 
.

;//---------------
MessageId = 1325
Severity = error
Facility = CCG
SymbolicName = CG_E_MODIFIER_TWICE
Language=english
error C1325: same declaration modifier used twice 
.

;//---------------
MessageId = 1326
Severity = error
Facility = CCG
SymbolicName = CG_E_ONLY_DUT_IMPL
Language=english
error C1326: type declaration feature not supported, only 'STRUCT' is supported
.

;//---------------
MessageId = 1327
Severity = error
Facility = CCG
SymbolicName = CG_E_UNDEF_RESOURCE
Language=english
error C1327: undefined resource '%1'
.

;//---------------
MessageId = 1330
Severity = error
Facility = CCG
SymbolicName = CG_E_NO_DEF_FOUND
Language=english
error C1330: data type '%1' undefined
.

;//---------------
MessageId = 1331
Severity = error
Facility = CCG
SymbolicName = CG_E_4CL_FUN_REF
Language=english
error C1331: function '%1' is called in precompiled body, but no definition found
.

;//---------------
MessageId = 1332  
Severity = error
Facility = CCG
SymbolicName = CG_E_INVALID_POU_TYPE
Language=english
error C1332: expected type '%1', but '%2' is a '%3'
.

;//---------------
MessageId = 1334
Severity = error
Facility = CCG
SymbolicName = CG_E_POUNAME_FILENAME
Language=english
error C1334: POU name '%1' differs from file name '%2'
.

;//---------------
MessageId = 1335
Severity = error
Facility = CCG
SymbolicName = CG_E_POU_REDEF
Language=english
error C1335: POU '%1' already defined in '%2'
.

;//---------------
MessageId = 1336
Severity = error
Facility = CCG
SymbolicName = CG_E_ONLY_ONE_DIM
Language=english
error C1336: more dimensional arrays are not supported
.

;//---------------
MessageId = 1337
Severity = error
Facility = CCG
SymbolicName = CG_E_UNDEF_POU_TYP
Language=english
error C1337: undefined %1 '%2'
.

;//---------------
MessageId = 1338
Severity = error
Facility = CCG
SymbolicName = CG_E_MODI_ILL_USE
Language=english
error C1338: declaration: modifier '%1' cannot be used together with '%s'
.

;//---------------
MessageId = 1344
Severity = error
Facility = CCG
SymbolicName = CG_E_NOT_SUPPORTED
Language=english
error C1344: not supported: %1
.

;//---------------
MessageId = 1345
Severity = error
Facility = CCG
SymbolicName = CG_E_NOT_SUPP_TRG
Language=english
error C1345: target type '%1' does not support %2
.

;//---------------
MessageId = 1346
Severity = error
Facility = CCG
SymbolicName = CG_E_ILL_USE_BUILDIN
Language=english
error C1346: illegal call of buildin function '%1' (can only be used in ST)
.

;//---------------
MessageId = 1350
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLEGAL_TASK_INIT
Language=english
error C1350: unknown task attribute '%1', valid are: %2
.

;//---------------
MessageId = 1352
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_TASK_PRIO
Language=english
error C1352: task priority %1 is out of range, allowed range: %2..%3
.

;//---------------
MessageId = 1354
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_ATTR_DATATYPE
Language=english
error C1354: attribute value of '%1' has bad data type '%2' (need '%3')
.

;//---------------
MessageId = 1358
Severity = error
Facility = CCG
SymbolicName = CG_E_DOUBLE_ATTRIB
Language=english
error C1358: attribute '%1' already specified
.

;//---------------
MessageId = 1360
Severity = error
Facility = CCG
SymbolicName = CG_E_TASK_NAME_REQUIRED
Language=english
error C1360: missing task name for program instance '%1'
.

;//---------------
MessageId = 1362
Severity = error
Facility = CCG
SymbolicName = CG_E_TASK_IOSYNC_NOT_DPFBM
Language=english
error C1362: task '%1': 'IOSYNC=%2': need a Profibus-DP fiedbus master, '%3' isn't
.

;//---------------
MessageId = 1363
Severity = error
Facility = CCG
SymbolicName = CG_E_TASK_IOSYNC_BAD_CHANNEL
Language=english
error C1363: task '%1': 'IOSYNC=%2' channel number is not defined as IO-Component of '%3'
.

;//---------------
MessageId = 1364
Severity = error
Facility = CCG
SymbolicName = CG_E_TASK_UNDEFINED
Language=english
error C1364: task '%1' is not defined
.

;//---------------
MessageId = 1365
Severity = error
Facility = CCG
SymbolicName = CG_E_TASK_ATTR_COMBI
Language=english
error C1365: task '%1': illegal combination of task attributes
.

;//---------------
MessageId = 1366
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_TASK_INTERVAL_RANGE
Language=english
error C1366: task interval (=%1ms) is out of range, allowed range: %2ms..%3ms
.

MessageId = 1367
Severity = error
Facility = CCG
SymbolicName = CG_E_TASKNAME_IS_GLOBVAR
Language=english
error C1367: task name '%1' already used as global variable name in %2
.

;//---------------
MessageId = 1368
Severity = error
Facility = CCG
SymbolicName = CG_E_PRIO_OR_INTERVAL_MISSING
Language=english
error C1368: task priority or interval has not been specified
.

MessageId = 1369
Severity = error
Facility = CCG
SymbolicName = CG_E_PRGINSTNAME_IS_GLOBVAR
Language=english
error C1369: program instance name '%1' already used as global variable name in %2
.

;//---------------
MessageId = 1370
Severity = error
Facility = CCG
SymbolicName = CG_E_TASK_ALREADY_DEFINED
Language=english
error C1370: task '%1' already defined
.

;//---------------
MessageId = 1371
Severity = error
Facility = CCG
SymbolicName = CG_E_PRGINSTNAME_IS_TASKNAME
Language=english
error C1371: program instance name '%1' already used as task name
.
;//---------------
MessageId = 1373
Severity = error
Facility = CCG
SymbolicName = CG_E_SYMBOLIC_PC_UNDEF
Language=english
error C1373: the PC '%1' does not exist
.

;//---------------
;//TODO remove  1376 all %-addr diags are done by Configurator
MessageId = 1376
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_STRING_ADDR_DP
Language=english
error C1376: direct address '%1' of IEC type '%2' not valid for Profibus-DP fiedbus master
.

;//---------------
MessageId = 1378
Severity = error
Facility = CCG
SymbolicName = CG_E_VAR_NOT_ADDRESSABLE
Language=english
error C1378: declaration '%1': data type '%2' not allowed with direct address
.

;//---------------
MessageId = 1379
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLUSE_EDGE
Language=english
error C1379: type modifier R_EGDE/F_EDGE can only be applied to function block boolean inputs
.

;//---------------
MessageId = 1380
Severity = error
Facility = CCG
SymbolicName = CG_E_RECURSIV
Language=english
error C1380: recursive call or instantiation of '%1'
.

;//---------------
MessageId = 1382
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLEGAL_USE_OF_NATIVE
Language=english
error C1382: illegal use of '[VAR_]NATIVE'
.

;//---------------
MessageId = 1384
Severity = error
Facility = CCG
SymbolicName = CG_E_PRJ_UNKNOWN_KEY
Language=english
error C1384: unknown key '%1=' in section '%2'
.

;//---------------
MessageId = 1386
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_TYPE
Language=english
error C1386: initial values: type mismatch, initial value has type '%1' but variable is '%2'
.

;//---------------
MessageId = 1387
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_ADDR_INPUT
Language=english
error C1387: initial values: member '%1' AT '%2' cannot be initialized, direct address is an input location
.

;//---------------
MessageId = 1388
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_MEMBER_UNDEF
Language=english
error C1388: initial values: '%1' is not a member of '%2'
.

;//---------------
MessageId = 1389
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_MEMBER_NOACCESS
Language=english
error C1389: initial values: '%1' is not an VAR_INPUT of '%2'
.

;//---------------
MessageId = 1390
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_REPEAT_NOT_INT
Language=english
error C1390: initial values: array init repetition factor has bad type '%1', need ANY_INT 
.

;//---------------
MessageId = 1391
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_REPEAT_BAD_VALUE
Language=english
error C1391: initial values: repetition factor '%1' is out of range, allowed: 1..%2
.

;//---------------
MessageId = 1392
Severity = error
Facility = CCG
SymbolicName = CG_E_QVT_INIT_NOT_ALLOWED
Language=english
error C1392: initial values: QVT variables cannot be initialized
.



;//---------------
MessageId = 1394
Severity = error
Facility = CCG
SymbolicName = CG_E_CONST_NOT_SIMPLE_TYPE
Language=english
error C1394: CONST not possible for data type '%1', only ANY_SIMPLE allowed
.

;//---------------
MessageId = 1395
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_ON_VAR_EXTERN
Language=english
error C1395: declaration '%1': cannot initialize VAR_EXTERNAL
.

;//---------------
MessageId = 1396
Severity = error
Facility = CCG
SymbolicName = CG_E_NO_DEFAULT_INIT
Language=english
error C1396: data type '%1' does not have a default initial value
.










;//-----------------------------------------------------
;//CATEGORY: ST statement, expressions semantic,
;//          limits and external compiler errors 1500 - 1999
;//---------------
MessageId = 1500
Severity = error
Facility = CCG
SymbolicName = CG_E_BACKEND_ERROR
Language=english
error C1500: bad return from %1 compiler
.

;//---------------
MessageId = 1501
Severity = error
Facility = CCG
SymbolicName = CG_E_LIBRARIAN_ERROR
Language=english
error C1501: bad return from librarian: %1
.

;//---------------
MessageId = 1502
Severity = error
Facility = CCG
SymbolicName = CG_E_TOO_MANY_FUN_PARAMS
Language=english
error C1502: limit: more than %1 function arguments
.

;//---------------
MessageId = 1503
Severity = error
Facility = CCG
SymbolicName = CG_E_TOO_MANY_ARRAY_ELEMS
Language=english
error C1503: limit: too many array elements, found %1 max is %2
.

;//---------------
MessageId = 1504
Severity = error
Facility = CCG
SymbolicName = CG_E_MAX_JAVA_STACK_VARS
Language=english
error C1504: limit: body too big, need %1 java stack variables, maximum allowed is %2
.

;//---------------
MessageId = 1505
Severity = error
Facility = CCG
SymbolicName = CG_E_FILE_TO_LARGE
Language=english
error C1505: limit: input file '%1' too large to be processed
.

;//---------------
MessageId = 1506
Severity = error
Facility = CCG
SymbolicName = CG_E_EXIT 
Language=english
error C1506: EXIT outside a loop statement
.

;//---------------
MessageId = 1507
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_STMT_IN_SYNCBLOCK 
Language=english
error C1507: illegal statement inside SYNCHRONIZED block
.

;//---------------
MessageId = 1508
Severity = error
Facility = CCG
SymbolicName = CG_E_BAD_RANGE 
Language=english
error C1508: invalid range '%1'
.

;//---------------
MessageId = 1509
Severity = error
Facility = CCG
SymbolicName = CG_E_MAX_NESTING_CASE_FOR
Language=english
error C1509: control structures nested to deep, level='%1' allowed='%2
.

;//---------------
MessageId = 1510
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLEGAL_CALL
Language=english
error C1510: call on non-function block
.

;//---------------
MessageId = 1511
Severity = error
Facility = CCG
SymbolicName = CG_E_CLASS_FILE_TOO_BIG
Language=english
error C1511: limit: class file '%1' too big, code exeeds max size
.

;//---------------
MessageId = 1512
Severity = error
Facility = CCG
SymbolicName = CG_E_INIT_MISSING
Language=english
error C1512: VAR_IN_OUT formal parameter '%1' must be initialized
.

;//---------------
MessageId = 1513
Severity = error
Facility = CCG
SymbolicName = CG_E_DIVIDE_BY_ZERO
Language=english
error C1513: division by zero
.

;//---------------
MessageId = 1514
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_MEMBER
Language=english
error C1514: type mismatch: '%1' %2 '%3'
.

;//--------------- 
MessageId = 1515
Severity = error
Facility = CCG
SymbolicName = CG_E_FUNCALL_NOFUN
Language=english
error C1515:  '%1' is not the name of a function ('%1' is a '%2')
.

;//---------------
MessageId = 1516
Severity = error
Facility = CCG
SymbolicName = CG_E_LVAL_MEMBER
Language=english
error C1516: VAR_IN_OUT or VAR_OUTPUT parameter '%1' needs l-value
.

;//---------------
MessageId = 1517
Severity = error
Facility = CCG
SymbolicName = CG_E_NOVOID_FUNCTION
Language=english
error C1517: function '%1' returns '%2', return value not used
.

;//---------------
MessageId = 1518
Severity = error
Facility = CCG
SymbolicName = CG_E_ILL_MEMBER
Language=english
error C1518: member '%1' is neither VAR_INPUT nor VAR_IN_OUT
.

;//---------------
MessageId = 1519
Severity = error
Facility = CCG
SymbolicName = CG_E_MEMBER_NOT_OUT
Language=english
error C1519: member '%1' is not VAR_OUTPUT
.

;//---------------
MessageId = 1520
Severity = error
Facility = CCG
SymbolicName = CG_E_MULT_MEMBER
Language=english
error C1520: input '%1' was assigned more than once
.

;//---------------
MessageId = 1521
Severity = error
Facility = CCG
SymbolicName = CG_E_LABEL_IN_CONTROL_STMT
Language=english
error C1521: jump label '%1': labels not allowed inside control statements
.

;//---------------
MessageId = 1522
Severity = error
Facility = CCG
SymbolicName = CG_E_UNKNOWN_MEMBER
Language=english
error C1522: input/output '%1' unknown
.

;//---------------
MessageId = 1523
Severity = error
Facility = CCG
SymbolicName = CG_E_LABEL_DUPLY
Language=english
error C1523: jump label '%1': already defined above
.

;//---------------
MessageId = 1524
Severity = error
Facility = CCG
SymbolicName = CG_E_DIRECTVAR
Language=english
error C1524: direct variables may be used only in programs
.

;//---------------
MessageId = 1525
Severity = error
Facility = CCG
SymbolicName = CG_E_LABEL_UNDEF
Language=english
error C1525: jump label '%1': is undefined
.

;//---------------
MessageId = 1526
Severity = error
Facility = CCG
SymbolicName = CG_E_UNDEF_VAR
Language=english
error C1526: '%1': undeclared identifier
.

;//---------------
MessageId = 1527
Severity = error
Facility = CCG
SymbolicName = CG_E_ENDLESS_LOOP
Language=english
error C1527: endless loop
.

;//---------------
MessageId = 1528
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_SUBSCR
Language=english
error C1528: expected ANY_INT as array subscript '%1'
.

;//---------------
MessageId = 1529
Severity = error
Facility = CCG
SymbolicName = CG_E_SYNC_IN_FUN
Language=english
error C1529: synchronized statement not allowed in functions
.

;//---------------
MessageId = 1530
Severity = error
Facility = CCG
SymbolicName = CG_E_RANGE_SUBSCR
Language=english
error C1530: array subscript %1 is out of bounds
.

;//---------------
MessageId = 1531
Severity = error
Facility = CCG
SymbolicName = CG_E_INPUT_FB_CALL_IN_FUN
Language=english
error C1531: call of a FB passed as VAR_INPUT not allowed in a function
.

;//---------------
MessageId = 1532
Severity = error
Facility = CCG
SymbolicName = CG_E_ILL_DIMENSION
Language=english
error C1532: expected %1 subscripts
.

;//---------------
MessageId = 1534
Severity = error
Facility = CCG
SymbolicName = CG_E_NON_ARRAY
Language=english
error C1534: subscript on non-array
.

;//---------------
MessageId = 1535
Severity = error
Facility = CCG
SymbolicName = CG_E_NO_DEEPCOPY
Language=english
error C1535: cannot copy this structure. The author of '%1' disabled the assignment operation
.

;//---------------
MessageId = 1536
Severity = error
Facility = CCG
SymbolicName = CG_E_UNDEF_MEMBER
Language=english
error C1536: member '%1' of '%2' undefined
.

;//---------------
MessageId = 1538
Severity = error
Facility = CCG
SymbolicName = CG_E_PRIV_MEMBER
Language=english
error C1538: member '%1' of '%2' is inaccessible
.

;//---------------
MessageId = 1540
Severity = error
Facility = CCG
SymbolicName = CG_E_INPUT_MEMBER
Language=english
error C1540: member '%1' of '%2' may only be used in a call statement
.

;//---------------
MessageId = 1542
Severity = error
Facility = CCG
SymbolicName = CG_E_NON_STRUCT
Language=english
error C1542: structure selection on non-structure
.

;//---------------
MessageId = 1544
Severity = error
Facility = CCG
SymbolicName = CG_E_LVAL_PARAM
Language=english
error C1544: parameter %1 of '%2' needs non-CONSTANT expression
.

;//---------------
MessageId = 1546
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_PARAM
Language=english
error C1546: type mismatch: parameter %1 of '%2' needs type '%3' but actual is '%4' 
.

;//---------------
MessageId = 1547
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_ASSIGN
Language=english
error C1547: type mismatch in assignment: '%1' := '%2'
.

;//---------------
MessageId = 1548
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_FUNCT
Language=english
error C1548: type mismatch for '%1'
.

;//---------------
MessageId = 1550
Severity = error
Facility = CCG
SymbolicName = CG_E_ILL_PARAMS
Language=english
error C1550: no overload of '%1' takes %2 parameters
.

;//---------------
MessageId = 1551
Severity = error
Facility = CCG
SymbolicName = CG_E_MACRBODY_TWICE
Language=english
error C1551: macro body 'inline{...}': already defined
.

;//---------------
MessageId = 1552
Severity = error
Facility = CCG
SymbolicName = CG_E_UNKNOWN_FUNCT
Language=english
error C1552: unknown function '%1'
.

;//---------------
MessageId = 1553
Severity = error
Facility = CCG
SymbolicName = CG_E_POLYFUN_NEED_OVERLOAD
Language=english
error C1553: polymorph function '%1' needs 'OVERLOAD <type>(<type>,...)' body description
.

;//---------------
MessageId = 1554
Severity = error
Facility = CCG
SymbolicName = CG_E_WRONG_TYPE
Language=english
error C1554: expression of data type '%1' expected
.

;//---------------
MessageId = 1555
Severity = error
Facility = CCG
SymbolicName = CG_E_OVERLOAD_FUN_NOT_POLY
Language=english
error C1555: OVERLOAD: illegal in non polymorph function '%1'
.

;//---------------
MessageId = 1556
Severity = error
Facility = CCG
SymbolicName = CG_E_LVAL_FOR
Language=english
error C1556: FOR-loop variable is not writable
.

;//---------------
MessageId = 1558
Severity = error
Facility = CCG
SymbolicName = CG_E_FOR_VAR
Language=english
error C1558: FOR-loop variable must be of type SINT|INT|DINT
.

;//---------------
MessageId = 1559
Severity = error
Facility = CCG
SymbolicName = CG_E_POLYTYPE_VAR_USE
Language=english
error C1559: '%1' has type '%2', can access polymorph variables only inside NATIVE..END_NATIVE
.

;//---------------
MessageId = 1560
Severity = error
Facility = CCG
SymbolicName = CG_E_BODY_IN_POLY_FUN
Language=english
error C1560: body of polymorphic function may only contain 'overload <result>(<arg types>) inline{<native macro>}' directives
.

;//---------------
MessageId = 1561
Severity = error
Facility = CCG
SymbolicName = CG_E_FUN_OVERLOAD_NO_BODY_ARGC
Language=english
error C1561: no implementation found for function '%1' that takes %2 arguments
.

;//---------------
MessageId = 1562
Severity = error
Facility = CCG
SymbolicName = CG_E_FUN_OVERLOAD_NO_BODY
Language=english
error C1562: no implementation found for function overload '%1'
.

;//---------------
MessageId = 1563
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_NOT_SUPPORTED
Language=english
error C1563: only possible overload of function or operator yields expression of unsupported type: '%1'
.

;//---------------
MessageId = 1564
Severity = error
Facility = CCG
SymbolicName = CG_E_MACRO_EXPANSION
Language=english
error C1564: syntax error in inline code
.

;//---------------
MessageId = 1565
Severity = error
Facility = CCG
SymbolicName = CG_E_LIBFUNFB_ID_NOT_DEFINED
Language=english
error C1565: no ID defined for macro function '%1' in library '%2'
.

;//---------------
MessageId = 1566
Severity = error
Facility = CCG
SymbolicName = CG_E_NO_LIBFUNFBFILE_INLIB
Language=english
error C1566: cannot locate file '%1' in library '2.4cl'. This file is required for defining an id for a library function or function block
.

;//---------------
MessageId = 1567
Severity = error
Facility = CCG
SymbolicName = CG_E_INL_NOOP_TO_MANY_ARG
Language=english
error C1567: inline code '%1' supportes only functions or operators with one argument
.

;//---------------
MessageId = 1568
Severity = error
Facility = CCG
SymbolicName = CG_E_MULTIPLE_VAR_IN_EXT
Language=english
error C1567: multiple var extended input mismatch: the number of actual used var extended inputs must be a multiple of the number of formal var extended input
.





;//CATEGORY: "sfc" errors
;//---------------
MessageId = 1700
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_UNDEF
Language=english 
error C1700: SFC: %1 undefined
.

MessageId = 1701
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_REDEF
Language=english 
error C1701: SFC: redefinition of %1
.

;// trans, alt/sim Con/Div has no, multiple, only one successor or predecessor
MessageId = 1702
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_ELEM_ORDER
Language=english 
error C1702: SFC element order: %1%2%3
.

MessageId = 1703
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_UNSAFE
Language=english 
error C1703: SFC unsafe or unreachable: %1
.

MessageId = 1704
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_STEP_MULT_REF
Language=english
error C1704: SFC: step '%1' has multiple references in one transition
.


MessageId = 1706
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_ELEM_IN_ACTION
Language=english 
error C1706: SFC-element in action, action '%1'
.


MessageId = 1709
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_HOR_LINE_CONDIV
Language=english 
error C1709: SFC: horizontal line at convergence or divergence
.

MessageId = 1710
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_CONDIV_DIV_PREDSUCC
Language=english 
error C1710: SFC: convergence or divergence has mixed steps and transitions as %1
.

MessageId = 1711
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_CONDIV_STEPS_ONLY
Language=english 
error C1711: SFC: convergence or divergence has steps as predecessors and successors 
.

MessageId = 1712
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_CONDIV_NO_PREDSUCC
Language=english 
error C1712: SFC: convergence or divergence has no %1
.

MessageId = 1713
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_FBD_ELEM
Language=english 
error C1713: FBD-element in SFC
.

MessageId = 1714
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_UNUSED_ELEM
Language=english 
error C1714: unused element in SFC
.

MessageId = 1715
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_TRANS_EXPR_NET
Language=english 
error C1715: SFC: transition condition contains ST-expression and FBD-network
.

MessageId = 1716
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_TRANS_NO_EXPR_NET
Language=english 
error C1716: SFC: transition condition contains no ST-expression and no FBD-network
.

MessageId = 1717
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_TRANS_WRONG_ELEM
Language=english 
error C1717: SFC: %1 is not allowed in a transition condition FBD-network
.

MessageId = 1718
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_4GRV
Language=english 
error C1718: error creating SFC-visualisation
.


MessageId = 1719
Severity = error
Facility = CCG
SymbolicName = CG_E_SFC_MANY_EXCEPTION_ACTION
Language=english 
error C1718: sfc contains more than one sfc exception action
.








;//CATEGORY: "4gr" graphics  errors
;//---------------
MessageId = 2000
Severity = error
Facility = CCG
SymbolicName = CG_E_OVERLAPPING_NWS
Language=english
error C2000: overlapping networks
.

;//---------------
MessageId = 2001
Severity = error
Facility = CCG
SymbolicName = CG_E_CANNOT_EVAL_BOX
Language=english
error C2001: function or function block call cannot be evaluated (dataflow deadlock)
.

;//---------------
MessageId = 2002
Severity = error
Facility = CCG
SymbolicName = CG_E_MULTIBLE_OUTPUTS
Language=english
error C2002: signal connects more than one output
.

;//---------------
MessageId = 2003
Severity = error
Facility = CCG
SymbolicName = CG_E_BOX_UNCONNECTED
Language=english
error C2003: %1 '%2' is not connected
.

;//---------------
MessageId = 2004
Severity = error
Facility = CCG
SymbolicName = CG_E_FBTYPENAME_MISMATCH
Language=english
error C2004: function block type name differs from the POU header definition ('%1')
.

;//---------------
MessageId = 2005
Severity = error
Facility = CCG
SymbolicName = CG_E_FUNCT_IO_UNCONNECTED
Language=english
error C2005: function call: not all in/outputs are connected
.

;//---------------
MessageId = 2006
Severity = error
Facility = CCG
SymbolicName = CG_E_FUNCT_IO_NAME_UNDEF
Language=english
error C2006: function call: '%1' is not an input/output of '%2'
.

;//---------------
MessageId = 2007
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_MISMATCH_SIMPLE
Language=english
error C2007: type mismatch: %1 has type '%2' but connected signal is '%3'
.

;//---------------
MessageId = 2008
Severity = error
Facility = CCG
SymbolicName = CG_E_TYPE_MISMATCH_POLY
Language=english
error C2008: polymorph type '%1': not all connected signals have the same data type 
.

;//---------------
MessageId = 2009
Severity = error
Facility = CCG
SymbolicName = CG_E_VAROUT_NO_LVAL
Language=english
error C2009: '%1' is not writable  
.
;//---------------
MessageId = 2010
Severity = error
Facility = CCG
SymbolicName = CG_E_FEEDBACK_WRITTEN_FIRST
Language=english
error C2010: illegal use of feedback variable '%1': write before read
.

;//---------------
MessageId = 2011
Severity = error
Facility = CCG
SymbolicName = CG_E_PIN_WRONG_STORECLASS
Language=english
error C2011: %1: access type differs from POU header '%2'
.

;//---------------
MessageId = 2012
Severity = error
Facility = CCG
SymbolicName = CG_E_ILLEGAL_NEGATE_PIN
Language=english
error C2012: %1: only boolean VAR_INPUT/VAR_OUTPUT can be negated
.

;//---------------
MessageId = 2013
Severity = error
Facility = CCG
SymbolicName = CG_E_PIN_WRONG_EDGE_DECL
Language=english
error C2013: %1: R_EDGE/F_EDGE of this pin does not match the POU header
.

;//---------------
MessageId = 2014
Severity = error
Facility = CCG
SymbolicName = CG_E_INOUT_CONNECTION
Language=english
error C2014: %1: var_in_out must be connected to a var read box without signal split
.

;//---------------
MessageId = 2015
Severity = error
Facility = CCG
SymbolicName = CG_E_NO_OUTPUTS
Language=english
error C2015: signal connects only inputs
.

;//---------------
MessageId = 2016
Severity = error
Facility = CCG
SymbolicName = CG_E_VARINOUT_NOT_PAIR
Language=english
error C2016: VAR_IN_OUT '%1': missing left or right pin (invalid 4gr input?)
.

;//---------------
MessageId = 2017
Severity = error
Facility = CCG
SymbolicName = CG_E_MISSING_CONNECTOR
Language=english
error C2017: continuation '%1': there is no corresponding connector
.

;//---------------
MessageId = 2018
Severity = error
Facility = CCG
SymbolicName = CG_E_MISSING_CONTINUATION
Language=english
error C2018: connector '%1': there is no corresponding continuation
.

;//---------------
MessageId = 2019
Severity = error
Facility = CCG
SymbolicName = CG_E_CONNPAIR_NOT_UNIQUE
Language=english
error C2019: connector/continuation '%1': resuse of connection pair name '%1' 
.

;//---------------
MessageId = 2020
Severity = error
Facility = CCG
SymbolicName = CG_E_CONNPAIR_NAME_IS_VAR
Language=english
error C2020: connection pair name '%1' already used as %2
.

;//---------------
MessageId = 2021
Severity = error
Facility = CCG
SymbolicName = CG_E_LD_TRIGGER_ELEM_IN_FUN
Language=english
error C2021: transition sensing contact/coils not allowed in functions
.

;//---------------
MessageId = 2022
Severity = error
Facility = CCG
SymbolicName = CG_E_4GR_HEADER_MISMATCH
Language=english
error C2022: layout of '%1' does not match POU header: %2
.

;//---------------
MessageId = 2023
Severity = error
Facility = CCG
SymbolicName = CG_E_ENENO_ILLEGAL_PIN_LAYOUT
Language=english
error C2023: EN/ENO pins are at illegal position (invalid 4gr input?)
.

;//---------------
MessageId = 2024
Severity = error
Facility = CCG
SymbolicName = CG_E_EN_USAGE
Language=english
error C2024: EN/ENO usage: %1
.

;//---------------
MessageId = 2025
Severity = error
Facility = CCG
SymbolicName = CG_E_LD_CONTACT_UNUSED
Language=english
error C2025: contact '%1': useless contact, right signal not used
.

;//---------------
MessageId = 2026
Severity = error
Facility = CCG
SymbolicName = CG_E_LD_WIREDOR_UNCONNECTED
Language=english
error C2026: wired or: unconnected signal lines
.

;//---------------
MessageId = 2027
Severity = error
Facility = CCG
SymbolicName = CG_E_LD_LAYOUT
Language=english
error C2027: ladder layout: %1 
.




;//CATEGORY: "il" errors
;//---------------
MessageId = 2300
Severity = error
Facility = CCG
SymbolicName = CG_E_IL_ACC_UNDEF
Language=english 
error C2300: use of unitialized accumulator
.

;//---------------
MessageId = 2301
Severity = error
Facility = CCG
SymbolicName = CG_E_IL_INVALID_OP_IN_EXPRESSION
Language=english 
error C2301: invalid operator '%1' inside expression
.

;//---------------
MessageId = 2302
Severity = error
Facility = CCG
SymbolicName = CG_E_IL_LABEL_IN_EXPRESSION
Language=english
error C2302: jump label '%1': labels not allowed inside expression
.








;//-----------------------------------------------------
;//CATEGORY: linker errors 3000 - 3999
;//---------------
MessageId = 3000
Severity = error
Facility = CCG
SymbolicName = CG_E_RTCE_RESTRICTION
Language=english
error C3000: RTCE: restriction: %1%2%3
.

;//---------------
MessageId = 3001
Severity = error
Facility = CCG
SymbolicName = CG_E_RTCE_BAD_ADDR
Language=english
error C3001: RTCE: direct address '%1' not valid for IOSYNC DP-Master '%2'
.

;//---------------
MessageId = 3002
Severity = error
Facility = CCG
SymbolicName = CG_E_RTCE_SECOND_SYNC_TASK
Language=english
error C3002: RTCE: only one task may be synchronized to the same field bus master
.

;//---------------
MessageId = 3003
Severity = error
Facility = CCG
SymbolicName = CG_E_RTCE_GLOBVAR_REF_TWICE
Language=english
error C3003: RTCE: global variable '%1' used in more than one IOSYNC Task
.

;//---------------
MessageId = 3004
Severity = error
Facility = CCG
SymbolicName = CG_E_FBM_CHANNEL_NUMBER_REUSE
Language=english
error C3004: field bus master '%1': channel number %2 already assigned to master '%3'
.

;//---------------
MessageId = 3005
Severity = error
Facility = CCG
SymbolicName = CG_E_MAX_FBNESTING
Language=english
error C3005: instance path exceeds the maximal nesting level of %1, path='%2'
.

;//---------------
MessageId = 3006
Severity = error
Facility = CCG
SymbolicName = CG_E_GV_UNDEFINED
Language=english
error C3006: unresolved external '%1': variable not found in gvl of resource '%2'
.

;//---------------
MessageId = 3007
Severity = error
Facility = CCG
SymbolicName = CG_E_GV_IMPORT_NOT_IN_RES
Language=english
error C3007: gvl file '%1' is not imported in corresponding resource '%2'
.

;//---------------
MessageId = 3008
Severity = error
Facility = CCG
SymbolicName = CG_E_GV_TYPE_MISMATCH
Language=english
error C3008: declaration '%1': data type mismatch with VAR_GLOBAL declaration in '%2'
.

;//---------------
MessageId = 3009
Severity = error
Facility = CCG
SymbolicName = CG_E_READING_MAP_FILE
Language=english
error C3009: error while reading map file '%1' at line '%2'
.

;//---------------
MessageId = 3010
Severity = error
Facility = CCG
SymbolicName = CG_E_GV_CONSTANT_MISMATCH
Language=english
error C3010: declaration '%1': CONSTANT qualifier mismatch with VAR_GLOBAL declaration in %2
.

;//---------------
MessageId = 3011
Severity = error
Facility = CCG
SymbolicName = CG_E_GV_INPUT_WRITE
Language=english
error C3011: write access to input location '%1 AT %2'
.

;//---------------
MessageId = 3012
Severity = error
Facility = CCG
SymbolicName = CG_E_SEGM_EXCEEDS_MAX_SIZE
Language=english
error C3012: %1 segment to large. Used %2 bytes (maximum allowed %3)
.

;//---------------
MessageId = 3014
Severity = error
Facility = CCG
SymbolicName = CG_E_SECOND_PC_DEFINITION
Language=english
error C3014: PC '%1' already defined
.

;//---------------
MessageId = 3015
Severity = error
Facility = CCG
SymbolicName = CG_E_RTCE_ASS_ERR
Language=english
error C3015: RTCE: resource '%1': bad return from rtce assembler 
.

;//---------------
MessageId = 3016
Severity = error
Facility = CCG
SymbolicName = CG_E_MULT_TARGET_ADDR
Language=english
error C3016: target '%1': control address '%2' already used for '%3'
.

;//---------------
MessageId = 3017
Severity = error
Facility = CCG
SymbolicName = CG_E_NAME_CLASH_RESOURCE
Language=english
error C3017: name clash: resource named '%1' already defined in '%2'
.

;//---------------
MessageId = 3018
Severity = error
Facility = CCG
SymbolicName = CG_E_FBM_NOT_CONFIGURED
Language=english
error C3018: RTCE: IO-Component '%1' of PC '%2' is not configured (need at least a DP-Master and one slave)
.


;//---------------
MessageId = 3020
Severity = error
Facility = CCG
SymbolicName = CG_E_XML_TARGET_NOT_FOUND_IN_KAD
Language=english
fatal error C3020: 4cvm KAD missing entry: target '%1' not found (error during setup?)
.

;//---------------
MessageId = 3021
Severity = error
Facility = CCG
SymbolicName = CG_E_MULTIPLE_FBUS_NO_CHANNEL_NUM
Language=english
error C3021: multiple field busses used: field bus configuration for '%1' must have a channel number
.















;//-----------------------------------------------------
;//CATEGORY: syntactical warnings 
MessageId = 4000
Severity = warning
Facility = CCG
SymbolicName = CG_W_NESTED_COMMENT
Language=english
warning C4000: nested '(* ... *)' style comments
.

;//---------------
MessageId = 4001
Severity = warning
Facility = CCG
SymbolicName = CG_W_BAD_UNICODE_CHAR
Language=english
warning C4001: current codepage does not support the UNICODE character '%1'
.

;//---------------
MessageId = 4002
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNKNOWN_PRAGMA
Language=english
warning C4002: unknown pragma '%1'
.

;//---------------
MessageId = 4003
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_PROGRAM_AT_TASK 
Language=english
warning C4003: task '%1' has no programs to execute
.

;//---------------
MessageId = 4004
Severity = warning
Facility = CCG
SymbolicName = CG_W_IMPORT_FILE_TWICE     
Language=english
warning C4004: #import "%1": file is already imported
.

;//---------------
MessageId = 4005
Severity = warning
Facility = CCG
SymbolicName = CG_W_FILE_ALREADY_ATTACHED
Language=english
warning C4005: file '%1' already attached to project
.

;//---------------
MessageId = 4006
Severity = warning
Facility = CCG
SymbolicName = CG_W_CONST_NO_INIT
Language=english
warning C4006: CONST '%1' not initialized (using default initial value)
.

;//---------------
MessageId = 4007
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNKNOWN_ATTRIB
Language=english
warning C4007: unknown object attribute '%1'
.

;//---------------
MessageId = 4008
Severity = warning
Facility = CCG
SymbolicName = CG_W_BAD_ATTR_FOR_OBJ 
Language=english
warning C4008: attribute '%1' not valid for this object
.

;//---------------
MessageId = 4009
Severity = warning
Facility = CCG
SymbolicName = CG_W_ATTR_VAR_IGNORED
Language=english
warning C4009: object attributes for '%1 %2' will be ignored
.

;//---------------
MessageId = 4010
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_GUID_GIVEN
Language=english
warning C4010: no GUID specified, using default
.

;//---------------
MessageId = 4011
Severity = warning
Facility = CCG
SymbolicName = CG_W_SPECIFIED_ATTR_TWICE
Language=english
warning C4011: attribute '%1' already specified for this object, ignoring this one
.

;//---------------
MessageId = 4012
Severity = warning
Facility = CCG
SymbolicName = CG_W_TASK_INTERVAL_JITTER
Language=english
warning C4012: short task interval %1ms: tasks running below %2ms may have large jitter 
.

;//---------------
MessageId = 4013
Severity = warning
Facility = CCG
SymbolicName = CG_W_INIT_MEMBER_TWICE    
Language=english
warning C4013: initial values: member '%1' already initialized
.

;//---------------
MessageId = 4014
Severity = warning
Facility = CCG
SymbolicName = CG_W_LIB_ALREADY_INCL    
Language=english
warning C4014: library '%1' is already included
.

;//---------------
MessageId = 4015
Severity = warning
Facility = CCG
SymbolicName = CG_W_TYPE_SUBSCR
Language=english
warning C4015: array subscript %1 has ambiguous type '%2'
.

;//---------------
MessageId = 4016
Severity = warning
Facility = CCG
SymbolicName = CG_W_TYPE_PARAM    
Language=english
warning C4016: parameter %1 of '%2' has ambiguous type '%3'
.

;//---------------
MessageId = 4017
Severity = warning
Facility = CCG
SymbolicName = CG_W_TYPE_RESULT 
Language=english
warning C4017: result of '%1' has ambiguous type '%2'
.

;//---------------
MessageId = 4018
Severity = warning
Facility = CCG
SymbolicName = CG_W_ALLREADY_IN_SYNCBLOCK
Language=english
warning C4018: nested SYNCHRONIZED has no effect
.

;//---------------
MessageId = 4019
Severity = warning
Facility = CCG
SymbolicName = CG_W_INVALID_RETAIN_SPEC_IOI
Language=english
warning C4019: declaration '%1': RETAIN/NON_RETAIN modifier has no effect for IO-Inputs
.

;//---------------
MessageId = 4020
Severity = warning
Facility = CCG
SymbolicName = CG_W_INIT_ARRAY_TOO_LESSMUCH
Language=english
warning C4020: initial values: %1 initial values but %2 array elements
.
;//---------------
MessageId = 4021
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNUSED_LABEL
Language=english
warning C4021: jump label '%1': label never used
.

;//---------------
MessageId = 4022
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNREACHABLE_STMT
Language=english
warning C4022: unreachable statement after '%1'  
.

;//---------------
MessageId = 4023
Severity = warning
Facility = CCG
SymbolicName = CG_W_4CS_READ
Language=english
warning C4023: error reading project settings '%1' (searched section '%2')
.

;//---------------
MessageId = 4024
Severity = warning
Facility = CCG
SymbolicName = CG_W_ATTR_BAD_TYPE
Language=english
warning C4024: declaration '%1': object attribute '%2' only allowed for datatype ANY_SIMPLE
.

;//---------------
MessageId = 4025
Severity = warning
Facility = CCG
SymbolicName = CG_W_STR_TRUNC
Language=english
warning C4025: truncation of string literal of type '%1' due to assignment to variable of type '%2'
.

;//---------------
MessageId = 4026
Severity = warning
Facility = CCG
SymbolicName = CG_W_CMDL_OPT_TWICE
Language=english
warning C4026: compiler option '%1%2' given twice, ignore second one
.

;//---------------
MessageId = 4027
Severity = warning
Facility = CCG
SymbolicName = CG_W_FUN_NOT_ASSIGN_RET
Language=english
warning C4027: no value has been assigned to function return
.

;//---------------
MessageId = 4028
Severity = warning
Facility = CCG
SymbolicName = CG_W_FB_INPUT_READ_OUTSIDE_FB
Language=english
warning C4028: FB input read outside of FB
.

;//---------------
MessageId = 4029
Severity = warning
Facility = CCG
SymbolicName = CG_W_INPUT_FB_CALL
Language=english
warning C4029: call of a FB passed as VAR_INPUT
.

;//---------------
MessageId = 4030
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_DEEPCOPY
Language=english
warning C4030: inefficient code: The author of '%1' wants you to inform that a deep copy is not recommended
.

;//---------------
MessageId = 4031
Severity = warning
Facility = CCG
SymbolicName = CG_W_XML_STRUCTURE_ELEM_UNKNOWN
Language=english
warning C4031: XML structure: node named '%1' is unknown and will be ignored (version conflict?)
.

;//---------------
MessageId = 4032
Severity = warning
Facility = CCG
SymbolicName = CG_W_XML_ATTRVAL_WHITESPACE
Language=english
warning C4032: XML structure: attribute value '%1' has unexpected leading or trailing white space
.

;//---------------
MessageId = 4033
Severity = warning
Facility = CCG
SymbolicName = CG_W_CMDL_MLAL_DEFAULT_TARGET
Language=english
warning C4033: option '%1' ambiguous, assume default target type '%2'
.

;//---------------
MessageId = 4034
Severity = warning
Facility = CCG
SymbolicName = CG_W_OLD_SYNTAX
Language=english
warning C4034: old syntax: %1 in ATCMControl V2 better use the following: %2
.

;//---------------
MessageId = 4035
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_LANGTYPE_EXT
Language=english
warning C4035: ignore source file '%1': unknown source type (known types are: '%2') 
.

;//---------------
MessageId = 4036
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_TARGETTYPE_EXT
Language=english
warning C4036: ignore target '%1': unknown target type (known types are: '%2') 
.

;//---------------
MessageId = 4037
Severity = warning
Facility = CCG
SymbolicName = CG_W_FILEEXT_CLASSES    
Language=english
warning C4037: ignore input file '%1': invalid type '%2' for section '%3' (expected only '%4')
.

;//---------------
MessageId = 4038
Severity = warning
Facility = CCG
SymbolicName = CG_W_WRONG_CLASS_FOR_FILE    
Language=english
warning C4038: source file '%1' defines a '%2' and should therefore be in section '%3'
.

;//---------------
MessageId = 4039
Severity = warning
Facility = CCG
SymbolicName = CG_W_FUNOUT_USED_BEFORE_INIT
Language=english
warning C4039: function output '%1' possibly used before initialised; is this the intent?
.

;//---------------
MessageId = 4040
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNDEF_INSERT    
Language=english
warning C4040: undefined insert '$(%1)' 
.

;//--------------- remove this one !
MessageId = 4041
Severity = warning
Facility = CCG
SymbolicName = CG_W_SETUP_REG
Language=english
warning C4041: reading info from registry '%1' problems with setup? use default %2
.

;//---------------
MessageId = 4042
Severity = warning
Facility = CCG
SymbolicName = CG_W_SYNTAX_EXPECTED_FOUND
Language=english
warning C4042: syntax: expected '%1' but found '%2'
.

;//---------------
MessageId = 4043
Severity = warning
Facility = CCG
SymbolicName = CG_W_INVALID_ADDR
Language=english
warning C4043: '%1' invalid: %2
.



;//---------------
MessageId = 4044
Severity = warning
Facility = CCG
SymbolicName = CG_W_FLOAT_LIT_UNDERFLOW
Language=english
warning C4044: float literal '%1' underflow (smallest positive is: %2)
.


;//---------------
MessageId = 4045
Severity = warning
Facility = CCG
SymbolicName = CG_W_REG_READ
Language=english
warning C4045: failed to read registry Key='%1%2',  SubKey='%3' (hr=%4)
.

;//---------------
MessageId = 4046
Severity = warning
Facility = CCG
SymbolicName = CG_W_CASE_LABEL_DUPLY
Language=english
warning C4046: case label '%1': already used above
.

;//---------------
MessageId = 4048
Severity = warning
Facility = CCG
SymbolicName = CG_W_INST_INIT_CONSTANT
Language=english
warning C4048: instance initialisation: member '%1' is declared CONSTANT
.





;//---------------
MessageId = 4300
Severity = warning
Facility = CCG
SymbolicName = CG_W_SIGNAL_CYCLIC
Language=english
warning C4300: signal is cyclic
.

;//---------------
MessageId = 4301
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNCONNECTED_LINE
Language=english
warning C4301: unconnected line ends
.

;//---------------
MessageId = 4302
Severity = warning
Facility = CCG
SymbolicName = CG_W_RUBBER_LINE
Language=english
warning C4302: line is still a rubberline 
.

;//---------------
MessageId = 4303
Severity = warning
Facility = CCG
SymbolicName = CG_W_MULTIWRITE_IN_NW
Language=english
warning C4303: ambiguous network: '%1' gets written more than once
.

;//---------------
MessageId = 4304
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNUSED_ACTION
Language=english
warning C4304: action '%1' not used in sfc
.

;//---------------
MessageId = 4305
Severity = warning
Facility = CCG
SymbolicName = CG_W_TEXT
Language=english
warning C4305: %1
.

;//---------------
MessageId = 4306
Severity = warning
Facility = CCG
SymbolicName = CG_W_DEPRECATED
Language=english
warning C4306: '%1' is deprecated:%2
.

;//---------------
MessageId = 4307
Severity = warning
Facility = CCG
SymbolicName = CG_W_MISPLACED_PRAGMA
Language=english
warning C4307: misplaced pragma '%1'
.

;//---------------
MessageId = 4308
Severity = warning
Facility = CCG
SymbolicName = CG_W_VARIABLE_UNUSED
Language=english
warning C4308: unreferenced local variable(s): %1 
.

;//---------------
MessageId = 4309
Severity = warning
Facility = CCG
SymbolicName = CG_W_FBI_NOTCALLED
Language=english
warning C4309: '%1' : function block instance never called
.

;//---------------
MessageId = 4310
Severity = warning
Facility = CCG
SymbolicName = CG_W_LD_LAYOUT
Language=english
warning C4310: ladder layout: %1
.

;//---------------##AC
MessageId = 4311
Severity = warning
Facility = CCG
SymbolicName = CG_W_EXPR_NO_EFFECT
Language=english
warning C4311: expression '%1' has no effect %2
.

;//-----------------------------------------------------
;//CATEGORY: linker warnings 5000 
;//---------------
MessageId = 4600
Severity = warning
Facility = CCG
SymbolicName = CG_W_IL_ACC_NOT_SAVED
Language=english
warning C4600: current value of accumulator overwritten without being saved
.



;//-----------------------------------------------------
;//CATEGORY: linker warnings 5000 
;//---------------
MessageId = 5001
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_RESOURCE_FOR_TARGET
Language=english
warning C5001: stop compiling '%1': no resource assigned (no download possible)
.

;//---------------
MessageId = 5002
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_CONFIG
Language=english
warning C5002: stop compiling targets: no configuration found (no download possible)
.

;//---------------
MessageId = 5003
Severity = warning
Facility = CCG
SymbolicName = CG_W_CANNOT_PUT_LIB
Language=english
warning C5003: cannot put '%1' to library because POU contains VAR_EXTERNAL
.

;//---------------
MessageId = 5004
Severity = warning
Facility = CCG
SymbolicName = CG_W_UNKNOWN_RES_TYP
Language=english
warning C5004: unknown resource type '%1', assume default '%2'
.

;//---------------
MessageId = 5005
Severity = warning
Facility = CCG
SymbolicName = CG_W_LICENSE_DEMO_IO
Language=english
warning C5005: no license for '%1'  
   - switch to demo mode, no IO possible -
.

;//---------------
MessageId = 5006  
Severity = warning
Facility = CCG
SymbolicName = CG_W_PRJ_POU_ALREADY_DEF_IN_LIB
Language=english
warning C5006: POU '%1' of this project also is defined in library '%2', ignore library definition 
.

;//---------------
MessageId = 5007
Severity = warning
Facility = CCG
SymbolicName = CG_W_LIB_POU_ALREADY_DEF_IN_LIB
Language=english
warning C5007: %1: POU '%2' also defined in library '%3' (use definition in '%4')
.

;//---------------
MessageId = 5008
Severity = warning
Facility = CCG
SymbolicName = CG_W_GUID_REUSED
Language=english
warning C5008: reuse of global unique identifier '%1' from '%2' (did you copy the file ?)
.

;//---------------
MessageId = 5009
Severity = error
Facility = CCG
SymbolicName = CG_E_ADDR_NO_MASTER
Language=english
error C5009: direct address '%1': master with channel number '%2' not defined for target '%3'
.

;//---------------
MessageId = 5010
Severity = warning
Facility = CCG
SymbolicName = CG_W_ADDR_RANGE
Language=english
warning C5010: direct address '%1': invalid for master '%2'
.

;//---------------
MessageId = 5011
Severity = warning
Facility = CCG
SymbolicName = CG_W_ADDR_UNKNOWN_MODIFIER
Language=english
warning C5011: direct address '%1': ignore unknown modifier '%2'
.

;//---------------
MessageId = 5012
Severity = warning
Facility = CCG
SymbolicName = CG_W_ADDR_SIMULATION
Language=english
warning C5012: target '%1' at '%2' has no hardware, all IOs are treated as memory
.

;//---------------
MessageId = 5013
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_ADDR_FOR_PC
Language=english
warning C5013: stop linking '%1': PC '%2' has no machine name (no download possible) 
.

;//---------------
MessageId = 5014
Severity = warning
Facility = CCG
SymbolicName = CG_W_TOO_MANY_VAR_OBJS
Language=english
warning C5014: resource '%1' uses a total of '%2' variables (recommended maximum is %3)
.

;//---------------
MessageId = 5015
Severity = warning
Facility = CCG
SymbolicName = CG_W_TOO_MANY_RETAIN_ITEMS
Language=english
warning C5015: resource '%1' uses a total of %2 RETAIN variables (recommended maximum is %3)
.

;//---------------
MessageId = 5016
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_TARGETS_IN_4CP
Language=english
warning C5016: no targets found, nothing to build...
.

;//---------------
MessageId = 5017
Severity = warning
Facility = CCG
SymbolicName = CG_W_NO_IDFILE_IN_PRJDIR
Language=english
warning C5017: file '%1' does not exist in the project folder
.

;//---------------
MessageId = 5018
Severity = warning
Facility = CCG
SymbolicName = CG_W_DEMO_USED_IO
Language=english
warning C5018: DEMO MODE: target '%1': all IOs are treated as memory!
.

;//---------------
MessageId = 5019
Severity = warning
Facility = CCG
SymbolicName = CG_W_TARGET_NO_ADDR
Language=english
warning C5019: target '%1': no control address specified (no download possible)
.

;//---------------
MessageId = 5020
Severity = error
Facility = CCG
SymbolicName = CG_E_LICENSE_NOT_FOUND
Language=english
error C5020: no license found for target '%1'  
.




