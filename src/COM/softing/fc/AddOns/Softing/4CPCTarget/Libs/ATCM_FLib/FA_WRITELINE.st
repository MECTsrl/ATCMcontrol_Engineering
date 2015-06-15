
(*[[
    UserURL= "ATCM:FA_WRITELINE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Functionblock FA_WRITELINE
 *  Purpose: Writes data to a file.
 *)
FUNCTION_BLOCK FA_WRITELINE
VAR_INPUT
    EXEC        : BOOL;             (* raising edge starts operation *)
    FILENAME    : WSTRING;          (* filename *)
    TOUT        : TIME;             (* timeout for this operation *)
    DATA        : WSTRING;          (* data to write *)
END_VAR
VAR_IN_OUT
    POSITION    : DINT;             (* start at position; returns last position *)
END_VAR
VAR_OUTPUT
    DONE        : BOOL;             (* operation finished *)
    ERROR       : BOOL;             (* error flag *)
    ERRCD       : DINT;             (* error code *)
    ERRTXT      : WSTRING;          (* error text *)
END_VAR

VAR
    file        : FA_ASYNC_FILE;
    aopen       : FA_ASYNC_OPENFILE;
    awrite      : FA_ASYNC_WRITEFILE;
    aclose      : FA_ASYNC_CLOSEFILE;
    length      : DINT;
    ldata       : WSTRING;
    bstrData    : BYTE_STRING;
    rtWrite     : R_TRIG;
    rtDone      : R_TRIG;
    rtInit      : R_TRIG;
    rtError     : R_TRIG;
    posStr      : DINT := 0;
    posPtr      : DINT;
END_VAR

(* init structure *)
file.FILENAME := FILENAME;
file.MODE := CFA_READ_WRITE;
file.TEXTMODE := TRUE;
file.TOUT := TOUT;

(* delete output *)
IF NOT EXEC THEN
    DONE := FALSE;
    ERROR := FALSE;
    ERRCD := 0;
    ERRTXT := "";
    file.ISERROR := FALSE;
END_IF;

(* open file *)
aopen(EXEC := EXEC, FILE := file);

(* init write file *)
rtInit(CLK := aopen.DONE);
IF rtInit.Q THEN
    (* init *)
    file.POSITION := POSITION;
    (* convert WString to BYTE_STRING *)
    posStr := 0;
    COPY_WSTRING_TO_BYTE_STRING(CONCAT(DATA, "$n"), posStr, bstrData);
    bstrData.size := posStr;
END_IF;
(* write into file *)
awrite(EXEC := aopen.DONE AND NOT aopen.ERROR, FILE := file, DATA := bstrData);
(* write file done *)
rtWrite(CLK := awrite.DONE);
IF rtWrite.Q THEN
    posPtr := file.POSITION;
END_IF;

(* close file *)
aclose(EXEC := awrite.DONE, FILE := file);

(* init output vars *)
rtDone(CLK := aclose.DONE);
IF rtDone.Q THEN
    DONE        := TRUE;
    POSITION    := posPtr;
END_IF;

(* init error vars *)
rtError(CLK := file.ISERROR);
IF rtError.Q THEN
    DONE        := TRUE;
    ERROR       := file.ISERROR;
    ERRCD       := file.LSTERRCD;
    ERRTXT      := file.LSTERRTXT;
END_IF;
            
END_FUNCTION_BLOCK
