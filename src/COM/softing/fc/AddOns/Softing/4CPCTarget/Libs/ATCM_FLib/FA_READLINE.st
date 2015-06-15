
(*[[
    UserURL= "ATCM:FA_READLINE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Functionblock FA_READLINE
 *  Purpose: Reads data from a file.
 *)
FUNCTION_BLOCK FA_READLINE
VAR_INPUT
    EXEC        : BOOL;             (* raising edge starts operation *)
    FILENAME    : WSTRING;          (* filename *)
    TOUT        : TIME;             (* timeout for this operation *)
END_VAR
VAR_IN_OUT
    POSITION    : DINT;             (* start at position; returns last position *)
END_VAR
VAR_OUTPUT
    DATA        : WSTRING;          (* read data *)
    DONE        : BOOL;             (* operation finished *)
    ERROR       : BOOL;             (* error flag *)
    ERRCD       : DINT;             (* error code *)
    ERRTXT      : WSTRING;          (* error text *)
END_VAR

VAR
    file        : FA_ASYNC_FILE;
    aopen       : FA_ASYNC_OPENFILE;
    aread       : FA_ASYNC_READFILE;
    aclose      : FA_ASYNC_CLOSEFILE;
    length      : DINT;
    rtDone      : R_TRIG;
    rtInit      : R_TRIG;
    rtRead      : R_TRIG;
    rtError     : R_TRIG;
    pos         : DINT;
    bstrData    : BYTE_STRING;
END_VAR

(* init structure *)
file.FILENAME := FILENAME;
file.MODE := CFA_READ;
file.TEXTMODE := TRUE;
file.TOUT := TOUT;
length := CFA_EOL;

(* delete output *)
IF NOT EXEC THEN
    DONE := FALSE;
    ERROR := FALSE;
    ERRCD := 0;
    ERRTXT := "";
    DATA := "";
    file.ISERROR := FALSE;
END_IF;

(* open file *)
aopen(EXEC := EXEC, FILE := file);

(* init read file *)
rtInit(CLK := aopen.DONE);
IF rtInit.Q THEN
    (* init *)
    file.POSITION := POSITION;
END_IF;
aread(EXEC := aopen.DONE AND NOT aopen.ERROR, FILE := file, LENGTH := length);
(* read file done *)
rtRead(CLK := aread.DONE);
IF rtRead.Q THEN
    bstrData := aread.DATA;
END_IF;

(* close file *)
aclose(EXEC := aread.DONE AND NOT aread.ERROR, FILE := file);
        
(* init output vars *)
rtDone(CLK := aclose.DONE);
IF rtDone.Q THEN
    DONE        := TRUE;
    (* convert WString to BYTE_STRING *)
    pos := 0;
    DATA        := COPY_WSTRING_FROM_BYTE_STRING(bstrdata.SIZE, pos, bstrdata);
    POSITION    := file.POSITION;
END_IF;

(* init error vars *)
rtError(CLK := file.ISERROR);
IF rtError.Q THEN
    DONE        := TRUE;
    DATA        := "";
    ERROR       := file.ISERROR;
    ERRCD       := file.LSTERRCD;
    ERRTXT      := file.LSTERRTXT;
END_IF;

END_FUNCTION_BLOCK
