
(*[[
    UserURL= "ATCM:FA_ASYNC_READFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Functionblock FA_ASYNC_READFILE
 *  Purpose: Reads data from a file.
 *)

FUNCTION_BLOCK FA_ASYNC_READFILE
VAR_INPUT
    EXEC        : BOOL;             (* raising edge starts operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_ASYNC_FILE;    (* file instance to operate on *)
    LENGTH      : DINT;             (* bytes to read; return read bytes *)
END_VAR
VAR_OUTPUT
    DATA        : BYTE_STRING;      (* read data *)
    DONE        : BOOL;             (* operation finished *)
    ERROR       : BOOL;             (* error flag *)
    ERRCD       : DINT;             (* error code *)
    ERRTXT      : WSTRING;          (* error text *)
END_VAR

VAR
    rtrigExec   : R_TRIG;           (* execute *)
    token       : BOOL;             (* instance access token *)
    ireq        : BOOL;             (* execute request *)
END_VAR

rtrigExec(CLK := EXEC);
ireq := rtrigExec.Q;

NATIVE
    _done.putBool( ((COM.softing.fclib.flib.AsyncFLib)_file).readFile(_exec.getBool(),
                        _ireq.getBool(),
                        _token,
                        this,
                        (FcObject)_data,
                        _length,
                        _error,
                        _errcd,
                        _errtxt) );
END_NATIVE;

END_FUNCTION_BLOCK
