;//predefined system wide facility codes are listed in 
;//inc\cFacility.h there E_FACILITY_CODEGEN is defined as 0x230
;//The Message Compiler can only use hardcoded values...
FacilityNames=(CCG=0x230)

;//currently supported languages:
LanguageNames=(English=0x409:ASMMessagesENU)
OutputBase=16



;//pure information messages, starting from 10 up to 100
MessageId= 10
Severity = Informational
Facility = CCG
SymbolicName = E_4CCG_INF_COMPILE_FILES

Language=English
compile files...
.


;//---------------
MessageId= 13
Severity = Informational
Facility = CCG
SymbolicName = E_4CCG_FINAL_ERR_SUM

Language=English

%1 error(s), %2 warning(s)

.

;//---------------
MessageId = 15
Severity = Informational
Facility = CCG
SymbolicName = E_4CCG_ERR_TO_MANY_ERRORS
Language=english
too many errors, stop compilation
.


MessageId = 20
Severity = Informational
Facility = CCG
SymbolicName = E_4CCG_INF_CMD_LINE_HELP
Language=english
Usage: 
      4asm <resource name>.4asm 

4asm: compiles the .4asm file into .rtce interpreter code

                    ATCMControl Assembler Options
------------------------------------------------------------------------------- 
-nologo                     Suppress copyright message
-? (or -help)               Displays this help text
.



;//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MessageId = 0000  
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INTERNAL
Language=english
fatal error C0000: internal compiler error: compiler file '%1' line %2
.
;//---------------
MessageId = 0001  
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_COM_ERROR
Language=english
fatal error C0001: bad return from OLE or System call: '%1': '%2'
.
;//---------------
MessageId = 1000
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_OUTOFMEM
Language=english
fatal error C1000: out of memory
.
;//---------------
MessageId = 1001
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_CMD_LINE
Language=english
error C1001: bad command line, type "4asm -?" for brief help
.
;//---------------
MessageId = 1002
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_CML_FILETYPE
Language=english
error C1002: argument '%1' must have file extension '%2'
.
;//---------------
MessageId = 1003
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_CML_NEEDPARAM
Language=english
error C1003: compiler switch '-%1' needs a value
.
;//---------------
MessageId = 1004
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_CML_NOASM
Language=english
error C1004: no 4asm file specified, type "4asm -?" for brief help 
.




;//---------------
MessageId = 1010
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_CMD_UNKNOWN_OPT
Language=english
error C1010: unknown option '%1', type "4asm -?" for brief help
.

;//-----------------------------------------------------
MessageId = 1017
Severity = error
Facility = CCG
SymbolicName = E_4ASM_ERR_CMDL_OPTION_NOT_COMPINE
Language=english
error C1017: command line option '%1' cannot be used with option '%2'
.
;//---------------
MessageId = 1018
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_CMD_LINE_OPTION
Language=english
error C1018: bad command line syntax '%1', correct syntax is '%2'
.
;//---------------
MessageId = 1019
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TOO_MANY_OBJECTS
Language=english
error C1019: too many instance objects. Maximum is %1 (used %2)
.

;//---------------
MessageId = 1020
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TOO_MANY_SECTION
Language=english
error C1020: too many '%1 section in instance '%2'
.

;//---------------
MessageId = 1021
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_CPY_SECTION
Language=english
error C1021: not supported: .copy %1 %2: item %3 has bit offset or size
.

;//---------------
MessageId = 1022
Severity = error
Facility = CCG
SymbolicName = E_ERR_SECTION_DUPLY
Language=english
error C1022: second definition of section '%1'
.



;//-----------------------------------------------------
;//CATEGORY: I/O errors 1100 - 1199
;//---------------
MessageId = 1100
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILE_OPEN
Language=english
error C1100: cannot open file '%1'
.
;//---------------
MessageId = 1101
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILE_DELETE
Language=english
error C1101: cannot delete file '%1' (file in use?)
.
;//---------------
MessageId = 1102
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_MKDIR_FAILED
Language=english
error C1102: failed to create directory '%1'
.
;//---------------
MessageId = 1103
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILE_NOT_EXIST
Language=english
error C1103: file '%1' does not exist 
.
;//---------------
MessageId = 1104
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_CLEANUP_FILES
Language=english
error C1104: cannot cleanup directory '%1' from previous build results (files read only?)
.
;//---------------
MessageId = 1106
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_NAME_TOO_LONG
Language=english
error C1106: identifier or filename '%1' too long
.

;//---------------
MessageId = 1108
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILE_COPY
Language=english
error C1108: cannot copy file from '%1' to '%2'
.

;//---------------
MessageId = 1112
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILE_READ_ERROR
Language=english
error C1112: error reading file '%1'
.
;//---------------
MessageId = 1113
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_WRITE_FILE_FAILED
Language=english
fatal error C1113: cannot write file '%1' (disk full?)
.








;//-----------------------------------------------------
;//CATEGORY: syntactical errors 1200 - 1299
;//---------------
MessageId = 1200
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_OPCODE
Language=english
error C1200: opcode '%1': must be of type 'dint' and in range %2..%3
.
;//---------------
MessageId = 1201
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_OPCODE_FOR_NON_FUNFB
Language=english
error C1201: opcode only allowed for .fun and .fb
.
;//---------------
MessageId = 1202
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_OPCODE_AND_CODE
Language=english
error C1202: opcode or code, not both
.
;//---------------
MessageId = 1203
Severity = error
Facility = CCG
SymbolicName = E_ASM_TOO_MANY_PARAM
Language=english
error C1203: too many parameters for opcode '%1'
.
;//---------------
MessageId = 1204
Severity = error
Facility = CCG
SymbolicName = E_ASM_TOO_LESS_PARAM
Language=english
error C1204: opcode '%1' needs more parameters
.
;//---------------
MessageId = 1205
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_IDENT_SYNTAX
Language=english
error C1205: syntax error: '%1' is not a valid IEC1131-3 identifier
.
;//---------------
MessageId = 1206
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ATTR_VAL_RANGE
Language=english
error C1206: attribute '%1': value '%2' out of range (allowed range %3..%4)
.
;//---------------
MessageId = 1207
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ID_TOO_LONG
Language=english
error C1207: identifier '%1' is too long, maximum is %2 characters
.
;//---------------
MessageId = 1208
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_MISSING
Language=english
error C1208: missing '%1'
.

;//---------------
MessageId = 1210
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ADDR_RANGE
Language=english
error C1210: direct address '%1': out of range, allowed: '%2=%3 bytes'
.
;//---------------
MessageId = 1211
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_NEWLINE_IN_STRING
Language=english
error C1211: string literal contains new line
.


;//---------------
MessageId = 1213
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_DT_LITERAL 
Language=english
error C1213: DATE_AND_TIME literal '%1' is out of range  
.
;//---------------
MessageId = 1214
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INT_LITERAL
Language=english
error C1214: invalid int literal '%1'
.
;//---------------
MessageId = 1215
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TIME_LITERAL 
Language=english
error C1215: TIME literal '%1' is out of range  
.
;//---------------
MessageId = 1216
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FLOAT_LITERAL
Language=english
error C1216: invalid float literal '%1'
.
;//---------------
MessageId = 1217
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_DATE_LITERAL 
Language=english
error C1217: DATE literal '%1' is out of range  
.
;//---------------
MessageId = 1218
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BIT_LITERAL
Language=english
error C1218: invalid bit string '%1'
.
;//---------------
MessageId = 1219
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TOD_LITERAL 
Language=english
error C1219: TIME_OF_DAY literal '%1' is out of range  
.
;//---------------
MessageId = 1220
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_EOF_IN_COMMENT
Language=english
error C1220: end of file in comment
.
;//---------------
MessageId = 1221
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_NEED_LOCAL
Language=english
error C1221: parameter %1 of '%2' must be a POU local variable or byte offset literal
.
;//---------------
MessageId = 1222
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_YACC_STACK
Language=english
error C1222: parser stack overflow
.
;//---------------
MessageId = 1223
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_EXPECTED_LIT_OF_TYPE
Language=english
error C1223: expected a literal of type '%1' but found '%2'
.
;//---------------
MessageId = 1224
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_SYNTAX
Language=english
error C1224: syntax error
.
;//---------------
MessageId = 1225
Severity = error
Facility = CCG
SymbolicName = ST_ERR_CONSTFOLDING_OVERFLOW
Language=english
error C1225: constant folding '%1': over- or underflow occured
.
;//---------------
MessageId = 1226
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ADDR_ALIGNMENT
Language=english
error C1226: direct address '%1': bad alignment (bitoffset mod bitsize >0)
.
;//---------------
MessageId = 1227
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_EXPR_NOT_CONST
Language=english
error C1227: expression cannot be evaluated, need a constant expression here
.
;//---------------
MessageId = 1229
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TYPED_LIT_TYPE_MISMATCH
Language=english
error C1229: typed literal '%1': type mismatch, value has wrong type '%2'
.
;//---------------
MessageId = 1230
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TOKEN_TO_LONG
Language=english
error C1230: input token is too long to be processed
.
;//---------------
MessageId = 1231
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNTERM_STRING
Language=english
error C1231: unterminated string
.


;//---------------
MessageId = 1234
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNEXPECTED_EOF
Language=english
error C1234: unexpected end of file
.
;//---------------
MessageId = 1235
Severity = error
Facility = CCG
SymbolicName = E_4CCG_EXPECTED_FOUND
Language=english
error C1235: expected '%1' but found '%2'
.

;//---------------
MessageId = 1237
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_SYNTAX_UNKNOWNCHAR
Language=english
error C1237: unknown character %1
.
;//---------------
MessageId = 1238
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_EXPECTED
Language=english
error C1238: expected '%1'
.

;//---------------
MessageId = 1240
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNEXPECTED
Language=english
error C1240: '%1' unexpected here
.
;//---------------
MessageId = 1241
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INVALID_ESCAPE
Language=english
error C1241: string literal: invalid escape '$%1'
.

;//---------------
MessageId = 1248
Severity = error
Facility = CCG
SymbolicName = ST_MISSING_X_BEFOR_Y
Language=english
error C1248: syntax error: missing '%1' before '%2'
.

;//---------------
MessageId = 1250
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ADDR_SYNTAX
Language=english
error C1250: direct address '%1': invalid syntax, expected '%2'
.

    
    

;//-----------------------------------------------------
;//CATEGORY: ST declaration errors 1300 - 1499
;//---------------
MessageId = 1300
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_REDEF_VAR
Language=english
error C1300: declaration '%1': variable already defined in %2
.

;//---------------
MessageId = 1305
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ADDR_ILL_USE
Language=english
error C1305: direct address '%1': illegal for declaration scope '%2'
.
;//---------------
MessageId = 1306
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INVALID_DECL_SCOPE
Language=english
error C1306: declaration: illegal use of declaration scope '%1' in POU type '%2'
.

;//---------------
MessageId = 1310
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ADDR_ILL_MODIFIER
Language=english
error C1310: direct address '%1': modifiers are illegal for incomplete address definition
.
;//---------------
MessageId = 1311
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ADDR_ILL_MODIFIER_SWAP
Language=english
error C1311: direct address '%1': modifier '%2' requires WORD or DWORD address size
.
;//---------------
MessageId = 1312
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_AT_INOUT_VAR
Language=english
error C1312: declaration '%1': VAR_IN_OUT variable cannot be initialized
.
;//---------------
MessageId = 1313
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_STRLENGTH
Language=english
error C1313: string declaration: explicit length [%1] out of range (allowed: 1 to %2)  
.
;//---------------
MessageId = 1314
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_AT_FUN_INPUT
Language=english
error C1314: declaration '%1': function input cannot be initialized
.
;//---------------
MessageId = 1316
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_AT_NON_LVALUE_ADR
Language=english
error C1316: initial value is not allowed for IO-Input '%1' 
.
;//---------------
MessageId = 1318
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_VAR_IN_EXT_NOT_LAST_IN
Language=english
error C1318: declaration '%1': VAR_IN_EXT must not be followed by another VAR_INPUT or VAR_IN_EXT
.
;//---------------
MessageId = 1324
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILEEXT    
Language=english
error C1324: input file '%1' has illegal extension
.
;//---------------
MessageId = 1328
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TYPE_NOT_IMPL
Language=english
error C1328: data type '%1' not supported
.
;//---------------
MessageId = 1330
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_NO_DEF_FOUND
Language=english
error C1330: data type '%1' undefined
.

;//---------------
MessageId = 1332  
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INVALID_POU_TYPE
Language=english
error C1332: expected type '%1', but '%2' is a '%3'
.


;//---------------
MessageId = 1335
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_POU_REDEF
Language=english
error C1335: POU '%1' already defined in '%2'
.
;//---------------
MessageId = 1336
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ONLY_ONE_DIM
Language=english
error C1336: more dimensional arrays are not supported
.
;//---------------
MessageId = 1344
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_NOT_SUPPORTED
Language=english
error C1344: not supported: %1
.
;//---------------
MessageId = 1350
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ILLEGAL_TASK_INIT
Language=english
error C1350: unknown task attribute '%1', valid attributes are INTERVAL, PRIORITY, SINGLE, AUTOSTART
.
;//---------------
MessageId = 1352
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_TASK_PRIO
Language=english
error C1352: task priority %1 is out of range, allowed range: %2..%3
.
;//---------------
MessageId = 1354
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_ATTR_DATATYPE
Language=english
error C1354: attribute value of '%1' has bad data type '%2' (need '%3')
.
;//---------------
MessageId = 1358
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_DOUBLE_ATTRIP
Language=english
error C1358: attribute '%1' already specified
.
;//---------------
MessageId = 1360
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TASK_NAME_REQUIRED
Language=english
error C1360: missing task name for program instance '%1'
.
;//---------------
MessageId = 1364
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TASK_UNDEFINED
Language=english
error C1364: task '%1' is not defined
.
;//---------------
MessageId = 1366
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_BAD_TASK_INTERVAL_RANGE
Language=english
error C1366: task interval (=%1ms) is out of range, allowed range: %2ms..%3ms
.
;//---------------
MessageId = 1367
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TASKNAME_IS_GLOBVAR
Language=english
error C1367: task name '%1' already used as global variable name in %2
.
;//---------------
MessageId = 1368
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_PRIO_OR_INTERVAL_MISSING
Language=english
error C1368: task priority or interval has not been specified
.
MessageId = 1369
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_PRGINSTNAME_IS_GLOBVAR
Language=english
error C1369: program instance name '%1' already used as global variable name in %2
.
;//---------------
MessageId = 1370
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TASK_ALREADY_DEFINED
Language=english
error C1370: task '%1' already defined
.
;//---------------
MessageId = 1371
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_PRGINSTNAME_IS_TASKNAME
Language=english
error C1371: program instance name '%1' already used as task name
.
;//---------------
MessageId = 1377
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_DIRADDR_TYPE
Language=english
error C1377: direct address '%1' illegal for data type '%2'
.
;//---------------
MessageId = 1378
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_VAR_NOT_ADDRESSABLE
Language=english
error C1378: declaration '%1': data type '%2' not allowed with direct address
.
;//---------------
MessageId = 1380
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_RECURSIV
Language=english
error C1380: recursive call or instantiation of '%1'
.
;//---------------
MessageId = 1384
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_PRJ_UNKNOWN_KEY
Language=english
error C1384: unknown entry key '%1=' in section '%2'
.
;//---------------
MessageId = 1386
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_TYPE
Language=english
error C1386: initial values: type mismatch, initial value has type '%1' but variable is '%2'
.
;//---------------
MessageId = 1387
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_ADDR_INPUT
Language=english
error C1387: initial values: member '%1' AT '%2' cannot be initialized, direct address is an input location
.
;//---------------
MessageId = 1388
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_MEMBER_UNDEF
Language=english
error C1388: initial values: '%1' is not a member of '%2'
.
;//---------------
MessageId = 1389
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_MEMBER_NOACCESS
Language=english
error C1389: initial values: '%1' is not an VAR_INPUT of '%2'
.
;//---------------
MessageId = 1390
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_REPEAT_NOT_INT
Language=english
error C1390: initial values: array init repetition factor has bad type '%1', need ANY_INT 
.
;//---------------
MessageId = 1391
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INIT_REPEAT_BAD_VALUE
Language=english
error C1391: initial values: repetition factor '%1' is out of range, allowed: 1..%2
.




;//CATEGORY: ST statement, expressions semantic,
;//          limits and external compiler errors 1500 - 1999
;//---------------
MessageId = 1503
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TOO_MANY_ARRAY_ELEMS
Language=english
error C1503: limit: too many array elements, found %1 max is %2
.
;//---------------
MessageId = 1505
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FILE_TO_LARGE
Language=english
error C1505: limit: input file '%1' too large to be processed
.
;//---------------
MessageId = 1508
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_RANGE 
Language=english
error C1508: invalid range
.
;//---------------
MessageId = 1510
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ILLEGAL_CALL
Language=english
error C1510: call on non-function block
.
;//---------------
MessageId = 1513
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_DIVIDE_BY_ZERO
Language=english
error C1513: division by zero
.
;//--------------- 
MessageId = 1515
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_FUNCALL_NOFUN
Language=english
error C1515:  '%1' is not the name of a function ('%1' is a '%2')
.
;//---------------
MessageId = 1516
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_LVAL_MEMBER
Language=english
error C1516: VAR_IN_OUT or VAR_OUTPUT parameter '%1' needs l-value
.
;//---------------
MessageId = 1518
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ILL_MEMBER
Language=english
error C1518: member '%1' is neither VAR_INPUT nor VAR_IN_OUT
.
;//---------------
MessageId = 1519
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_MEMBER_NOT_OUT
Language=english
error C1519: member '%1' is not VAR_OUTPUT
.
;//---------------
MessageId = 1522
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNKNOWN_MEMBER
Language=english
error C1522: input/output '%1' unknown
.
;//---------------
MessageId = 1523
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_LABEL_DUPLY
Language=english
error C1523: jump label '%1': already defined above
.
;//---------------
MessageId = 1525
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_LABEL_UNDEF
Language=english
error C1525: jump label '%1': is undefined
.
;//---------------
MessageId = 1526
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNDEF_VAR
Language=english
error C1526: variable '%1' undefined
.

;//---------------
MessageId = 1527
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNDEF_OBJ
Language=english
error C1527: object '%1' undefined
.

;//---------------
MessageId = 1528
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TYPE_SUBSCR
Language=english
error C1528: expected ANY_INT as array subscript '%1'
.
;//---------------
MessageId = 1530
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_RANGE_SUBSCR
Language=english
error C1530: array subscript %1 is out of bounds
.
;//---------------
MessageId = 1532
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_ILL_DIMENSION
Language=english
error C1532: expected %1 subscripts
.
;//---------------
MessageId = 1534
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_NON_ARRAY
Language=english
error C1534: subscript on non-array
.
;//---------------
MessageId = 1536
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNDEF_MEMBER
Language=english
error C1536: member '%1' of '%2' undefined
.
;//---------------
MessageId = 1538
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_PRIV_MEMBER
Language=english
error C1538: member '%1' of '%2' is inaccessible
.
;//---------------
MessageId = 1540
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_INPUT_MEMBER
Language=english
error C1540: member '%1' of '%2' may only be used in a call statement
.
;//---------------
MessageId = 1542
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_NON_STRUCT
Language=english
error C1542: structure selection on non-structure
.
;//---------------
MessageId = 1544
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_LVAL_PARAM
Language=english
error C1544: l-value expected at parameter %1 of '%2'
.

;//---------------
MessageId = 1545
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_NEED_BYTEOFF
Language=english
error C1545: parameter %1 of '%2': need byte offset: %3 is not a multiple of 8.
.

;//---------------
MessageId = 1546
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TYPE_PARAM
Language=english
error C1546: type mismatch at parameter %1 of '%2'
.
;//---------------
MessageId = 1547
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TYPE_ASSIGN
Language=english
error C1547: type mismatch in assignment: '%1' := '%2'
.
;//---------------
MessageId = 1548
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_TYPE_FUNCT
Language=english
error C1548: type mismatch for '%1'
.
;//---------------
MessageId = 1552
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNKNOWN_FUNCT
Language=english
error C1552: unknown function '%1'
.
;//---------------
MessageId = 1553
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_WRONG_TYPE
Language=english
error C1553: expression of data type '%1' expected
.

;//---------------
MessageId = 1554
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_NEED_GLOBAL
Language=english
error C1554: parameter %1 of '%2' must be a global variable or byte offset literal
.
;//---------------
MessageId = 1555
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_NEED_BYTESIZE
Language=english
error C1555: parameter %1 of '%2' does not evaluate to a byte size
.
;//---------------
MessageId = 1556
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_BAD_BYTESIZE
Language=english
error C1556: parameter %1 of '%2' does not evaluate to a byte size: %3 not a multiple of 8
.
;//---------------
MessageId = 1557
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_OFFSETOF
Language=english
error C1557: parameter %1 of '%2' does not evaluate to a offset
.
;//---------------
MessageId = 1558
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_BAD_BITOFFSETOF
Language=english
error C1558: parameter %1 of '%2' does not evaluate to a bit offset: %3 not a multiple of 8
.

;//---------------
MessageId = 1560
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_NEED_X
Language=english
error C1560: parameter %1 of '%2' does not evaluate to a %3
.

;//---------------
MessageId = 1561
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_NEED_CLSID
Language=english
error C1561: parameter %1 of '%2' does not evaluate to a classid
.

;//---------------
MessageId = 1562
Severity = error
Facility = CCG
SymbolicName = E_ASM_BADPAR_RANGE
Language=english
error C1562: parameter %1 of '%2': %3 out of range %4
.

;//---------------
MessageId = 1563
Severity = error
Facility = CCG
SymbolicName = E_ASM_BAD_CLSID
Language=english
error C1563: '%1' does not have a clsid
.

;//---------------
MessageId = 1564
Severity = error
Facility = CCG
SymbolicName = E_ASM_BAD_FBID
Language=english
error C1564: '%1' does not have a 'fbid' (no system function block)
.


;//---------------
MessageId = 1565
Severity = error
Facility = CCG
SymbolicName = E_ASM_AIDX_PARM_INCOMPAT
Language=english
error C1565: array index size and opcode paramter size are incompatible
.

;//---------------
MessageId = 1566
Severity = error
Facility = CCG
SymbolicName = E_ASM_BAD_AIDX
Language=english
error C1566: '%1' is not a valid array index type
.




;//-----------------------------------------------------
;//CATEGORY: linker errors 3000 - 3999
;//---------------
;//---------------
MessageId = 3006
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_GV_UNDEFINED
Language=english
error C3006: unresolved external '%1': variable not found in gvl of resource '%2'
.
;//---------------
MessageId = 3011
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_GV_INPUT_WRITE
Language=english
error C3011: write access to input location '%1 AT %2'
.
;//---------------
MessageId = 3012
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE
Language=english
error C3012: %1 segment to large. Used %2 bytes (maximum allowed %3)
.
;//---------------
MessageId = 3016
Severity = error
Facility = CCG
SymbolicName = E_4CCG_ERR_UNKNOWN_SEGM_NAME
Language=english
error C3016: invalid segment name '%1' used
.
;//---------------







;//---------------
MessageId = 4001
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_BAD_UNICODE_CHAR
Language=english
warning C4001: current codepage does not support the UNICODE character '%1'
.
;//---------------
MessageId = 4003
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_NO_PROGRAM_AT_TASK 
Language=english
warning C4003: task '%1.%2' has no programs to execute
.
;//---------------
MessageId = 4007
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_UNKNOWN_ATTRIB
Language=english
warning C4007: unknown attribute '%1'
.
;//---------------
MessageId = 4020
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_INIT_ARRAY_TOO_LESSMUCH
Language=english
warning C4020: initial values: %1 initial values but %2 array elements
.
;//---------------
MessageId = 4021
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_UNUSED_LABEL
Language=english
warning C4021: jump label '%1': label never used
.
;//---------------
MessageId = 4026
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_CMD_OPT_TWICE
Language=english
warning C4026: compiler switch '-%1' given twice (ignore second one)
.










;//-----------------------------------------------------
;//CATEGORY: linker warnings 5000 
;//---------------
MessageId = 5010
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_ADDR_RANGE
Language=english
warning C5010: direct address '%1': invalid for master '%2'
.
;//---------------
MessageId = 5011
Severity = warning
Facility = CCG
SymbolicName = E_4CCG_WARN_ADDR_UNKNOWN_MODIFIER
Language=english
warning C5011: direct address '%1': ignore unknown modifier '%2'
.

